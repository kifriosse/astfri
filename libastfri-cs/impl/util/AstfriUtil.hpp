/**
 * @file AstfriUtil.hpp
 * @brief Header for helper functions and structs used for creating FRI AST
 * nodes from C# syntax.
 */

#ifndef CSHARP_ASTFRI_UTIL_H
#define CSHARP_ASTFRI_UTIL_H

#include <libastfri-cs/impl/data/Metadata.hpp>
#include <astfri/Astfri.hpp>

#include <tree_sitter/api.h>

#include <string_view>
#include <vector>

namespace astfri::csharp::util {

/**
 * @brief Helper struct to hold discovered parameter definitions and their
 * metadata.
 */
struct ParamSignature {
    std::vector<ParamVarDefStmt*> defs;
    std::vector<ParamMetadata> metadata;
};

/**
 * @brief Makes scope in which the givne TSNode is located. Node should be a
 * definition of type or using directive or any top level statement
 * @param node TSNode representing defintion node (Type defintion or using
 * directive)
 * @param currentSrc reference to source file in the wich the node is located
 * @return scope in which the node is located.
 */
Scope mk_scope(const TSNode& node, const SourceFile& currentSrc);

/**
 * @brief Makes scope from given dot seperated qualifier string
 * @param qualifier string representing the qualifier (qualifiers have to be
 * seperated by dots)
 * @return Scope form given qualifier string
 */
Scope mk_scope(std::string_view qualifier);

/**
 *
 * @param start scope node from which to start building scope
 * @param currentSrc reference to source file in which the scope node is located
 * @return scope built from given scope node
 */
Scope mk_scope(ScopeNode* start, const SourceFile& currentSrc);

/**
 * @brief Creates a ParamVarDefStmt from the given TSNode.
 * @param node TSNode representing the parameter definition
 * @param src reference to SourceCode instance linked to the TSNode parameter
 * @param typeTrs TypeTranslator instance for translating type nodes
 * @return pointer to the created ParamVarDefStmt
 * @note Creates a ParamVarDefStmt but without initialization expression -
 * needs to be handled by the caller
 */
ParamVarDefStmt* mk_param_def(const TSNode& node, std::string_view src, TypeTranslator& typeTrs);

/**
 * @brief Discovers parameters from the given TSNode representing a parameter
 * list.
 * @param node TSNode representing the parameter list
 * @param src original source code string
 * @param typeTrs TypeTranslator instance for translating type nodes
 * @return ParamSignature containing parameter definitions and metadata
 */
ParamSignature discover_params(const TSNode& node, std::string_view src, TypeTranslator& typeTrs);

FuncMetadata make_func_metadata(const TSNode& node, std::string_view src, TypeTranslator& typeTrs);

std::vector<GenericParam*> make_generic_params(const TSNode& node, std::string_view src);

} // namespace astfri::csharp::util

#endif // CSHARP_ASTFRI_UTIL_H
