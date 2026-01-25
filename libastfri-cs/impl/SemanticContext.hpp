#ifndef CSHARP_SEMANTIC_CONTEXT_HPP
#define CSHARP_SEMANTIC_CONTEXT_HPP

#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/AccessType.hpp>
#include <libastfri-cs/impl/data/Metadata.hpp>

#include <stack>
#include <unordered_map>
#include <vector>

namespace astfri
{
// Forward declarations
struct VarDefStmt;
struct LocalVarDefStmt;
struct Stmt;
struct UserTypeDefStmt;
} // namespace astfri

namespace astfri::csharp
{
// Forward declaration
struct SymbolTable;

/**
 * @brief Context for tracking current type during semantic analysis
 */
struct TypeContext
{
    std::stack<UserTypeDefStmt*> typeStack;
};

/**
 * @brief Context for tracking scopes for resolving variable references
 */
struct ScopeContext
{
    std::stack<std::vector<Stmt*>> scopeStack{};
    IdentifierMap<ParamVarDefStmt*> params{};
    IdentifierMap<LocalVarDefStmt*> localVars{};
    IdentifierMap<FuncMetadata> functions{};
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
        const std::vector<UserTypeDefStmt*>& userTypesMetadata;
        using iterator = std::vector<UserTypeDefStmt*>::const_iterator;
        [[nodiscard]] iterator begin() const;
        [[nodiscard]] iterator end() const;
    };

private:
    ScopeContext scopeContext_{};
    TypeContext typeContext_{};
    std::stack<Type*> retTypeContext_{};
    SymbolTable& symbTable_;

public:
    explicit SemanticContext(SymbolTable& symbTable);

    /**
     * @brief Type for iterating over keys of user types in symbol table
     * @return An iterable view over user types in symbol table
     * @note Keys will be in order of their insertion into symbol table
     */
    SymbolTableKV get_user_types() const;
    TypeMetadata* get_type_metadata(UserTypeDefStmt* userType) const;

    void enter_type(UserTypeDefStmt* def);
    void enter_scope();

    void reg_return(Type* returnType);
    void reg_local_var(LocalVarDefStmt* varDef);
    void reg_param(ParamVarDefStmt* varDef);
    void reg_local_func(FuncMetadata funcMeta);

    void leave_type();
    void leave_scope();
    void unregister_return_type();

    UserTypeDefStmt* current_type() const;
    Type* current_return_type() const;
    VarDefStmt* find_var(
        std::string_view name,
        access::Qualifier qualifier
    ) const;
    const FuncMetadata* find_func(std::string_view funcName) const;
    const MethodMetadata* find_method(
        const MethodId& methodId,
        UserTypeDefStmt* owner
    ) const;
    MemberVarMetadata* find_memb_var(
        std::string_view name,
        UserTypeDefStmt* owner
    ) const;
    CallType find_invoc_type(
        InvocationId id,
        // todo redo this into an InvocationID
        access::Qualifier quelifier
    ) const;
};
} // namespace astfri::csharp

#endif // CSHARP_SEMANTIC_CONTEXT_HPP