#ifndef CSHARP_SYMB_TREE_HPP
#define CSHARP_SYMB_TREE_HPP

#include <libastfri-cs/impl/CSFwd.hpp>

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace astfri
{
// forward declaration
struct UserTypeDefStmt;
struct ScopedType;
} // namespace astfri

namespace astfri::csharp
{

/**
 * @brief Helper struct to group type and its definition statement
 */
struct TypeBinding
{
    ScopedType* type;
    UserTypeDefStmt* def;
};

class SymbolNode;

using Alias = std::variant<std::monostate, std::string, SymbolNode*>;

/**
 * @brief class for representing namespace
 */
class Nms
{
private:
    std::string name_{};
    std::vector<ScopedType*> staticUsings_{};
    IdentifierMap<Alias> aliases_{};
public:
    explicit Nms(std::string name);
};

class SymbolNode
{
private:
    using Content = std::variant<Nms, TypeBinding>;
    Content content_;
    IdentifierMap<std::unique_ptr<SymbolNode>> children_;
    SymbolNode* parent_;

public:
    explicit SymbolNode(Content content, SymbolNode* parent = nullptr);

    SymbolNode(const SymbolNode& other)            = delete;
    SymbolNode(SymbolNode&& other)                 = delete;
    SymbolNode& operator=(const SymbolNode& other) = delete;
    SymbolNode& operator=(SymbolNode&& other)      = delete;

    SymbolNode* parent();
    SymbolNode* find_child(std::string_view childName);
    SymbolNode* try_add_child(
        std::string name,
        Content content,
        SymbolNode* parent
    );

    template<typename T>
    requires requires(Content v) { std::get_if<T>(&v); }
    T* is_content();
};

class SymbolTree
{
private:
    std::unique_ptr<SymbolNode> root_;

public:
    SymbolTree();
    SymbolNode* root();
    /**
     * @brief Adds a namespace/scope to the symbol tree
     * @param scope scope/namespace to add
     * @return pointer to the last node of the added namespace (can be ignored)
     */
    SymbolNode* add_scope(const Scope& scope);
    /**
     * @brief Adds a type to the symbol tree under the given scope
     * @param scope scope/namespace under which to add the type
     * @param typeBinding
     */
    SymbolNode* add_type(const Scope& scope, TypeBinding typeBinding);
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
        SymbolNode& start,
        std::string_view typeName
    );
};

class SymbolTreeCursor
{
private:
    SymbolNode* current_;

public:
    explicit SymbolTreeCursor(SymbolNode& root);
    SymbolNode* current();
    bool go_to_parent();
    bool go_to_child(std::string_view childName);
};

TypeBinding* type_from_alias(const Alias& alias);

} // namespace astfri::csharp

#include <libastfri-cs/impl/data/SymbolTree.inl>

#endif // CSHARP_SYMB_TREE_HPP
