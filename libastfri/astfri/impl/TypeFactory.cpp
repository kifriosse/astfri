#include <astfri/impl/TypeFactory.hpp>
#include <astfri/impl/Utils.hpp>

#include <cassert>


namespace astfri {


TypeFactory &TypeFactory::get_instance() {
    static TypeFactory instance;
    return instance;
}

IntType *TypeFactory::mk_int() {
    return &m_intType;
}

FloatType *TypeFactory::mk_float() {
    return &m_floatType;
}

CharType *TypeFactory::mk_char() {
    return &m_charType;
}

BoolType *TypeFactory::mk_bool() {
    return &m_boolType;
}

VoidType *TypeFactory::mk_void() {
    return &m_voidType;
}

UnknownType *TypeFactory::mk_unknown() {
    return &m_unknownType;
}

DynamicType *TypeFactory::mk_dynamic() {
    return &m_dynamicType;
}

IndirectionType *TypeFactory::mk_indirect(Type *type) {
    return details::get_or_emplace<IndirectionType>(
        m_indirectTypeMap,
        type,
        [type](){
            IndirectionType i;
            i.indirect = type;
            return i;
        });
}

ClassType *TypeFactory::mk_class(const std::string &name, const Scope &scope) {
    return this->mk_class(name, scope, nullptr);
}

ClassType *TypeFactory::mk_class(const std::string &name, const Scope &scope, ClassDefStmt *def) {
    ClassType *t = details::get_or_emplace<ClassType>(
        m_classTypeMap,
        mk_fqn(scope, name),
        [&name, &scope, def](){
            ClassType t;
            t.name = name;
            t.scope = scope;
            t.def = def;
            return t;
        });

#ifndef NDEBUG
    if (t->def && def) {
        assert(t->def == def);
    }
#endif

    if (! t->def && def) {
        t->def = def;
    }

    return t;
}

InterfaceType *TypeFactory::mk_interface(const std::string &name, const Scope &scope) {
    return this->mk_interface(name, scope, nullptr);
}

InterfaceType *TypeFactory::mk_interface(
    const std::string &name,
    const Scope &scope,
    InterfaceDefStmt *def
) {
    InterfaceType *i = details::get_or_emplace<InterfaceType>(
        m_interfaceTypeMap,
        mk_fqn(scope, name),
        [&name, &scope, def](){
            InterfaceType i;
            i.name = name;
            i.scope = scope;
            i.def = def;
            return i;
        });

#ifndef NDEBUG
    if (i->def && def) {
        assert(i->def == def);
    }
#endif

    if (! i->def && def) {
        i->def = def;
    }

    return i;
}

LambdaType *TypeFactory::mk_lambda(std::string name, LambdaExpr *def) {
    return details::create_store_get<LambdaType>(
        m_otherTypes,
        [&name, def](LambdaType &l){
            l.name = std::move(name);
            l.def = def;
        });
}

DeducedType *TypeFactory::mk_deduced(Type *realType) {
    return details::get_or_emplace<DeducedType>(
        m_deducedTypeMap,
        realType,
        [realType](){
            DeducedType d;
            d.realType = realType;
            return d;
        });
}

IncompleteType *TypeFactory::mk_incomplete(const std::string &name) {
    return details::get_or_emplace<IncompleteType>(
        m_incompleteTypeMap,
        name,
        [&name](){
            IncompleteType i;
            i.name = name;
            return i;
        });
}


} // namespace astfri
