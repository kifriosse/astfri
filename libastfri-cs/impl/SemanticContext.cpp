#include <libastfri-cs/impl/SemanticContext.hpp>

namespace astfri::csharp
{

void ScopeContext::enter_scope()
{
    scope_stack_.emplace();
}

void ScopeContext::add_member_var(MemberVarDefStmt* var_def)
{
    scope_stack_.top().push_back(var_def);
    member_var_map_.emplace(var_def->name_, var_def);
}

void ScopeContext::add_local_var(LocalVarDefStmt* var_def)
{
    scope_stack_.top().push_back(var_def);
    local_var_map_.emplace(var_def->name_, var_def);
}

void ScopeContext::add_param_var(ParamVarDefStmt* var_def)
{
    scope_stack_.top().push_back(var_def);
    param_var_map_.emplace(var_def->name_, var_def);
}

void ScopeContext::leave_scope()
{
    if (scope_stack_.empty())
        return;

    const std::vector<VarDefStmt*> var_scope = scope_stack_.top();
    for (const auto variable : var_scope)
    {
        if (is_a<MemberVarDefStmt>(variable))
            member_var_map_.erase(variable->name_);
        else if (is_a<ParamVarDefStmt>(variable))
            param_var_map_.erase(variable->name_);
        else if (is_a<LocalVarDefStmt>(variable))
            local_var_map_.erase(variable->name_);
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
    scope_context_.add_member_var(var_def);
}

void SemanticContext::add_local_var(LocalVarDefStmt* var_def)
{
    scope_context_.add_local_var(var_def);
}

void SemanticContext::add_param_var(ParamVarDefStmt* var_def)
{
    scope_context_.add_param_var(var_def);
}

void SemanticContext::register_return_type(Type* func_def)
{
    return_type_context_.push(func_def);
}

void SemanticContext::leave_type()
{
    if (!type_context_.empty())
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

} // namespace astfri::csharp
