#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri/inc/Astfri.hpp>

namespace astfri::csharp
{

SymbolTable::SymbolTable(std::vector<ScopeNode*> implicitUsings)
    : globUsings(std::move(implicitUsings))
{
}

const SymbolTree& SymbolTable::symbTree()
{
    return symbTree_;
}

void SymbolTable::add_glob_using(ScopeNode* node)
{
    globUsings.push_back(node);
}

void SymbolTable::add_glob_static_using(const TypeBinding& type)
{
    globStaticUsings.push_back(type);
}

void SymbolTable::add_glob_alias(std::string name, Alias alias)
{
    globAliases.try_emplace(std::move(name), std::move(alias));
}

ScopeNode* SymbolTable::add_type(
    const TypeBinding& tb,
    const TSNode& node,
    SourceFile* src
)
{
    auto [it, inserted] = userTypeMetadata.try_emplace(tb.def);
    if (inserted)
    {
        TypeMetadata metadata {
            .typeBinding = tb,
            .scope = symbTree_.add_type(tb.type->scope_, tb.type, tb.def)
        };
        it->second = std::move(metadata);
        userTypes_.push_back(tb.def);
    }
    it->second.defs.emplace_back(node, src);
    return it->second.scope;
}

TypeMetadata* SymbolTable::get_type_metadata(UserTypeDefStmt* def)
{
    const auto it = userTypeMetadata.find(def);
    return it != userTypeMetadata.end() ? &it->second : nullptr;
}

std::span<UserTypeDefStmt* const> SymbolTable::get_user_types()
{
    return userTypes_;
}

SymbolTable::span<const ScopeNode* const> SymbolTable::get_glob_usings()
{
    return globUsings;
}

SymbolTable::span<const TypeBinding> SymbolTable::get_glob_static_usings()
{
    return globStaticUsings;
}

Alias* SymbolTable::get_glob_alias(const std::string_view name)
{
    const auto it = globAliases.find(name);
    return it != globAliases.end() ? &it->second : nullptr;
}

} // namespace astfri::csharp
