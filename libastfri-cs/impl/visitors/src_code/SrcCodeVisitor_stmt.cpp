#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/visitors/src_code/SrcCodeVisitor.hpp>
#include <libastfri-cs/impl/util/AstfriUtil.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
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
    static const TSSymbol sFunc
        = util::symbol_for_name("local_function_statement", true);

    self->semanticContext_.enter_scope();
    auto discoverFunc = [self](const TSNode& current) -> void
    {
        if (! ts_node_is_named(current) || ts_node_symbol(current) != sFunc)
            return;

        self->semanticContext_.reg_local_func(
            util::make_func_metadata(current, self->src_str(), self->typeTrs_)
        );
    };
    util::for_each_child_node(node, discoverFunc);

    std::vector<Stmt*> stmts;
    auto processBody = [self, &stmts](const TSNode& nCurrent) -> void
    {
        if (ts_node_is_named(nCurrent))
        {
            const StmtHandler hStmt = RegManager::get_stmt_handler(nCurrent);
            stmts.push_back(hStmt(self, nCurrent));
        }
    };
    util::for_each_child_node(node, processBody);

    self->semanticContext_.leave_scope();

    return stmtFact_.mk_compound(std::move(stmts));
}

Stmt* SrcCodeVisitor::visit_arrow_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode nBody      = ts_node_named_child(node, 0);
    const ExprHandler hBody = RegManager::get_expr_handler(nBody);
    Type* returnType        = self->semanticContext_.current_return_type();
    Expr* expr              = hBody(self, nBody);
    Stmt* body              = nullptr;

    if (is_a<VoidType>(returnType))
        body = stmtFact_.mk_expr(expr);
    else
        body = stmtFact_.mk_return(expr);

    return stmtFact_.mk_compound({body});
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
    self->semanticContext_.enter_scope();
    const TSNode nInit  = util::child_by_field_name(node, "initializer");
    const TSNode nCond  = util::child_by_field_name(node, "condition");
    const TSNode nStep  = util::child_by_field_name(node, "update");
    const TSNode nBody  = util::child_by_field_name(node, "body");
    const bool condNull = ts_node_is_null(nCond);
    const bool stepNull = ts_node_is_null(nStep);
    Stmt* init          = nullptr;
    Expr* cond          = nullptr;
    Stmt* step          = nullptr;

    if (! ts_node_is_null(nInit))
    {
        if (RegManager::is_expr(nInit))
        {
            const TSNode* nEnd = &nBody;
            if (! condNull)
                nEnd = &nCond;
            else if (! stepNull)
                nEnd = &nStep;

            init = stmtFact_.mk_expr(self->expr_list_to_comma_op(nInit, nEnd));
        }
        else
        {
            init = self->visit_for_init_var_def(nInit);
        }
    }

    if (! condNull)
    {
        const ExprHandler hCond = RegManager::get_expr_handler(nCond);
        cond                    = hCond(self, nCond);
    }

    if (! stepNull)
    {
        step = stmtFact_.mk_expr(self->expr_list_to_comma_op(nStep, &nBody));
    }

    const StmtHandler hBody = RegManager::get_stmt_handler(nBody);
    Stmt* body              = hBody(self, nBody);
    self->semanticContext_.leave_scope();
    return stmtFact_.mk_for(init, cond, step, body);
}

Stmt* SrcCodeVisitor::visit_for_each_loop(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    self->semanticContext_.enter_scope();
    const TSNode nType  = util::child_by_field_name(node, "type");
    const TSNode hLeft  = util::child_by_field_name(node, "left");
    const TSNode nRight = util::child_by_field_name(node, "right");
    const TSNode nBody  = util::child_by_field_name(node, "body");

    if (ts_node_is_null(nType))
    {
        // todo handle deconstruction syntax
        throw std::logic_error(
            "Foreach loop with deconstruction syntax is not supported yet"
        );
    }

    std::string name         = util::extract_text(hLeft, self->src_str());
    const ExprHandler hRight = RegManager::get_expr_handler(nRight);
    const StmtHandler hBody  = RegManager::get_stmt_handler(nBody);
    const TypeHandler th     = RegManager::get_type_handler(nType);
    Type* type               = th(&self->typeTrs_, nType);
    LocalVarDefStmt* left
        = stmtFact_.mk_local_var_def(std::move(name), type, nullptr);
    self->semanticContext_.reg_local_var(left);
    Expr* right = hRight(self, nRight);
    Stmt* body  = hBody(self, nBody);
    self->semanticContext_.leave_scope();
    return stmtFact_.mk_for_each(left, right, body);
}

Stmt* SrcCodeVisitor::visit_if_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    static constexpr std::string_view falseSw = "alternative";

    std::stack<TSNode> nIfs;
    nIfs.push(node);
    TSNode nCurrent    = util::child_by_field_name(node, falseSw);
    const TSSymbol sIf = ts_node_symbol(node);
    TSSymbol sCurrent
        = ts_node_is_null(nCurrent) ? 0 : ts_node_symbol(nCurrent);
    while (sCurrent == sIf)
    {
        nIfs.push(nCurrent);
        nCurrent = util::child_by_field_name(nCurrent, falseSw);
        if (ts_node_is_null(nCurrent))
            break;

        sCurrent = ts_node_symbol(nCurrent);
    }

    // handling of else in last node
    const TSNode nElse = util::child_by_field_name(nIfs.top(), falseSw);

    Stmt* currentElse  = nullptr;
    if (! ts_node_is_null(nElse))
    {
        const StmtHandler hElse = RegManager::get_stmt_handler(nElse);
        currentElse             = hElse(self, nElse);
    }

    while (! nIfs.empty())
    {
        const TSNode ifNode = nIfs.top();
        nIfs.pop();
        const TSNode nTrue = util::child_by_field_name(ifNode, "consequence");
        const TSNode nCond = util::child_by_field_name(ifNode, "condition");
        StmtHandler hTrue  = RegManager::get_stmt_handler(nTrue);
        ExprHandler hCond  = RegManager::get_expr_handler(nCond);

        Stmt* trueStmt     = hTrue(self, nTrue);
        Expr* condExpr     = hCond(self, nCond);
        IfStmt* ifStmt     = stmtFact_.mk_if(condExpr, trueStmt, currentElse);
        currentElse        = ifStmt;
    }

    return currentElse;
}

Stmt* SrcCodeVisitor::visit_try_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    if (ts_node_child_count(node) < 2)
        throw std::logic_error("Not a try-catch node");

    const TSNode nBody      = util::child_by_field_name(node, "body");
    const StmtHandler hBody = RegManager::get_stmt_handler(nBody);

    Stmt* finally           = nullptr;
    std::vector<CatchStmt*> catchStmts;

    auto process = [&](const TSNode& current) -> void
    {
        if (ts_node_eq(current, nBody))
            return;

        const StmtHandler hCurrent = RegManager::get_stmt_handler(current);
        Stmt* currentStmt          = hCurrent(self, current);
        if (is_a<CompoundStmt>(currentStmt))
            finally = currentStmt;
        else if (auto* catchStmt = as_a<CatchStmt>(currentStmt))
            catchStmts.push_back(catchStmt);
    };

    util::for_each_child_node(node, process);

    self->semanticContext_.leave_scope();

    return stmtFact_.mk_try(hBody(self, nBody), finally, std::move(catchStmts));
}

Stmt* SrcCodeVisitor::visit_catch_clause(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    self->semanticContext_.enter_scope();

    LocalVarDefStmt* catchVar = nullptr;
    Stmt* body                = nullptr;
    auto process              = [&](const TSNode& n_current) -> void
    {
        const StmtHandler hCurrent = RegManager::get_stmt_handler(n_current);
        Stmt* currentStmt          = hCurrent(self, n_current);
        if (const auto var = as_a<LocalVarDefStmt>(currentStmt))
        {
            catchVar = var;
            self->semanticContext_.reg_local_var(catchVar);
        }
        else if (is_a<CompoundStmt>(currentStmt))
        {
            body = currentStmt;
        }
    };
    util::for_each_child_node(node, process);

    self->semanticContext_.leave_scope();
    return stmtFact_.mk_catch(catchVar, body);
}

Stmt* SrcCodeVisitor::visit_finally(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode nBody = ts_node_named_child(node, 0);
    return RegManager::get_stmt_handler(nBody)(self, nBody);
}

Stmt* SrcCodeVisitor::visit_catch_decl(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode nType   = util::child_by_field_name(node, "type");
    const TSNode nName   = util::child_by_field_name(node, "name");
    const TypeHandler th = RegManager::get_type_handler(nType);
    Type* type           = th(&self->typeTrs_, nType);
    std::string name     = ts_node_is_null(nName)
                             ? std::string{}
                             : util::extract_text(nName, self->src_str());
    return stmtFact_.mk_local_var_def(std::move(name), type, nullptr);
}

Stmt* SrcCodeVisitor::visit_switch_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode nValue      = util::child_by_field_name(node, "value");
    const TSNode nSwitchBody = util::child_by_field_name(node, "body");
    const ExprHandler hValue = RegManager::get_expr_handler(nValue);
    Expr* value              = hValue(self, nValue);

    std::vector<CaseBaseStmt*> cases;
    auto process = [self, &cases](const TSNode& nCurrent) -> void
    {
        const StmtHandler hStmt = RegManager::get_stmt_handler(nCurrent);
        Stmt* stmt              = hStmt(self, nCurrent);
        if (auto* caseStmt = as_a<CaseBaseStmt>(stmt))
            cases.push_back(caseStmt);
    };
    util::for_each_child_node(nSwitchBody, process);
    return stmtFact_.mk_switch(value, std::move(cases));
}

Stmt* SrcCodeVisitor::visit_case_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    TSTreeCursor cursor = ts_tree_cursor_new(node);
    if (ts_tree_cursor_goto_first_child(&cursor))
    {
        std::vector<Stmt*> bodyStmts;
        Expr* pattern = nullptr;
        TSNode nCurrent;
        do
        {
            nCurrent = ts_tree_cursor_current_node(&cursor);
            if (std::strcmp(ts_node_type(nCurrent), ":") == 0)
                break;
            if (! ts_node_is_named(nCurrent))
                continue;

            // todo handle more complex patterns
            const ExprHandler hParrent = RegManager::get_expr_handler(nCurrent);
            pattern                    = hParrent(self, nCurrent);
        } while (ts_tree_cursor_goto_next_sibling(&cursor));

        while (ts_tree_cursor_goto_next_sibling(&cursor))
        {
            nCurrent = ts_tree_cursor_current_node(&cursor);
            if (! ts_node_is_named(nCurrent))
                continue;

            StmtHandler hStmt = RegManager::get_stmt_handler(nCurrent);
            Stmt* stmt        = hStmt(self, nCurrent);
            bodyStmts.push_back(stmt);
        }

        ts_tree_cursor_delete(&cursor);

        Stmt* body
            = bodyStmts.size() == 1 && is_a<CompoundStmt>(bodyStmts.back())
                ? bodyStmts.back()
                : stmtFact_.mk_compound(std::move(bodyStmts));

        if (pattern)
            return stmtFact_.mk_case(pattern, body);

        return stmtFact_.mk_default_case(body);
    }

    ts_tree_cursor_delete(&cursor);
    return stmtFact_.mk_uknown();
}

Stmt* SrcCodeVisitor::visit_expr_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode nExpr      = ts_node_child(node, 0);
    const ExprHandler hExpr = RegManager::get_expr_handler(nExpr);
    return stmtFact_.mk_expr(hExpr(self, nExpr));
}

Stmt* SrcCodeVisitor::visit_continue(
    [[maybe_unused]] SrcCodeVisitor* self,
    [[maybe_unused]] const TSNode& node
)
{
    return stmtFact_.mk_continue();
}

Stmt* SrcCodeVisitor::visit_break(
    [[maybe_unused]] SrcCodeVisitor* self,
    [[maybe_unused]] const TSNode& node
)
{
    return stmtFact_.mk_break();
}

Stmt* SrcCodeVisitor::visit_return(SrcCodeVisitor* self, const TSNode& node)
{
    Expr* expr = nullptr;
    if (ts_node_named_child_count(node) > 0)
    {
        const TSNode nExpr      = ts_node_named_child(node, 0);
        const ExprHandler hExpr = RegManager::get_expr_handler(nExpr);
        expr                    = hExpr(self, nExpr);
    }
    return stmtFact_.mk_return(expr);
}

Stmt* SrcCodeVisitor::visit_throw(SrcCodeVisitor* self, const TSNode& node)
{
    Expr* expr = nullptr;
    if (ts_node_named_child_count(node) > 0)
    {
        const TSNode nExpr       = ts_node_named_child(node, 0);
        const ExprHandler hhExpr = RegManager::get_expr_handler(nExpr);
        expr                     = hhExpr(self, nExpr);
    }
    return stmtFact_.mk_throw(expr);
}

} // namespace astfri::csharp
