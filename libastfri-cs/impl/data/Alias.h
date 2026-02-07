#ifndef CSHARP_ALIAS_HPP
#define CSHARP_ALIAS_HPP

#include <libastfri-cs/impl/data/SymbolTree.hpp>

#include <string>
#include <variant>

namespace astfri
{
// forward declaration
struct Type;
} // namespace astfri

namespace astfri::csharp
{

struct SourceFile;

using AliasTarget
    = std::variant<std::monostate, std::string, SymbolTree::ScopeNode*>;

struct Alias
{
    SourceFile* srcFile;
    AliasTarget target;
    TSNode nTarget; // right side of alias declaration

    bool is_type(TypeBinding*& out) const;
};

} // namespace astfri::csharp

#endif // CSHARP_ALIAS_HPP
