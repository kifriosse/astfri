#ifndef LIBASTFRI_TYPE_HPP
#define LIBASTFRI_TYPE_HPP

#include <libastfri/impl/Utils.hpp>

#include <string>
#include <vector>

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
    explicit IndirectionType(Type* indirect); // TODO remove
};

/**
 * @brief TODO
 * @deprecated Deprecated in favor of more specific representation of user types
 */
struct UserType : Type, details::MkVisitable<UserType>
{
    std::string name_;

    explicit UserType(std::string name);
};

/**
 * @brief TODO
 */
struct Scope
{
    std::vector<std::string> names_;
};

/**
 * @brief TODO
 */
struct ScopedType : Type
{
    Scope scope_;
    std::string name_;
};

/**
 * @brief TODO
 */
struct ClassType : ScopedType, details::MkVisitable<ClassType>
{
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
