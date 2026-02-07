#include <libastfri-cs/impl/data/SymbolTree.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <string_view>
#include <variant>

namespace astfri::csharp
{

SymbolTree::ScopeNode* SymbolTree::root()
{
    return tree_.root();
}

SymbolTree::ScopeNode* SymbolTree::add_namespace(const Scope& scope)
{
    return tree_.add_namespace(scope);
}

SymbolTree::ScopeNode* SymbolTree::add_type(
    const Scope& scope,
    ScopedType* type,
    UserTypeDefStmt* def
)
{
    return tree_
        .add_data(scope, TypeBinding{.type = type, .def = def}, type->name_);
}

TypeBinding* SymbolTree::find_type(
    const Scope& scope,
    const std::string_view typeName,
    const bool searchParents
) const
{
    return find_type({}, scope, typeName, searchParents);
}

TypeBinding* SymbolTree::find_type(
    const Scope& start,
    const Scope& end,
    const std::string_view typeName,
    const bool searchParents
) const
{
    ScopeNode* node = tree_.find_node(start, end, typeName, searchParents);
    return node ? std::get_if<TypeBinding>(&node->data) : nullptr;
}

TypeBinding* SymbolTree::find_type(
    ScopeNode* start,
    const std::string_view typeName
) const
{
    ScopeNode* node = tree_.find_node(*start, typeName);
    return node ? std::get_if<TypeBinding>(&node->data) : nullptr;
}

SymbolTree::ScopeNode* SymbolTree::find_node(
    ScopeNode* start,
    const std::string_view name
) const
{
    return tree_.find_node(*start, name);
}

} // namespace astfri::csharp