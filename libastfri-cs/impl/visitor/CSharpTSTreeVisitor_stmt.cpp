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
    {
        return stmt_factory_.mk_compound({});
    }

    do
    {
        TSNode current_node = ts_tree_cursor_current_node(&cursor);
        StmtHandler handler = NodeRegistry::get_stmt_handler(current_node);
        statements.push_back(handler(self, &current_node));
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
    const TSNode expr_node         = ts_node_child(*node, 0);
    const ExprHandler expr_handler = NodeRegistry::get_expr_handler(expr_node);
    return stmt_factory_.mk_return(expr_handler(self, &expr_node));
}

Stmt* CSharpTSTreeVisitor::handle_throw(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode expr_node         = ts_node_child(*node, 0);
    const ExprHandler expr_handler = NodeRegistry::get_expr_handler(expr_node);
    return stmt_factory_.mk_throw(expr_handler(self, &expr_node));
}

} // namespace astfri::csharp
