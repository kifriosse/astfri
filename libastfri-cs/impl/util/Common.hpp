/**
 * @file common.hpp
 * @brief Common utility functions and enums
 */

#ifndef CSHARP_COMMON_HPP
#define CSHARP_COMMON_HPP

#include <cstddef>
// ReSharper disable once CppUnusedIncludeDirective
#include <functional>

namespace astfri::csharp::util
{

enum class VarDefType
{
    Local,
    Member,
    Global
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
