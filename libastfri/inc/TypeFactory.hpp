#ifndef LIBASTFRI_TYPE_FACTORY
#define LIBASTFRI_TYPE_FACTORY

#include <libastfri/inc/Type.hpp>

#include <map>
#include <string>

namespace astfri
{
class TypeFactory
{
public:
    static TypeFactory& get_instance ();

public:
    IntType* mk_int ();
    FloatType* mk_float ();
    CharType* mk_char ();
    BoolType* mk_bool ();
    VoidType* mk_void ();
    UnknownType* mk_unknown ();
    IndirectionType* mk_indirect (Type* type);
    UserType* mk_user (std::string const& name);

public:
    TypeFactory(TypeFactory const&)     = delete;
    void operator= (TypeFactory const&) = delete;

private:
    TypeFactory() = default;

private:
    IntType int_;
    FloatType float_;
    CharType char_;
    BoolType bool_;
    VoidType void_;
    UnknownType unknown_;
    DynamicType dynamic_;
    std::map<Type*, IndirectionType> indirect_;
    std::map<std::string, UserType> user_;
};
} // namespace astfri

#endif