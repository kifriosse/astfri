/**
 * @file CSFwd.hpp
 * @brief Type aliases for astfri-cs.
 * @note Used to mitigate circular dependencies.
 */
#ifndef CSHARP_FWD_H
#define CSHARP_FWD_H

#include <libastfri-cs/impl/regs/NodeType.hpp>
#include <libastfri-cs/impl/util/Utils.hpp>

#include <tree_sitter/api.h>

#include <functional>
#include <string_view>

namespace astfri {
// Forward declarations
struct Type;
struct Stmt;
struct Expr;
} // namespace astfri

namespace astfri::csharp {
// Forward declarations
class ScopeNode;
class TypeTranslator;
class SrcCodeVisitor;
class SymbTableBuilder;
class SymbolTable;

/**
 * @brief Mask type used for modifiers and similar bitmask values.
 */
using MaskType = uint32_t;
/**
 * @brief Helper alias for tree-sitter capture IDs from query matches.
 */
using CaptureId = uint32_t;

/**
 * @brief Alias defining visit function signiture for tree-sitter tree
 * processing visitor classes
 * @tparam ReturnType return type of the visit function
 * @tparam Owner type of the visitor class that owns the visit function
 */
template<typename ReturnType, typename Owner>
using TSNodeProcessor = ReturnType (*)(Owner*, const TSNode&);

/**
 * @brief Alias for source code visitor visit functions used for mapping
 * tree-sitter expression nodes to AST FRI expressions and statements
 */
using ExprMapper = TSNodeProcessor<Expr*, SrcCodeVisitor>;
/**
 * @brief Alias for source code visitor visit functions used for mapping
 * tree-sitter statement nodes to AST FRI statements
 */
using StmtMapper = TSNodeProcessor<Stmt*, SrcCodeVisitor>;
/**
 * @brief Alias for type translator visit functions used for mapping tree-sitter
 * type nodes to AST FRI types
 */
using TypeMapper = TSNodeProcessor<Type*, TypeTranslator>;
/**
 * @brief Alias for symbol table builder visit functions used for collecting
 * type definitions. Function return pointer to ScopeNode representing location
 * of the type in symbol tree
 */
using TypeCollector = TSNodeProcessor<ScopeNode*, SymbTableBuilder>;
/**
 * @brief Alias for symbol table builder visit functions used for collecting
 * member definitions.
 */
using SymbCollector = TSNodeProcessor<void, SymbTableBuilder>;

/**
 * @brief Map with NodeType enum as keys
 * @tparam Value value type of the map
 */
template<typename Value>
using RegistryMap = std::unordered_map<NodeType, Value>;
/**
 * @brief Map with string identifiers as keys, using string_view as keys
 * @tparam Value value type of the map
 */
template<typename Value>
using RegistryStrViewMap = std::unordered_map<std::string_view, Value>;
/**
 * @brief Map with string identifiers as keys, using custom string hash for
 * better performance - this map can also use string_view for lookups
 * @tparam Value value type of the map
 * @note Source:
 * https://www.cppstories.com/2021/heterogeneous-access-cpp20/#how-to-enable-it-for-unordered-containers
 */
template<typename Value>
using IdentifierMap = std::unordered_map<std::string, Value, util::StringHash, std::equal_to<>>;

} // namespace astfri::csharp

#endif // CSHARP_FWD_H