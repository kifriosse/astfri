#ifndef CSHARP_SCOPE_TREE_HPP
#define CSHARP_SCOPE_TREE_HPP

#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <memory>
#include <string_view>
// ReSharper disable once CppUnusedIncludeDirective
#include <ranges>

namespace astfri::csharp
{

template<typename T>
struct ScopeNode
{
    T data;
    IdentifierMap<std::unique_ptr<ScopeNode>> children{};
    ScopeNode* parent{nullptr};
};

template<typename T>
class ScopeTree
{
public:
    using Node = ScopeNode<T>;

private:
    std::unique_ptr<Node> root_;

public:
    ScopeTree();
    Node* root();
    Node* add_namespace(const Scope& scope);
    Node* add_data(
        const Scope& scope,
        const T& data,
        const std::string& identifier
    );
    [[nodiscard]] Node* find_node(
        const Scope& scope,
        std::string_view typeName,
        bool searchParents = false
    ) const;
    [[nodiscard]] Node* find_node(
        const Scope& start,
        const Scope& end,
        std::string_view typeName,
        bool searchParents = false
    ) const;
    [[nodiscard]] Node* find_node(Node& start, std::string_view typeName) const;
};

template<typename T>
class ScopeTreeCursor
{
private:
    ScopeNode<T>* current_;

public:
    explicit ScopeTreeCursor(ScopeNode<T>& root);
    ScopeNode<T>* current();
    bool try_goto_parent();
    bool try_goto_child(std::string_view child);
};

} // namespace astfri::csharp

#include <libastfri-cs/impl/data/ScopeTree.inl>

#endif // CSHARP_SCOPE_TREE_HPP
