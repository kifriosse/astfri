#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/visitor/CSharpTSTreeVisitor.hpp>

#include <cstring>
#include <iostream>

namespace astfri::csharp
{

Stmt* CSharpTSTreeVisitor::handle_block_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    self->semantic_context_.enter_scope();
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

    ts_tree_cursor_delete(&cursor);
    self->semantic_context_.leave_scope();
    return stmt_factory_.mk_compound(statements);
}

Stmt* CSharpTSTreeVisitor::handle_arrow_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode body_node         = ts_node_child(*node, 1);
    const ExprHandler body_handler = NodeRegistry::get_expr_handler(body_node);

    Type* return_type = self->semantic_context_.current_return_type();
    Expr* body_expr   = body_handler(self, &body_node);
    Stmt* body_stmt   = nullptr;
    if (is_a<VoidType>(return_type) || ! return_type)
        body_stmt = stmt_factory_.mk_expr(body_expr);
    else
        body_stmt = stmt_factory_.mk_return(body_expr);

    return stmt_factory_.mk_compound({body_stmt});
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
    self->semantic_context_.enter_scope();
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
        if (NodeRegistry::is_expr(init_node))
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
        else
        {
            init = handle_for_init_var_def(self, &init_node);
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
    self->semantic_context_.leave_scope();
    return stmt_factory_.mk_for(init, condition, updater, body);
}

Stmt* CSharpTSTreeVisitor::handle_for_each_loop(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    self->semantic_context_.enter_scope();
    const TSNode type_node  = ts_node_child_by_field_name(*node, "type", 4);
    const TSNode left_node  = ts_node_child_by_field_name(*node, "left", 4);
    const TSNode right_node = ts_node_child_by_field_name(*node, "right", 5);
    const TSNode body_node  = ts_node_child_by_field_name(*node, "body", 4);

    if (ts_node_is_null(type_node))
    {
        // todo handle deconstruction syntax
        throw std::logic_error(
            "Foreach loop with deconstruction syntax is not supported yet"
        );
    }

    const std::string var_name
        = extract_node_text(left_node, self->source_code_);
    const ExprHandler right_handler
        = NodeRegistry::get_expr_handler(right_node);
    const StmtHandler body_handler = NodeRegistry::get_stmt_handler(body_node);
    Type* type                     = make_type(self, type_node);
    LocalVarDefStmt* left
        = stmt_factory_.mk_local_var_def(var_name, type, nullptr);
    self->semantic_context_.add_local_var(left);
    Expr* right = right_handler(self, &right_node);
    Stmt* body  = body_handler(self, &body_node);
    self->semantic_context_.leave_scope();
    return stmt_factory_.mk_for_each(left, right, body);
}

Stmt* CSharpTSTreeVisitor::handle_if_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    static const std::string if_node_type = "if_statement";
    static const std::string if_false     = "alternative";
    std::stack<TSNode> if_nodes;
    if_nodes.push(*node);
    TSNode current_node = ts_node_child_by_field_name(
        *node,
        if_false.c_str(),
        if_false.length()
    );

    const uint32_t if_symbol = ts_node_symbol(*node);
    TSSymbol current_symbol
        = ts_node_is_null(current_node) ? 0 : ts_node_symbol(current_node);
    while (current_symbol == if_symbol)
    {
        if_nodes.push(current_node);
        current_node = ts_node_child_by_field_name(
            current_node,
            if_false.c_str(),
            if_false.length()
        );
        if (ts_node_is_null(current_node))
            break;

        current_symbol = ts_node_symbol(current_node);
    }

    // handling of else in last node
    static const std::string if_true   = "consequence";
    static const std::string condition = "condition";

    const TSNode else_node             = ts_node_child_by_field_name(
        if_nodes.top(),
        if_false.c_str(),
        if_false.length()
    );

    Stmt* current_else = nullptr;
    if (! ts_node_is_null(else_node))
    {
        const StmtHandler else_handler
            = NodeRegistry::get_stmt_handler(else_node);
        current_else = else_handler(self, &else_node);
    }

    while (! if_nodes.empty())
    {
        const TSNode if_node = if_nodes.top();
        if_nodes.pop();
        const TSNode if_true_node = ts_node_child_by_field_name(
            if_node,
            if_true.c_str(),
            if_true.length()
        );
        const TSNode cond_node = ts_node_child_by_field_name(
            if_node,
            condition.c_str(),
            condition.length()
        );
        StmtHandler if_true_handler
            = NodeRegistry::get_stmt_handler(if_true_node);
        ExprHandler cond_handler = NodeRegistry::get_expr_handler(cond_node);

        Stmt* if_true_stmt       = if_true_handler(self, &if_true_node);
        Expr* cond_expr          = cond_handler(self, &cond_node);
        IfStmt* if_stmt
            = stmt_factory_.mk_if(cond_expr, if_true_stmt, current_else);
        current_else = if_stmt;
    }

    return current_else;
}

Stmt* CSharpTSTreeVisitor::handle_try_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{

    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    if (ts_node_child_count(*node) < 2)
        throw std::logic_error("Not a try-catch node");

    const TSNode body_node = ts_node_child_by_field_name(*node, "body", 4);
    const StmtHandler body_handler = NodeRegistry::get_stmt_handler(body_node);
    ts_tree_cursor_goto_descendant(&cursor, 2);

    Stmt* finally_stmt = nullptr;
    std::vector<CatchStmt*> catch_stmts;
    do
    {
        TSNode current              = ts_tree_cursor_current_node(&cursor);
        StmtHandler current_handler = NodeRegistry::get_stmt_handler(current);
        Stmt* current_stmt          = current_handler(self, &current);
        if (is_a<CompoundStmt>(current_stmt))
            finally_stmt = current_stmt;
        else if (auto* catch_stmt = as_a<CatchStmt>(current_stmt))
            catch_stmts.push_back(catch_stmt);
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    self->semantic_context_.leave_scope();
    ts_tree_cursor_delete(&cursor);

    return stmt_factory_
        .mk_try(body_handler(self, &body_node), finally_stmt, catch_stmts);
}

Stmt* CSharpTSTreeVisitor::handle_catch_clause(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    self->semantic_context_.enter_scope();
    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    ts_tree_cursor_goto_first_child(&cursor);

    LocalVarDefStmt* expr_var_def = nullptr;
    Stmt* body                    = nullptr;
    do
    {
        TSNode current_node = ts_tree_cursor_current_node(&cursor);
        StmtHandler current_handler
            = NodeRegistry::get_stmt_handler(current_node);
        Stmt* current_stmt = current_handler(self, &current_node);
        if (const auto expr = as_a<LocalVarDefStmt>(current_stmt))
        {
            expr_var_def = expr;
            self->semantic_context_.add_local_var(expr_var_def);
        }
        else if (is_a<CompoundStmt>(current_stmt))
        {
            body = current_stmt;
        }
        // todo add exception filter
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    ts_tree_cursor_delete(&cursor);

    self->semantic_context_.leave_scope();
    return stmt_factory_.mk_catch(expr_var_def, body);
}

Stmt* CSharpTSTreeVisitor::handle_finally_clause(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode body_node = ts_node_child(*node, 1);
    return NodeRegistry::get_stmt_handler(body_node)(self, &body_node);
}

Stmt* CSharpTSTreeVisitor::handle_catch_decl(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode type_node = ts_node_child_by_field_name(*node, "type", 4);
    const TSNode name_node = ts_node_child_by_field_name(*node, "name", 4);
    Type* type             = make_type(self, type_node);
    const std::string name = extract_node_text(name_node, self->source_code_);
    return stmt_factory_.mk_local_var_def(name, type, nullptr);
}

Stmt* CSharpTSTreeVisitor::handle_switch_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode value_node  = ts_node_child_by_field_name(*node, "value", 5);
    const TSNode switch_body = ts_node_child_by_field_name(*node, "body", 4);
    const ExprHandler val_handler = NodeRegistry::get_expr_handler(value_node);
    Expr* value                   = val_handler(self, &value_node);

    std::vector<CaseBaseStmt*> cases;
    TSTreeCursor cursor = ts_tree_cursor_new(switch_body);
    if (ts_tree_cursor_goto_first_child(&cursor))
    {
        do
        {
            TSNode current = ts_tree_cursor_current_node(&cursor);
            if (NodeRegistry::is_structural_node(ts_node_type(current)))
                continue;

            StmtHandler handler = NodeRegistry::get_stmt_handler(current);
            Stmt* stmt          = handler(self, &current);
            if (auto* case_stmt = as_a<CaseBaseStmt>(stmt))
            {
                cases.push_back(case_stmt);
            }
        } while (ts_tree_cursor_goto_next_sibling(&cursor));
    }

    ts_tree_cursor_delete(&cursor);

    return stmt_factory_.mk_switch(value, cases);
}

Stmt* CSharpTSTreeVisitor::handle_case_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    std::vector<Stmt*> body_stmts;
    Expr* pattern       = nullptr;
    TSNode current      = {};
    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    if (! ts_tree_cursor_goto_first_child(&cursor))
        throw std::logic_error("Invalid case node");

    do
    {
        current = ts_tree_cursor_current_node(&cursor);
        if (strcmp(ts_node_type(current), ":") == 0)
            break;
        // todo handle more complex patterns
        const ExprHandler pattern_handler
            = NodeRegistry::get_expr_handler(current);
        pattern = pattern_handler(self, &current);
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    while (ts_tree_cursor_goto_next_sibling(&cursor))
    {
        current = ts_tree_cursor_current_node(&cursor);
        if (! ts_node_is_named(current))
            continue;

        StmtHandler handler = NodeRegistry::get_stmt_handler(current);
        Stmt* stmt          = handler(self, &current);
        body_stmts.push_back(stmt);
    }

    Stmt* body = body_stmts.size() == 1 && is_a<CompoundStmt>(body_stmts.back())
                   ? body_stmts.back()
                   : stmt_factory_.mk_compound(body_stmts);

    if (pattern)
        return stmt_factory_.mk_case(pattern, body);

    return stmt_factory_.mk_default_case(body);
}

Stmt* CSharpTSTreeVisitor::handle_expr_stmt(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
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
    if (ts_node_child_count(*node) <= 2)
        return stmt_factory_.mk_throw(nullptr);

    const TSNode expr_node         = ts_node_child(*node, 1);
    const ExprHandler expr_handler = NodeRegistry::get_expr_handler(expr_node);
    return stmt_factory_.mk_throw(expr_handler(self, &expr_node));
}

} // namespace astfri::csharp
