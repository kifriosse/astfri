#ifndef CSHARP_SEMANTIC_CONTEXT_HPP
#define CSHARP_SEMANTIC_CONTEXT_HPP

#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri-cs/impl/data/AccessType.hpp>
#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri-cs/impl/data/SymbolTable.hpp>

#include <unordered_map>
#include <vector>

namespace astfri {
// Forward declarations
struct VarDefStmt;
struct LocalVarDefStmt;
struct Stmt;
struct UserTypeDefStmt;
} // namespace astfri

namespace astfri::csharp {

/**
 * @brief Context for tracking scopes for resolving variable references and
 * local functions
 */
struct ScopeContext {
    std::vector<std::vector<Stmt*>> scopeStack{};
    IdentifierMap<ParamVarDefStmt*> params{};
    IdentifierMap<LocalVarDefStmt*> localVars{};
    IdentifierMap<FuncMetadata> functions{};
};

/**
 * @brief Context for semantics. Contains symbol table and contexts
 * @note Class contains reference to SymbolTable. SymbolTable's lifetime
 * must exceed or be same as SemanticContext's lifetime.
 */
class SemanticContext {
private:
    ScopeContext scopeContext_{};
    std::optional<TypeBinding> typeContext_{};
    std::vector<Type*> retTypeContext_{};
    SymbolTable& symbTable_;

public:
    explicit SemanticContext(SymbolTable& symbTable);

    auto get_type_metadata() const;

    void enter_type(TypeBinding& tb);
    void enter_scope();

    void reg_return(Type* returnType);
    void reg_local_var(LocalVarDefStmt* varDef);
    void reg_param(ParamVarDefStmt* varDef);
    void reg_local_func(FuncMetadata funcMeta);

    void leave_type();
    void leave_scope();
    void unregister_return_type();

    std::optional<TypeBinding> current_type() const;
    Type* current_return_type() const;
    VarDefStmt* find_var(std::string_view name, access::Qualifier qualifier) const;
    const FuncMetadata* find_func(std::string_view funcName) const;
    const MethodMetadata* find_method(const MethodId& methodId, UserTypeDefStmt* owner) const;
    MemberVarMetadata* find_memb_var(std::string_view name, UserTypeDefStmt* owner) const;
    InvocationType find_invoc_type(InvocationId id, access::Qualifier quelifier) const;
};

} // namespace astfri::csharp

#include <libastfri-cs/impl/SemanticContext.inl>

#endif // CSHARP_SEMANTIC_CONTEXT_HPP