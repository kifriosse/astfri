#include <libastfri/impl/Utils.hpp>
#include <libastfri/inc/TypeFactory.hpp>

#include <cassert>

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
    assert(type != nullptr);
    return details::emplace_get<IndirectionType>(type, indirect_, type);
}

// UserType* TypeFactory::mk_user(std::string const& name)
// {
    // return details::emplace_get<UserType>(name, user_, name);
// }

ClassType *TypeFactory::mk_class(const std::string &name, const Scope &scope)
{
    return mk_class(name, scope, nullptr);
}

ClassType *TypeFactory::mk_class(const std::string &name, const Scope &scope, ClassDefStmt *def)
{
    ClassType *t = details::emplace_get<ClassType>(
        mk_fqn(scope, name),
        m_classes,
        name,
        scope,
        def);
    if (!t->m_def && def) {
        t->m_def = def;
    }
    if (t->m_def && def) {
        assert(t->m_def == def);
    }
    return t;
}

InterfaceType *TypeFactory::mk_interface(const std::string &name, const Scope &scope)
{
    return mk_interface(name, scope, nullptr);
}

InterfaceType *TypeFactory::mk_interface(const std::string &name, const Scope &scope, InterfaceDefStmt *def)
{
    InterfaceType *i = details::emplace_get<InterfaceType>(
        mk_fqn(scope, name),
        m_interfaces,
        name,
        scope,
        def);
    if (!i->m_def && def) {
        i->m_def = def;
    }
    if (i->m_def && def) {
        assert(i->m_def == def);
    }
    return i;
}

LambdaType* TypeFactory::mk_lambda(std::string name, LambdaExpr *def)
{
    return details::emplace_get<LambdaType>(
        m_types,
        std::move(name),
        def);
}

} // namespace astfri
