#ifndef LIBASTFRI_TYPE_HPP
#define LIBASTFRI_TYPE_HPP

#include <libastfri/impl/Utils.hpp>
#include <libastfri/Visitor.hpp>

#include <string>

namespace astfri
{
/**
 * @brief TODO
 */
struct Type
{
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
struct UnknownType : PrimitiveType, details::MkVisitable<VoidType>
{
};

/**
 * @brief TODO
 */
struct UserType: Type, details::MkVisitable<UserType>
{
    std::string name_;

    UserType(std::string name);
};

/**
 * @brief TODO
 */
struct IndirectionType : Type, details::MkVisitable<IndirectionType>
{
    Type* indirect_;

    IndirectionType(Type* indirect);
};
} // namespace astfri

#endif