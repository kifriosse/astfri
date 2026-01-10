/**
 * @file astfri_util.hpp
 * @brief Header for helper functions and structs used for creating FRI AST
 * nodes from C# syntax.
 */

#ifndef CSHARP_ASTFRI_UTIL_H
#define CSHARP_ASTFRI_UTIL_H

#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <functional>
#include <string_view>
#include <vector>

namespace astfri::csharp::util
{

Scope create_scope(
    const TSNode& node,
    const TSLanguage* lang,
    std::string_view source
);

Scope create_scope(
    std::string_view qualifier
);

/**
 * @param node name node of the type (for example class or interface name)
 * @param src_code instance of CSharpTSTreeVisitor
 * @return instance of a Type
 */
Type* make_type(const TSNode& node, std::string_view src_code);

/**
 * @brief Creates a ParamVarDefStmt from the given TSNode.
 * @param node TSNode representing the parameter definition
 * @param lang Tree-sitter language
 * @param source_code original source code string
 * @return pointer to the created ParamVarDefStmt
 * @note Creates a ParamVarDefStmt but without initialization expression -
 * needs to be handled by the caller
 */
ParamVarDefStmt* make_param_def(
    const TSNode& node,
    const TSLanguage* lang,
    std::string_view source_code
);

/**
 * @brief Helper struct to hold discovered parameter definitions and their
 * metadata.
 */
struct ParamSignature
{
    std::vector<ParamVarDefStmt*> defs;
    std::vector<ParamMetadata> metadata;
};

/**
 * @brief Discovers parameters from the given TSNode representing a parameter
 * list.
 * @param node TSNode representing the parameter list
 * @param src_code original source code string
 * @return ParamSignature containing parameter definitions and metadata
 */
ParamSignature discover_params(const TSNode& node, std::string_view src_code);

/**
 * @brief Processes each parameter node in the parameter list node by invoking
 * the provided collector function.
 * @param node parameter list TSNode
 * @param collector function to be called for each parameter TSNode
 */
void process_param_list(
    const TSNode& node,
    const std::function<void(const TSNode&)>& collector
);

FunctionMetadata make_func_metadata(
    const TSNode& node,
    std::string_view src_code
);

} // namespace astfri::csharp::util

#endif // CSHARP_ASTFRI_UTIL_H
