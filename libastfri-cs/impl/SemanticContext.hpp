#ifndef CSHARP_SEMANTIC_CONTEXT
#define CSHARP_SEMANTIC_CONTEXT

#include <libastfri/inc/Astfri.hpp>

#include <stack>

namespace astfri::csharp
{

enum class VarType
{
    Global,
    Member,
    Param,
    Local,
};

enum class MemberBinding
{
    None,
    Instance,
    Static
};

// struct VarScope
// {
//     VarType type;
//     std::vector<UserTypeDefStmt*> scope_types;
// };

class ScopeContext
{
private:
    std::stack<std::vector<Stmt*>> scope_stack_;
    std::unordered_map<std::string, MemberVarDefStmt*> member_var_map_;
    std::unordered_map<std::string, ParamVarDefStmt*> param_var_map_;
    std::unordered_map<std::string, LocalVarDefStmt*> local_var_map_;
    std::unordered_map<std::string, std::vector<MethodDefStmt*>> method_map;
    std::unordered_map<std::string, std::vector<MethodDefStmt*>> static_method_map;
    std::unordered_map<std::string, std::vector<FunctionDefStmt*>> function_map;
public:
    void enter_scope();
    void register_member_var(MemberVarDefStmt* var_def);
    void register_local_var(LocalVarDefStmt* var_def);
    void register_param_var(ParamVarDefStmt* var_def);

    void register_method(MethodDefStmt* method_def);
    void register_static_method(MethodDefStmt* method_def);
    void register_local_func(FunctionDefStmt* func_def);
    void leave_scope();
    VarDefStmt* get_var_type(const std::string& name) const;
};

class SemanticContext
{
private:
    ScopeContext scope_context_;
    std::stack<UserTypeDefStmt*> type_context_;
    std::stack<Type*> return_type_context_;

public:
    void enter_type(UserTypeDefStmt* def_stmt);
    void enter_scope();
    void register_return_type(Type* return_type);

    void add_member_var(MemberVarDefStmt* var_def);
    void add_local_var(LocalVarDefStmt* var_def);
    void add_param_var(ParamVarDefStmt* var_def);

    void leave_type();
    void leave_scope();
    void unregister_return_type();

    UserTypeDefStmt* current_user_type() const;
    Type* current_return_type() const;
    VarDefStmt* find_var(const std::string& name) const;
    MemberVarDefStmt* find_member_var(const std::string& name) const;
};

} // namespace astfri::csharp

#endif // CSHARP_SEMANTIC_CONTEXT