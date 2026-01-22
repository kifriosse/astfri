#ifndef CSHARP_SYMBOL_TABLE_HPP
#define CSHARP_SYMBOL_TABLE_HPP

#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/util/AstfriUtil.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace astfri::csharp
{
// forward declaration
struct TypeMetadata;

struct SymbolTreeNode
{
    IdentifierMap<std::unique_ptr<SymbolTreeNode>> children{};
    std::variant<std::string, util::TypeBinding> data;
};

class SymbolTree
{
private:
    std::unique_ptr<SymbolTreeNode> root_;

public:
    SymbolTree();
    /**
     * @brief Adds a namespace/scope to the symbol tree
     * @param scope scope/namespace to add
     * @return pointer to the last node of the added namespace (can be ignored)
     */
    SymbolTreeNode* add_namespace(const Scope& scope);
    /**
     * @brief Adds a type to the symbol tree under the given scope
     * @param scope scope/namespace under which to add the type
     * @param type type to add
     * @param def pointer to the type definition statement
     */
    void add_type(const Scope& scope, ScopedType* type, UserTypeDefStmt* def);
    /**
     * @brief Resolves type by traversing the given scope
     * @param scope namespace/scope to search in
     * @param typeName identifier of the type to find
     * @param searchParents if true, it will also search in parent
     * scopes/namespaces
     * @return pointer to the found type, or nullptr if not found
     */
    [[nodiscard]] util::TypeBinding* find_type(
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
    [[nodiscard]] util::TypeBinding* find_type(
        const Scope& start,
        const Scope& end,
        std::string_view typeName,
        bool searchParents = false
    ) const;
};

/**
 * @brief Symbol table containing metadata about user defined types
 */
struct SymbolTable
{
    std::unordered_map<UserTypeDefStmt*, TypeMetadata> userTypeMetadata;
    std::vector<UserTypeDefStmt*> userTypeKeys;
    SymbolTree symbTree{};

    std::vector<Scope> globUsings;
    std::vector<UserTypeDefStmt*> globStaticUsings;
};

} // namespace astfri::csharp

#endif // CSHARP_SYMBOL_TABLE_HPP