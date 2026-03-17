#include <libastfri/inc/Stmt.hpp>

#include <vector>

namespace astfri {

VarDefStmt::VarDefStmt(std::string name, Type* type, Expr* initializer) :
    name(std::move(name)),
    type(type),
    initializer(initializer) {
}

LocalVarDefStmt::LocalVarDefStmt(std::string name, Type* type, Expr* initializer) :
    VarDefStmt(name, type, initializer) {
}

ParamVarDefStmt::ParamVarDefStmt(std::string name, Type* type, Expr* initializer) :
    VarDefStmt(name, type, initializer) {
}

MemberVarDefStmt::MemberVarDefStmt(
    std::string name,
    Type* type,
    Expr* initializer,
    AccessModifier access
) :
    VarDefStmt(name, type, initializer),
    access(access) {
}

GlobalVarDefStmt::GlobalVarDefStmt(std::string name, Type* type, Expr* initializer) :
    VarDefStmt(name, type, initializer) {
}

DefStmt::DefStmt(std::vector<VarDefStmt*> defs) :
    defs(std::move(defs)) {
}

FunctionDefStmt::FunctionDefStmt(
    std::string name,
    std::vector<ParamVarDefStmt*> params,
    Type* retType,
    CompoundStmt* body
) :
    name(std::move(name)),
    params(std::move(params)),
    retType(retType),
    body(body) {
}

BaseInitializerStmt::BaseInitializerStmt(std::string base, std::vector<Expr*> args) :
    base_(std::move(base)),
    args(std::move(args)) {
}

BaseInitializerStmt::BaseInitializerStmt(ClassType* type, std::vector<Expr*> args) :
    base_(""),
    type(type),
    args(std::move(args)) {
}

SelfInitializerStmt::SelfInitializerStmt(std::vector<Expr*> args) :
    args(std::move(args)) {
}

MemberInitializerStmt::MemberInitializerStmt(MemberVarDefStmt* member, Expr* arg) :
    member(member),
    arg(arg) {
}

ConstructorDefStmt::ConstructorDefStmt() :
    owner(nullptr),
    params({}),
    baseInit({}),
    body(nullptr),
    access(AccessModifier::Public) {
}

ConstructorDefStmt::ConstructorDefStmt(
    ClassDefStmt* owner,
    std::vector<ParamVarDefStmt*> params,
    std::vector<BaseInitializerStmt*> baseInit,
    CompoundStmt* body,
    AccessModifier access
) :
    owner(owner),
    params(std::move(params)),
    baseInit(std::move(baseInit)), // TODO
    body(body),
    access(access) {
}

DestructorDefStmt::DestructorDefStmt(ClassDefStmt* owner, CompoundStmt* body) :
    owner(owner),
    body(body) {
}

GenericParam::GenericParam(std::string constraint, std::string name) :
    constraint(std::move(constraint)),
    name(std::move(name)) {
}

CompoundStmt::CompoundStmt(std::vector<Stmt*> stmts) :
    stmts(std::move(stmts)) {
}

ReturnStmt::ReturnStmt(Expr* val) :
    val(val) {
}

ExprStmt::ExprStmt(Expr* expr) :
    expr(expr) {
}

IfStmt::IfStmt(Expr* cond, Stmt* iftrue, Stmt* iffalse) :
    cond(cond),
    iftrue(iftrue),
    iffalse(iffalse) {
}

CaseBaseStmt::CaseBaseStmt(Stmt* body) :
    body(body) {
}

CaseStmt::CaseStmt(std::vector<Expr*> exprs, Stmt* body) :
    CaseBaseStmt(body),
    exprs(std::move(exprs)) {
}

DefaultCaseStmt::DefaultCaseStmt(Stmt* body) :
    CaseBaseStmt(body) {
}

SwitchStmt::SwitchStmt(Expr* expr, std::vector<CaseBaseStmt*> cases) :
    expr_(expr),
    cases(std::move(cases)) {
}

LoopStmt::LoopStmt(Expr* cond, Stmt* body) :
    cond(cond),
    body(body) {
}

WhileStmt::WhileStmt(Expr* cond, Stmt* body) :
    LoopStmt(cond, body) {
}

DoWhileStmt::DoWhileStmt(Expr* cond, Stmt* body) :
    LoopStmt(cond, body) {
}

ForStmt::ForStmt(Stmt* init, Expr* cond, Stmt* step, Stmt* body) :
    LoopStmt(cond, body),
    init(init),
    step(step) {
}

ForEachStmt::ForEachStmt(LocalVarDefStmt* var, Expr* container, Stmt* body) :
    var(var),
    container(container),
    body(body) {
}

ThrowStmt::ThrowStmt(Expr* val) :
    val(val) {
}

CatchStmt::CatchStmt(LocalVarDefStmt* param, Stmt* body) :
    param(param),
    body(body) {
}

TryStmt::TryStmt(Stmt* body, Stmt* finally, std::vector<CatchStmt*> catches) :
    body(body),
    finally(finally),
    catches(std::move(catches)) {
}

} // namespace astfri
