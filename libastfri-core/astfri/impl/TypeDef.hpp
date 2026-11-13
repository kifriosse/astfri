#ifndef ASTFRI_IMPL_TYPE_DEF_HPP
#define ASTFRI_IMPL_TYPE_DEF_HPP

#include <astfri/impl/ASTNode.hpp>
#include <astfri/impl/ExprFwd.hpp>
#include <astfri/impl/Scope.hpp>
#include <astfri/impl/StmtFwd.hpp>
#include <astfri/impl/TypeInfo.hpp>
#include <astfri/impl/Visitor.hpp>

#include <string>


namespace astfri {


/**
 * @brief TODO
 */
struct Type : ASTNode {
};
ASTFRI_REGISTER_TYPE(Type,ASTNode)


/**
 * @brief TODO
 */
struct DynamicType : Type {
    DynamicType();
};
ASTFRI_REGISTER_TYPE(DynamicType,Type)
ASTFRI_ADD_NODE_CONSTRUCTOR(DynamicType)


/**
 * @brief TODO
 */
struct IntType : Type {
    IntType();
};
ASTFRI_REGISTER_TYPE(IntType,Type)
ASTFRI_ADD_NODE_CONSTRUCTOR(IntType)


/**
 * @brief TODO
 */
struct FloatType : Type {
    FloatType();
};
ASTFRI_REGISTER_TYPE(FloatType,Type)
ASTFRI_ADD_NODE_CONSTRUCTOR(FloatType)


/**
 * @brief TODO
 */
struct CharType : Type {
    CharType();
};
ASTFRI_REGISTER_TYPE(CharType,Type)
ASTFRI_ADD_NODE_CONSTRUCTOR(CharType)


/**
 * @brief TODO
 */
struct BoolType : Type {
    BoolType();
};
ASTFRI_REGISTER_TYPE(BoolType,Type)
ASTFRI_ADD_NODE_CONSTRUCTOR(BoolType)


/**
 * @brief TODO
 */
struct VoidType : Type {
    VoidType();
};
ASTFRI_REGISTER_TYPE(VoidType,Type)
ASTFRI_ADD_NODE_CONSTRUCTOR(VoidType)


/**
 * @brief TODO
 */
struct IndirectionType : Type {
    Type *indirect{nullptr};

    IndirectionType();
};
ASTFRI_REGISTER_TYPE(IndirectionType,Type)
ASTFRI_ADD_NODE_CONSTRUCTOR(IndirectionType)


/**
 * @brief TODO
 */
struct ScopedType : Type {
    std::string name{};
    Scope scope{};
};
ASTFRI_REGISTER_TYPE(ScopedType,Type)


/**
 * @brief TODO
 */
struct ClassType : ScopedType {
    ClassDefStmt *def{nullptr};

    ClassType();
};
ASTFRI_REGISTER_TYPE(ClassType,ScopedType)
ASTFRI_ADD_NODE_CONSTRUCTOR(ClassType)


/**
 * @brief TODO
 */
struct InterfaceType : ScopedType {
    InterfaceDefStmt *def{nullptr};

    InterfaceType();
};
ASTFRI_REGISTER_TYPE(InterfaceType,ScopedType)
ASTFRI_ADD_NODE_CONSTRUCTOR(InterfaceType)


/**
 * @brief TODO
 */
struct LambdaType : Type {
    std::string name{};
    LambdaExpr *def{nullptr};

    LambdaType();
};
ASTFRI_REGISTER_TYPE(LambdaType,Type)
ASTFRI_ADD_NODE_CONSTRUCTOR(LambdaType)


/**
 * @brief Represents type deduced by compiler.
 * Not to be confused with @c DynamicType. Can be used as type of variables declared using
 * keywords such as @c var, @c auto, @c let, ...
 */
struct DeducedType : Type {
    /**
     * @brief Real type if available, otherwise @c Type::NULL.
     */
    Type* realType{nullptr};

    DeducedType();
};
ASTFRI_REGISTER_TYPE(DeducedType,Type)
ASTFRI_ADD_NODE_CONSTRUCTOR(DeducedType)


// /**
//  * @brief TODO
//  */
// struct EnumType : ScopedType {
//      ASTFRI_ADD_NODE_CONSTRUCTOR(EnumType);
// };

// /**
//  * @brief Product type
//  */
// struct RecordType : ScopedType {
//     std::vector<Type*> types_;
//     ASTFRI_ADD_NODE_CONSTRUCTOR(RecordType);
// };

// /**
//  * @brief Sum type
//  */
// struct UnionType : ScopedType {
//     std::vector<Type*> types_; // TODO otazka ci by mali mat aj typy
//     ASTFRI_ADD_NODE_CONSTRUCTOR(UnionType);
// };

/**
 * @brief Represents type that is unknown. Only its name is available.
 */
struct IncompleteType : Type {
    /**
     * @brief Name of the type.
     */
    std::string name{};

    IncompleteType();
};
ASTFRI_REGISTER_TYPE(IncompleteType,Type)
ASTFRI_ADD_NODE_CONSTRUCTOR(IncompleteType)


/**
 * @brief TODO, tento pojde prec, namiesto neho bude nullptr a test to vyhlasi za bug
 */
struct UnknownType : Type {
    UnknownType();
};
ASTFRI_REGISTER_TYPE(UnknownType,Type)
ASTFRI_ADD_NODE_CONSTRUCTOR(UnknownType)


} // namespace astfri

#endif
