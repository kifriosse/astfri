#include <libastfri-cs/impl/SemanticContext.hpp>

namespace astfri::csharp
{

void ScopeContext::enter_scope()
{
    scope_stack_.emplace();
}

void ScopeContext::register_member_var(MemberVarDefStmt* var_def)
{
    scope_stack_.top().push_back(var_def);
    member_var_map_.emplace(var_def->name_, var_def);
}

void ScopeContext::register_local_var(LocalVarDefStmt* var_def)
{
    scope_stack_.top().push_back(var_def);
    local_var_map_.emplace(var_def->name_, var_def);
}

void ScopeContext::register_param_var(ParamVarDefStmt* var_def)
{
    scope_stack_.top().push_back(var_def);
    param_var_map_.emplace(var_def->name_, var_def);
}

void ScopeContext::register_method(MethodDefStmt* method_def)
{
    scope_stack_.top().push_back(method_def);
    const std::string name = method_def->func_->name_;
    method_map[name].push_back(method_def);
}

void ScopeContext::register_static_method(MethodDefStmt* method_def)
{
    scope_stack_.top().push_back(method_def);
    const std::string name = method_def->func_->name_;
    static_method_map[name].push_back(method_def);
}

void ScopeContext::register_local_func(FunctionDefStmt* func_def)
{
    scope_stack_.top().push_back(func_def);
    const std::string name = func_def->name_;
    function_map[name].push_back(func_def);
}

void ScopeContext::leave_scope()
{
    if (scope_stack_.empty())
        return;

    const std::vector<Stmt*> scope = scope_stack_.top();
    for (const auto scope_memb : scope)
    {
        if (const auto memb_var = as_a<MemberVarDefStmt>(scope_memb))
            member_var_map_.erase(memb_var->name_);
        else if (const auto param = as_a<ParamVarDefStmt>(scope_memb))
            param_var_map_.erase(param->name_);
        else if (const auto var = as_a<LocalVarDefStmt>(scope_memb))
            local_var_map_.erase(var->name_);
        else if (const auto method = as_a<MethodDefStmt>(scope_memb))
            method_map.erase(method->func_->name_);
        else if (const auto func = as_a<FunctionDefStmt>(scope_memb))
            function_map.erase(func->name_);
    }
    scope_stack_.pop();
}

VarDefStmt* ScopeContext::get_var_type(const std::string& name) const
{
    // todo add ClassRef and Property ref
    const auto it_local = local_var_map_.find(name);
    if (it_local != local_var_map_.end())
        return it_local->second;

    const auto it_param = param_var_map_.find(name);
    if (it_param != param_var_map_.end())
        return it_param->second;

    const auto it_member = member_var_map_.find(name);
    if (it_member != member_var_map_.end())
        return it_member->second;

    return nullptr;
}

void SemanticContext::enter_type(UserTypeDefStmt* def_stmt)
{
    type_context_.push(def_stmt);
    scope_context_.enter_scope();
}

void SemanticContext::enter_scope()
{
    scope_context_.enter_scope();
}

void SemanticContext::add_member_var(MemberVarDefStmt* var_def)
{
    scope_context_.register_member_var(var_def);
}

void SemanticContext::add_local_var(LocalVarDefStmt* var_def)
{
    scope_context_.register_local_var(var_def);
}

void SemanticContext::add_param_var(ParamVarDefStmt* var_def)
{
    scope_context_.register_param_var(var_def);
}

void SemanticContext::register_return_type(Type* return_type)
{
    return_type_context_.push(return_type);
}

void SemanticContext::leave_type()
{
    if (! type_context_.empty())
        type_context_.pop();
    scope_context_.leave_scope();
}

void SemanticContext::leave_scope()
{
    scope_context_.leave_scope();
}

void SemanticContext::unregister_return_type()
{
    if (! return_type_context_.empty())
        return_type_context_.pop();
}

UserTypeDefStmt* SemanticContext::current_user_type() const
{
    return type_context_.empty() ? nullptr : type_context_.top();
}

Type* SemanticContext::current_return_type() const
{
    return return_type_context_.empty() ? nullptr : return_type_context_.top();
}

VarDefStmt* SemanticContext::find_var(const std::string& name) const
{
    return scope_context_.get_var_type(name);
}

MemberVarDefStmt* SemanticContext::find_member_var(
    const std::string& name
) const
{
    return as_a<MemberVarDefStmt>(scope_context_.get_var_type(name));
}

} // namespace astfri::csharp
