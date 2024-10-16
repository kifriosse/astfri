#include <libastfri/TypeFactory.hpp>

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

IndirectionType* TypeFactory::mk_indirect(Type* type)
{
    const auto it = indirect_.find(type);
    if (it != indirect_.end())
    {
        return &it->second;
    }
    return &indirect_.try_emplace(type, type).first->second;
}

UserType* TypeFactory::mk_user(const std::string& name)
{
    const auto it = user_.find(name);
    if (it != user_.end())
    {
        return &it->second;
    }
    return &user_.try_emplace(name, name).first->second;
}
} // namespace astfri
