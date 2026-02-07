#ifndef CSHARP_ALIAS_HPP
#define CSHARP_ALIAS_HPP

#include <libastfri-cs/impl/data/SymbolTree.hpp>

#include <string>
#include <variant>

namespace astfri::csharp
{

using Alias = std::variant<std::monostate, std::string, SymbolTree::ScopeNode*>;

bool is_type_alias(const Alias& alias, TypeBinding*& out);

} // namespace astfri::csharp

#endif // CSHARP_ALIAS_HPP
