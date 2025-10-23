#include <libastfri/impl/Utils.hpp>
#include <libastfri/inc/TypeFactory.hpp>

#include <cassert>

namespace astfri
{

namespace
{

std::string scope_to_key(const std::string &name, const Scope &scope)
{
    std::string key = name;
    for (const std::string &s : scope.names_) {
        key += "::";
        key += s;
    }
    return key;
}

}

TypeFactory& TypeFactory::get_instance()
{
    static TypeFactory instance;
    return instance;
}

IntType* TypeFactory::mk_int()
{
    return &int_;
}

FloatType* TypeFactory::mk_float()
{
    return &float_;
}

CharType* TypeFactory::mk_char()
{
    return &char_;
}

BoolType* TypeFactory::mk_bool()
{
    return &bool_;
}

VoidType* TypeFactory::mk_void()
{
    return &void_;
}

UnknownType* TypeFactory::mk_unknown()
{
    return &unknown_;
}

DynamicType* TypeFactory::mk_dynamic()
{
    return &dynamic_;
}

IndirectionType* TypeFactory::mk_indirect(Type* type)
{
    assert(type != nullptr);
    return details::emplace_get<IndirectionType>(type, indirect_, type);
}

UserType* TypeFactory::mk_user(std::string const& name)
{
    return details::emplace_get<UserType>(name, user_, name);
}

ClassType *TypeFactory::mk_class(std::string name, Scope scope)
{
    std::string key = scope_to_key(name, scope);
    ClassType *t = details::emplace_get<ClassType>(std::move(key), classes_);
    t->name_ = std::move(name);
    t->scope_ = std::move(scope);
    return t;
}

} // namespace astfri
