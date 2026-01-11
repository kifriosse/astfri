#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/util/astfri_util.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri-cs/impl/visitor/SrcCodeVisitor.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <cstring>
#include <stack>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace astfri::csharp
{

Stmt* SrcCodeVisitor::handle_block_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    std::vector<Stmt*> statements;
    static constexpr std::string_view local_func_type
        = "local_function_statement";
    static const TSSymbol local_func_symb = ts_language_symbol_for_name(
        self->get_lang(),
        local_func_type.data(),
        local_func_type.size(),
        true
    );

    if (ts_node_named_child_count(*node) != 0)
    {
        TSTreeCursor cursor = ts_tree_cursor_new(*node);
        ts_tree_cursor_goto_first_child(&cursor);
        self->semantic_context_.enter_scope();
        do
        {
            TSNode current_node = ts_tree_cursor_current_node(&cursor);
            if (! ts_node_is_named(current_node)
                || ts_node_symbol(current_node) != local_func_symb)
                continue;

            self->semantic_context_.reg_local_func(
                util::make_func_metadata(current_node, self->get_src_code())
            );
        } while (ts_tree_cursor_goto_next_sibling(&cursor));

        ts_tree_cursor_reset(&cursor, *node);

        ts_tree_cursor_goto_first_child(&cursor);
        do
        {
            TSNode current_node = ts_tree_cursor_current_node(&cursor);
            if (ts_node_is_named(current_node))
            {
                StmtHandler handler
                    = RegManager::get_stmt_handler(current_node);
                statements.push_back(handler(self, &current_node));
            }
        } while (ts_tree_cursor_goto_next_sibling(&cursor));
        self->semantic_context_.leave_scope();
        ts_tree_cursor_delete(&cursor);
    }

    return stmt_factory_.mk_compound(statements);
}

Stmt* SrcCodeVisitor::handle_arrow_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode body_node         = ts_node_child(*node, 1);
    const ExprHandler body_handler = RegManager::get_expr_handler(body_node);

    Type* return_type = self->semantic_context_.current_return_type();
    Expr* body_expr   = body_handler(self, &body_node);
    Stmt* body_stmt   = nullptr;
    if (is_a<VoidType>(return_type) || ! return_type)
        body_stmt = stmt_factory_.mk_expr(body_expr);
    else
        body_stmt = stmt_factory_.mk_return(body_expr);

    return stmt_factory_.mk_compound({body_stmt});
}

Stmt* SrcCodeVisitor::handle_while_loop(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    return self->make_while_loop(node, false);
}

Stmt* SrcCodeVisitor::handle_do_while_loop(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    return self->make_while_loop(node, true);
}

Stmt* SrcCodeVisitor::handle_for_loop(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    self->semantic_context_.enter_scope();
    Stmt* init                = nullptr;
    Expr* condition           = nullptr;
    Stmt* updater             = nullptr;
    const TSNode init_node    = util::child_by_field_name(*node, "initializer");
    const TSNode cond_node    = util::child_by_field_name(*node, "condition");
    const TSNode updater_node = util::child_by_field_name(*node, "update");
    const TSNode body_node    = util::child_by_field_name(*node, "body");
    const bool cond_null      = ts_node_is_null(cond_node);
    const bool updater_null   = ts_node_is_null(updater_node);

    if (! ts_node_is_null(init_node))
    {
        if (RegManager::is_expr(init_node))
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
            = RegManager::get_expr_handler(cond_node);
        condition = cond_handler(self, &cond_node);
    }

    if (! updater_null)
    {
        updater = stmt_factory_.mk_expr(
            self->expr_list_to_comma_op(updater_node, &body_node)
        );
    }

    const StmtHandler body_handler = RegManager::get_stmt_handler(body_node);
    Stmt* body                     = body_handler(self, &body_node);
    self->semantic_context_.leave_scope();
    return stmt_factory_.mk_for(init, condition, updater, body);
}

Stmt* SrcCodeVisitor::handle_for_each_loop(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    self->semantic_context_.enter_scope();
    const TSNode type_node  = util::child_by_field_name(*node, "type");
    const TSNode left_node  = util::child_by_field_name(*node, "left");
    const TSNode right_node = util::child_by_field_name(*node, "right");
    const TSNode body_node  = util::child_by_field_name(*node, "body");

    if (ts_node_is_null(type_node))
    {
        // todo handle deconstruction syntax
        throw std::logic_error(
            "Foreach loop with deconstruction syntax is not supported yet"
        );
    }

    const std::string var_name
        = util::extract_node_text(left_node, self->get_src_code());
    const ExprHandler right_handler = RegManager::get_expr_handler(right_node);
    const StmtHandler body_handler  = RegManager::get_stmt_handler(body_node);
    Type* type = util::make_type(type_node, self->get_src_code());
    LocalVarDefStmt* left
        = stmt_factory_.mk_local_var_def(var_name, type, nullptr);
    self->semantic_context_.reg_local_var(left);
    Expr* right = right_handler(self, &right_node);
    Stmt* body  = body_handler(self, &body_node);
    self->semantic_context_.leave_scope();
    return stmt_factory_.mk_for_each(left, right, body);
}

Stmt* SrcCodeVisitor::handle_if_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    // static const std::string if_node_type = "if_statement";
    static constexpr std::string_view if_false  = "alternative";
    static constexpr std::string_view if_true   = "consequence";
    static constexpr std::string_view condition = "condition";

    std::stack<TSNode> if_nodes;
    if_nodes.push(*node);
    TSNode current_node      = util::child_by_field_name(*node, if_false);

    const uint32_t if_symbol = ts_node_symbol(*node);
    TSSymbol current_symbol
        = ts_node_is_null(current_node) ? 0 : ts_node_symbol(current_node);
    while (current_symbol == if_symbol)
    {
        if_nodes.push(current_node);
        current_node = util::child_by_field_name(current_node, if_false);
        if (ts_node_is_null(current_node))
            break;

        current_symbol = ts_node_symbol(current_node);
    }

    // handling of else in last node
    const TSNode else_node
        = util::child_by_field_name(if_nodes.top(), if_false);

    Stmt* current_else = nullptr;
    if (! ts_node_is_null(else_node))
    {
        const StmtHandler else_handler
            = RegManager::get_stmt_handler(else_node);
        current_else = else_handler(self, &else_node);
    }

    while (! if_nodes.empty())
    {
        const TSNode if_node = if_nodes.top();
        if_nodes.pop();
        const TSNode if_true_node = util::child_by_field_name(if_node, if_true);
        const TSNode cond_node = util::child_by_field_name(if_node, condition);
        StmtHandler if_true_handler
            = RegManager::get_stmt_handler(if_true_node);
        ExprHandler cond_handler = RegManager::get_expr_handler(cond_node);

        Stmt* if_true_stmt       = if_true_handler(self, &if_true_node);
        Expr* cond_expr          = cond_handler(self, &cond_node);
        IfStmt* if_stmt
            = stmt_factory_.mk_if(cond_expr, if_true_stmt, current_else);
        current_else = if_stmt;
    }

    return current_else;
}

Stmt* SrcCodeVisitor::handle_try_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    if (ts_node_child_count(*node) < 2)
        throw std::logic_error("Not a try-catch node");

    const TSNode body_node         = util::child_by_field_name(*node, "body");
    const StmtHandler body_handler = RegManager::get_stmt_handler(body_node);
    ts_tree_cursor_goto_descendant(&cursor, 2);

    Stmt* finally_stmt = nullptr;
    std::vector<CatchStmt*> catch_stmts;
    do
    {
        TSNode current              = ts_tree_cursor_current_node(&cursor);
        StmtHandler current_handler = RegManager::get_stmt_handler(current);
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

Stmt* SrcCodeVisitor::handle_catch_clause(
    SrcCodeVisitor* self,
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
            = RegManager::get_stmt_handler(current_node);
        Stmt* current_stmt = current_handler(self, &current_node);
        if (const auto var_def = as_a<LocalVarDefStmt>(current_stmt))
        {
            expr_var_def = var_def;
            self->semantic_context_.reg_local_var(expr_var_def);
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

Stmt* SrcCodeVisitor::handle_finally(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode body_node = ts_node_child(*node, 1);
    return RegManager::get_stmt_handler(body_node)(self, &body_node);
}

Stmt* SrcCodeVisitor::handle_catch_decl(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode type_node = util::child_by_field_name(*node, "type");
    const TSNode name_node = util::child_by_field_name(*node, "name");
    Type* type             = util::make_type(type_node, self->get_src_code());
    const std::string name = ts_node_is_null(name_node)
        ? std::string{}
        : util::extract_node_text(name_node, self->get_src_code());
    return stmt_factory_.mk_local_var_def(name, type, nullptr);
}

Stmt* SrcCodeVisitor::handle_switch_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode value_node       = util::child_by_field_name(*node, "value");
    const TSNode switch_body      = util::child_by_field_name(*node, "body");
    const ExprHandler val_handler = RegManager::get_expr_handler(value_node);
    Expr* value                   = val_handler(self, &value_node);

    std::vector<CaseBaseStmt*> cases;
    TSTreeCursor cursor = ts_tree_cursor_new(switch_body);
    if (ts_tree_cursor_goto_first_child(&cursor))
    {
        do
        {
            TSNode current = ts_tree_cursor_current_node(&cursor);
            if (! ts_node_is_named(current))
                continue;

            StmtHandler handler = RegManager::get_stmt_handler(current);
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

Stmt* SrcCodeVisitor::handle_case_stmt(
    SrcCodeVisitor* self,
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
        if (std::strcmp(ts_node_type(current), ":") == 0)
            break;
        // todo handle more complex patterns
        const ExprHandler pattern_handler
            = RegManager::get_expr_handler(current);
        pattern = pattern_handler(self, &current);
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    while (ts_tree_cursor_goto_next_sibling(&cursor))
    {
        current = ts_tree_cursor_current_node(&cursor);
        if (! ts_node_is_named(current))
            continue;

        StmtHandler handler = RegManager::get_stmt_handler(current);
        Stmt* stmt          = handler(self, &current);
        body_stmts.push_back(stmt);
    }

    Stmt* body = body_stmts.size() == 1 && is_a<CompoundStmt>(body_stmts.back())
                   ? body_stmts.back()
                   : stmt_factory_.mk_compound(body_stmts);

    if (pattern)
        return stmt_factory_.mk_case(pattern, body);

    ts_tree_cursor_delete(&cursor);
    return stmt_factory_.mk_default_case(body);
}

Stmt* SrcCodeVisitor::handle_expr_stmt(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode expr_node    = ts_node_child(*node, 0);
    const ExprHandler handler = RegManager::get_expr_handler(expr_node);
    return stmt_factory_.mk_expr(handler(self, &expr_node));
}

Stmt* SrcCodeVisitor::handle_continue(
    [[maybe_unused]] SrcCodeVisitor* self,
    [[maybe_unused]] const TSNode* node
)
{
    return stmt_factory_.mk_continue();
}

Stmt* SrcCodeVisitor::handle_break(
    [[maybe_unused]] SrcCodeVisitor* self,
    [[maybe_unused]] const TSNode* node
)
{
    return stmt_factory_.mk_break();
}

Stmt* SrcCodeVisitor::handle_return(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    Expr* expr = nullptr;
    if (ts_node_child_count(*node) > 2)
    {
        const TSNode expr_node = ts_node_child(*node, 1);
        const ExprHandler expr_handler
            = RegManager::get_expr_handler(expr_node);
        expr = expr_handler(self, &expr_node);
    }
    return stmt_factory_.mk_return(expr);
}

Stmt* SrcCodeVisitor::handle_throw(
    SrcCodeVisitor* self,
    const TSNode* node
)
{
    if (ts_node_child_count(*node) <= 2)
        return stmt_factory_.mk_throw(nullptr);

    const TSNode expr_node         = ts_node_child(*node, 1);
    const ExprHandler expr_handler = RegManager::get_expr_handler(expr_node);
    return stmt_factory_.mk_throw(expr_handler(self, &expr_node));
}

} // namespace astfri::csharp
