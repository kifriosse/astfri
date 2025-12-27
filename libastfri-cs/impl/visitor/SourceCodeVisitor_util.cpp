#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/utils.hpp>
#include <libastfri-cs/impl/visitor/SourceCodeVisitor.hpp>

#include <cstring>
#include <queue>

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

std::vector<ParamVarDefStmt*> SourceCodeVisitor::handle_param_list(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    std::vector<ParamVarDefStmt*> parameters;
    if (ts_tree_cursor_goto_first_child(&cursor))
    {
        static const TSSymbol param_keyword = ts_language_symbol_for_name(
            self->get_lang(),
            "params",
            strlen("params"),
            false
        );
        const StmtHandler handler = RegManager::get_stmt_handler("parameter");
        bool is_variadic          = false;
        do
        {
            TSNode current = ts_tree_cursor_current_node(&cursor);
            if (! ts_node_is_named(current))
            {
                is_variadic = ts_node_symbol(current) == param_keyword;
                if (is_variadic)
                    break;
                continue;
            }

            parameters.emplace_back(
                as_a<ParamVarDefStmt>(handler(self, &current))
            );
        } while (ts_tree_cursor_goto_next_sibling(&cursor));

        if (is_variadic && parameters.empty())
        {
            // todo redo this when we add variadic parameters
            // temporary solution
            const TSNode type_node = util::child_by_field_name(*node, "type");
            const TSNode name_node = util::child_by_field_name(*node, "name");
            ParamVarDefStmt* param = stmt_factory_.mk_param_var_def(
                util::extract_node_text(name_node, self->get_src_code()),
                util::make_type(type_node, self->get_src_code()),
                nullptr
            );
            parameters.emplace_back(param);
        }
    }

    ts_tree_cursor_delete(&cursor);
    return parameters;
}

std::vector<Expr*> SourceCodeVisitor::handle_argument_list(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    TSNode current      = ts_tree_cursor_current_node(&cursor);
    if (! ts_tree_cursor_goto_first_child(&cursor))
        throw std::logic_error("Invalid node");

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

std::string& SourceCodeVisitor::get_src_code() const
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
