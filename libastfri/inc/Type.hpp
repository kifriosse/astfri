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
struct Type : virtual IVisitable
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
    Type* indirect_;
    explicit IndirectionType(Type* indirect);
};

/**
 * @brief TODO
 */
struct ScopedType : Type
{
    std::string name_;
    Scope scope_;
    ScopedType(std::string name, Scope scope);
};

/**
 * @brief TODO
 */
struct ClassType : ScopedType, details::MkVisitable<ClassType>
{
    ClassDefStmt *m_def;
    ClassType(std::string name, Scope scope, ClassDefStmt *def);
};

/**
 * @brief TODO
 */
struct InterfaceType : ScopedType, details::MkVisitable<InterfaceType>
{
    InterfaceDefStmt *m_def;
    InterfaceType(std::string name, Scope scope, InterfaceDefStmt *def);
};

/**
 * @brief TODO
 */
struct LambdaType : Type, details::MkVisitable<LambdaType>
{
    std::string m_name;
    LambdaExpr *m_def;
    LambdaType(std::string name, LambdaExpr *def);
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
 * @brief TODO
 */
struct UnknownType : PrimitiveType, details::MkVisitable<VoidType>
{
};

} // namespace astfri

#endif
