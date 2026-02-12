#ifndef CSHARP_SYMB_TREE_HPP
#define CSHARP_SYMB_TREE_HPP

#include <libastfri-cs/impl/CSFwd.hpp>

#include <memory>
#include <span>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace astfri
{
// forward declaration
struct Scope;
struct UserTypeDefStmt;
struct ScopedType;
} // namespace astfri

namespace astfri::csharp
{
struct SourceFile;
class ScopeNode;

/**
 * @brief Helper struct to group type and its definition statement
 */
struct TypeBinding
{
    ScopeNode* treeNode{nullptr};
    ScopedType* type;
    UserTypeDefStmt* def;
};

struct ExternalMarker
{
    std::string fqn;
};

using Alias = std::variant<std::monostate, std::string, ScopeNode*>;

/**
 * @brief class for representing namespace
 */
class Nms
{
private:
    std::string name_{};
    std::unordered_map<SourceFile*, std::vector<TypeBinding>> staticUsings_{};
    IdentifierMap<std::unordered_map<SourceFile*, Alias>> aliases_{};

public:
    explicit Nms(std::string name);

    void add_static_using(SourceFile* src, TypeBinding type);
    void add_alias(std::string aliasName, SourceFile* src, Alias alias);

    const Alias* find_alias(std::string_view aliasName, SourceFile* src) const;
    std::span<const TypeBinding> get_static_usings(SourceFile* src) const;
};

class ScopeNode
{
private:
    using NodeData = std::variant<Nms, TypeBinding, ExternalMarker>;
    NodeData data_;
    IdentifierMap<std::unique_ptr<ScopeNode>> children_{};
    ScopeNode* parent_;

public:
    explicit ScopeNode(NodeData content, ScopeNode* parent = nullptr);

    ScopeNode(const ScopeNode& other)            = delete;
    ScopeNode(ScopeNode&& other)                 = delete;
    ScopeNode& operator=(const ScopeNode& other) = delete;
    ScopeNode& operator=(ScopeNode&& other)      = delete;

    [[nodiscard]] const NodeData& data() const;

    ScopeNode* parent() const;
    ScopeNode* find_child(std::string_view childName);
    ScopeNode* try_add_child(
        std::string name,
        NodeData content,
        ScopeNode* parent
    );

    template<typename T>
    requires requires(NodeData v) { std::get_if<T>(&v); }
    T* has_data();
    template<typename T>
    requires requires(NodeData v) { std::get_if<T>(&v); }
    T& data();
};

class SymbolTree
{
private:
    std::unique_ptr<ScopeNode> root_;

public:
    SymbolTree();
    [[nodiscard]] ScopeNode* root() const;
    /**
     * @brief Adds a namespace/scope to the symbol tree
     * @param scope scope/namespace to add
     * @return pointer to the last node of the added namespace (can be ignored)
     */
    ScopeNode* add_scope(const Scope& scope);
    /**
     * @brief Adds a type to the symbol tree under the given scope
     * @param scope scope/namespace under which to add the type
     * @param type type to add
     * @param def definition of the type
     */
    ScopeNode* add_type(
        const Scope& scope,
        ScopedType* type,
        UserTypeDefStmt* def
    );

    [[nodiscard]] ScopeNode* find_node(const Scope& scope) const;
    [[nodiscard]] ScopeNode* find_node(
        const Scope& start,
        const Scope& end
    ) const;
    /**
     * @brief Resolves type by traversing the given scope
     * @param scope namespace/scope to search in
     * @param typeName identifier of the type to find
     * @param searchParents if true, it will also search in parent
     * scopes/namespaces
     * @return pointer to the found type, or nullptr if not found
     */
    [[nodiscard]] TypeBinding* find_type(
        const Scope& scope,
        std::string_view typeName,
        bool searchParents = false
    ) const;
    /**
     * @brief Resolves a type by traversing a concatenated path (start + end).
     * @param start base namespace/scope.
     * @param end relative namespace/scope qualification.
     * @param typeName identifier of the type to find.
     * @param searchParents if true, it will also search in parent
     * scopes/namespaces
     * @return pointer to the found type, or nullptr if type was not found in
     * that namespace/scope
     */
    [[nodiscard]] TypeBinding* find_type(
        const Scope& start,
        const Scope& end,
        std::string_view typeName,
        bool searchParents = false
    ) const;
    [[nodiscard]] static TypeBinding* find_type(
        ScopeNode& start,
        std::string_view typeName
    );
};

class SymbolTreeCursor
{
private:
    ScopeNode* current_;

public:
    explicit SymbolTreeCursor(ScopeNode& root);
    ScopeNode* current();
    bool go_to_parent();
    bool go_to_child(std::string_view childName);
};

TypeBinding* type_from_alias(const Alias& alias);

} // namespace astfri::csharp

#include <libastfri-cs/impl/data/SymbolTree.inl>

#endif // CSHARP_SYMB_TREE_HPP
