#ifndef CSHARP_UTILS_HPP
#define CSHARP_UTILS_HPP

#include <libastfri-cs/impl/util/Common.hpp>

#include <deque>
#include <string>
#include <string_view>

namespace astfri::csharp::util
{

/**
 * @brief Hash struct that allows hashing both std::string and std::string_view
 * @note Source:
 * https://www.cppstories.com/2021/heterogeneous-access-cpp20/#how-to-enable-it-for-unordered-containers
 */
struct StringHash
{
    using is_transparent = void;
    size_t operator()(std::string_view str) const noexcept;
    size_t operator()(const std::string& str) const noexcept;
};

/**
 * @brief Gets the C# integer suffix type from the given suffix string
 * @param suffix suffix string to analyze
 * @return corresponding IntSuffix enum value
 */
IntSuffix get_suffix_type(std::string_view suffix);

/**
 * @brief Splits a dot-separated namespace qualifier string into its components
 * and stores them in the deque parameter.
 * @param qualifs deque to store the split components of the namespace qualifier
 * @param nmsQualif dot-separated namespace qualifier string to split
 */
void split_namespace(
    std::deque<std::string>& qualifs,
    std::string_view nmsQualif
);

/**
 * @brief Checks if the given name is a interface name according to C# naming
 * conventions (starts with 'I' followed by an uppercase letter).
 * @param name name of the type
 * @return true if the name is a valid interface name, false otherwise
 */
bool is_interface_name(std::string_view name);

/**
 * @brief Escapes special characters in verbatim and raw C# strings.
 * @param str input string to escape
 * @param isVerbatim whether the string is a verbatim string literal
 * @return escaped string
 */
std::string escape_string(std::string_view str, bool isVerbatim);

} // namespace astfri::csharp::util

#endif // CSHARP_UTILS_HPP