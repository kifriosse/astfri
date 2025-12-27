#include <libastfri-cs/impl/SemanticContext.hpp>

#include <optional>

namespace astfri::csharp
{

bool FunctionIdentifier::operator==(const FunctionIdentifier& other) const
{
    return name == other.name && param_count == other.param_count;
}

bool MethodIdentifier::operator==(const MethodIdentifier& other) const
{
    return func_id == other.func_id && is_static == other.is_static;
}

SemanticContext::SymbolTableKV::iterator SemanticContext::SymbolTableKV::
    begin() const
{
    return user_types_metadata_.begin();
}

SemanticContext::SymbolTableKV::iterator SemanticContext::SymbolTableKV::
    end() const
{
    return user_types_metadata_.end();
}

SemanticContext::SemanticContext(SymbolTable& symbol_table) :
    symbol_table_(symbol_table)
{
}

SemanticContext::SymbolTableKV SemanticContext::get_user_types() const
{
    return SymbolTableKV{symbol_table_.user_type_keys};
}

std::optional<TypeMetadata> SemanticContext::get_type_metadata(
    UserTypeDefStmt* user_type
) const
{
    const auto& it = symbol_table_.user_types_metadata.find(user_type);
    if (it != symbol_table_.user_types_metadata.end())
        return it->second;
    return {};
}

void SemanticContext::enter_type(UserTypeDefStmt* def_stmt)
{
    type_context_.type_stack.push(def_stmt);
    enter_scope();
}

void SemanticContext::enter_scope()
{
    scope_context_.scope_stack.emplace();
}

void SemanticContext::reg_local_var(LocalVarDefStmt* var_def)
{
    scope_context_.scope_stack.top().push_back(var_def);
    scope_context_.local_vars.emplace(var_def->name_, var_def);
}

void SemanticContext::reg_param(ParamVarDefStmt* var_def)
{
    scope_context_.scope_stack.top().push_back(var_def);
    scope_context_.params.emplace(var_def->name_, var_def);
}

void SemanticContext::reg_local_func(FunctionDefStmt* func_def)
{
    scope_context_.scope_stack.top().push_back(func_def);
    FunctionIdentifier func_id{
        .name        = func_def->name_,
        .param_count = func_def->params_.size(),
    };
    scope_context_.function_map.emplace(func_id, func_def);
}

void SemanticContext::reg_return(Type* return_type)
{
    return_type_context_.push(return_type);
}

void SemanticContext::leave_type()
{
    if (type_context_.type_stack.empty())
        type_context_.type_stack.pop();
    leave_scope();
}

void SemanticContext::leave_scope()
{
    if (scope_context_.scope_stack.empty())
        return;

    for (const auto scope_memb : scope_context_.scope_stack.top())
    {
        if (const auto param = as_a<ParamVarDefStmt>(scope_memb))
            scope_context_.params.erase(param->name_);
        else if (const auto var = as_a<LocalVarDefStmt>(scope_memb))
            scope_context_.local_vars.erase(var->name_);
        else if (const auto func = as_a<FunctionDefStmt>(scope_memb))
        {
            scope_context_.function_map.erase(
                FunctionIdentifier{
                    .name        = func->name_,
                    .param_count = func->params_.size(),
                }
            );
        }
    }
    scope_context_.scope_stack.pop();
}

void SemanticContext::unregister_return_type()
{
    if (! return_type_context_.empty())
        return_type_context_.pop();
}

UserTypeDefStmt* SemanticContext::current_type() const
{
    const auto& type_stack = type_context_.type_stack;
    return type_stack.empty() ? nullptr : type_stack.top();
}

Type* SemanticContext::current_return_type() const
{
    return return_type_context_.empty() ? nullptr : return_type_context_.top();
}

VarDefStmt* SemanticContext::find_var(
    const std::string& name,
    const AccessType type
) const
{
    switch (type)
    {
    case AccessType::Static:
    case AccessType::Instance:
    {
        // todo implement search in parent classes - only for instance
        const auto it_local = scope_context_.local_vars.find(name);
        return it_local != scope_context_.local_vars.end() ? it_local->second
                                                           : nullptr;
    }
    case AccessType::None:
    {
        if (const auto it_local = scope_context_.local_vars.find(name);
            it_local != scope_context_.local_vars.end())
            return it_local->second;

        if (const auto it_param = scope_context_.params.find(name);
            it_param != scope_context_.params.end())
            return it_param->second;

        UserTypeDefStmt* current_type = this->current_type();

        if (! current_type)
            return nullptr;

        const auto& meta_data = symbol_table_.user_types_metadata;
        const auto it_type    = meta_data.find(current_type);
        if (it_type != symbol_table_.user_types_metadata.end())
        {
            const auto& member_vars = it_type->second.member_vars;
            const auto& it_memb     = member_vars.find(name);
            if (it_memb != member_vars.end())
                return it_memb->second.var_def;
        }
        break;
    }
    case AccessType::Base:
        // todo implement base access
        break;
    case AccessType::Unknown:
        // can't resolve type of expression
        break;
    }

    return nullptr;
}

FunctionDefStmt* SemanticContext::find_func(
    const FunctionIdentifier& func_id
) const
{
    const auto it_func = scope_context_.function_map.find(func_id);
    return it_func == scope_context_.function_map.end() ? nullptr
                                                        : it_func->second;
}

InvocationType SemanticContext::find_invocation_type(
    const FunctionIdentifier& func_id,
    AccessType access_type
) const
{
    return InvocationType::Unknown;
}

MemberVarMetadata* SemanticContext::get_memb_var_data(
    const std::string& name,
    UserTypeDefStmt* owner
) const
{
    auto& user_types             = symbol_table_.user_types_metadata;
    const auto& it_type_metadata = user_types.find(owner);
    if (it_type_metadata == user_types.end())
        return nullptr;

    auto& member_vars  = it_type_metadata->second.member_vars;
    const auto& it_var = member_vars.find(name);
    return it_var == member_vars.end() ? nullptr : &it_var->second;
}

MethodMetadata* SemanticContext::find_method(
    const MethodIdentifier& method_id,
    UserTypeDefStmt* owner
) const
{
    auto& user_types            = symbol_table_.user_types_metadata;
    const auto it_type_metadata = user_types.find(owner);
    if (it_type_metadata == user_types.end())
        return nullptr;

    auto& methods        = it_type_metadata->second.methods;
    const auto it_method = methods.find(method_id);
    return it_method == methods.end() ? nullptr : &it_method->second;
}

} // namespace astfri::csharp
