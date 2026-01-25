#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <string>
#include <string_view>
#include <variant>

namespace astfri::csharp
{

SymbolTree::SymbolTreeNode* SymbolTree::add_namespace(const Scope& scope)
{
    return tree_.add_namespace(scope);
}

SymbolTree::SymbolTreeNode* SymbolTree::add_type(
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
    SymbolTreeNode* node = tree_.find_node(start, end, typeName, searchParents);
    return node ? std::get_if<TypeBinding>(&node->data) : nullptr;
}

} // namespace astfri::csharp