#ifndef CSHARP_UTILS_HPP
#define CSHARP_UTILS_HPP

#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <filesystem>
#include <stack>
#include <string>
#include <vector>

namespace astfri::csharp::util
{

/**
 * @brief Suffix type of integer literal
 */
enum class VarDefType
{
    Local,
    Member,
    Global
};

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

/**
 * @brief Gets the C# integer suffix type from the given suffix string
 * @param suffix suffix string to analyze
 * @return corresponding IntSuffix enum value
 */
IntSuffix get_suffix_type(const std::string& suffix);

bool almost_equal(double a, double b, double epsilon = 1e-9);

/**
 * @brief Extracts the text corresponding to the given TSNode from the source
 * code
 * @param node TSNode to extract text from
 * @param source_code original source code string
 * @return extracted text from node
 */
std::string extract_node_text(
    const TSNode& node,
    const std::string& source_code
);

void split_namespace(
    std::stack<std::string>& scope_str,
    const std::string& namespace_name
);

bool is_interface_name(const std::string& name);

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
 * @brief Escapes special characters in verbatim and raw C# strings.
 * @param str_view input string view to escape
 * @param is_verbatim whether the string is a verbatim string literal
 * @return escaped string
 */
std::string escape_string(std::string_view str_view, bool is_verbatim);

void print_child_nodes_types(const TSNode& node);
void print_child_nodes_types(const TSNode& node, const std::string& source);

Scope create_scope(
    const TSNode& node,
    const TSLanguage* lang,
    const std::string& source
);

/**
 * @param node name node of the type (for example class or interface name)
 * @param source_code instance of CSharpTSTreeVisitor
 * @return instance of a Type
 */
Type* make_type(const TSNode& node, const std::string& source_code);

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

} // namespace astfri::csharp::util

#include <libastfri-cs/impl/utils.inl>

#endif // CSHARP_UTILS_HPP