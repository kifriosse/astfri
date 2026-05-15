#ifndef ASTFRI_IMPL_TYPE_DEF_HPP
#define ASTFRI_IMPL_TYPE_DEF_HPP

#include <astfri/impl/ASTNode.hpp>
#include <astfri/impl/Scope.hpp>
#include <astfri/impl/TypeKind.hpp>

#include <string>


namespace astfri {


/**
 * @brief TODO
 */
struct Type : detail::ASTNode<TypeKind> {
};


/**
 * @brief Helper CRTP base. Indirectly inherits @c SelfType from @c Type.
 *
 * See @c MakeA doc for more details.
 *
 * @tparam SelfType child class type.
 */
template<typename SelfType>
using MakeAType = detail::MakeA<Type, SelfType>;


/**
 * @brief TODO
 */
struct DynamicType : MakeAType<DynamicType> { };

/**
 * @brief TODO
 */
struct IntType : MakeAType<IntType> { };

/**
 * @brief TODO
 */
struct FloatType : MakeAType<FloatType> { };

/**
 * @brief TODO
 */
struct CharType : MakeAType<CharType> { };

/**
 * @brief TODO
 */
struct BoolType : MakeAType<BoolType> { };

/**
 * @brief TODO
 */
struct VoidType : MakeAType<VoidType> { };

/**
 * @brief TODO
 */
struct IndirectionType : MakeAType<IndirectionType> {
    Type *indirect{nullptr};
};

/**
 * @brief TODO
 */
struct ScopedType : Type {
    std::string name{};
    Scope scope{};
};

/**
 * @brief TODO
 */
struct ClassType : ScopedType {
    ClassDefStmt *def{nullptr};
    ASTFRI_ADD_NODE_CONSTRUCTOR(ClassType)
};

/**
 * @brief TODO
 */
struct InterfaceType : ScopedType {
    InterfaceDefStmt *def{nullptr};
    ASTFRI_ADD_NODE_CONSTRUCTOR(InterfaceType)
};

/**
 * @brief TODO
 */
struct LambdaType : MakeAType<LambdaType> {
    std::string name;
    LambdaExpr *def;
};

/**
 * @brief Represents type deduced by compiler.
 * Not to be confused with @c DynamicType. Can be used as type of variables declared using
 * keywords such as @c var, @c auto, @c let, ...
 */
struct DeducedType : MakeAType<DeducedType> {
    /**
     * @brief Real type if available, otherwise @c Type::NULL.
     */
    Type* realType;
};

// /**
//  * @brief TODO
//  */
// struct EnumType : ScopedType, details::MkVisitable<EnumType>
// {
// };

// /**
//  * @brief Product type
//  */
// struct RecordType : ScopedType, details::MkVisitable<RecordType>
// {
//     std::vector<Type*> types_;
// };

// /**
//  * @brief Sum type
//  */
// struct UnionType : ScopedType, details::MkVisitable<UnionType>
// {
//     std::vector<Type*> types_; // TODO otazka ci by mali mat aj typy
// };

/**
 * @brief Represents type that is unknown. Only its name is available.
 */
struct IncompleteType : MakeAType<IncompleteType> {
    /**
     * @brief Name of the type.
     */
    std::string name;
};

/**
 * @brief TODO, tento pojde prec, namiesto neho bude nullptr a test to vyhlasi za bug
 */
struct UnknownType : MakeAType<UnknownType> {
};


} // namespace astfri

#endif
