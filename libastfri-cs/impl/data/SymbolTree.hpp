#ifndef CSHARP_SYMB_TREE_HPP
#define CSHARP_SYMB_TREE_HPP

#include <libastfri-cs/impl/CSFwd.hpp>

#include <memory>
#include <span>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace astfri {
// forward declaration
struct PrimitiveType;
struct Scope;
struct UserTypeDefStmt;
struct ScopedType;
} // namespace astfri

namespace astfri::csharp {
struct SourceFile;
class ScopeNode;

/**
 * @brief Helper struct to group type and its definition statement.
 * Also contains pointer to node in symbol tree where type is stored.
 */
struct TypeBinding {
    ScopeNode* treeNode{nullptr};
    ScopedType* type;
    UserTypeDefStmt* def;
};

/**
 * @brief Helper struct for storing temporary string representation of a
 * qualified name. Used whne qualified name can't be resolved
 */
struct ExternalMarker {
    std::string qualifiedName;
};

/**
 * @brief wrapper struct for primitive types
 */
struct CSPrimitiveType {
    Type* primitiveType;
};

/**
 * @brief represents an alias for a type or namespace. If it contains string and
 * not a pointer to ScopeNode, it means that alias couldn't be resolved.
 */
using Alias = std::variant<std::string, ScopeNode*>;

/**
 * @brief Class for representing namespace
 */
class Nms {
private:
    std::string name_{};
    std::unordered_map<SourceFile*, std::vector<TypeBinding>> staticUsings_{};
    IdentifierMap<std::unordered_map<SourceFile*, Alias>> aliases_{};

public:
    /**
     * @brief Constructor of Nms
     * @param name name of the namespace
     */
    explicit Nms(std::string name);

    /**
     * @brief Adds local static using directive into a namespace.
     * @param src source file in which the static using directive is defined
     * @param type type binding of type used in static using directive
     */
    void add_static_using(SourceFile* src, TypeBinding type);
    /**
     * @brief Adds local alias using directive into a namespace
     * @param aliasName name of the alias
     * @param src source file in which the alias using directive is defined
     * @param alias alias for type or namespace to be added
     */
    void add_alias(std::string aliasName, SourceFile* src, Alias alias);

    /**
     * @brief Finds local alias in this namespace by its name and source file in
     * which it's defined.
     * @param aliasName name of the alias searched for
     * @param src pointer source file in which the alias using directive is
     * defined
     * @return pointer to alias if found, nullptr otherwise
     */
    const Alias* find_alias(std::string_view aliasName, SourceFile* src) const;
    /**
     * @brief Returns a view of type bindings for local static using directives
     * defined in a this namespace for a given source file.
     * @param src pointer to source file in which the static using directives
     * are defined
     * @return a view of type bindings for local static using directives
     * defined in a this namespace for a given source file
     */
    std::span<const TypeBinding> get_static_usings(SourceFile* src) const;
    const std::string& get_name() const;
};

/**
 * @brief Class for representing a node in symbol tree. It can represent either
 * a namespace, type or external marker.
 * @node Node is not copyable or movable
 */
class ScopeNode {
private:
    /**
     * @brief Content of the node. Can be namespace, type or external marker.
     */
    using NodeData = std::variant<Nms, TypeBinding, ExternalMarker, CSPrimitiveType>;
    NodeData data_;
    IdentifierMap<std::unique_ptr<ScopeNode>> children_{};
    ScopeNode* parent_;

public:
    /**
     * @brief Constructor of ScopeNode
     * @param content content of the node
     * @param parent pointer to parent node in symbol tree, default value is
     * nullptr
     */
    explicit ScopeNode(NodeData content, ScopeNode* parent = nullptr);

    ScopeNode(const ScopeNode& other)            = delete;
    ScopeNode(ScopeNode&& other)                 = delete;
    ScopeNode& operator=(const ScopeNode& other) = delete;
    ScopeNode& operator=(ScopeNode&& other)      = delete;

    /**
     * @brief Returns content of the node
     * @return content of the node
     */
    [[nodiscard]] ScopeNode::NodeData& data();

    ScopeNode* parent() const;
    ScopeNode* find_child(std::string_view childName) const;
    ScopeNode* add_child(std::string name, NodeData content);

    /**
     * @brief Checks if node contains data of type \c T and returns pointer to
     * it if it does.
     * @tparam T type to check for
     * @return pointer to data of type \c T if node contains it,
     * nullptr otherwise
     * @note requires that \c T is one of the types in \c NodeData variant
     */
    template<typename T>
    requires requires(NodeData v) { std::get_if<T>(&v); }
    T* is_a();
    /**
     * @brief Returns reference to data of type \c T if node contains it.
     * @tparam T type to get from node
     * @return reference to data of type \c T if node contains it
     * @throws std::bad_variant_access if node doesn't contain data of type \c T
     * @note requires that \c T is one of the types in \c NodeData variant
     */
    template<typename T>
    requires requires(NodeData v) { std::get_if<T>(&v); }
    T& data();
};

/**
 * @brief Class for representing symbol (type) tree.
 * @note SymbolTree is not copyable or movable
 */
class SymbolTree {
private:
    std::unique_ptr<ScopeNode> root_;

public:
    SymbolTree();
    SymbolTree(const SymbolTree& other)            = delete;
    SymbolTree& operator=(const SymbolTree& other) = delete;
    SymbolTree(SymbolTree&&) noexcept              = delete;
    SymbolTree& operator=(SymbolTree&&) noexcept   = delete;

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
     * @param tb
     */
    ScopeNode* add_type(const Scope& scope, const TypeBinding& tb);

    ScopeNode* add_primitive(std::string name, CSPrimitiveType primitive);

    /**
     * @brief Finds a node in the symbol tree by its scope.
     * @param scope scope of the node to find
     * @return pointer to the node if found, nullptr otherwise
     */
    [[nodiscard]] ScopeNode* find_node(const Scope& scope) const;
    /**
     * @brief Finds a node in the symbol tree with given start and end scope.
     * Used for searching for partialy qualified names
     * @param start scope of the node to find.
     * @param end scope of the node to find
     * @return pointer to the node if found, nullptr otherwise
     */
    [[nodiscard]] ScopeNode* find_node(const Scope& start, const Scope& end) const;
};

/**
 * @brief Converts alias to type binding
 * @param alias alias to convert
 * @return pointer to type binding if alias is resolved and points to type,
 * nullptr otherwise
 */
TypeBinding* type_from_alias(const Alias& alias);

} // namespace astfri::csharp

#include <libastfri-cs/impl/data/SymbolTree.inl>

#endif // CSHARP_SYMB_TREE_HPP
