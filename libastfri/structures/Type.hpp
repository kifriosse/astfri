#ifndef LIBASTFRI_TYPE_HPP
#define LIBASTFRI_TYPE_HPP

#include <libastfri/utils/OutputVisitor.hpp>

#include <string>

namespace astfri
{
// typ
struct Type : virtual IOutputVisitable
{
    std::string name_;

    Type(std::string name);
};

//// primitivne typy
struct PrimitiveType : Type
{
    PrimitiveType(std::string name);
};

struct IntType : PrimitiveType, OutputVisitable<IntType>
{
    IntType();
};

struct FloatType : PrimitiveType, OutputVisitable<FloatType>
{
    FloatType();
};

struct CharType : PrimitiveType, OutputVisitable<CharType>
{
    CharType();
};

struct BoolType : PrimitiveType, OutputVisitable<BoolType>
{
    BoolType();
};

struct VoidType : PrimitiveType, OutputVisitable<VoidType>
{
    VoidType();
};

//// uzivatelske typy
struct UserType : Type, OutputVisitable<UserType>
{
    UserType(std::string name);
};
} // namespace libastfri::structures

#endif