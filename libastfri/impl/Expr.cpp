#include <libastfri/inc/Expr.hpp>

namespace astfri {
IntLiteralExpr::IntLiteralExpr(int val) :
    Literal(val) {
}

FloatLiteralExpr::FloatLiteralExpr(float val) :
    Literal(val) {
}

CharLiteralExpr::CharLiteralExpr(char val) :
    Literal(val) {
}

StringLiteralExpr::StringLiteralExpr(std::string val) :
    Literal(std::move(val)) {
}

BoolLiteralExpr::BoolLiteralExpr(bool val) :
    Literal(val) {
}

IfExpr::IfExpr(Expr* cond, Expr* iftrue, Expr* iffalse) :
    cond(cond),
    iftrue(iftrue),
    iffalse(iffalse) {
}

BinOpExpr::BinOpExpr(Expr* left, BinOpType op, Expr* right) :
    left(left),
    op(op),
    right(right) {
}

UnaryOpExpr::UnaryOpExpr(UnaryOpType op, Expr* arg) :
    op(op),
    arg(arg) {
}

ParamVarRefExpr::ParamVarRefExpr(std::string param) :
    param(std::move(param)) {
}

LocalVarRefExpr::LocalVarRefExpr(std::string var) :
    var(std::move(var)) {
}

MemberVarRefExpr::MemberVarRefExpr(Expr* owner, std::string member) :
    owner(owner),
    member(std::move(member)) {
}

GlobalVarRefExpr::GlobalVarRefExpr(std::string global) :
    global(std::move(global)) {
}

ClassRefExpr::ClassRefExpr(std::string name) :
    name(std::move(name)) {
}

FunctionCallExpr::FunctionCallExpr(std::string name, std::vector<Expr*> args) :
    name(std::move(name)),
    args(std::move(args)) {
}

MethodCallExpr::MethodCallExpr(Expr* owner, std::string name, std::vector<Expr*> args) :
    owner(owner),
    name(std::move(name)),
    args(std::move(args)) {
}

LambdaExpr::LambdaExpr(std::vector<ParamVarDefStmt*> params, Stmt* body) :
    params(std::move(params)),
    body(body) {
}

ConstructorCallExpr::ConstructorCallExpr(Type* type, std::vector<Expr*> args) :
    type(type),
    args(std::move(args)) {
}

NewExpr::NewExpr(ConstructorCallExpr* init) :
    init(init) {
}

BracketExpr::BracketExpr(Expr* e) :
    expr(e) {
}

DeleteExpr::DeleteExpr(Expr* arg) :
    arg(arg) {
}

} // namespace astfri
