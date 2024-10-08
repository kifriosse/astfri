#include <libastfri/structures/Type.hpp>

namespace astfri
{

Type::Type(std::string name) : name(std::move(name))
{
}

PrimitiveType::PrimitiveType(std::string name) : Type(std::move(name))
{
}

IntType::IntType() : PrimitiveType("int")
{
}

FloatType::FloatType() : PrimitiveType("float")
{
}

CharType::CharType() : PrimitiveType("char")
{
}

BoolType::BoolType() : PrimitiveType("bool")
{
}

VoidType::VoidType() : PrimitiveType("void")
{
}

UserType::UserType(std::string name) : Type(std::move(name))
{
}

} // namespace libastfri::structures