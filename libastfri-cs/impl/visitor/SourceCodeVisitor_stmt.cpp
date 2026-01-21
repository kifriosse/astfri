#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
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

Stmt* SrcCodeVisitor::visit_block_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    static const TSSymbol func_symb
        = util::symbol_for_name(self->lang_, "local_function_statement", true);

    std::vector<Stmt*> stmts;
    self->semantic_context_.enter_scope();
    auto dicover_func = [self](const TSNode& current) -> void
    {
        if (! ts_node_is_named(current) || ts_node_symbol(current) != func_symb)
            return;

        self->semantic_context_.reg_local_func(
            util::make_func_metadata(current, self->src_str(), self->type_tr_)
        );
    };
    util::for_each_child_node(node, dicover_func);

    auto process_body = [self, &stmts](const TSNode& n_current) -> void
    {
        if (ts_node_is_named(n_current))
        {
            const StmtHandler h_stmt = RegManager::get_stmt_handler(n_current);
            stmts.push_back(h_stmt(self, n_current));
        }
    };
    util::for_each_child_node(node, process_body);

    self->semantic_context_.leave_scope();

    return stmt_f_.mk_compound(std::move(stmts));
}

Stmt* SrcCodeVisitor::visit_arrow_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode n_body      = ts_node_named_child(node, 0);
    const ExprHandler h_body = RegManager::get_expr_handler(n_body);
    Type* ret_type           = self->semantic_context_.current_return_type();
    Expr* expr               = h_body(self, n_body);
    Stmt* body               = nullptr;

    if (is_a<VoidType>(ret_type))
        body = stmt_f_.mk_expr(expr);
    else
        body = stmt_f_.mk_return(expr);

    return stmt_f_.mk_compound({body});
}

Stmt* SrcCodeVisitor::visit_while_loop(SrcCodeVisitor* self, const TSNode& node)
{
    return self->make_while_loop(node, true);
}

Stmt* SrcCodeVisitor::visit_do_while_loop(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    return self->make_while_loop(node, false);
}

Stmt* SrcCodeVisitor::visit_for_loop(SrcCodeVisitor* self, const TSNode& node)
{
    self->semantic_context_.enter_scope();
    const TSNode n_init  = util::child_by_field_name(node, "initializer");
    const TSNode n_cond  = util::child_by_field_name(node, "condition");
    const TSNode n_step  = util::child_by_field_name(node, "update");
    const TSNode n_body  = util::child_by_field_name(node, "body");
    const bool cond_null = ts_node_is_null(n_cond);
    const bool step_null = ts_node_is_null(n_step);
    Stmt* init           = nullptr;
    Expr* cond           = nullptr;
    Stmt* step           = nullptr;

    if (! ts_node_is_null(n_init))
    {
        if (RegManager::is_expr(n_init))
        {
            const TSNode* n_end = &n_body;
            if (! cond_null)
                n_end = &n_cond;
            else if (! step_null)
                n_end = &n_step;

            init = stmt_f_.mk_expr(self->expr_list_to_comma_op(n_init, n_end));
        }
        else
        {
            init = self->visit_for_init_var_def(n_init);
        }
    }

    if (! cond_null)
    {
        const ExprHandler h_cond = RegManager::get_expr_handler(n_cond);
        cond                     = h_cond(self, n_cond);
    }

    if (! step_null)
    {
        step = stmt_f_.mk_expr(self->expr_list_to_comma_op(n_step, &n_body));
    }

    const StmtHandler h_body = RegManager::get_stmt_handler(n_body);
    Stmt* body               = h_body(self, n_body);
    self->semantic_context_.leave_scope();
    return stmt_f_.mk_for(init, cond, step, body);
}

Stmt* SrcCodeVisitor::visit_for_each_loop(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    self->semantic_context_.enter_scope();
    const TSNode n_type  = util::child_by_field_name(node, "type");
    const TSNode n_left  = util::child_by_field_name(node, "left");
    const TSNode n_right = util::child_by_field_name(node, "right");
    const TSNode n_body  = util::child_by_field_name(node, "body");

    if (ts_node_is_null(n_type))
    {
        // todo handle deconstruction syntax
        throw std::logic_error(
            "Foreach loop with deconstruction syntax is not supported yet"
        );
    }

    std::string name          = util::extract_text(n_left, self->src_str());
    const ExprHandler h_right = RegManager::get_expr_handler(n_right);
    const StmtHandler h_body  = RegManager::get_stmt_handler(n_body);
    const TypeHandler th      = RegManager::get_type_handler(n_type);
    Type* type                = th(&self->type_tr_, n_type);
    LocalVarDefStmt* left
        = stmt_f_.mk_local_var_def(std::move(name), type, nullptr);
    self->semantic_context_.reg_local_var(left);
    Expr* right = h_right(self, n_right);
    Stmt* body  = h_body(self, n_body);
    self->semantic_context_.leave_scope();
    return stmt_f_.mk_for_each(left, right, body);
}

Stmt* SrcCodeVisitor::visit_if_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    static constexpr std::string_view if_false = "alternative";

    std::stack<TSNode> if_nodes;
    if_nodes.push(node);
    TSNode n_current       = util::child_by_field_name(node, if_false);
    const TSSymbol if_symb = ts_node_symbol(node);
    TSSymbol current_symb
        = ts_node_is_null(n_current) ? 0 : ts_node_symbol(n_current);
    while (current_symb == if_symb)
    {
        if_nodes.push(n_current);
        n_current = util::child_by_field_name(n_current, if_false);
        if (ts_node_is_null(n_current))
            break;

        current_symb = ts_node_symbol(n_current);
    }

    // handling of else in last node
    const TSNode n_else = util::child_by_field_name(if_nodes.top(), if_false);

    Stmt* current_else  = nullptr;
    if (! ts_node_is_null(n_else))
    {
        const StmtHandler h_else = RegManager::get_stmt_handler(n_else);
        current_else             = h_else(self, n_else);
    }

    while (! if_nodes.empty())
    {
        const TSNode if_node = if_nodes.top();
        if_nodes.pop();
        const TSNode n_true = util::child_by_field_name(if_node, "consequence");
        const TSNode n_cond = util::child_by_field_name(if_node, "condition");
        StmtHandler h_true  = RegManager::get_stmt_handler(n_true);
        ExprHandler h_cond  = RegManager::get_expr_handler(n_cond);

        Stmt* true_stmt     = h_true(self, n_true);
        Expr* cond_expr     = h_cond(self, n_cond);
        IfStmt* if_stmt     = stmt_f_.mk_if(cond_expr, true_stmt, current_else);
        current_else        = if_stmt;
    }

    return current_else;
}

Stmt* SrcCodeVisitor::visit_try_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    if (ts_node_child_count(node) < 2)
        throw std::logic_error("Not a try-catch node");

    const TSNode n_body      = util::child_by_field_name(node, "body");
    const StmtHandler h_body = RegManager::get_stmt_handler(n_body);

    Stmt* finally            = nullptr;
    std::vector<CatchStmt*> catch_stmts;

    auto process = [&](const TSNode& current) -> void
    {
        if (ts_node_eq(current, n_body))
            return;

        const StmtHandler h_current = RegManager::get_stmt_handler(current);
        Stmt* current_stmt          = h_current(self, current);
        if (is_a<CompoundStmt>(current_stmt))
            finally = current_stmt;
        else if (auto* catch_stmt = as_a<CatchStmt>(current_stmt))
            catch_stmts.push_back(catch_stmt);
    };

    util::for_each_child_node(node, process);

    self->semantic_context_.leave_scope();

    return stmt_f_
        .mk_try(h_body(self, n_body), finally, std::move(catch_stmts));
}

Stmt* SrcCodeVisitor::visit_catch_clause(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    self->semantic_context_.enter_scope();

    LocalVarDefStmt* catch_var = nullptr;
    Stmt* body                 = nullptr;
    auto process               = [&](const TSNode& n_current) -> void
    {
        const StmtHandler h_current = RegManager::get_stmt_handler(n_current);
        Stmt* current_stmt          = h_current(self, n_current);
        if (const auto var = as_a<LocalVarDefStmt>(current_stmt))
        {
            catch_var = var;
            self->semantic_context_.reg_local_var(catch_var);
        }
        else if (is_a<CompoundStmt>(current_stmt))
        {
            body = current_stmt;
        }
    };
    util::for_each_child_node(node, process);

    self->semantic_context_.leave_scope();
    return stmt_f_.mk_catch(catch_var, body);
}

Stmt* SrcCodeVisitor::visit_finally(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode n_body = ts_node_named_child(node, 0);
    return RegManager::get_stmt_handler(n_body)(self, n_body);
}

Stmt* SrcCodeVisitor::visit_catch_decl(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode n_type  = util::child_by_field_name(node, "type");
    const TSNode n_name  = util::child_by_field_name(node, "name");
    const TypeHandler th = RegManager::get_type_handler(n_type);
    Type* type           = th(&self->type_tr_, n_type);
    std::string name     = ts_node_is_null(n_name)
                             ? std::string{}
                             : util::extract_text(n_name, self->src_str());
    return stmt_f_.mk_local_var_def(std::move(name), type, nullptr);
}

Stmt* SrcCodeVisitor::visit_switch_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode n_value       = util::child_by_field_name(node, "value");
    const TSNode n_switch_body = util::child_by_field_name(node, "body");
    const ExprHandler h_value  = RegManager::get_expr_handler(n_value);
    Expr* value                = h_value(self, n_value);

    std::vector<CaseBaseStmt*> cases;
    auto process = [self, &cases](const TSNode& n_current) -> void
    {
        const StmtHandler h_stmt = RegManager::get_stmt_handler(n_current);
        Stmt* stmt               = h_stmt(self, n_current);
        if (auto* case_stmt = as_a<CaseBaseStmt>(stmt))
            cases.push_back(case_stmt);
    };
    util::for_each_child_node(n_switch_body, process);
    return stmt_f_.mk_switch(value, std::move(cases));
}

Stmt* SrcCodeVisitor::visit_case_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    TSTreeCursor cursor = ts_tree_cursor_new(node);
    if (ts_tree_cursor_goto_first_child(&cursor))
    {
        std::vector<Stmt*> body_stmts;
        Expr* pattern    = nullptr;
        TSNode n_current = {};
        do
        {
            n_current = ts_tree_cursor_current_node(&cursor);
            if (std::strcmp(ts_node_type(n_current), ":") == 0)
                break;
            if (! ts_node_is_named(n_current))
                continue;

            // todo handle more complex patterns
            const ExprHandler h_pattern
                = RegManager::get_expr_handler(n_current);
            pattern = h_pattern(self, n_current);
        } while (ts_tree_cursor_goto_next_sibling(&cursor));

        while (ts_tree_cursor_goto_next_sibling(&cursor))
        {
            n_current = ts_tree_cursor_current_node(&cursor);
            if (! ts_node_is_named(n_current))
                continue;

            StmtHandler h_stmt = RegManager::get_stmt_handler(n_current);
            Stmt* stmt         = h_stmt(self, n_current);
            body_stmts.push_back(stmt);
        }

        ts_tree_cursor_delete(&cursor);

        Stmt* body
            = body_stmts.size() == 1 && is_a<CompoundStmt>(body_stmts.back())
                ? body_stmts.back()
                : stmt_f_.mk_compound(std::move(body_stmts));

        if (pattern)
            return stmt_f_.mk_case(pattern, body);

        return stmt_f_.mk_default_case(body);
    }

    ts_tree_cursor_delete(&cursor);
    return stmt_f_.mk_uknown();
}

Stmt* SrcCodeVisitor::visit_expr_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode n_expr      = ts_node_child(node, 0);
    const ExprHandler h_expr = RegManager::get_expr_handler(n_expr);
    return stmt_f_.mk_expr(h_expr(self, n_expr));
}

Stmt* SrcCodeVisitor::visit_continue(
    [[maybe_unused]] SrcCodeVisitor* self,
    [[maybe_unused]] const TSNode& node
)
{
    return stmt_f_.mk_continue();
}

Stmt* SrcCodeVisitor::visit_break(
    [[maybe_unused]] SrcCodeVisitor* self,
    [[maybe_unused]] const TSNode& node
)
{
    return stmt_f_.mk_break();
}

Stmt* SrcCodeVisitor::visit_return(SrcCodeVisitor* self, const TSNode& node)
{
    Expr* expr = nullptr;
    if (ts_node_named_child_count(node) > 0)
    {
        const TSNode n_expr      = ts_node_named_child(node, 0);
        const ExprHandler h_expr = RegManager::get_expr_handler(n_expr);
        expr                     = h_expr(self, n_expr);
    }
    return stmt_f_.mk_return(expr);
}

Stmt* SrcCodeVisitor::visit_throw(SrcCodeVisitor* self, const TSNode& node)
{
    Expr* expr = nullptr;
    if (ts_node_named_child_count(node) > 0)
    {
        const TSNode n_expr      = ts_node_named_child(node, 0);
        const ExprHandler h_expr = RegManager::get_expr_handler(n_expr);
        expr                     = h_expr(self, n_expr);
    }
    return stmt_f_.mk_throw(expr);
}

} // namespace astfri::csharp
