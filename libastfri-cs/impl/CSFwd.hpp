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

namespace astfri
{
// Forward declarations
struct Type;
struct Stmt;
struct Expr;
} // namespace astfri

namespace astfri::csharp
{
// Forward declarations
class ScopeNode;
class TypeTranslator;
class SrcCodeVisitor;
class SymbTableBuilder;
class SymbolTable;

using MaskType  = uint32_t;
using CaptureId = uint32_t;

template<typename ReturnType, typename Owner>
using TSNodeProcessor = std::function<ReturnType(Owner*, const TSNode&)>;
using ExprMapper      = TSNodeProcessor<Expr*, SrcCodeVisitor>;
using StmtMapper      = TSNodeProcessor<Stmt*, SrcCodeVisitor>;
using TypeMapper      = TSNodeProcessor<Type*, TypeTranslator>;
using TypeCollector   = TSNodeProcessor<ScopeNode*, SymbTableBuilder>;
using SymbCollector   = TSNodeProcessor<void, SymbTableBuilder>;

/**
 * @brief Map with NodeType enum as keys
 */
template<typename Value>
using RegistryMap = std::unordered_map<NodeType, Value>;
template<typename Value>
using RegistryStrViewMap = std::unordered_map<std::string_view, Value>;
/**
 * @brief Map with string identifiers as keys, using custom string hash for
 * better performance - this map can also use string_view for lookups
 * @note Source:
 * https://www.cppstories.com/2021/heterogeneous-access-cpp20/#how-to-enable-it-for-unordered-containers
 */
template<typename Value>
using IdentifierMap
    = std::unordered_map<std::string, Value, util::StringHash, std::equal_to<>>;

} // namespace astfri::csharp

#endif // CSHARP_FWD_H