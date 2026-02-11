/**
 * @file AstfriUtil.hpp
 * @brief Header for helper functions and structs used for creating FRI AST
 * nodes from C# syntax.
 */

#ifndef CSHARP_ASTFRI_UTIL_H
#define CSHARP_ASTFRI_UTIL_H

#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

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

Scope mk_scope(const TSNode& node, const SourceFile& src_file);

Scope mk_scope(std::string_view qualifier);

/**
 * @brief Creates a ParamVarDefStmt from the given TSNode.
 * @param node TSNode representing the parameter definition
 * @param src reference to SourceCode instance linked to the TSNode parameter
 * @param typeTrs TypeTranslator instance for translating type nodes
 * @return pointer to the created ParamVarDefStmt
 * @note Creates a ParamVarDefStmt but without initialization expression -
 * needs to be handled by the caller
 */
ParamVarDefStmt* make_param_def(
    const TSNode& node,
    std::string_view src,
    TypeTranslator& typeTrs
);

/**
 * @brief Discovers parameters from the given TSNode representing a parameter
 * list.
 * @param node TSNode representing the parameter list
 * @param src original source code string
 * @param typeTrs TypeTranslator instance for translating type nodes
 * @return ParamSignature containing parameter definitions and metadata
 */
ParamSignature discover_params(
    const TSNode& node,
    std::string_view src,
    TypeTranslator& typeTrs
);

FuncMetadata make_func_metadata(
    const TSNode& node,
    std::string_view src,
    TypeTranslator& typeTrs
);

std::vector<GenericParam*> make_generic_params(
    const TSNode& node,
    std::string_view src
);

} // namespace astfri::csharp::util

#endif // CSHARP_ASTFRI_UTIL_H
