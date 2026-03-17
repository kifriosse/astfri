#ifndef LIBASTFRI_TYPE_HPP
#define LIBASTFRI_TYPE_HPP

#include <libastfri/impl/Scope.hpp>
#include <libastfri/inc/Visitor.hpp>

#include <string>

namespace astfri
{


/**
 * @brief TODO
 */
struct Type : virtual Visitable
{
    virtual ~Type() = default;
};

/**
 * @brief TODO
 */
struct DynamicType : Type, details::MkVisitable<DynamicType>
{
};

/**
 * @brief TODO
 */
struct PrimitiveType : Type
{
};

/**
 * @brief TODO
 */
struct IntType : PrimitiveType, details::MkVisitable<IntType>
{
};

/**
 * @brief TODO
 */
struct FloatType : PrimitiveType, details::MkVisitable<FloatType>
{
};

/**
 * @brief TODO
 */
struct CharType : PrimitiveType, details::MkVisitable<CharType>
{
};

/**
 * @brief TODO
 */
struct BoolType : PrimitiveType, details::MkVisitable<BoolType>
{
};

/**
 * @brief TODO
 */
struct VoidType : PrimitiveType, details::MkVisitable<VoidType>
{
};

/**
 * @brief TODO
 */
struct IndirectionType : Type, details::MkVisitable<IndirectionType>
{
    Type* indirect;
    explicit IndirectionType(Type* indirect);
};

/**
 * @brief TODO
 */
struct ScopedType : Type
{
    std::string name;
    Scope scope;
    ScopedType(std::string name, Scope scope);
};

/**
 * @brief TODO
 */
struct ClassType : ScopedType, details::MkVisitable<ClassType>
{
    ClassDefStmt *def;
    ClassType(std::string name, Scope scope, ClassDefStmt *def);
};

/**
 * @brief TODO
 */
struct InterfaceType : ScopedType, details::MkVisitable<InterfaceType>
{
    InterfaceDefStmt *def;
    InterfaceType(std::string name, Scope scope, InterfaceDefStmt *def);
};

/**
 * @brief TODO
 */
struct LambdaType : Type, details::MkVisitable<LambdaType>
{
    std::string name;
    LambdaExpr *def;
    LambdaType(std::string name, LambdaExpr *def);
};

/**
 * @brief Represents type deduced by compiler.
 * Not to be confused with @c DynamicType. Can be used as type of variables declared using
 * keywords such as @c var, @c auto, @c let, ...
 */
struct DeducedType : Type, details::MkVisitable<DeducedType>
{
    /**
     * @brief Real type if available, otherwise @c Type::NULL.
     */
    Type *realType;

    /**
     * @brief Constructor.
     * @param realType Real type if available, otherwise @c Type::NULL.
     */
    explicit DeducedType(Type* realType);
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
struct IncompleteType : Type, details::MkVisitable<IncompleteType>
{
    /**
     * @brief Name of the type.
     */
    std::string name;

    /**
     * @brief Constructor, initializes the name.
     * @param name Name of the type.
     */
    explicit IncompleteType(std::string name);
};

/**
 * @brief TODO, tento pojde prec, namiesto neho bude nullptr a test to vyhlasi za bug
 */
struct UnknownType : PrimitiveType, details::MkVisitable<VoidType>
{
};


} // namespace astfri

#endif
