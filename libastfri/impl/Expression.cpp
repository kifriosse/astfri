#include <libastfri/Expression.hpp>

namespace astfri
{

IntLiteral::IntLiteral(int value) : Literal(value)
{
}

FloatLiteral::FloatLiteral(float value) : Literal(value)
{
}

CharLiteral::CharLiteral(char value) : Literal(value)
{
}

StringLiteral::StringLiteral(std::string value) : Literal(std::move(value))
{
}

BoolLiteral::BoolLiteral(bool value) : Literal(value)
{
}

BinOpExpr::BinOpExpr(
    Expr* left,
    BinOpType op,
    Expr* right
) :
    left_(left),
    op_(op),
    right_(right)
{
}

UnaryOpExpr::UnaryOpExpr(UnaryOpType op, Expr* arg) :
    op_(op),
    arg_(arg)
{
}

ParamVarRefExpr::ParamVarRefExpr(std::string param) :
    param_(std::move(param))
{
}

LocalVarRefExpr::LocalVarRefExpr(std::string var) :
    var_(std::move(var))
{
}

MemberVarRefExpr::MemberVarRefExpr(std::string member) :
    member_(std::move(member))
{
}

FunctionCallExpr::FunctionCallExpr(
    std::string name,
    std::vector<Expr*> args
) :
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

UnknownExpr::UnknownExpr(std::string message) :
    message_(std::move(message))
{
}

} // namespace astfri
