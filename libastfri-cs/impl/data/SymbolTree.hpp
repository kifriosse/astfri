#ifndef CSHARP_SYMB_TREE_HPP
#define CSHARP_SYMB_TREE_HPP

#include <libastfri-cs/impl/data/ScopeTree.hpp>

#include <string>
#include <variant>

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

class SymbolTree
{
public:
    using Data      = std::variant<std::string, TypeBinding>;
    using Tree      = ScopeTree<Data>;
    using ScopeNode = Tree::Node;

private:
    Tree tree_{};

public:
    ScopeNode* root();
    /**
     * @brief Adds a namespace/scope to the symbol tree
     * @param scope scope/namespace to add
     * @return pointer to the last node of the added namespace (can be ignored)
     */
    ScopeNode* add_namespace(const Scope& scope);
    /**
     * @brief Adds a type to the symbol tree under the given scope
     * @param scope scope/namespace under which to add the type
     * @param type type to add
     * @param def pointer to the type definition statement
     */
    ScopeNode* add_type(
        const Scope& scope,
        ScopedType* type,
        UserTypeDefStmt* def
    );
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
    [[nodiscard]] TypeBinding* find_type(
        ScopeNode* start,
        std::string_view typeName
    ) const;
    ScopeNode* find_node(ScopeNode* start, std::string_view name) const;
};
} // namespace astfri::csharp

#endif // CSHARP_SYMB_TREE_HPP
