#include <astfri/Astfri.hpp>

#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri-cs/impl/regs/Maps.hpp>
#include <libastfri-cs/impl/util/AstfriUtil.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/visitors/src_code/SrcCodeTransformer.hpp>

#include <tree_sitter/api.h>

#include <cstring>
#include <string_view>
#include <vector>

namespace astfri::csharp {

Stmt* SrcCodeTransformer::visit_block(SrcCodeTransformer* self, const TSNode& node) {
    self->semContext_.enter_scope();
    auto discoverFunc = [self](const TSNode& current) -> void {
        using enum NodeType;
        if (! ts_node_is_named(current)
            || ts_node_symbol(current) != MapManager::get_symbol(LocalFuncDecl))
            return;

        self->semContext_.reg_local_func(
            util::make_func_metadata(current, self->src_str(), self->typeTrs_)
        );
    };

    util::for_each_child_node(node, discoverFunc);

    std::vector<Stmt*> stmts;
    auto processBody = [self, &stmts](const TSNode& nCurrent) -> void {
        if (ts_node_is_named(nCurrent)) {
            const StmtMapper mStmt = MapManager::get_stmt_mapper(nCurrent);
            stmts.push_back(mStmt(self, nCurrent));
        }
    };
    util::for_each_child_node(node, processBody);

    self->semContext_.leave_scope();

    return stmtFact_.mk_compound(std::move(stmts));
}

Stmt* SrcCodeTransformer::visit_arrow_body(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nBody     = ts_node_named_child(node, 0);
    const ExprMapper mBody = MapManager::get_expr_mapper(nBody);
    Type* returnType       = self->semContext_.current_return_type();
    Expr* expr             = mBody(self, nBody);
    Stmt* body             = nullptr;

    if (is_a<VoidType>(returnType))
        body = stmtFact_.mk_expr(expr);
    else
        body = stmtFact_.mk_return(expr);

    return stmtFact_.mk_compound({body});
}

Stmt* SrcCodeTransformer::visit_while(SrcCodeTransformer* self, const TSNode& node) {
    return self->make_while_loop(node, true);
}

Stmt* SrcCodeTransformer::visit_do_while(SrcCodeTransformer* self, const TSNode& node) {
    return self->make_while_loop(node, false);
}

Stmt* SrcCodeTransformer::visit_for_loop(SrcCodeTransformer* self, const TSNode& node) {
    self->semContext_.enter_scope();
    const TSNode nInit  = util::child_by_field_name(node, "initializer");
    const TSNode nCond  = util::child_by_field_name(node, "condition");
    const TSNode nStep  = util::child_by_field_name(node, "update");
    const TSNode nBody  = util::child_by_field_name(node, "body");
    const bool condNull = ts_node_is_null(nCond);
    const bool stepNull = ts_node_is_null(nStep);
    Stmt* init          = nullptr;
    Expr* cond          = nullptr;
    Stmt* step          = nullptr;

    if (! ts_node_is_null(nInit)) {
        if (MapManager::is_expr(nInit)) {
            const TSNode* nEnd = &nBody;
            if (! condNull)
                nEnd = &nCond;
            else if (! stepNull)
                nEnd = &nStep;

            init = stmtFact_.mk_expr(self->expr_list_to_comma_op(nInit, nEnd));
        }
        else {
            init = self->visit_for_init_var_def(nInit);
        }
    }

    if (! condNull) {
        const ExprMapper mCond = MapManager::get_expr_mapper(nCond);
        cond                   = mCond(self, nCond);
    }

    if (! stepNull) {
        step = stmtFact_.mk_expr(self->expr_list_to_comma_op(nStep, &nBody));
    }

    const StmtMapper mBody = MapManager::get_stmt_mapper(nBody);
    Stmt* body             = mBody(self, nBody);
    self->semContext_.leave_scope();
    return stmtFact_.mk_for(init, cond, step, body);
}

Stmt* SrcCodeTransformer::visit_for_each(SrcCodeTransformer* self, const TSNode& node) {
    self->semContext_.enter_scope();
    const TSNode nType  = util::child_by_field_name(node, "type");
    const TSNode hLeft  = util::child_by_field_name(node, "left");
    const TSNode nRight = util::child_by_field_name(node, "right");
    const TSNode nBody  = util::child_by_field_name(node, "body");

    if (ts_node_is_null(nType)) {
        // todo handle deconstruction syntax
        return stmtFact_.mk_uknown();
    }

    std::string name        = util::extract_text(hLeft, self->src_str());
    const ExprMapper mRight = MapManager::get_expr_mapper(nRight);
    const StmtMapper mBody  = MapManager::get_stmt_mapper(nBody);
    const TypeMapper th     = MapManager::get_type_mapper(nType);
    Type* type              = th(&self->typeTrs_, nType);
    LocalVarDefStmt* left   = stmtFact_.mk_local_var_def(std::move(name), type, nullptr);
    self->semContext_.reg_local_var(left);
    Expr* right = mRight(self, nRight);
    Stmt* body  = mBody(self, nBody);
    self->semContext_.leave_scope();
    return stmtFact_.mk_for_each(left, right, body);
}

Stmt* SrcCodeTransformer::visit_if(SrcCodeTransformer* self, const TSNode& node) {
    static constexpr std::string_view falseSw = "alternative";

    std::vector<TSNode> nIfs;
    nIfs.push_back(node);
    TSNode nCurrent    = util::child_by_field_name(node, falseSw);
    const TSSymbol sIf = ts_node_symbol(node);
    TSSymbol sCurrent  = ts_node_is_null(nCurrent) ? 0 : ts_node_symbol(nCurrent);
    while (sCurrent == sIf) {
        nIfs.push_back(nCurrent);
        nCurrent = util::child_by_field_name(nCurrent, falseSw);
        if (ts_node_is_null(nCurrent))
            break;

        sCurrent = ts_node_symbol(nCurrent);
    }

    // handling of else in last node
    const TSNode nElse = util::child_by_field_name(nIfs.back(), falseSw);

    Stmt* currentElse  = nullptr;
    if (! ts_node_is_null(nElse)) {
        const StmtMapper mElse = MapManager::get_stmt_mapper(nElse);
        currentElse            = mElse(self, nElse);
    }

    while (! nIfs.empty()) {
        const TSNode ifNode = nIfs.back();
        nIfs.pop_back();
        const TSNode nTrue = util::child_by_field_name(ifNode, "consequence");
        const TSNode nCond = util::child_by_field_name(ifNode, "condition");
        StmtMapper mTrue   = MapManager::get_stmt_mapper(nTrue);
        ExprMapper mCond   = MapManager::get_expr_mapper(nCond);

        Stmt* trueStmt     = mTrue(self, nTrue);
        Expr* condExpr     = mCond(self, nCond);
        IfStmt* ifStmt     = stmtFact_.mk_if(condExpr, trueStmt, currentElse);
        currentElse        = ifStmt;
    }

    return currentElse;
}

Stmt* SrcCodeTransformer::visit_try(SrcCodeTransformer* self, const TSNode& node) {
    if (ts_node_child_count(node) < 2)
        return stmtFact_.mk_uknown();

    const TSNode nBody     = util::child_by_field_name(node, "body");
    const StmtMapper mBody = MapManager::get_stmt_mapper(nBody);

    Stmt* finally          = nullptr;
    std::vector<CatchStmt*> catchStmts;

    auto process = [&](const TSNode& current) -> void {
        if (ts_node_eq(current, nBody))
            return;

        const StmtMapper mCurrent = MapManager::get_stmt_mapper(current);
        Stmt* currentStmt         = mCurrent(self, current);
        if (is_a<CompoundStmt>(currentStmt))
            finally = currentStmt;
        else if (auto* catchStmt = as_a<CatchStmt>(currentStmt))
            catchStmts.push_back(catchStmt);
    };

    util::for_each_child_node(node, process);

    self->semContext_.leave_scope();

    return stmtFact_.mk_try(mBody(self, nBody), finally, std::move(catchStmts));
}

Stmt* SrcCodeTransformer::visit_catch(SrcCodeTransformer* self, const TSNode& node) {
    self->semContext_.enter_scope();

    LocalVarDefStmt* catchVar = nullptr;
    Stmt* body                = nullptr;
    auto process              = [&](const TSNode& n_current) -> void {
        const StmtMapper mCurrent = MapManager::get_stmt_mapper(n_current);
        Stmt* currentStmt         = mCurrent(self, n_current);
        if (const auto var = as_a<LocalVarDefStmt>(currentStmt)) {
            catchVar = var;
            self->semContext_.reg_local_var(catchVar);
        }
        else if (is_a<CompoundStmt>(currentStmt)) {
            body = currentStmt;
        }
    };
    util::for_each_child_node(node, process);

    self->semContext_.leave_scope();
    return stmtFact_.mk_catch(catchVar, body);
}

Stmt* SrcCodeTransformer::visit_finally(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nBody = ts_node_named_child(node, 0);
    return MapManager::get_stmt_mapper(nBody)(self, nBody);
}

Stmt* SrcCodeTransformer::visit_catch_decl(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nType  = util::child_by_field_name(node, "type");
    const TSNode nName  = util::child_by_field_name(node, "name");
    const TypeMapper tm = MapManager::get_type_mapper(nType);
    Type* type          = tm(&self->typeTrs_, nType);
    std::string name
        = ts_node_is_null(nName) ? std::string{} : util::extract_text(nName, self->src_str());
    return stmtFact_.mk_local_var_def(std::move(name), type, nullptr);
}

Stmt* SrcCodeTransformer::visit_switch(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nValue      = util::child_by_field_name(node, "value");
    const TSNode nSwitchBody = util::child_by_field_name(node, "body");
    const ExprMapper mValue  = MapManager::get_expr_mapper(nValue);
    Expr* value              = mValue(self, nValue);

    std::vector<CaseBaseStmt*> cases;
    auto process = [self, &cases](const TSNode& nCurrent) -> void {
        const StmtMapper mStmt = MapManager::get_stmt_mapper(nCurrent);
        Stmt* stmt             = mStmt(self, nCurrent);
        if (auto* caseStmt = as_a<CaseBaseStmt>(stmt))
            cases.push_back(caseStmt);
    };
    util::for_each_child_node(nSwitchBody, process);
    return stmtFact_.mk_switch(value, std::move(cases));
}

Stmt* SrcCodeTransformer::visit_case_stmt(SrcCodeTransformer* self, const TSNode& node) {
    TSTreeCursor cursor = ts_tree_cursor_new(node);
    if (ts_tree_cursor_goto_first_child(&cursor)) {
        std::vector<Stmt*> bodyStmts;
        Expr* pattern = nullptr;
        TSNode nCurrent;
        do {
            nCurrent = ts_tree_cursor_current_node(&cursor);
            if (std::strcmp(ts_node_type(nCurrent), ":") == 0)
                break;
            if (! ts_node_is_named(nCurrent))
                continue;

            // todo handle more complex patterns
            const ExprMapper mParrent = MapManager::get_expr_mapper(nCurrent);
            pattern                   = mParrent(self, nCurrent);
        }
        while (ts_tree_cursor_goto_next_sibling(&cursor));

        while (ts_tree_cursor_goto_next_sibling(&cursor)) {
            nCurrent = ts_tree_cursor_current_node(&cursor);
            if (! ts_node_is_named(nCurrent))
                continue;

            StmtMapper mStmt = MapManager::get_stmt_mapper(nCurrent);
            Stmt* stmt       = mStmt(self, nCurrent);
            bodyStmts.push_back(stmt);
        }

        ts_tree_cursor_delete(&cursor);

        Stmt* body = bodyStmts.size() == 1 && is_a<CompoundStmt>(bodyStmts.back())
                       ? bodyStmts.back()
                       : stmtFact_.mk_compound(std::move(bodyStmts));

        if (pattern)
            return stmtFact_.mk_case(pattern, body);

        return stmtFact_.mk_default_case(body);
    }

    ts_tree_cursor_delete(&cursor);
    return stmtFact_.mk_uknown();
}

Stmt* SrcCodeTransformer::visit_expr_stmt(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nExpr     = ts_node_child(node, 0);
    const ExprMapper mExpr = MapManager::get_expr_mapper(nExpr);
    return stmtFact_.mk_expr(mExpr(self, nExpr));
}

Stmt* SrcCodeTransformer::visit_continue(
    [[maybe_unused]] SrcCodeTransformer* self,
    [[maybe_unused]] const TSNode& node
) {
    return stmtFact_.mk_continue();
}

Stmt* SrcCodeTransformer::visit_break(
    [[maybe_unused]] SrcCodeTransformer* self,
    [[maybe_unused]] const TSNode& node
) {
    return stmtFact_.mk_break();
}

Stmt* SrcCodeTransformer::visit_return(SrcCodeTransformer* self, const TSNode& node) {
    Expr* expr = nullptr;
    if (ts_node_named_child_count(node) > 0) {
        const TSNode nExpr     = ts_node_named_child(node, 0);
        const ExprMapper mExpr = MapManager::get_expr_mapper(nExpr);
        expr                   = mExpr(self, nExpr);
    }
    return stmtFact_.mk_return(expr);
}

Stmt* SrcCodeTransformer::visit_throw(SrcCodeTransformer* self, const TSNode& node) {
    Expr* expr = nullptr;
    if (ts_node_named_child_count(node) > 0) {
        const TSNode nExpr      = ts_node_named_child(node, 0);
        const ExprMapper mExpr = MapManager::get_expr_mapper(nExpr);
        expr                    = mExpr(self, nExpr);
    }
    return stmtFact_.mk_throw(expr);
}

} // namespace astfri::csharp
