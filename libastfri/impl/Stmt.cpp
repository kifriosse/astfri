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

MethodDefStmt::MethodDefStmt(
    ClassDefStmt* owner,
    FunctionDefStmt* func,
    AccessModifier access
) :
    owner_(owner),
    func_(func),
    access_(access)
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

ClassDefStmt::ClassDefStmt(
    std::string name,
    std::vector<MemberVarDefStmt*> vars,
    std::vector<MethodDefStmt*> methods,
    std::vector<GenericParam*> tparams
) :
    name_(std::move(name)),
    vars_(std::move(vars)),
    methods_(std::move(methods)),
    tparams_(std::move(tparams))
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

CaseStmt::CaseStmt(Expr* expr, Stmt* body) :
    expr_(expr),
    body_(body)
{
}

SwitchStmt::SwitchStmt(Expr* expr, std::vector<CaseStmt*> cases) :
    expr_(expr),
    cases_(std::move(cases))
{
}

LoopStmt::LoopStmt(Expr* cond, CompoundStmt* body) :
    cond_(cond),
    body_(body)
{
}

WhileStmt::WhileStmt(Expr* cond, CompoundStmt* body) :
    LoopStmt(cond, body)
{
}

DoWhileStmt::DoWhileStmt(Expr* cond, CompoundStmt* body) :
    LoopStmt(cond, body)
{
}

ForStmt::ForStmt(Stmt* init, Expr* cond, Stmt* step, CompoundStmt* body) :
    LoopStmt(cond, body),
    init_(init),
    step_(step)
{
}

ThrowStmt::ThrowStmt(Expr* val) :
    val_(val)
{
}

TranslationUnit::TranslationUnit(
    std::vector<ClassDefStmt*> classes,
    std::vector<FunctionDefStmt*> functions,
    std::vector<GlobalVarDefStmt*> globals
) :
    classes_(std::move(classes)),
    functions_(std::move(functions)),
    globals_(std::move(globals))
{
}
} // namespace astfri
