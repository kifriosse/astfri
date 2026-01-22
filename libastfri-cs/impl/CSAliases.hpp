/**
 * @file CSAliases.hpp
 * @brief Type aliases for astfri-cs.
 * @note Used to mitigate circular dependencies.
 */

#ifndef CSHARP_FWD_H
#define CSHARP_FWD_H

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
class TypeTranslator;
class SrcCodeVisitor;
class SymbolTableBuilder;
struct SymbolTable;

using MaskType  = uint32_t;
using CaptureId = uint32_t;

template<class ReturnType, class Owner>
using Handler     = std::function<ReturnType(Owner*, const TSNode&)>;
using ExprHandler = Handler<Expr*, SrcCodeVisitor>;
using StmtHandler = Handler<Stmt*, SrcCodeVisitor>;
using TypeHandler = Handler<Type*, TypeTranslator>;
using RegHandler  = Handler<void, SymbolTableBuilder>;

/**
 * @brief Map with string_view identifiers as keys
 */
template<class Value>
using RegistryMap = std::unordered_map<std::string_view, Value>;
/**
 * @brief Map with string identifiers as keys, using custom string hash for
 * better performance - this map can also use string_view for lookups
 * @note Source:
 * https://www.cppstories.com/2021/heterogeneous-access-cpp20/#how-to-enable-it-for-unordered-containers
 */
template<class Value>
using IdentifierMap
    = std::unordered_map<std::string, Value, util::StringHash, std::equal_to<>>;

} // namespace astfri::csharp

#endif // CSHARP_FWD_H