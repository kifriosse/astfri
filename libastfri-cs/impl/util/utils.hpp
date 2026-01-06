#ifndef CSHARP_UTILS_HPP
#define CSHARP_UTILS_HPP

#include <libastfri-cs/impl/util/common.hpp>

#include <stack>
#include <string>
#include <string_view>

namespace astfri::csharp::util
{

/**
 * @brief Gets the C# integer suffix type from the given suffix string
 * @param suffix suffix string to analyze
 * @return corresponding IntSuffix enum value
 */
IntSuffix get_suffix_type(std::string_view suffix);

bool almost_equal(double a, double b, double epsilon = 1e-9);

void split_namespace(
    std::stack<std::string>& scope_str,
    std::string_view namespace_name
);

bool is_interface_name(const std::string& name);

/**
 * @brief Escapes special characters in verbatim and raw C# strings.
 * @param str input string to escape
 * @param is_verbatim whether the string is a verbatim string literal
 * @return escaped string
 */
std::string escape_string(std::string_view str, bool is_verbatim);

} // namespace astfri::csharp::util

#endif // CSHARP_UTILS_HPP