#ifndef CSHARP_SCOPE_TREE_INL
#define CSHARP_SCOPE_TREE_INL

namespace astfri::csharp
{

template<typename T>
ScopeTree<T>::ScopeTree() :
    root_(std::make_unique<ScopeNode<T>>(ScopeNode<T>{.data = T{}}))
{
}

template<typename T>
ScopeTree<T>::Node* ScopeTree<T>::add_namespace(const Scope& scope)
{
    Node* current = root_.get();
    for (const std::string& str : scope.names_)
    {
        auto [it, inserted] = current->children.try_emplace(str);
        if (inserted)
        {
            it->second         = std::make_unique<Node>();
            it->second->data   = str;
            it->second->parent = current;
        }
        current = it->second.get();
    }
    return current;
}

template<typename T>
ScopeTree<T>::Node* ScopeTree<T>::add_data(
    const Scope& scope,
    const T& data,
    const std::string& identifier
)
{
    Node* last          = add_namespace(scope);
    auto [it, inserted] = last->children.try_emplace(identifier);
    if (inserted)
    {
        it->second         = std::make_unique<Node>();
        it->second->data   = data;
        it->second->parent = last;
        return it->second.get();
    }
    return nullptr;
}

template<typename T>
ScopeTree<T>::Node* ScopeTree<T>::find_node(
    const Scope& scope,
    const std::string_view typeName,
    const bool searchParents
) const
{
    return find_node({}, scope, typeName, searchParents);
}

template<typename T>
ScopeTree<T>::Node* ScopeTree<T>::find_node(
    const Scope& start,
    const Scope& end,
    std::string_view typeName,
    const bool searchParents
) const
{
    Node* current = root_.get();
    auto process = [&](const Scope& scope) -> bool
    {
        for (std::string_view name : scope.names_)
        {
            auto it = current->children.find(name);
            if (it == current->children.end())
                return false;

            current = it->second.get();
        }
        return true;
    };

    if (! process(start) || ! process(end))
        return nullptr;

    if (! searchParents)
        return current;

    while (current)
    {
        auto it = current->children.find(typeName);
        if (it != current->children.end())
            return it->second.get();

        current = current->parent;
    }

    return nullptr;
}

} // namespace astfri::csharp

#endif // CSHARP_SCOPE_TREE_INL