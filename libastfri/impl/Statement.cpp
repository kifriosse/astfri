#include <libastfri/Statement.hpp>

namespace astfri
{
VarDef::VarDef(std::string name, Type* type, Expr* initializer) :
    name_(std::move(name)),
    type_(type),
    initializer_(initializer)
{
}

LocalVarDef::LocalVarDef(std::string name, Type* type, Expr* initializer) :
    VarDef(name, type, initializer)
{
}

ParamVarDef::ParamVarDef(std::string name, Type* type, Expr* initializer) :
    VarDef(name, type, initializer)
{
}

MemberVarDef::MemberVarDef(std::string name, Type* type, Expr* initializer) :
    VarDef(name, type, initializer)
{
}

GlobalVarDef::GlobalVarDef(std::string name, Type* type, Expr* initializer) :
    VarDef(name, type, initializer)
{
}

FunctionDef::FunctionDef(
    std::string name,
    std::vector<ParamVarDef*> params,
    Type* retType,
    CompoundStmt* body
) :
    name_(std::move(name)),
    params_(std::move(params)),
    retType_(retType),
    body_(body)
{
}

MethodDef::MethodDef(ClassDef* owner, FunctionDef* func) :
    owner_(owner),
    func_(func)
{
}

ClassDef::ClassDef(
    std::string name,
    std::vector<MemberVarDef*> vars,
    std::vector<GenericParam*> tparams
) :
    name_(std::move(name)),
    vars_(std::move(vars)),
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

UnknownStmt::UnknownStmt(std::string message) :
    message_(std::move(message))
{
}

TranslationUnit::TranslationUnit(
    std::vector<ClassDef*> classes,
    std::vector<FunctionDef*> functions,
    std::vector<GlobalVarDef*> globals
) :
    classes_(std::move(classes)),
    functions_(std::move(functions)),
    globals_(std::move(globals))
{
}
} // namespace astfri