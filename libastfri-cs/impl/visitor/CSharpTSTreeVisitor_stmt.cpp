#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/visitor/CSharpTSTreeVisitor.hpp>

namespace astfri::csharp
{

Stmt* CSharpTSTreeVisitor::handle_block_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    std::vector<Stmt*> statements;

    if (! ts_tree_cursor_goto_first_child(&cursor))
        return nullptr;

    do
    {
        TSNode current_node = ts_tree_cursor_current_node(&cursor);
        if (! NodeRegistry::is_structural_or_null_node(current_node))
        {
            StmtHandler handler = NodeRegistry::get_stmt_handler(current_node);
            statements.push_back(handler(self, &current_node));
        }
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    return stmt_factory_.mk_compound(statements);
}

Stmt* CSharpTSTreeVisitor::handle_arrow_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    // todo not implemented
    return stmt_factory_.mk_compound({});
}

Stmt* CSharpTSTreeVisitor::handle_while_loop(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    return self->make_while_loop(node, false);
}

Stmt* CSharpTSTreeVisitor::handle_do_while_loop(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    return self->make_while_loop(node, true);
}

Stmt* CSharpTSTreeVisitor::handle_for_loop(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    Stmt* init      = nullptr;
    Expr* condition = nullptr;
    Stmt* updater   = nullptr;
    const TSNode init_node
        = ts_node_child_by_field_name(*node, "initializer", 11);
    const TSNode cond_node = ts_node_child_by_field_name(*node, "condition", 9);
    const TSNode updater_node = ts_node_child_by_field_name(*node, "update", 6);
    const TSNode body_node    = ts_node_child_by_field_name(*node, "body", 4);
    const bool cond_null      = ts_node_is_null(cond_node);
    const bool updater_null   = ts_node_is_null(updater_node);

    if (! ts_node_is_null(init_node))
    {
        if (NodeRegistry::is_stmt(init_node))
        {
            const StmtHandler init_handler
                = NodeRegistry::get_stmt_handler(init_node);
            init = init_handler(self, &init_node);
        }
        else if (NodeRegistry::is_expr(init_node))
        {
            const TSNode* end_node = &body_node;
            if (! cond_null)
                end_node = &cond_node;
            else if (! updater_null)
                end_node = &updater_node;

            init = stmt_factory_.mk_expr(
                self->expr_list_to_comma_op(init_node, end_node)
            );
        }
    }

    if (! cond_null)
    {
        const ExprHandler cond_handler
            = NodeRegistry::get_expr_handler(cond_node);
        condition = cond_handler(self, &cond_node);
    }

    if (! updater_null)
    {
        updater = stmt_factory_.mk_expr(
            self->expr_list_to_comma_op(updater_node, &body_node)
        );
    }

    const StmtHandler body_handler = NodeRegistry::get_stmt_handler(body_node);
    Stmt* body                     = body_handler(self, &body_node);
    return stmt_factory_.mk_for(init, condition, updater, body);
}

Stmt* CSharpTSTreeVisitor::handle_expr_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    if (ts_node_child_count(*node) == 0)
    {
        throw std::logic_error(
            "Invalid node. Node isn't expression statement node"
        );
    }
    const TSNode expr_node    = ts_node_child(*node, 0);
    const ExprHandler handler = NodeRegistry::get_expr_handler(expr_node);
    return stmt_factory_.mk_expr(handler(self, &expr_node));
}

Stmt* CSharpTSTreeVisitor::handle_continue(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] const TSNode* node
)
{
    return stmt_factory_.mk_continue();
}

Stmt* CSharpTSTreeVisitor::handle_break(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] const TSNode* node
)
{
    return stmt_factory_.mk_break();
}

Stmt* CSharpTSTreeVisitor::handle_return(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    Expr* expr = nullptr;
    if (ts_node_child_count(*node) > 2)
    {
        const TSNode expr_node = ts_node_child(*node, 1);
        const ExprHandler expr_handler
            = NodeRegistry::get_expr_handler(expr_node);
        expr = expr_handler(self, &expr_node);
    }
    return stmt_factory_.mk_return(expr);
}

Stmt* CSharpTSTreeVisitor::handle_throw(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode expr_node         = ts_node_child(*node, 1);
    const ExprHandler expr_handler = NodeRegistry::get_expr_handler(expr_node);
    return stmt_factory_.mk_throw(expr_handler(self, &expr_node));
}

} // namespace astfri::csharp
