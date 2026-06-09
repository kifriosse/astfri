#ifndef ASTFRI_IMPL_TYPE_KIND_HPP
#define ASTFRI_IMPL_TYPE_KIND_HPP

#include <astfri/impl/Kind.hpp>
#include <astfri/impl/TypeFwd.hpp>


namespace astfri {


enum class TypeKind {
    UNINITIALIZED = 0,
    Dynamic,
    Int,
    Float,
    Char,
    Bool,
    Void,
    Indirection,
    Class,
    Interface,
    Lambda,
    Deduced,
    Incomplete,
    Unknown,
};


template<> struct KindOf<DynamicType> {
    static constexpr TypeKind value{TypeKind::Dynamic};
};

template<> struct KindOf<IntType> {
    static constexpr TypeKind value{TypeKind::Int};
};

template<> struct KindOf<FloatType> {
    static constexpr TypeKind value{TypeKind::Float};
};

template<> struct KindOf<CharType> {
    static constexpr TypeKind value{TypeKind::Char};
};

template<> struct KindOf<BoolType> {
    static constexpr TypeKind value{TypeKind::Bool};
};

template<> struct KindOf<VoidType> {
    static constexpr TypeKind value{TypeKind::Void};
};

template<> struct KindOf<IndirectionType> {
    static constexpr TypeKind value{TypeKind::Indirection};
};

template<> struct KindOf<ClassType> {
    static constexpr TypeKind value{TypeKind::Class};
};

template<> struct KindOf<InterfaceType> {
    static constexpr TypeKind value{TypeKind::Interface};
};

template<> struct KindOf<LambdaType> {
    static constexpr TypeKind value{TypeKind::Lambda};
};

template<> struct KindOf<DeducedType> {
    static constexpr TypeKind value{TypeKind::Deduced};
};

template<> struct KindOf<IncompleteType> {
    static constexpr TypeKind value{TypeKind::Incomplete};
};

template<> struct KindOf<UnknownType> {
    static constexpr TypeKind value{TypeKind::Unknown};
};


} // namespace astfri

#endif
