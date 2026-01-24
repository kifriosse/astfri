#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
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
    const util::VarDefType defType
)
{
    TSNode nVarDecl{};

    [[maybe_unused]] CSModifiers modifs
        = CSModifiers::handle_var_modifs(node, src_str(), &nVarDecl);

    const TSNode nType   = util::child_by_field_name(nVarDecl, "type");
    const TypeHandler th = RegManager::get_type_handler(nType);
    Type* type           = th(&typeTrs_, nType);

    std::vector<VarDefStmt*> varDefs;
    auto process = [&](const TSQueryMatch& match)
    {
        for (uint32_t i = 0; i < match.capture_count; ++i)
        {
            const TSNode nVarDecltotr = match.captures[i].node;
            TSNode n_varName          = ts_node_named_child(nVarDecltotr, 0);
            TSNode nInit              = ts_node_named_child(nVarDecltotr, 1);
            std::string name = util::extract_text(n_varName, src_str());
            Expr* init       = nullptr;
            if (! ts_node_is_null(nInit))
            {
                ExprHandler hInit = RegManager::get_expr_handler(nInit);
                init              = hInit(this, nInit);
            }

            VarDefStmt* varDef = nullptr;
            switch (defType)
            {
            case util::VarDefType::Member:
            {
                MemberVarMetadata* varMeta = semanticContext_.find_memb_var(
                    name,
                    semanticContext_.current_type()
                );
                varDef               = varMeta->varDef;
                varDef->initializer_ = init;
                varMeta->processed   = true;
                break;
            }
            case util::VarDefType::Local:
                // todo handle const
                varDef
                    = stmtFact_.mk_local_var_def(std::move(name), type, init);
                semanticContext_.reg_local_var(as_a<LocalVarDefStmt>(varDef));
                break;
            case util::VarDefType::Global:
                // todo handle const
                varDef
                    = stmtFact_.mk_global_var_def(std::move(name), type, init);
                break;
            }

            if (varDef)
            {
                // var_def->name_ = var_name;
                varDefs.push_back(varDef);
            }
        }
    };
    util::for_each_match(node, regs::QueryType::VarDecltor, process);

    if (varDefs.size() > 1)
        return stmtFact_.mk_def(std::move(varDefs));

    return varDefs.front();
}

Stmt* SrcCodeVisitor::make_while_loop(const TSNode& node, const bool is_while)
{
    const TSNode nCond      = util::child_by_field_name(node, "condition");
    const TSNode nBody      = util::child_by_field_name(node, "body");
    const ExprHandler hCond = RegManager::get_expr_handler(nCond);
    const StmtHandler hBody = RegManager::get_stmt_handler(nBody);
    Expr* cond              = hCond(this, nCond);
    Stmt* body              = hBody(this, nBody);

    if (is_while)
        return stmtFact_.mk_while(cond, body);
    return stmtFact_.mk_do_while(cond, body);
}

FunctionDefStmt* SrcCodeVisitor::make_func_stmt(
    const TSNode& node,
    const bool isMethod
)
{
    semanticContext_.enter_scope();

    FunctionDefStmt* funcDef = stmtFact_.mk_function_def();
    const TSNode nRetType
        = util::child_by_field_name(node, isMethod ? "returns" : "type");
    const TSNode nName  = util::child_by_field_name(node, "name");
    const TSNode nParam = util::child_by_field_name(node, "parameters");
    const TSNode nBody  = util::child_by_field_name(node, "body");

    // todo handle generic parameters
    const TypeHandler th    = RegManager::get_type_handler(nRetType);
    const StmtHandler hBody = RegManager::get_stmt_handler(nBody);
    Type* retType           = th(&typeTrs_, nRetType);

    semanticContext_.reg_return(retType);

    funcDef->retType_ = retType;
    funcDef->name_    = util::extract_text(nName, src_str());
    funcDef->params_  = make_param_list(nParam, false);
    funcDef->body_    = as_a<CompoundStmt>(hBody(this, nBody));

    semanticContext_.leave_scope();
    semanticContext_.unregister_return_type();
    return funcDef;
}

Expr* SrcCodeVisitor::expr_list_to_comma_op(
    const TSNode& nStart,
    const TSNode* nEnd
)
{
    std::queue<Expr*> exprs;
    TSNode nNext = nStart;
    while (! ts_node_is_null(nNext))
    {
        TSNode nCurrent = nNext;
        nNext           = ts_node_next_sibling(nCurrent);
        if (nEnd && ts_node_eq(nCurrent, *nEnd))
            break;

        if (! ts_node_is_named(nCurrent))
            continue;

        const ExprHandler hExpr = RegManager::get_expr_handler(nCurrent);
        exprs.push(hExpr(this, nCurrent));
    }

    if (exprs.empty())
        return nullptr;

    Expr* initExpr = exprs.front();
    exprs.pop();

    while (! exprs.empty())
    {
        Expr* right = exprs.front();
        exprs.pop();
        initExpr = exprFact_.mk_bin_on(initExpr, BinOpType::Comma, right);
    }
    return initExpr;
}

std::vector<ParamVarDefStmt*> SrcCodeVisitor::make_param_list(
    const TSNode& node,
    const bool makeShallow
)
{
    std::vector<ParamVarDefStmt*> params;
    auto collector = [&](const TSNode& current)
    {
        const TSNode nName   = util::child_by_field_name(current, "name");
        const TSNode nType   = util::child_by_field_name(current, "type");
        const TSNode nInit   = ts_node_next_named_sibling(nName);
        std::string name     = util::extract_text(nName, src_str());
        const TypeHandler th = RegManager::get_type_handler(nType);
        Type* type           = th(&typeTrs_, nType);
        Expr* init           = nullptr;
        if (! makeShallow && ! ts_node_is_null(nInit))
        {
            const ExprHandler hInit = RegManager::get_expr_handler(nInit);
            init                    = hInit(this, nInit);
        }
        ParamVarDefStmt* paramDef
            = stmtFact_.mk_param_var_def(std::move(name), type, init);
        this->semanticContext_.reg_param(paramDef);
        params.push_back(paramDef);
    };
    util::process_param_list(node, std::move(collector));
    return params;
}

std::vector<Expr*> SrcCodeVisitor::visit_arg_list(const TSNode& node)
{
    std::vector<Expr*> exprs;
    auto process = [this, &exprs](const TSNode& current) -> void
    {
        const TSNode nChild     = ts_node_child(current, 0);
        const ExprHandler hExpr = RegManager::get_expr_handler(nChild);
        exprs.emplace_back(hExpr(this, nChild));
    };
    util::for_each_child_node(node, process);
    return exprs;
}

Stmt* SrcCodeVisitor::visit_for_init_var_def(const TSNode& node)
{
    std::vector<VarDefStmt*> varDefs;
    const TSNode nType   = util::child_by_field_name(node, "type");
    const TypeHandler th = RegManager::get_type_handler(nType);

    auto proces          = [&](const TSQueryMatch& match)
    {
        for (uint32_t i = 0; i < match.capture_count; ++i)
        {
            const TSNode nDecltor   = match.captures[i].node;
            TSNode nName            = ts_node_named_child(nDecltor, 0);
            TSNode nRight           = ts_node_named_child(nDecltor, 1);
            ExprHandler hRight      = RegManager::get_expr_handler(nRight);
            LocalVarDefStmt* varDef = stmtFact_.mk_local_var_def(
                util::extract_text(nName, src_str()),
                th(&typeTrs_, nType),
                hRight(this, nRight)
            );
            varDefs.push_back(varDef);
            semanticContext_.reg_local_var(varDef);
        }
    };
    util::for_each_match(node, regs::QueryType::VarDecltor, proces);

    if (varDefs.size() > 1)
        return stmtFact_.mk_def(std::move(varDefs));
    return varDefs.front();
}

std::string_view SrcCodeVisitor::src_str() const
{
    return currentSrc_
             ? currentSrc_->srcStr
             : throw std::logic_error("Current source code is not set");
}

SourceFile* SrcCodeVisitor::src() const
{
    return currentSrc_
             ? currentSrc_
             : throw std::logic_error("Current source code is not set");
}

} // namespace astfri::csharp