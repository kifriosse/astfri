/**
 * @file CSAliases.hpp
 * @brief Type aliases for astfri-cs.
 * @note Used to mitigate circular dependencies.
 */

#ifndef CSHARP_FWD_H
#define CSHARP_FWD_H

#include <tree_sitter/api.h>

#include <functional>
#include <string_view>

namespace astfri
{
// Forward declarations
struct Stmt;
struct Expr;

} // namespace astfri

namespace astfri::csharp
{
// Forward declarations
struct SymbolTable;
class SymbolTableBuilder;
class SourceCodeVisitor;

using MaskType    = uint32_t;

using ExprHandler = std::function<Expr*(SourceCodeVisitor*, const TSNode*)>;
using StmtHandler = std::function<
    Stmt*(SourceCodeVisitor*, const TSNode*)>; // todo change this to TSNode&
using RegHandler
    = std::function<void(SymbolTableBuilder*, const TSNode&, SymbolTable&)>;

template<class Value>
using RegMap = std::unordered_map<std::string_view, Value>;

} // namespace astfri::csharp

#endif // CSHARP_FWD_H