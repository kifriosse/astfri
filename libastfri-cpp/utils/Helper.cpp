#include <libastfri/structures/Expression.hpp>

#include <libastfri/utils/Helper.hpp>

namespace astfri_cpp
{

std::string Helper::convertBinaryOperator(BinaryOperators op)
{
    switch (op)
    {
    case BinaryOperators::Assign:
        return "=";
    case BinaryOperators::Add:
        return "+";
    case BinaryOperators::Subtract:
        return "-";
    case BinaryOperators::Multiply:
        return "*";
    case BinaryOperators::Divide:
        return "/";
    case BinaryOperators::Modulo:
        return "%";
    case BinaryOperators::Less:
        return "<";
    case BinaryOperators::LessEqual:
        return "<=";
    case BinaryOperators::Greater:
        return ">";
    case BinaryOperators::GreaterEqual:
        return ">=";
    case BinaryOperators::Equal:
        return "==";
    case BinaryOperators::NotEqual:
        return "!=";
    default:
        return "";
    }
}

std::string Helper::convertUnaryOperator(UnaryOperators op)
{
    switch (op)
    {
    case UnaryOperators::Negative:
        return "-";
    case UnaryOperators::Not:
        return "!";
    default:
        return "";
    }
}

} // namespace libastfri::utils
