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
    static const std::string using_directives_query;
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
 * @brief Wrapper function for ts_language_symbol_for_name
 * @param lang
 * @param name
 * @param named
 * @return
 */
TSSymbol symbol_for_name(
    const TSLanguage* lang,
    std::string_view name,
    bool named
);

/**
 * @brief Extracts the text corresponding to the given TSNode from the source
 * code
 * @param node TSNode to extract text from
 * @param src_code original source code string
 * @return extracted text from node
 */
std::string extract_text(const TSNode& node, std::string_view src_code);

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
    std::string_view query_str
);

TSNode find_first_node(
    const TSNode& root,
    const TSLanguage* lang,
    std::string_view query_str
);

void print_child_nodes_types(const TSNode& node);
void print_child_nodes_types(
    const TSNode& node,
    std::string_view source,
    bool named = false
);

/**
 * @brief Removes comments from the given source code using the syntax tree.
 * @param root starting node of the syntax tree (usually the root node of the
 * TSTree)
 * @param source_code original source code with comments
 * @param lang Tree-sitter language
 * @param path file path of the source code file
 * @return source code with comments removed
 */
std::string remove_comments(
    const TSNode& root,
    std::string_view source_code,
    const TSLanguage* lang,
    const std::filesystem::path& path = {}
);

/**
 * @brief Checks if the parameter list node contains a variadic parameter.
 * @param node parameter list node
 * @param type_node output parameter that will hold node to the type of the
 * variadic parameter if found (nullptr if not found).
 * @return true if the parameter list contains a variadic parameter
 */
bool has_variadic_param(const TSNode& node, TSNode* type_node = nullptr);

/**
 * @brief Iterates over each child node of the given TSNode and applies the
 * provided function.
 * @tparam Func A callable type that takes a TSNode as parameter and returns
 * bool.
 * @param node TSNode whose child nodes will be iterated over.
 * @param process Function to apply to each child node. If it returns false, the
 * iteration stops.
 * @param only_named If true, only named child nodes will be processed.
 */
template<typename Func>
requires std::same_as<std::invoke_result_t<Func, TSNode>, bool>
void for_each_child_node(TSNode node, Func process, bool only_named = true);

/**
 * @brief Iterates over each child node of the given TSNode and applies the
 * provided function.
 * @tparam Func A callable type that takes a TSNode as parameter.
 * @param node TSNode whose child nodes will be iterated over.
 * @param process Function to apply to each child node.
 * @param only_named If true, only named child nodes will be processed.
 */
template<typename Func>
requires std::same_as<std::invoke_result_t<Func, TSNode>, void>
void for_each_child_node(TSNode node, Func process, bool only_named = true);

/**
 * @brief Processes each parameter node in the parameter list node by invoking
 * the provided collector function.
 * @param node parameter list TSNode
 * @param collector function to be called for each parameter TSNode
 */
template<std::invocable<TSNode> Func>
void process_param_list(TSNode node, Func collector);

} // namespace astfri::csharp::util

#include <libastfri-cs/impl/util/ts_util.inl>

#endif // CSHARP_TS_UTIL_HPP