#ifndef CSHARP_SEMANTIC_CONTEXT_HPP
#define CSHARP_SEMANTIC_CONTEXT_HPP

#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/AccessType.hpp>
#include <libastfri-cs/impl/data/Metadata.hpp>

#include <optional>
#include <stack>
#include <unordered_map>
#include <vector>

namespace astfri
{
// Forward declarations
struct VarDefStmt;
struct Type;
struct LocalVarDefStmt;
struct Stmt;
struct UserTypeDefStmt;
} // namespace astfri

namespace astfri::csharp
{

/**
 * @brief Symbol table containing metadata about user defined types
 */
struct SymbolTable
{
    std::unordered_map<UserTypeDefStmt*, TypeMetadata> user_types_metadata;
    std::vector<UserTypeDefStmt*> user_type_keys;
};

/**
 * @brief Context for tracking current type during semantic analysis
 */
struct TypeContext
{
    std::stack<UserTypeDefStmt*> type_stack;
};

/**
 * @brief Context for tracking scopes for resolving variable references
 */
struct ScopeContext
{
    std::stack<std::vector<Stmt*>> scope_stack{};
    IdentifierMap<ParamVarDefStmt*> params{};
    IdentifierMap<LocalVarDefStmt*> local_vars{};
    std::unordered_map<FuncId, FunctionMetadata> function_map{};
};

/**
 * @brief Context for semantics. Contains symbol table and contexts
 * @note Class contains reference to SymbolTable. SymbolTable's lifetime
 * must exceed or be same as SemanticContext's lifetime.
 */
class SemanticContext
{
private:
    /**
     * @brief Helper stuct representing a view over keys of user types in symbol
     * table
     */
    struct SymbolTableKV
    {
        const std::vector<UserTypeDefStmt*>& user_types_metadata_;
        using iterator = std::vector<UserTypeDefStmt*>::const_iterator;
        [[nodiscard]] iterator begin() const;
        [[nodiscard]] iterator end() const;
    };

private:
    SymbolTable& symbol_table_;
    ScopeContext scope_context_{};
    TypeContext type_context_{};
    std::stack<Type*> return_type_context_{};

public:
    explicit SemanticContext(SymbolTable& symbol_table);

    /**
     * @brief Type for iterating over keys of user types in symbol table
     * @return An iterable view over user types in symbol table
     * @note Keys will be in order of their insertion into symbol table
     */
    SymbolTableKV get_user_types() const;
    std::optional<TypeMetadata> get_type_metadata(
        UserTypeDefStmt* user_type
    ) const;

    void enter_type(UserTypeDefStmt* def_stmt);
    void enter_scope();

    void reg_return(Type* return_type);
    void reg_local_var(LocalVarDefStmt* var_def);
    void reg_param(ParamVarDefStmt* var_def);
    void reg_local_func(FunctionMetadata func_data);

    void leave_type();
    void leave_scope();
    void unregister_return_type();

    UserTypeDefStmt* current_type() const;
    Type* current_return_type() const;
    VarDefStmt* find_var(
        std::string_view name,
        access::Qualifier qualifier
    ) const;
    const FunctionMetadata* find_func(const FuncId& func_id) const;
    const MethodMetadata* find_method(
        const MethodId& method_id,
        UserTypeDefStmt* owner
    ) const;
    MemberVarMetadata* find_memb_var(
        std::string_view name,
        UserTypeDefStmt* owner
    ) const;
    CallType find_invoc_type(
        const FuncId& func_id,
        access::Qualifier quelifier
    ) const;
};
} // namespace astfri::csharp

#endif // CSHARP_SEMANTIC_CONTEXT_HPP