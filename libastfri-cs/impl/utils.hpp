#ifndef CSHARP_UTILS_HPP
#define CSHARP_UTILS_HPP

#include <cmath>
#include <string>
#include <tree_sitter/api.h>
#include <unordered_map>
#include <vector>

namespace astfri::csharp
{
enum class IntSuffix {
    None,
    U,   // unsigned int
    L,   // long
    UL,  // unsigned long
};

// const std::unordered_map<char, std::string> csharp_escape_map = {
//     {'\'', "\\\'"},
//     {'\"', "\\\""},
//     {'\\', "\\\\"},
//     {'\0', "\\0"},
//     {'\a', "\\a"},
//     {'\b', "\\b"},
//     {'\f', "\\f"},
//     {'\n', "\\n"},
//     {'\r', "\\r"},
//     {'\t', "\\t"},
//     {'\v', "\\v"},
// };

std::vector<TSNode> find_nodes(TSNode root, const TSLanguage *lang, const char *query_str);

IntSuffix get_suffix_type(const std::string &suffix);

inline bool almost_equal(double const a, double const b, double const epsilon = 1e-9) {
    return std::fabs(a - b) < epsilon;
}

}

#endif // CSHARP_UTILS_HPP