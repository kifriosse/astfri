#ifndef CSHARP_SEMANTIC_CONTEXT_HPP
#define CSHARP_SEMANTIC_CONTEXT_HPP

#include <libastfri-cs/impl/Source.hpp>
#include <libastfri-cs/impl/utils.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <optional>
#include <stack>

namespace astfri::csharp
{

struct FunctionIdentifier
{
    std::string name;
    size_t param_count{0};

    bool operator==(const FunctionIdentifier& other) const;
};

struct MethodIdentifier
{
    FunctionIdentifier func_id;
    bool is_static{false};

    bool operator==(const MethodIdentifier& other) const;
};

} // namespace astfri::csharp

#include <libastfri-cs/impl/SemanticContext.inl>

namespace astfri::csharp
{

/**
 * @brief Type of member access
 */
enum class AccessType
{
    /**
     * @brief When member access doesn't have \c this, \c base or \c ClassRef
     * prefix
     */
    None,
    /**
     * @brief When member access has \c this prefix
     */
    Instance,
    /**
     * @brief When member access has \c ClassRef prefix
     */
    Static,
    /**
     * @brief When member access has \c base prefix
     */
    Base,
    /**
     * @brief When member access is on expression (can't resolve the type)
     */
    Unknown
};

enum class InvocationType
{
    Unknown,
    Method,
    LocalFunc,
    Lambda
};

/**
 * @brief Metadata about member variable
 */
struct MemberVarMetadata
{
    MemberVarDefStmt* var_def{nullptr};
    TSNode var_node{};
    TSNode initializer{}; // right side of assignment
    // todo maybe add flag for static variable
    bool processed{false};
};

/**
 * @brief Metadata about method parameter
 */
struct ParamMetadata
{
    ParamVarDefStmt* param_def{nullptr};
    TSNode param_node{};
    bool processed{false};
};

/**
 * @brief Metadata about method
 */
struct MethodMetadata
{
    std::vector<ParamMetadata> params{};
    MethodDefStmt* method_def{nullptr};
    TSNode method_node{};
    bool processed{false};
};

/**
 * @brief Metadata about property
 */
struct PropertyNode
{
    MemberVarMetadata backing_field;
    MethodMetadata getter;
    MethodMetadata setter;
};

/**
 * @brief Location of type definition in source code
 */
struct TypeDefLoc
{
    TSNode type_node{};
    SourceCode* src_code{nullptr};
};

/**
 * @brief Metadata about user defined type
 */
struct TypeMetadata
{
    UserTypeDefStmt* user_type{nullptr};
    std::unordered_map<MethodIdentifier, MethodMetadata> methods{};
    std::unordered_map<std::string, MemberVarMetadata> member_vars{};
    std::unordered_map<std::string, PropertyNode> properties{};
    std::vector<TypeDefLoc> defs{};
    bool processed{false};
};

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
    std::unordered_map<std::string, ParamVarDefStmt*> params{};
    std::unordered_map<std::string, LocalVarDefStmt*> local_vars{};
    std::unordered_map<FunctionIdentifier, FunctionDefStmt*> function_map{};
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
    void reg_local_func(FunctionDefStmt* func_def);

    void leave_type();
    void leave_scope();
    void unregister_return_type();

    UserTypeDefStmt* current_type() const;
    Type* current_return_type() const;
    VarDefStmt* find_var(const std::string& name, AccessType type) const;
    FunctionDefStmt* find_func(const FunctionIdentifier& func_id) const;
    InvocationType find_invocation_type(
        const FunctionIdentifier& func_id,
        AccessType access_type
    ) const;

    MemberVarMetadata* get_memb_var_data(
        const std::string& name,
        UserTypeDefStmt* owner
    ) const;
    MethodMetadata* find_method(
        const MethodIdentifier& method_id,
        UserTypeDefStmt* owner
    ) const;
};
} // namespace astfri::csharp

#endif // CSHARP_SEMANTIC_CONTEXT_HPP