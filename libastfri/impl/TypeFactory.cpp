#include <libastfri/impl/Utils.hpp>
#include <libastfri/inc/TypeFactory.hpp>

namespace astfri
{

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
    return details::emplace_get<IndirectionType>(type, indirect_, type);
}

UserType* TypeFactory::mk_user(std::string const& name)
{
    return details::emplace_get<UserType>(name, user_, name);
}

} // namespace astfri
