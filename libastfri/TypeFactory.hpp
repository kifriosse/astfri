#ifndef LIBASTFRI_TYPE_FACTORY
#define LIBASTFRI_TYPE_FACTORY

#include <libastfri/Type.hpp>

#include <map>
#include <string>

namespace astfri
{
class TypeFactory
{
public:
    static TypeFactory& getInstance ();

private:
    IntType intType;
    FloatType floatType;
    CharType charType;
    BoolType boolType;
    VoidType voidType;

    std::map<std::string, UserType> userTypes;

    TypeFactory()  = default;
    ~TypeFactory() = default;

public:
    IntType* getIntType ();
    FloatType* getFloatType ();
    CharType* getCharType ();
    BoolType* getBoolType ();
    VoidType* getVoidType ();
    UserType* getUserType (std::string const& name);

    TypeFactory(TypeFactory const&)     = delete;
    void operator= (TypeFactory const&) = delete;
};
} // namespace astfri

#endif