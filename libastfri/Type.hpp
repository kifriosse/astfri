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
struct DynamicType : Type, details::MakeVisitable<DynamicType>
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
struct IntType : PrimitiveType, details::MakeVisitable<IntType>
{
};

/**
 * @brief TODO
 */
struct FloatType : PrimitiveType, details::MakeVisitable<FloatType>
{
};

/**
 * @brief TODO
 */
struct CharType : PrimitiveType, details::MakeVisitable<CharType>
{
};

/**
 * @brief TODO
 */
struct BoolType : PrimitiveType, details::MakeVisitable<BoolType>
{
};

/**
 * @brief TODO
 */
struct VoidType : PrimitiveType, details::MakeVisitable<VoidType>
{
};

/**
 * @brief TODO
 */
struct UnknownType : PrimitiveType, details::MakeVisitable<VoidType>
{
};

/**
 * @brief TODO
 */
struct UserType: Type, details::MakeVisitable<UserType>
{
    std::string name_;

    UserType(std::string name);
};

/**
 * @brief TODO
 */
struct IndirectionType : Type, details::MakeVisitable<IndirectionType>
{
    Type* indirect_;

    IndirectionType(Type* indirect);
};
} // namespace astfri

#endif