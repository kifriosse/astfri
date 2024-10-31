#ifndef LIBASTFRI_TYPE_HPP
#define LIBASTFRI_TYPE_HPP

#include <libastfri/impl/TypeQueries.hpp>
#include <libastfri/impl/Utils.hpp>

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
struct DynamicType :
    Type,
    details::MkVisitable<DynamicType>,
    details::TypeQueriesAdapter
{
    DynamicType* as_dynamic () noexcept override;
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
struct IntType :
    PrimitiveType,
    details::MkVisitable<IntType>,
    details::TypeQueriesAdapter
{
    IntType* as_int () noexcept override;
};

/**
 * @brief TODO
 */
struct FloatType :
    PrimitiveType,
    details::MkVisitable<FloatType>,
    details::TypeQueriesAdapter
{
    FloatType* as_float () noexcept override;
};

/**
 * @brief TODO
 */
struct CharType :
    PrimitiveType,
    details::MkVisitable<CharType>,
    details::TypeQueriesAdapter
{
    CharType* as_char () noexcept override;
};

/**
 * @brief TODO
 */
struct BoolType :
    PrimitiveType,
    details::MkVisitable<BoolType>,
    details::TypeQueriesAdapter
{
    BoolType* as_bool () noexcept override;
};

/**
 * @brief TODO
 */
struct VoidType :
    PrimitiveType,
    details::MkVisitable<VoidType>,
    details::TypeQueriesAdapter
{
    VoidType* as_void () noexcept override;
};

/**
 * @brief TODO
 */
struct UserType :
    Type,
    details::MkVisitable<UserType>,
    details::TypeQueriesAdapter
{
    std::string name_;

    explicit UserType(std::string name);

    UserType* as_user () noexcept override;
};

/**
 * @brief TODO
 */
struct IndirectionType :
    Type,
    details::MkVisitable<IndirectionType>,
    details::TypeQueriesAdapter
{
    Type* indirect_;

    explicit IndirectionType(Type* indirect);

    IndirectionType* as_indirection () noexcept override;
};

/**
 * @brief TODO
 */
struct UnknownType : PrimitiveType, details::MkVisitable<VoidType>
{
};
} // namespace astfri

#endif