#ifndef LIBASTFRI_EXPRESSION_HPP
#define LIBASTFRI_EXPRESSION_HPP

#include <libastfri/Declaration.hpp>
#include <libastfri/Visitor.hpp>
#include <libastfri/impl/Utils.hpp>

#include <string>
#include <vector>

namespace astfri
{
struct VariableDefintion;
struct ParameterDefinition;

// vyraz
struct Expression : virtual IVisitable
{
};

//// konstaty (literals) pouzivane v vyrazoch su vlastne tiez vyrazy
template<typename T>
struct Literal : Expression
{
    T value;

    Literal(T value) : value(value)
    {
    }
};

struct IntLiteral : Literal<int>, details::MakeVisitable<IntLiteral>
{
    IntLiteral(int value);
};

struct FloatLiteral : Literal<float>, details::MakeVisitable<FloatLiteral>
{
    FloatLiteral(float value);
};

struct CharLiteral : Literal<char>, details::MakeVisitable<CharLiteral>
{
    CharLiteral(char value);
};

struct StringLiteral :
    Literal<std::string>,
    details::MakeVisitable<StringLiteral>
{
    StringLiteral(std::string value);
};

struct BoolLiteral : Literal<bool>, details::MakeVisitable<BoolLiteral>
{
    BoolLiteral(bool value);
};

//// operatory pouzivane v vyrazoch
enum class BinaryOperators
{
    Assign,
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual
};
enum class UnaryOperators
{
    Not,
    Negative,
    GetValue
};

//// vyrazy
struct BinaryExpression : Expression, details::MakeVisitable<BinaryExpression>
{
    Expression* left;
    BinaryOperators op;
    Expression* right;

    BinaryExpression(Expression* left, BinaryOperators op, Expression* right);
};

struct UnaryExpression : Expression, details::MakeVisitable<UnaryExpression>
{
    UnaryOperators op;
    Expression* arg;

    UnaryExpression(UnaryOperators op, Expression* arg);
};

//// referencie na premenne
struct RefExpression : Expression
{
};

struct ParamRefExpression :
    RefExpression,
    details::MakeVisitable<ParamRefExpression>
{
    ParameterDefinition* parameter;

    ParamRefExpression(ParameterDefinition* parameter);
};

struct VarRefExpression :
    RefExpression,
    details::MakeVisitable<VarRefExpression>
{
    VariableDefintion* variable;

    VarRefExpression(VariableDefintion* variable);
};

// volanie funkcie
struct FunctionCallExpression :
    RefExpression,
    details::MakeVisitable<FunctionCallExpression>
{
    // FunctionDefinition* function;
    std::string functionName;
    std::vector<Expression*> arguments;

    FunctionCallExpression(
        std::string functionName,
        std::vector<Expression*> arguments = {}
    );
};

struct UnknownExpression : Expression, details::MakeVisitable<UnknownExpression>
{
    std::string message;

    UnknownExpression(std::string message);
};

} // namespace libastfri::structures

#endif