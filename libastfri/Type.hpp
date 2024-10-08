#ifndef LIBASTFRI_TYPE_HPP
#define LIBASTFRI_TYPE_HPP

#include <libastfri/Visitor.hpp>
#include <libastfri/impl/Utils.hpp>

#include <string>

namespace astfri
{
struct Type : virtual IVisitable
{
    std::string name_;
    Type(std::string name);
};

struct PrimitiveType : Type
{
    PrimitiveType(std::string name);
};

struct IntType : PrimitiveType, details::MakeVisitable<IntType>
{
    IntType();
};

struct FloatType : PrimitiveType, details::MakeVisitable<FloatType>
{
    FloatType();
};

struct CharType : PrimitiveType, details::MakeVisitable<CharType>
{
    CharType();
};

struct BoolType : PrimitiveType, details::MakeVisitable<BoolType>
{
    BoolType();
};

struct VoidType : PrimitiveType, details::MakeVisitable<VoidType>
{
    VoidType();
};

struct UserType : Type, details::MakeVisitable<UserType>
{
    UserType(std::string name);
};
} // namespace astfri

#endif