#ifndef CSHARP_SYMBOL_TABLE_HPP
#define CSHARP_SYMBOL_TABLE_HPP

#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri-cs/impl/data/SymbolTree.hpp>

#include <ranges>
#include <span>
#include <unordered_map>
#include <vector>

namespace astfri
{
// forward declaration
struct UserTypeDefStmt;
} // namespace astfri

namespace astfri::csharp
{
// forward declaration
class TypeMetadata;
struct TypeBinding;

/**
 * @brief Symbol table containing metadata about user defined types
 * @note SymbolTable is not copyable
 */
class SymbolTable
{
private:
    template<typename T>
    using span = std::span<T>;

    std::unordered_map<UserTypeDefStmt*, TypeMetadata> userTypeMetadata_;
    std::vector<UserTypeDefStmt*> userTypes_;
    IdentifierMap<Alias> globAliases_;
    std::vector<ScopeNode*> globUsings_;
    std::vector<TypeBinding> globStaticUsings_;
    SymbolTree symbTree_{};

public:
    /**
     * @brief Constructor of SymbolTable
     * @param implicitUsings list of implicit global using directives
     */
    explicit SymbolTable(std::vector<ScopeNode*> implicitUsings = {});

    SymbolTable(const SymbolTable& other)            = delete;
    SymbolTable& operator=(const SymbolTable& other) = delete;

    const SymbolTree& symb_tree();

    /**
     * Adds global using directive
     * @param node scope node that points to namespace in using directive
     */
    void add_glob_using(ScopeNode* node);
    /**
     * Adds global static using directive
     * @param type type binding, which represents type used in static using
     * directive
     */
    void add_glob_static_using(const TypeBinding& type);
    /**
     * @brief Adds global alias using directive
     * @param name name of alias
     * @param alias alias for type or namespace
     */
    void add_glob_alias(std::string name, Alias alias);
    /**
     * @brief Adds type to symbol table and symbol tree and retuns pointer to
     * scope node of added type in symbol tree
     * @param tb type binding of type to be added
     * @param node tree-sitter node of type definition
     * @param src source file where type is defined
     * @return pointer to scope node of added type in symbol tree
     */
    ScopeNode* add_type(
        const TypeBinding& tb,
        const TSNode& node,
        SourceFile* src
    );

    /**
     * @brief Returns a view of pointers to type metadata of user defined type
     * inside symbol table
     * @return a view of pointers to type metadata of user defined types added
     * into symbol table
     * @note Order of metadata is in order in which they were discovered/added.
     * View won't contain nullpointers
     */
    auto get_type_metadata();

    /**
     * @brief Returns pointer to type metadata of given user defined type
     * @param def user defined type definition statement
     * @return pointer to type metadata of given user defined type if it exists
     * in symbol table, nullptr otherwise
     */
    TypeMetadata* get_type_metadata(UserTypeDefStmt* def);
    /**
     * @brief Returns a view of global using directives
     * @return a view of pointers to ScopeNodes to which the global using
     * directives point
     */
    span<const ScopeNode* const> get_glob_usings();
    /**
     * @brief Returns a view of global static using directives
     * @return a view of type bindings which represent types used in global
     * static using directives
     */
    span<const TypeBinding> get_glob_static_usings();
    /**
     * @brief Gets global alias for given name
     * @param name name of alias to get
     * @return pointer to alias for given name, if can't be found returns
     * nullptr
     */
    Alias* get_glob_alias(std::string_view name);
};

} // namespace astfri::csharp

#include <libastfri-cs/impl/data/SymbolTable.inl>

#endif // CSHARP_SYMBOL_TABLE_HPP