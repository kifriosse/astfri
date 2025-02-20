#include <libastfri/inc/StmtFactory.hpp>

namespace astfri
{
StmtFactory& StmtFactory::get_instance()
{
    static StmtFactory instance;
    return instance;
}

LocalVarDefStmt* StmtFactory::mk_local_var_def(
    std::string name,
    Type* type,
    Expr* initializer
)
{
    return details::emplace_get<LocalVarDefStmt>(
        stmts_,
        std::move(name),
        type,
        initializer
    );
}

ParamVarDefStmt* StmtFactory::mk_param_var_def(
    std::string name,
    Type* type,
    Expr* initializer
)
{
    return details::emplace_get<ParamVarDefStmt>(
        stmts_,
        std::move(name),
        type,
        initializer
    );
}

MemberVarDefStmt* StmtFactory::mk_member_var_def(
    std::string name,
    Type* type,
    Expr* initializer,
    AccessModifier access
)
{
    return details::emplace_get<MemberVarDefStmt>(
        stmts_,
        std::move(name),
        type,
        initializer,
        access
    );
}

GlobalVarDefStmt* StmtFactory::mk_global_var_def(
    std::string name,
    Type* type,
    Expr* initializer
)
{
    return details::emplace_get<GlobalVarDefStmt>(
        stmts_,
        std::move(name),
        type,
        initializer
    );
}

FunctionDefStmt* StmtFactory::mk_function_def(
    std::string name,
    std::vector<ParamVarDefStmt*> params,
    Type* retType,
    CompoundStmt* body
)
{
    return details::emplace_get<FunctionDefStmt>(
        stmts_,
        std::move(name),
        std::move(params),
        retType,
        body
    );
}

MethodDefStmt* StmtFactory::mk_method_def(
    ClassDefStmt* owner,
    FunctionDefStmt* func,
    AccessModifier access
)
{
    return details::emplace_get<MethodDefStmt>(stmts_, owner, func, access);
}

ClassDefStmt* StmtFactory::mk_class_def(
    std::string name,
    std::vector<MemberVarDefStmt*> vars,
    std::vector<MethodDefStmt*> methods,
    std::vector<GenericParam*> tparams
)
{
    return details::emplace_get<ClassDefStmt>(
        stmts_,
        std::move(name),
        std::move(vars),
        std::move(methods),
        std::move(tparams)
    );
}

ConstructorDefStmt* StmtFactory::mk_constructor_def(
    ClassDefStmt* owner,
    std::vector<ParamVarDefStmt*> params,
    std::vector<BaseInitializerStmt*> baseInit,
    CompoundStmt* body,
    AccessModifier access
)
{
    return details::emplace_get<ConstructorDefStmt>(
        stmts_,
        owner,
        std::move(params),
        std::move(baseInit),
        body,
        access
    );
}

BaseInitializerStmt* StmtFactory::mak_base_initializer(
    std::string base,
    std::vector<Expr*> args
)
{
    return details::emplace_get<BaseInitializerStmt>(
        stmts_,
        std::move(base),
        std::move(args)
    );
}

DestructorDefStmt* StmtFactory::mk_destructor_def(
    ClassDefStmt* owner,
    CompoundStmt* body
)
{
    return details::emplace_get<DestructorDefStmt>(stmts_, owner, body);
}

GenericParam* StmtFactory::mk_generic_param(
    std::string constraint,
    std::string name
)
{
    return details::emplace_get<GenericParam>(
        stmts_,
        std::move(constraint),
        std::move(name)
    );
}

CompoundStmt* StmtFactory::mk_compound(std::vector<Stmt*> stmts)
{
    return details::emplace_get<CompoundStmt>(stmts_, std::move(stmts));
}

ReturnStmt* StmtFactory::mk_return(Expr* val)
{
    return details::emplace_get<ReturnStmt>(stmts_, val);
}

ExprStmt* StmtFactory::mk_expr(Expr* expr)
{
    return details::emplace_get<ExprStmt>(stmts_, expr);
}

IfStmt* StmtFactory::mk_if(Expr* cond, Stmt* iftrue, Stmt* iffalse)
{
    return details::emplace_get<IfStmt>(stmts_, cond, iftrue, iffalse);
}

CaseStmt* StmtFactory::mk_case(Expr* expr, Stmt* body)
{
    return details::emplace_get<CaseStmt>(stmts_, expr, body);
}

SwitchStmt* StmtFactory::mk_switch(Expr* expr, std::vector<CaseStmt*> cases)
{
    return details::emplace_get<SwitchStmt>(stmts_, expr, std::move(cases));
}

WhileStmt* StmtFactory::mk_while(Expr* cond, CompoundStmt* body)
{
    return details::emplace_get<WhileStmt>(stmts_, cond, body);
}

DoWhileStmt* StmtFactory::mk_do_while(Expr* cond, CompoundStmt* body)
{
    return details::emplace_get<DoWhileStmt>(stmts_, cond, body);
}

ForStmt* StmtFactory::mk_for(
    Stmt* init,
    Expr* cond,
    Stmt* step,
    CompoundStmt* body
)
{
    return details::emplace_get<ForStmt>(stmts_, init, cond, step, body);
}

ThrowStmt* StmtFactory::mk_throw(Expr* val)
{
    return details::emplace_get<ThrowStmt>(stmts_, val);
}

UnknownStmt* StmtFactory::mk_uknown()
{
    return &unknown_;
}

TranslationUnit* StmtFactory::mk_translation_unit(
    std::vector<ClassDefStmt*> classes,
    std::vector<FunctionDefStmt*> functions,
    std::vector<GlobalVarDefStmt*> globals
)
{
    return details::emplace_get<TranslationUnit>(
        stmts_,
        std::move(classes),
        std::move(functions),
        std::move(globals)
    );
}

} // namespace astfri
