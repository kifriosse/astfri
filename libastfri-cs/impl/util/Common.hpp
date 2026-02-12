/**
 * @file Common.hpp
 * @brief Common utility functions and enums
 */

#ifndef CSHARP_COMMON_HPP
#define CSHARP_COMMON_HPP

#include <cstddef>
// ReSharper disable once CppUnusedIncludeDirective
#include <functional>

namespace astfri::csharp::util
{

enum class TypeKind
{
    Class,
    Interface,
    Record,
    Enum,
    Delegate
};

enum class VarDefType
{
    Local,
    Member,
    Global
};

/**
 * @brief Search scope for type resolution. This is used to determine where to
 * look for a name when resolving an identifier
 */
enum class SearchScope
{
    /**
     * @brief local alias in C#
     */
    LocalAlias,
    /**
     * @brief static using in C#
     */
    LocalStaticUsing,
    /**
     * @brief reference to user defined type inside a type member.
     * This includes:
     * - Methods - body, parameters, return type
     * - Member variable definitions
     * - Properties
     * - Indexers
     * - Intehitance/implementation
     *
     * or delegate definition e.g. \code Func<int, int>\endcode
     */
    UserTypeRef,
    /**
     * @brief basic using directive in C#. For example,
     * \code using System;\endcode
     */
    FileUsing,
    /**
     * @brief global alias in C#.
     */
    GlobAlias,
    /**
     * @brief global static using in C#.
     */
    GlobStaticUsing,
    /**
     * @brief global using directive in C#.
     */
    GlobUsing
};

/**
 * @brief Suffix type of integer literal
 */
enum class IntSuffix
{
    /**
     * @brief No suffix - basic integer type
     */
    None,
    /**
     * @brief Unsigned integer type
     */
    U,
    /**
     * @brief Long integer type
     */
    L,
    /**
     * @brief Unsigned long integer type
     */
    UL,
};

/**
 * @brief Combines the hash of a value into an existing hash seed.
 * @param seed The existing hash seed to combine into.
 * @param v The value to hash and combine.
 * Implementation based on `hash_combine` in Boost library.
 * Source:
 * https://www.boost.org/doc/libs/1_54_0/doc/html/hash/reference.html#boost.hash_combine
 */
template<typename T>
void hash_combine(size_t& seed, const T& v);

/**
 * @brief Helper struct to create overloaded function objects from multiple
 * lambdas, functions or functors. Mostly used for \c std::visit with
 * \c std::variant .
 * @code{.cpp}
auto visitor = astfri::csharp::util::overloaded{
   [](int i) { //... },
   [](double d) { //... },
   [](const std::string& s) { //... }
};
   @endcode
 * @tparam Ts Types of callables (lambdas, functions, functors) to combine.
 * @note Source: https://en.cppreference.com/w/cpp/utility/variant/visit2.html
 */
template<class... Ts>
struct Overloaded : Ts...
{
    using Ts::operator()...;
};

} // namespace astfri::csharp::util

#include <libastfri-cs/impl/util/Common.inl>

#endif // CSHARP_COMMON_HPP
