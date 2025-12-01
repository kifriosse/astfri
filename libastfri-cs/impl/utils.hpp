#ifndef CSHARP_UTILS_HPP
#define CSHARP_UTILS_HPP

#include <tree_sitter/api.h>

#include <stack>
#include <string>
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
    const TSNode& root,
    const TSLanguage* lang,
    const std::string& query_str
);

TSNode find_first_node(
    const TSNode& root,
    const TSLanguage* lang,
    const std::string& query_str
);

IntSuffix get_suffix_type(const std::string& suffix);

bool almost_equal(double a, double b, double epsilon = 1e-9);

std::string extract_node_text(
    const TSNode& node,
    const std::string& source_code
);

void split_namespace(
    std::stack<std::string>& scope_str,
    const std::string& namespace_name
);

bool is_interface_name(const std::string& name);

std::string remove_comments(
    const std::string& source_code,
    const TSNode& root,
    const TSLanguage& lang
);

} // namespace astfri::csharp

#endif // CSHARP_UTILS_HPP