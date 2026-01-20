#ifndef CSHARP_SYMBOL_TABLE_HPP
#define CSHARP_SYMBOL_TABLE_HPP

#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/util/astfri_util.hpp>
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
     * @param def
     */
    void add_type(const Scope& scope, ScopedType* type, UserTypeDefStmt* def);
    /**
     * @brief Resolves type by traversing the given scope
     * @param scope namespace/scope to search in
     * @param type_name identifier of the type to find
     * @param search_parents
     * @return pointer to the found type, or nullptr if not found
     */
    [[nodiscard]] util::TypeBinding* find_type(
        const Scope& scope,
        std::string_view type_name,
        bool search_parents = false
    ) const;
    /**
     * @brief Resolves a type by traversing a concatenated path (start + end).
     * @param start base namespace/scope.
     * @param end relative namespace/scope qualification.
     * @param type_name identifier of the type to find.
     * @param search_parents
     * @return pointer to the found type, or nullptr if type was not found in
     * that namespace/scope
     */
    [[nodiscard]] util::TypeBinding* find_type(
        const Scope& start,
        const Scope& end,
        std::string_view type_name,
        bool search_parents = false
    ) const;
};

/**
 * @brief Symbol table containing metadata about user defined types
 */
struct SymbolTable
{
    std::unordered_map<UserTypeDefStmt*, TypeMetadata> user_types_metadata;
    std::vector<UserTypeDefStmt*> user_type_keys;
    SymbolTree symb_tree{};

    std::vector<Scope> glob_usings;
    std::vector<UserTypeDefStmt*> glob_static_usings;
};

} // namespace astfri::csharp

#endif // CSHARP_SYMBOL_TABLE_HPP