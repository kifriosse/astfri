#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <variant>

namespace astfri::csharp
{
SymbolTree::SymbolTree() :
    root_(
        std::make_unique<SymbolTreeNode>(SymbolTreeNode{.data = std::string{}})
    )
{
}

SymbolTreeNode* SymbolTree::add_namespace(const Scope& scope)
{
    SymbolTreeNode* current = root_.get();
    for (const std::string& str : scope.names_)
    {
        auto [it, inserted] = current->children.try_emplace(str);
        if (inserted)
        {
            it->second       = std::make_unique<SymbolTreeNode>();
            it->second->data = str;
        }
        current = it->second.get();
    }
    return current;
}

void SymbolTree::add_type(
    const Scope& scope,
    ScopedType* type,
    UserTypeDefStmt* def
)
{
    SymbolTreeNode* last = add_namespace(scope);
    auto [it, inserted]  = last->children.try_emplace(type->name_);
    if (inserted)
    {
        it->second       = std::make_unique<SymbolTreeNode>();
        it->second->data = util::TypeBinding{type, def};
    }
}

util::TypeBinding* SymbolTree::find_type(
    const Scope& scope,
    const std::string_view type_name,
    const bool search_parents
) const
{
    return find_type({}, scope, type_name, search_parents);
}

util::TypeBinding* SymbolTree::find_type(
    const Scope& start,
    const Scope& end,
    const std::string_view type_name,
    const bool search_parents
) const
{
    SymbolTreeNode* current = root_.get();
    std::vector<SymbolTreeNode*> nodes;
    auto process = [&](const Scope& scope) -> bool
    {
        for (std::string_view name : scope.names_)
        {
            auto it = current->children.find(name);
            if (it == current->children.end())
                return false;

            current = it->second.get();
            if (search_parents)
                nodes.push_back(current);
        }
        return true;
    };

    if (! process(start))
        return nullptr;

    if (! process(end))
        return nullptr;

    if (! search_parents)
    {
        const auto it = current->children.find(type_name);
        return it == current->children.end()
                 ? nullptr
                 : std::get_if<util::TypeBinding>(&it->second->data);
    }

    for (const auto node : std::views::reverse(nodes))
    {
        auto it = node->children.find(type_name);
        if (it == node->children.end())
            continue;

        if (const auto res = std::get_if<util::TypeBinding>(&it->second->data))
            return res;
    }

    return nullptr;
}

} // namespace astfri::csharp