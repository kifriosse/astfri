#include <libastfri/structures/Expression.hpp>

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

BinaryExpression::BinaryExpression(
    Expression* left,
    BinaryOperators op,
    Expression* right
) :
    left(left),
    op(op),
    right(right)
{
}

UnaryExpression::UnaryExpression(UnaryOperators op, Expression* arg) :
    op(op),
    arg(arg)
{
}

ParamRefExpression::ParamRefExpression(ParameterDefinition* parameter) :
    parameter(parameter)
{
}

VarRefExpression::VarRefExpression(VariableDefintion* variable) :
    variable(variable)
{
}

FunctionCallExpression::FunctionCallExpression(
    std::string functionName,
    std::vector<Expression*> arguments
) :
    functionName(std::move(functionName)),
    arguments(std::move(arguments))
{
}

UnknownExpression::UnknownExpression(std::string message) :
    message(std::move(message))
{
}

} // namespace libastfri::structures
