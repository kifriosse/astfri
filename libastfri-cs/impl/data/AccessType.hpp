#ifndef CSHARP_ACCESS_TYPE_HPP
#define CSHARP_ACCESS_TYPE_HPP

#include <variant>

namespace astfri
{

struct Expr;
struct UserTypeDefStmt;

} // namespace astfri

namespace astfri::csharp
{

namespace access
{

/**
 * @brief When member access doesn't have \c this, \c base or \c ClassRef prefix
 */
struct None
{
};

/**
 * @brief When member access has \c this prefix
 */
struct Instance
{
};

/**
 * @brief When member access has \c ClassRef prefix
 */
struct Static
{
    UserTypeDefStmt* owner{nullptr};
};

/**
 * @brief When member access has \c base prefix
 */
struct Base
{
    UserTypeDefStmt* parent{nullptr};
};

/**
 * @brief When member access is on expression of unknown type
 */
struct Unknown
{
    Expr* leftSide{nullptr}; // todo might be useless
};

/**
 * @brief Qualifier of member access - \code this, base, ClassRef, expr \endcode
 */
using Qualifier = std::variant<None, Instance, Static, Base, Unknown>;

} // namespace access

// todo redo this into a std::variant like MemberAccessType
/**
 * @brief Type of a function call
 */
enum class CallType
{
    Unknown,
    Method,
    LocalFunc,
    Delegate,
    StaticMethod
};

} // namespace astfri::csharp

#endif // CSHARP_ACCESS_TYPE_HPP
