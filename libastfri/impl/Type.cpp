#include <libastfri/Type.hpp>

namespace astfri
{
DynamicType* DynamicType::as_dynamic() noexcept
{
    return this;
}

IntType* IntType::as_int() noexcept
{
    return this;
}

FloatType* FloatType::as_float() noexcept
{
    return this;
}

CharType* CharType::as_char() noexcept
{
    return this;
}

BoolType* BoolType::as_bool() noexcept
{
    return this;
}

VoidType* VoidType::as_void() noexcept
{
    return this;
}

UserType* UserType::as_user() noexcept
{
    return this;
}

IndirectionType* IndirectionType::as_indirection() noexcept
{
    return this;
}

UserType::UserType(std::string name) :
    name_(std::move(name))
{
}

IndirectionType::IndirectionType(Type* const indirect) :
    indirect_(indirect)
{
}
} // namespace astfri