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
    const std::vector<TSNode> modifier_nodes
        = util::find_nodes(node, lang(), regs::Queries::var_modif_query);

    const TSNode var_decl_node
        = modifier_nodes.empty() ? ts_node_child(node, 0)
                                 : ts_node_next_sibling(modifier_nodes.back());

    const TSNode type_node = util::child_by_field_name(var_decl_node, "type");
    const TypeHandler th   = RegManager::get_type_handler(type_node);
    Type* type             = th(&type_tr_, type_node);

    const std::vector<TSNode> var_decltor_nodes
        = util::find_nodes(var_decl_node, lang(), regs::Queries::decl_query);

    std::vector<VarDefStmt*> var_def_stmts;
    for (const TSNode& var_decltor_node : var_decltor_nodes)
    {
        // left side
        TSNode var_name_node = ts_node_named_child(var_decltor_node, 0);
        // right side
        TSNode init_node = ts_node_named_child(var_decltor_node, 1);

        const std::string var_name
            = util::extract_node_text(var_name_node, src_str());
        Expr* init = nullptr;
        if (! ts_node_is_null(init_node))
        {
            ExprHandler init_handler = RegManager::get_expr_handler(init_node);
            init                     = init_handler(this, init_node);
        }

        VarDefStmt* var_def_stmt = nullptr;
        switch (def_type)
        {
        case util::VarDefType::Member:
        {
            // todo handle other modifiers
            MemberVarMetadata* var_data = semantic_context_.find_memb_var(
                var_name,
                semantic_context_.current_type()
            );
            var_def_stmt               = var_data->var_def;
            var_def_stmt->initializer_ = init;
            var_data->processed        = true;
            break;
        }
        case util::VarDefType::Local:
            // const CSModifiers modifiers
            //     = CSModifiers::handle_modifiers(modifier_nodes,
            //     self->src_code_);
            // todo handle const
            var_def_stmt = stmt_factory_.mk_local_var_def(var_name, type, init);
            semantic_context_.reg_local_var(
                as_a<LocalVarDefStmt>(var_def_stmt)
            );
            break;
        case util::VarDefType::Global:
            // todo handle const
            var_def_stmt
                = stmt_factory_.mk_global_var_def(var_name, type, init);
            break;
        }
        // todo this might not be necessary in future
        if (var_def_stmt)
        {
            var_def_stmt->name_ = var_name;
            var_def_stmts.push_back(var_def_stmt);
        }
    }

    if (var_def_stmts.size() > 1)
        return stmt_factory_.mk_def(var_def_stmts);

    return var_def_stmts.front();
}

Stmt* SrcCodeVisitor::make_while_loop(
    const TSNode& node,
    const bool is_do_while
)
{
    const TSNode cond_node = util::child_by_field_name(node, "condition");
    const TSNode body_node = util::child_by_field_name(node, "body");
    const ExprHandler cond_handler = RegManager::get_expr_handler(cond_node);
    const StmtHandler body_handler = RegManager::get_stmt_handler(body_node);
    Expr* condition                = cond_handler(this, cond_node);
    Stmt* body                     = body_handler(this, body_node);

    if (is_do_while)
        return stmt_factory_.mk_do_while(condition, body);

    return stmt_factory_.mk_while(condition, body);
}

FunctionDefStmt* SrcCodeVisitor::make_func_stmt(
    const TSNode& node,
    const bool is_method
)
{
    semantic_context_.enter_scope();

    FunctionDefStmt* func_def = stmt_factory_.mk_function_def();
    const TSNode ret_type_node
        = util::child_by_field_name(node, is_method ? "returns" : "type");
    const TSNode n_name     = util::child_by_field_name(node, "name");
    const TSNode param_node = util::child_by_field_name(node, "parameters");
    const TSNode body_node  = util::child_by_field_name(node, "body");

    // todo handle generic parameters
    const TypeHandler th = RegManager::get_type_handler(ret_type_node);
    Type* ret_type       = th(&type_tr_, ret_type_node);
    semantic_context_.reg_return(ret_type);

    func_def->retType_             = ret_type;
    func_def->name_                = util::extract_node_text(n_name, src_str());
    func_def->params_              = make_param_list(param_node, false);
    const StmtHandler body_handler = RegManager::get_stmt_handler(body_node);
    func_def->body_ = as_a<CompoundStmt>(body_handler(this, body_node));

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
    TSNode next = start_node;
    while (! ts_node_is_null(next))
    {
        TSNode current_node = next;
        next                = ts_node_next_sibling(current_node);
        if (end_node && ts_node_eq(current_node, *end_node))
            break;

        if (! ts_node_is_named(current_node))
            continue;

        const ExprHandler expr_handler
            = RegManager::get_expr_handler(current_node);
        exprs.push(expr_handler(this, current_node));
    }

    if (exprs.empty())
        return nullptr;

    Expr* init_expr = exprs.front();
    exprs.pop();

    while (! exprs.empty())
    {
        Expr* right = exprs.front();
        exprs.pop();
        init_expr = expr_factory_.mk_bin_on(init_expr, BinOpType::Comma, right);
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
        const TSNode n_name    = util::child_by_field_name(current, "name");
        const TSNode type_node = util::child_by_field_name(current, "type");
        std::string name       = util::extract_node_text(n_name, src_str());
        const TypeHandler th   = RegManager::get_type_handler(type_node);
        Type* type             = th(&type_tr_, type_node);
        Expr* init             = nullptr;
        const TSNode init_node = ts_node_next_named_sibling(n_name);
        if (! make_shallow && ! ts_node_is_null(init_node))
        {
            const ExprHandler handler = RegManager::get_expr_handler(init_node);
            init                      = handler(this, init_node);
        }
        ParamVarDefStmt* param_def
            = stmt_factory_.mk_param_var_def(std::move(name), type, init);
        params.push_back(param_def);
    };
    util::process_param_list(node, collector);
    return params;
}

std::vector<Expr*> SrcCodeVisitor::visit_arg_list(const TSNode& node)
{
    TSTreeCursor cursor = ts_tree_cursor_new(node);
    if (! ts_tree_cursor_goto_first_child(&cursor))
        throw std::logic_error("Invalid node");

    std::vector<Expr*> exprs;
    do
    {
        TSNode current = ts_tree_cursor_current_node(&cursor);
        if (! ts_node_is_named(current))
            continue;

        ts_tree_cursor_goto_first_child(&cursor);

        current                  = ts_tree_cursor_current_node(&cursor);
        ExprHandler expr_handler = RegManager::get_expr_handler(current);
        exprs.emplace_back(expr_handler(this, current));

        ts_tree_cursor_goto_parent(&cursor);
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    ts_tree_cursor_delete(&cursor);
    return exprs;
}

Stmt* SrcCodeVisitor::visit_for_init_var_def(const TSNode& node)
{
    std::vector<VarDefStmt*> var_defs;
    const TSNode type_node = util::child_by_field_name(node, "type");
    const TypeHandler th   = RegManager::get_type_handler(type_node);
    Type* type             = th(&type_tr_, type_node);
    const std::vector<TSNode> decltr_nodes
        = util::find_nodes(node, lang(), regs::Queries::decl_query);
    for (const auto declarator_node : decltr_nodes)
    {
        TSNode var_name_node = ts_node_named_child(declarator_node, 0);
        TSNode right_node    = ts_node_named_child(declarator_node, 1);
        ExprHandler right_side_handler
            = RegManager::get_expr_handler(right_node);
        LocalVarDefStmt* var_def = stmt_factory_.mk_local_var_def(
            util::extract_node_text(var_name_node, src_str()),
            type,
            right_side_handler(this, right_node)
        );
        var_defs.push_back(var_def);
        semantic_context_.reg_local_var(var_def);
    }

    if (var_defs.size() > 1)
    {
        return stmt_factory_.mk_def(var_defs);
    }
    return var_defs.front();
}

std::string_view SrcCodeVisitor::src_str() const
{
    return current_src
             ? current_src->file.content
             : throw std::logic_error("Current source code is not set");
}

// todo remove this and replace with one lang
const TSLanguage* SrcCodeVisitor::lang() const
{
    return current_src
             ? ts_tree_language(current_src->tree)
             : throw std::logic_error("Current source code is not set");
}

SourceCode* SrcCodeVisitor::src() const
{
    return current_src
             ? current_src
             : throw std::logic_error("Current source code is not set");
}

} // namespace astfri::csharp
