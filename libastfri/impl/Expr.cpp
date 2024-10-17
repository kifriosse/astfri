#include <libastfri/inc/Expr.hpp>

namespace astfri
{
IntLiteralExpr::IntLiteralExpr(int val) : Literal(val) { }

FloatLiteralExpr::FloatLiteralExpr(float val) : Literal(val) { }

CharLiteralExpr::CharLiteralExpr(char val) : Literal(val) { }

StringLiteralExpr::StringLiteralExpr(std::string val) : Literal(std::move(val))
{
}

BoolLiteralExpr::BoolLiteralExpr(bool val) : Literal(val) { }

IfExpr::IfExpr(Expr* cond, Expr* iftrue, Expr* iffalse) :
    cond_(cond),
    iftrue_(iftrue),
    iffalse_(iffalse)
{
}

BinOpExpr::BinOpExpr(Expr* left, BinOpType op, Expr* right) :
    left_(left),
    op_(op),
    right_(right)
{
}

UnaryOpExpr::UnaryOpExpr(UnaryOpType op, Expr* arg) : op_(op), arg_(arg) { }

AssignExpr::AssignExpr(Expr* lhs, Expr* rhs) : lhs_(lhs), rhs_(rhs) { }

CompoundAssignExpr::CompoundAssignExpr(Expr* lhs, BinOpType op, Expr* rhs) :
    lhs_(lhs),
    op_(op),
    rhs_(rhs)
{
}

ParamVarRefExpr::ParamVarRefExpr(std::string param) : param_(std::move(param))
{
}

LocalVarRefExpr::LocalVarRefExpr(std::string var) : var_(std::move(var)) { }

MemberVarRefExpr::MemberVarRefExpr(std::string member) :
    member_(std::move(member))
{
}

GlobalVarRefExpr::GlobalVarRefExpr(std::string global) :
    global_(std::move(global))
{
}

FunctionCallExpr::FunctionCallExpr(std::string name, std::vector<Expr*> args) :
    name_(std::move(name)),
    args_(std::move(args))
{
}

MethodCallExpr::MethodCallExpr(
    Expr* owner,
    std::string name,
    std::vector<Expr*> args
) :
    owner_(owner),
    name_(std::move(name)),
    args_(std::move(args))
{
}

LambdaExpr::LambdaExpr(std::vector<ParamVarDefStmt*> params, Stmt* body) :
    params_(std::move(params)),
    body_(body)
{
}
} // namespace astfri
