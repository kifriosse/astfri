#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/util/astfri_util.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri-cs/impl/visitor/SrcCodeVisitor.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <queue>
#include <stdexcept>
#include <string>
#include <string_view>

namespace astfri::csharp
{

Stmt* SrcCodeVisitor::visit_var_def_stmt(
    const TSNode& node,
    const util::VarDefType def_type
)
{
    const std::vector<TSNode> n_modifs
        = util::find_nodes(node, lang_, regs::Queries::var_modif_query);

    const TSNode n_var_decl = n_modifs.empty()
                                ? ts_node_child(node, 0)
                                : ts_node_next_sibling(n_modifs.back());

    const TSNode n_type     = util::child_by_field_name(n_var_decl, "type");
    const TypeHandler th    = RegManager::get_type_handler(n_type);
    Type* type              = th(&type_tr_, n_type);

    const std::vector<TSNode> n_var_decltors
        = util::find_nodes(n_var_decl, lang_, regs::Queries::decl_query);

    std::vector<VarDefStmt*> var_defs;
    for (const TSNode& n_var_decltor : n_var_decltors)
    {
        TSNode n_var_name = ts_node_named_child(n_var_decltor, 0);
        TSNode n_init     = ts_node_named_child(n_var_decltor, 1);
        std::string name  = util::extract_text(n_var_name, src_str());
        Expr* init        = nullptr;
        if (! ts_node_is_null(n_init))
        {
            ExprHandler h_init = RegManager::get_expr_handler(n_init);
            init               = h_init(this, n_init);
        }

        VarDefStmt* var_def = nullptr;
        switch (def_type)
        {
        case util::VarDefType::Member:
        {
            // todo handle other modifiers
            MemberVarMetadata* var_meta = semantic_context_.find_memb_var(
                name,
                semantic_context_.current_type()
            );
            var_def               = var_meta->var_def;
            var_def->initializer_ = init;
            var_meta->processed   = true;
            break;
        }
        case util::VarDefType::Local:
            // const CSModifiers modifiers
            //     = CSModifiers::handle_modifiers(modifier_nodes,
            //     self->src_code_);
            // todo handle const
            var_def = stmt_f_.mk_local_var_def(std::move(name), type, init);
            semantic_context_.reg_local_var(as_a<LocalVarDefStmt>(var_def));
            break;
        case util::VarDefType::Global:
            // todo handle const
            var_def = stmt_f_.mk_global_var_def(std::move(name), type, init);
            break;
        }
        // todo this might be unnecessary in future
        if (var_def)
        {
            // var_def->name_ = var_name;
            var_defs.push_back(var_def);
        }
    }

    if (var_defs.size() > 1)
        return stmt_f_.mk_def(std::move(var_defs));

    return var_defs.front();
}

Stmt* SrcCodeVisitor::make_while_loop(const TSNode& node, const bool is_while)
{
    const TSNode n_cond      = util::child_by_field_name(node, "condition");
    const TSNode n_body      = util::child_by_field_name(node, "body");
    const ExprHandler h_cond = RegManager::get_expr_handler(n_cond);
    const StmtHandler h_body = RegManager::get_stmt_handler(n_body);
    Expr* cond               = h_cond(this, n_cond);
    Stmt* body               = h_body(this, n_body);

    if (is_while)
        return stmt_f_.mk_while(cond, body);
    return stmt_f_.mk_do_while(cond, body);
}

FunctionDefStmt* SrcCodeVisitor::make_func_stmt(
    const TSNode& node,
    const bool is_method
)
{
    semantic_context_.enter_scope();

    FunctionDefStmt* func_def = stmt_f_.mk_function_def();
    const TSNode n_ret_type
        = util::child_by_field_name(node, is_method ? "returns" : "type");
    const TSNode n_name  = util::child_by_field_name(node, "name");
    const TSNode n_param = util::child_by_field_name(node, "parameters");
    const TSNode n_body  = util::child_by_field_name(node, "body");

    // todo handle generic parameters
    const TypeHandler th     = RegManager::get_type_handler(n_ret_type);
    const StmtHandler h_body = RegManager::get_stmt_handler(n_body);
    Type* ret_type           = th(&type_tr_, n_ret_type);

    semantic_context_.reg_return(ret_type);

    func_def->retType_ = ret_type;
    func_def->name_    = util::extract_text(n_name, src_str());
    func_def->params_  = make_param_list(n_param, false);
    func_def->body_    = as_a<CompoundStmt>(h_body(this, n_body));

    semantic_context_.leave_scope();
    semantic_context_.unregister_return_type();
    return func_def;
}

Expr* SrcCodeVisitor::expr_list_to_comma_op(
    const TSNode& start_node,
    const TSNode* end_node
)
{
    std::queue<Expr*> exprs;
    TSNode n_next = start_node;
    while (! ts_node_is_null(n_next))
    {
        TSNode n_current = n_next;
        n_next           = ts_node_next_sibling(n_current);
        if (end_node && ts_node_eq(n_current, *end_node))
            break;

        if (! ts_node_is_named(n_current))
            continue;

        const ExprHandler h_expr = RegManager::get_expr_handler(n_current);
        exprs.push(h_expr(this, n_current));
    }

    if (exprs.empty())
        return nullptr;

    Expr* init_expr = exprs.front();
    exprs.pop();

    while (! exprs.empty())
    {
        Expr* right = exprs.front();
        exprs.pop();
        init_expr = expr_f_.mk_bin_on(init_expr, BinOpType::Comma, right);
    }
    return init_expr;
}

std::vector<ParamVarDefStmt*> SrcCodeVisitor::make_param_list(
    const TSNode& node,
    const bool make_shallow
)
{
    std::vector<ParamVarDefStmt*> params;
    auto collector = [&](const TSNode& current)
    {
        const TSNode n_name  = util::child_by_field_name(current, "name");
        const TSNode n_type  = util::child_by_field_name(current, "type");
        const TSNode n_init  = ts_node_next_named_sibling(n_name);
        std::string name     = util::extract_text(n_name, src_str());
        const TypeHandler th = RegManager::get_type_handler(n_type);
        Type* type           = th(&type_tr_, n_type);
        Expr* init           = nullptr;
        if (! make_shallow && ! ts_node_is_null(n_init))
        {
            const ExprHandler h_init = RegManager::get_expr_handler(n_init);
            init                     = h_init(this, n_init);
        }
        ParamVarDefStmt* param_def
            = stmt_f_.mk_param_var_def(std::move(name), type, init);
        params.push_back(param_def);
    };
    util::process_param_list(node, std::move(collector));
    return params;
}

std::vector<Expr*> SrcCodeVisitor::visit_arg_list(const TSNode& node)
{
    std::vector<Expr*> exprs;
    auto process = [this, &exprs](TSNode current)
    {
        const TSNode n_child     = ts_node_child(current, 0);
        const ExprHandler h_expr = RegManager::get_expr_handler(n_child);
        exprs.emplace_back(h_expr(this, n_child));
    };
    util::for_each_child_node(node, process);
    return exprs;
}

Stmt* SrcCodeVisitor::visit_for_init_var_def(const TSNode& node)
{
    std::vector<VarDefStmt*> var_defs;
    const TSNode n_type  = util::child_by_field_name(node, "type");
    const TypeHandler th = RegManager::get_type_handler(n_type);
    const std::vector<TSNode> n_decltr
        = util::find_nodes(node, lang_, regs::Queries::decl_query);
    for (const auto declarator_node : n_decltr)
    {
        TSNode n_name            = ts_node_named_child(declarator_node, 0);
        TSNode n_right           = ts_node_named_child(declarator_node, 1);
        ExprHandler h_right      = RegManager::get_expr_handler(n_right);
        LocalVarDefStmt* var_def = stmt_f_.mk_local_var_def(
            util::extract_text(n_name, src_str()),
            th(&type_tr_, n_type),
            h_right(this, n_right)
        );
        var_defs.push_back(var_def);
        semantic_context_.reg_local_var(var_def);
    }

    if (var_defs.size() > 1)
        return stmt_f_.mk_def(std::move(var_defs));
    return var_defs.front();
}

std::string_view SrcCodeVisitor::src_str() const
{
    return current_src_
             ? current_src_->file.content
             : throw std::logic_error("Current source code is not set");
}

SourceCode* SrcCodeVisitor::src() const
{
    return current_src_
             ? current_src_
             : throw std::logic_error("Current source code is not set");
}

} // namespace astfri::csharp