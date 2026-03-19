/**
 * @file TSUtil.hpp
 * @brief Utility functions for working with Tree-sitter with in C# grammer.
 */

#ifndef CSHARP_TS_UTIL_HPP
#define CSHARP_TS_UTIL_HPP

#include <libastfri-cs/impl/regs/QueryRegistry.hpp>

#include <tree_sitter/api.h>

#include <filesystem>
#include <string>
#include <string_view>

namespace astfri::csharp::util {

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
 * @param name type/name of node we want symbol of
 * @param named if the symbol we are looking for is for named node
 * @return TSSymbol corresponding to the given name
 */
TSSymbol symbol_for_name(std::string_view name, bool named);

/**
 * @brief Extracts the text corresponding to the given TSNode from the source
 * code
 * @param node TSNode to extract text from
 * @param src original source code string
 * @return extracted text from node
 */
std::string extract_text(const TSNode& node, std::string_view src);

/**
 * @brief Prints the types of child nodes of the given TSNode to standard
 * output.
 * @param node TSNode whose child nodes' types will be printed.
 * @param named If true, only named child nodes will be printed.
 */
void print_child_nodes_types(const TSNode& node, bool named = false);

/**
 * @brief Prints the type and representation inside source code of child nodes
 * of the given TSNode to standard output.
 * @param node TSNode whose child nodes' type and source code representation
 * will be printed.
 * @param src source code corresponding to the TSNode.
 * @param named If true, only named child nodes will be printed.
 */
void print_child_nodes_types(const TSNode& node, std::string_view src, bool named = false);

/**
 * @brief Removes comments from the given source code using the syntax tree.
 * @param root starting node of the syntax tree (usually the root node of the
 * TSTree)
 * @param src original source code with comments
 * @param path file path of the source code file
 * @return source code with comments removed
 */
std::string remove_comments(
    const TSNode& root,
    std::string src,
    const std::filesystem::path& path = {}
);

/**
 * @brief Checks if the parameter list node contains a variadic parameter.
 * @param node parameter list node
 * @param nType output parameter that will hold node to the type of the
 * variadic parameter if found (null node if not found). If set to nullptr,
 * result will be ignored.
 * @return true if the parameter list contains a variadic parameter
 */
bool has_variadic_param(const TSNode& node, TSNode* nType = nullptr);

/**
 * @brief Checks if the given node represents an anonymouse lambda. Anonymous
 * lambda call expression in C# looks like this:
 *  \code
 * ((Func<int, bool>)(x) => false; )(1)
 * \endcode
 * @param node node representing left side of invocation
 * @param lambda output parameter that will hold node to the lambda node is
 * anonymous lambda.
 * @param delegate output parameter that will hold node to the delegate type if
 * node is a anonymous lambda
 * @return true if the node parameter represents lambda
 */
bool is_anonymous_lambda(const TSNode& node, TSNode* lambda, TSNode* delegate);

/**
 * @brief Wrapper function for ts_parser_parse_string that creates a TSTree
 * from the given source code string using the provided parser.
 * @param parser pointer to the TSParser to use for parsing the source code
 * @param str source code string to parse into a TSTree
 * @return pointer to the created TSTree
 */
TSTree* make_tree(TSParser* parser, std::string_view str);

/**
 * @brief Unwraps parentheses expression and extracts the inner expression.
 * @param node tree-sitter node containing parentheses expression
 * @return TSNode representing the inner expression without parentheses
 */
TSNode unwrap_parantheses(const TSNode& node);

/**
 * @brief Checks if the given node is a type declaration (class, interface,
 * struct, enum or delegate declaration).
 * @param node TSNode to check
 * @return true if the node is a type declaration, false otherwise
 */
bool is_type_decl(const TSNode& node);

/**
 * @brief Checks if the given symbol is a type declaration (class, interface,
 * struct, enum or delegate declaration).
 * @param symbol TSSymbol to check
 * @return true if the node is a type declaration, false otherwise
 */
bool is_type_decl(TSSymbol symbol);

/**
 * @brief Iterates over each child node of the given TSNode and applies the
 * provided function.
 * @tparam F A callable type that takes a @code const TSNode&@endcode as
 * parameter.
 * @param node TSNode whose child nodes will be iterated over.
 * @param process Function to apply to each child node.
 * @param only_named If true, only named child nodes will be processed.
 * @note If @p process parameter returns @c bool, returning @c false will end
 * the iteration. When the @p process returns  @c void (or any type other than
 * bool), it will iterator over every children.
 */
template<std::invocable<const TSNode&> F>
void for_each_child_node(const TSNode& node, F process, bool only_named = true);

/**
 * @brief Processes each parameter node in the parameter list node by invoking
 * the provided collector function.
 * @tparam F a callable type that takes a TSNode as parameter.
 * @param node parameter list TSNode
 * @param collector function to be called for each parameter TSNode
 */
template<std::invocable<const TSNode&> F>
void process_param_list(const TSNode& node, F collector);

/**
 * @brief Iterates over captures of the specified query in sub-tree of given
 * root node and applies the provided function.
 * @tparam F A callable type that takes a const TSQueryMatch& as parameter.
 * @param root TSNode to search for captures.
 * @param type Type of query to use for finding captures.
 * @param process Function to apply to each capture match.
 */
template<std::invocable<const TSQueryMatch&> F>
void for_each_match(const TSNode& root, maps::QueryType type, F process);

} // namespace astfri::csharp::util

#include <libastfri-cs/impl/util/TSUtil.inl>

#endif // CSHARP_TS_UTIL_HPP