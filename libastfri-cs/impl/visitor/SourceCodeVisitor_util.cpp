#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/utils.hpp>
#include <libastfri-cs/impl/visitor/SourceCodeVisitor.hpp>

#include <queue>
#include <ranges>

namespace astfri::csharp
{

Stmt* SourceCodeVisitor::make_while_loop(
    const TSNode* node,
    const bool is_do_while
)
{
    const TSNode cond_node = util::child_by_field_name(*node, "condition");
    const TSNode body_node = util::child_by_field_name(*node, "body");
    const ExprHandler cond_handler = RegManager::get_expr_handler(cond_node);
    const StmtHandler body_handler = RegManager::get_stmt_handler(body_node);
    Expr* condition                = cond_handler(this, &cond_node);
    Stmt* body                     = body_handler(this, &body_node);

    if (is_do_while)
        return stmt_factory_.mk_do_while(condition, body);

    return stmt_factory_.mk_while(condition, body);
}

Expr* SourceCodeVisitor::expr_list_to_comma_op(
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
        exprs.push(expr_handler(this, &current_node));
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

FunctionMetadata SourceCodeVisitor::make_func_metadata(
    const TSNode& node,
    const std::string_view src_code
)
{
    const TSNode name_node     = util::child_by_field_name(node, "name");
    const TSNode return_node   = util::child_by_field_name(node, "type");
    const TSNode params_node   = util::child_by_field_name(node, "parameters");
    std::string name           = util::extract_node_text(name_node, src_code);
    Type* ret_type             = util::make_type(return_node, src_code);
    auto [params, params_data] = util::discover_params(params_node, src_code);
    return FunctionMetadata{
        .params   = std::move(params_data),
        .func_def = stmt_factory_.mk_function_def(
            std::move(name),
            std::move(params),
            ret_type,
            nullptr
        ),
        .function_node = node,
    };
}

std::vector<ParamVarDefStmt*> SourceCodeVisitor::make_param_list(
    SourceCodeVisitor* self,
    const TSNode* node,
    const bool make_shallow // todo this might be useless
)
{
    std::vector<ParamVarDefStmt*> params;
    auto collector = [&](const TSNode& current)
    {
        const TSNode name_node = util::child_by_field_name(current, "name");
        const TSNode type_node = util::child_by_field_name(current, "type");
        std::string name
            = util::extract_node_text(name_node, self->get_src_code());
        Type* type = util::make_type(type_node, self->get_src_code());
        Expr* init = nullptr;
        const TSNode init_node = ts_node_next_named_sibling(name_node);
        if (! make_shallow && ! ts_node_is_null(init_node))
        {
            const ExprHandler handler = RegManager::get_expr_handler(init_node);
            init                      = handler(self, &init_node);
        }
        ParamVarDefStmt* param_def
            = stmt_factory_.mk_param_var_def(std::move(name), type, init);
        params.push_back(param_def);
    };
    util::process_param_list(*node, collector);
    return params;
}

std::vector<Expr*> SourceCodeVisitor::handle_argument_list(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    if (! ts_tree_cursor_goto_first_child(&cursor))
        throw std::logic_error("Invalid node");

    TSNode current = ts_tree_cursor_current_node(&cursor);
    std::vector<Expr*> exprs;
    do
    {
        current = ts_tree_cursor_current_node(&cursor);
        if (! ts_node_is_named(current))
            continue;

        ts_tree_cursor_goto_first_child(&cursor);

        current                  = ts_tree_cursor_current_node(&cursor);
        ExprHandler expr_handler = RegManager::get_expr_handler(current);
        exprs.emplace_back(expr_handler(self, &current));

        ts_tree_cursor_goto_parent(&cursor);
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    ts_tree_cursor_delete(&cursor);
    return exprs;
}

Stmt* SourceCodeVisitor::handle_for_init_var_def(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    std::vector<VarDefStmt*> var_defs;
    const TSNode type_node = util::child_by_field_name(*node, "type");
    Type* type             = util::make_type(type_node, self->get_src_code());
    const std::vector<TSNode> decltr_nodes
        = util::find_nodes(*node, self->get_lang(), regs::Queries::decl_query);
    for (const auto declarator_node : decltr_nodes)
    {
        TSNode var_name_node = ts_node_child(declarator_node, 0);
        TSNode right_node    = ts_node_child(declarator_node, 2);
        ExprHandler right_side_handler
            = RegManager::get_expr_handler(right_node);
        LocalVarDefStmt* var_def = stmt_factory_.mk_local_var_def(
            util::extract_node_text(var_name_node, self->get_src_code()),
            type,
            right_side_handler(self, &right_node)
        );
        var_defs.push_back(var_def);
        self->semantic_context_.reg_local_var(var_def);
    }

    if (var_defs.size() > 1)
    {
        return stmt_factory_.mk_def(var_defs);
    }
    return var_defs.front();
}

std::string_view SourceCodeVisitor::get_src_code() const
{
    if (! current_src)
        throw std::logic_error("Current source code is not set");
    return current_src->file.content;
}

const TSLanguage* SourceCodeVisitor::get_lang() const
{
    if (! current_src)
        throw std::logic_error("Current source code is not set");
    return ts_tree_language(current_src->tree);
}

} // namespace astfri::csharp
