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
    return &m_intType;
}

FloatType* TypeFactory::mk_float()
{
    return &m_floatType;
}

CharType* TypeFactory::mk_char()
{
    return &m_charType;
}

BoolType* TypeFactory::mk_bool()
{
    return &m_boolType;
}

VoidType* TypeFactory::mk_void()
{
    return &m_voidType;
}

UnknownType* TypeFactory::mk_unknown()
{
    return &m_unknownType;
}

DynamicType* TypeFactory::mk_dynamic()
{
    return &m_dynamicType;
}

IndirectionType* TypeFactory::mk_indirect(Type* type)
{
    assert(type != nullptr);
    return details::emplace_get<IndirectionType>(type, m_indirectTypeMap, type);
}

ClassType *TypeFactory::mk_class(const std::string &name, const Scope &scope)
{
    return this->mk_class(name, scope, nullptr);
}

ClassType *TypeFactory::mk_class(const std::string &name, const Scope &scope, ClassDefStmt *def)
{
    ClassType *t = details::emplace_get<ClassType>(
        mk_fqn(scope, name),
        m_classTypeMap,
        name,
        scope,
        def);

    if (!t->def && def) {
        t->def = def;
    }

#ifndef NDEBUG
    if (t->def && def) {
        assert(t->def == def);
    }
#endif

    return t;
}

InterfaceType *TypeFactory::mk_interface(const std::string &name, const Scope &scope)
{
    return this->mk_interface(name, scope, nullptr);
}

InterfaceType *TypeFactory::mk_interface(const std::string &name, const Scope &scope, InterfaceDefStmt *def)
{
    InterfaceType *i = details::emplace_get<InterfaceType>(
        mk_fqn(scope, name),
        m_interfaceTypeMap,
        name,
        scope,
        def);

    if (!i->def && def) {
        i->def = def;
    }

#ifndef NDEBUG
    if (i->def && def) {
        assert(i->def == def);
    }
#endif

    return i;
}

LambdaType* TypeFactory::mk_lambda(std::string name, LambdaExpr *def)
{
    return details::emplace_get<LambdaType>(
        m_otherTypes,
        std::move(name),
        def);
}

DeducedType *TypeFactory::mk_deduced(Type* realType)
{
    return details::emplace_get<DeducedType>(realType, m_deducedTypeMap, realType);
}

IncompleteType *TypeFactory::mk_incomplete(const std::string &name)
{
    return details::emplace_get<IncompleteType>(name, m_incompleteTypeMap, name);
}


} // namespace astfri
