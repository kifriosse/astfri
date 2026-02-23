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
    explicit SymbolTable(std::vector<ScopeNode*> implicitUsings = {});

    SymbolTable(const SymbolTable& other)            = delete;
    SymbolTable& operator=(const SymbolTable& other) = delete;

    const SymbolTree& symb_tree();

    void add_glob_using(ScopeNode* node);
    void add_glob_static_using(const TypeBinding& type);
    void add_glob_alias(std::string name, Alias alias);
    ScopeNode* add_type(
        const TypeBinding& tb,
        const TSNode& node,
        SourceFile* src
    );

    auto get_type_metadata();

    TypeMetadata* get_type_metadata(UserTypeDefStmt* def);
    span<UserTypeDefStmt* const> get_user_types();
    span<const ScopeNode* const> get_glob_usings();
    span<const TypeBinding> get_glob_static_usings();

    Alias* get_glob_alias(std::string_view name);
};

} // namespace astfri::csharp

#include <libastfri-cs/impl/data/SymbolTable.inl>

#endif // CSHARP_SYMBOL_TABLE_HPP