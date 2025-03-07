#include <libastfri/inc/Stmt.hpp>

namespace astfri
{

VarDefStmt::VarDefStmt(std::string name, Type* type, Expr* initializer) :
    name_(std::move(name)),
    type_(type),
    initializer_(initializer)
{
}

LocalVarDefStmt::LocalVarDefStmt(
    std::string name,
    Type* type,
    Expr* initializer
) :
    VarDefStmt(name, type, initializer)
{
}

ParamVarDefStmt::ParamVarDefStmt(
    std::string name,
    Type* type,
    Expr* initializer
) :
    VarDefStmt(name, type, initializer)
{
}

MemberVarDefStmt::MemberVarDefStmt(
    std::string name,
    Type* type,
    Expr* initializer,
    AccessModifier access
) :
    VarDefStmt(name, type, initializer),
    access_(access)
{
}

GlobalVarDefStmt::GlobalVarDefStmt(
    std::string name,
    Type* type,
    Expr* initializer
) :
    VarDefStmt(name, type, initializer)
{
}

DefStmt::DefStmt(std::vector<VarDefStmt*> defs) :
    defs_(std::move(defs))
{
}

FunctionDefStmt::FunctionDefStmt(
    std::string name,
    std::vector<ParamVarDefStmt*> params,
    Type* retType,
    CompoundStmt* body
) :
    name_(std::move(name)),
    params_(std::move(params)),
    retType_(retType),
    body_(body)
{
}

BaseInitializerStmt::BaseInitializerStmt(
    std::string base,
    std::vector<Expr*> args
) :
    base_(std::move(base)),
    args_(std::move(args))
{
}

ConstructorDefStmt::ConstructorDefStmt(
    ClassDefStmt* owner,
    std::vector<ParamVarDefStmt*> params,
    std::vector<BaseInitializerStmt*> baseInit,
    CompoundStmt* body,
    AccessModifier access
) :
    owner_(owner),
    params_(std::move(params)),
    baseInit_(std::move(baseInit)),
    body_(body),
    access_(access)
{
}

DestructorDefStmt::DestructorDefStmt(ClassDefStmt* owner, CompoundStmt* body) :
    owner_(owner),
    body_(body)
{
}

GenericParam::GenericParam(std::string constraint, std::string name) :
    constraint_(std::move(constraint)),
    name_(std::move(name))
{
}

CompoundStmt::CompoundStmt(std::vector<Stmt*> stmts) :
    stmts_(std::move(stmts))
{
}

ReturnStmt::ReturnStmt(Expr* val) :
    val_(val)
{
}

ExprStmt::ExprStmt(Expr* expr) :
    expr_(expr)
{
}

IfStmt::IfStmt(Expr* cond, Stmt* iftrue, Stmt* iffalse) :
    cond_(cond),
    iftrue_(iftrue),
    iffalse_(iffalse)
{
}

CaseBaseStmt::CaseBaseStmt(Stmt* body) :
    body_(body)
{
}

CaseStmt::CaseStmt(std::vector<Expr*> exprs, Stmt* body) :
    CaseBaseStmt(body),
    exprs_(std::move(exprs))
{
}

DefaultCaseStmt::DefaultCaseStmt(Stmt* body) :
    CaseBaseStmt(body)
{
}

SwitchStmt::SwitchStmt(Expr* expr, std::vector<CaseBaseStmt*> cases) :
    expr_(expr),
    cases_(std::move(cases))
{
}

LoopStmt::LoopStmt(Expr* cond, Stmt* body) :
    cond_(cond),
    body_(body)
{
}

WhileStmt::WhileStmt(Expr* cond, Stmt* body) :
    LoopStmt(cond, body)
{
}

DoWhileStmt::DoWhileStmt(Expr* cond, Stmt* body) :
    LoopStmt(cond, body)
{
}

ForStmt::ForStmt(Stmt* init, Expr* cond, Stmt* step, Stmt* body) :
    LoopStmt(cond, body),
    init_(init),
    step_(step)
{
}

ThrowStmt::ThrowStmt(Expr* val) :
    val_(val)
{
}

} // namespace astfri
