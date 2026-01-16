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
 * @brief Helper struct to group type and its definition statement
 */
struct TypeBinding
{
    ScopedType* type;
    UserTypeDefStmt* def;
};

Scope create_scope(
    const TSNode& node,
    const TSLanguage* lang,
    std::string_view source
);

Scope create_scope(std::string_view qualifier);

/**
 * @brief Creates a ParamVarDefStmt from the given TSNode.
 * @param node TSNode representing the parameter definition
 * @param src reference to SourceCode instance linked to the TSNode parameter
 * @param type_tr TypeTranslator instance for translating type nodes
 * @return pointer to the created ParamVarDefStmt
 * @note Creates a ParamVarDefStmt but without initialization expression -
 * needs to be handled by the caller
 */
ParamVarDefStmt* make_param_def(
    const TSNode& node,
    const SourceCode& src,
    TypeTranslator& type_tr
);

/**
 * @brief Discovers parameters from the given TSNode representing a parameter
 * list.
 * @param node TSNode representing the parameter list
 * @param src_code original source code string
 * @param type_tr TypeTranslator instance for translating type nodes
 * @return ParamSignature containing parameter definitions and metadata
 */
ParamSignature discover_params(
    const TSNode& node,
    std::string_view src_code,
    TypeTranslator& type_tr
);

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

FuncMetadata make_func_metadata(
    const TSNode& node,
    std::string_view src,
    TypeTranslator& type_tr
);

} // namespace astfri::csharp::util

#endif // CSHARP_ASTFRI_UTIL_H
