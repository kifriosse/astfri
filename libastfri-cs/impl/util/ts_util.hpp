/**
 * @file ts_util.hpp
 * @brief Utility functions for working with Tree-sitter with in C# grammer.
 */

#ifndef CSHARP_TS_UTIL_HPP
#define CSHARP_TS_UTIL_HPP
#include <tree_sitter/api.h>

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace astfri::csharp::regs
{
// todo redo this into constexpr std::string_view
struct Queries
{
    static const std::string top_level_stmt_query;
    static const std::string decl_query;
    static const std::string var_modif_query;
    static const std::string method_modif_query;
    static const std::string file_namespace_query;
    static const std::string comment_error_query;
};
} // namespace astfri::csharp::regs

namespace astfri::csharp::util
{

/**
 * @brief Gets the child node of the given node by field name.
 * @param node parent TSNode
 * @param name field name to search for
 * @return child TSNode with the given field name, or a null TSNode if not
 * found
 */
TSNode child_by_field_name(const TSNode& node, std::string_view name);

/**
 * @brief Extracts the text corresponding to the given TSNode from the source
 * code
 * @param node TSNode to extract text from
 * @param src_code original source code string
 * @return extracted text from node
 */
std::string extract_node_text(const TSNode& node, std::string_view src_code);

/**
 * @brief Finds all nodes in subtree of given root node that match the given
 * query
 * @param root root node to start searching from
 * @param lang Tree-sitter language of the syntax tree
 * @param query_str Tree-sitter query string to match nodes
 * @return vector of matching nodes
 */
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

void print_child_nodes_types(const TSNode& node);
void print_child_nodes_types(const TSNode& node, std::string_view source);

/**
 * @brief Removes comments from the given source code using the syntax tree.
 * @param source_code original source code with comments
 * @param root starting node of the syntax tree (usually the root node of the
 * TSTree)
 * @param lang Tree-sitter language
 * @param path file path of the source code file
 * @return source code with comments removed
 */
std::string remove_comments(
    const std::string& source_code,
    const TSNode& root,
    const TSLanguage* lang,
    const std::filesystem::path& path
);

/**
 * @brief Checks if the parameter list node contains a variadic parameter.
 * @param node parameter list node
 * @param type_node output parameter that will hold node to the type of the
 * variadic parameter if found (nullptr if not found).
 * @return true if the parameter list contains a variadic parameter
 */
bool has_variadic_param(const TSNode& node, TSNode* type_node);

} // namespace astfri::csharp::util

#endif // CSHARP_TS_UTIL_HPP