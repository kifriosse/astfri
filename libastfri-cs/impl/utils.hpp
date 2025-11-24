#ifndef CSHARP_UTILS_HPP
#define CSHARP_UTILS_HPP

#include <cmath>
#include <stack>
#include <string>
#include <tree_sitter/api.h>
#include <vector>

namespace astfri::csharp
{
enum class IntSuffix
{
    None,
    U,  // unsigned int
    L,  // long
    UL, // unsigned long
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

std::vector<TSNode> find_nodes(
    TSNode const& root,
    TSLanguage const* lang,
    std::string const& query_str
);

TSNode find_first_node(TSNode const& root, TSLanguage const* lang, std::string const& query_str);

IntSuffix get_suffix_type(std::string const& suffix);

bool almost_equal(double const a, double const b, double const epsilon = 1e-9);

std::string extract_node_text(TSNode const& node, std::string const& source_code);

void split_namespace(std::stack<std::string>& scope_str, std::string const& namespace_name);
bool is_interface_name(std::string const& name);

} // namespace astfri::csharp

#endif // CSHARP_UTILS_HPP