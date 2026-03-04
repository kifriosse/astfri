#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri/inc/Astfri.hpp>

namespace astfri::csharp
{

SymbolTable::SymbolTable(std::vector<ScopeNode*> implicitUsings) :
    globUsings_(std::move(implicitUsings))
{
}

const SymbolTree& SymbolTable::symb_tree()
{
    return symbTree_;
}

void SymbolTable::add_glob_using(ScopeNode* node)
{
    globUsings_.push_back(node);
}

void SymbolTable::add_glob_static_using(const TypeBinding& type)
{
    globStaticUsings_.push_back(type);
}

void SymbolTable::add_glob_alias(std::string name, Alias alias)
{
    globAliases_.try_emplace(std::move(name), std::move(alias));
}

ScopeNode* SymbolTable::add_type(
    const TypeBinding& tb,
    const TSNode& node,
    SourceFile* src
)
{
    auto [it, inserted] = userTypeMetadata_.try_emplace(tb.def, tb);
    if (inserted)
    {
        it->second.type_binding().treeNode = add_type(tb);
        userTypes_.push_back(tb.def);
    }
    it->second.add_def(node, src);
    return it->second.type_binding().treeNode;
}

ScopeNode* SymbolTable::add_type(const TypeBinding& tb)
{
    return symbTree_.add_type(tb.type->scope_, tb);
}

ScopeNode* SymbolTable::add_primitive(
    std::string name,
    CSPrimitiveType primitive
)
{
    return symbTree_.add_primitive(name, primitive);
}

TypeMetadata* SymbolTable::get_type_metadata(UserTypeDefStmt* def)
{
    const auto it = userTypeMetadata_.find(def);
    return it != userTypeMetadata_.end() ? &it->second : nullptr;
}

SymbolTable::span<const ScopeNode* const> SymbolTable::get_glob_usings()
{
    return globUsings_;
}

SymbolTable::span<const TypeBinding> SymbolTable::get_glob_static_usings()
{
    return globStaticUsings_;
}

Alias* SymbolTable::get_glob_alias(const std::string_view name)
{
    const auto it = globAliases_.find(name);
    return it != globAliases_.end() ? &it->second : nullptr;
}

} // namespace astfri::csharp
