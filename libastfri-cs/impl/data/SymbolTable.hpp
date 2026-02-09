#ifndef CSHARP_SYMBOL_TABLE_HPP
#define CSHARP_SYMBOL_TABLE_HPP

#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri-cs/impl/data/SymbolTree.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <unordered_map>
#include <vector>

namespace astfri::csharp
{
// forward declaration
struct TypeMetadata;

/**
 * @brief Symbol table containing metadata about user defined types
 */
struct SymbolTable
{
    std::unordered_map<UserTypeDefStmt*, TypeMetadata> userTypeMetadata;
    std::vector<UserTypeDefStmt*> userTypeKeys;
    std::vector<Scope> globUsings;
    std::vector<UserTypeDefStmt*> globStaticUsings;
    IdentifierMap<Alias> globAliases;
    SymbolTree symbTree{};
};

} // namespace astfri::csharp

#endif // CSHARP_SYMBOL_TABLE_HPP