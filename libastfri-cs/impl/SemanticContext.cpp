#include <libastfri-cs/impl/data/AccessType.hpp>
#include <libastfri-cs/impl/data/Identifiers.hpp>
#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/util/common.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <algorithm>
#include <optional>
#include <string>

namespace astfri::csharp
{

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

TypeMetadata* SemanticContext::get_type_metadata(
    UserTypeDefStmt* user_type
) const
{
    const auto& it = symbol_table_.user_types_metadata.find(user_type);
    return it != symbol_table_.user_types_metadata.end() ? &it->second
                                                         : nullptr;
}

void SemanticContext::enter_type(UserTypeDefStmt* def)
{
    type_context_.type_stack.push(def);
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

void SemanticContext::reg_local_func(FuncMetadata func_meta)
{
    const auto func_def = func_meta.func_def;
    scope_context_.scope_stack.top().push_back(func_def);
    auto [it, inserted] = scope_context_.function_map.try_emplace(
        FuncId{
            .name        = func_def->name_,
            .param_count = func_def->params_.size(),
        }
    );
    if (inserted)
    {
        it->second = std::move(func_meta);
    }
    else
    {
        it->second.func_def = nullptr;
    }
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
                FuncId{
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
    auto& type_stack = type_context_.type_stack;
    return type_stack.empty() ? nullptr : type_stack.top();
}

Type* SemanticContext::current_return_type() const
{
    return return_type_context_.empty() ? nullptr : return_type_context_.top();
}

VarDefStmt* SemanticContext::find_var(
    const std::string_view name,
    const access::Qualifier qualifier
) const
{
    util::Overloaded overloaded{
        [&](const access::None&) -> VarDefStmt*
        {
            // local variables
            const auto it_local = scope_context_.local_vars.find(name);
            if (it_local != scope_context_.local_vars.end())
                return it_local->second;

            // parameters
            const auto it_param = scope_context_.params.find(name);
            if (it_param != scope_context_.params.end())
                return it_param->second;

            // member variables - includes both static and instance members
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
            return nullptr;
        },
        [&](const access::Instance&) -> VarDefStmt*
        {
            auto current_type = as_a<ClassDefStmt>(this->current_type());

            while (current_type)
            {
                if (const auto metadata = find_memb_var(name, current_type))
                {
                    return metadata->var_def;
                }
                current_type = ! current_type->bases_.empty()
                                 ? current_type->bases_.front()
                                 : nullptr;
            }
            // todo handle properties
            return nullptr;
        },
        [&](const access::Static& static_memb) -> VarDefStmt*
        {
            const auto metadata = find_memb_var(name, static_memb.owner);
            return metadata->var_def;
        },
        [&](const access::Base& base) -> VarDefStmt*
        {
            const auto metadata = find_memb_var(name, base.parent_type);
            return metadata->var_def;
        },
        [&](const access::Unknown&) -> VarDefStmt* { return nullptr; }
    };
    return std::visit(overloaded, qualifier);
}

const FuncMetadata* SemanticContext::find_func(const FuncId& func_id) const
{
    auto& funcs        = scope_context_.function_map;
    const auto it_func = funcs.find(func_id);
    return it_func == funcs.end() ? nullptr : &it_func->second;
}

const MethodMetadata* SemanticContext::find_method(
    const MethodId& method_id,
    UserTypeDefStmt* owner
) const
{

    auto& user_types = symbol_table_.user_types_metadata;
    if (auto* current = as_a<ClassDefStmt>(owner))
    {
        while (current)
        {
            const auto it_type_meta = user_types.find(current);
            if (it_type_meta == user_types.end())
                return nullptr;

            auto& [_, type_meta] = *it_type_meta;
            const auto it_method = type_meta.methods.find(method_id);
            if (it_method != type_meta.methods.end())
                return &it_method->second;

            current
                = ! current->bases_.empty() ? current->bases_.front() : nullptr;
        }
    }
    else if (is_a<InterfaceDefStmt>(owner))
    {
        const auto it_type_meta = user_types.find(owner);
        if (it_type_meta == user_types.end())
            return nullptr;

        auto& [_, type_meta] = *it_type_meta;
        const auto it_method = type_meta.methods.find(method_id);
        if (it_method != type_meta.methods.end())
            return &it_method->second;
    }

    return nullptr;
}

MemberVarMetadata* SemanticContext::find_memb_var(
    const std::string_view name,
    UserTypeDefStmt* owner
) const
{
    auto& user_types = symbol_table_.user_types_metadata;
    // todo add handling of records
    if (auto* current = as_a<ClassDefStmt>(owner))
    {
        while (current)
        {
            const auto& it_type_metadata = user_types.find(current);
            if (it_type_metadata == user_types.end())
                continue;

            auto& member_vars  = it_type_metadata->second.member_vars;

            const auto& it_var = member_vars.find(name);
            if (it_var != member_vars.end())
                return &it_var->second;

            current
                = ! current->bases_.empty() ? current->bases_.front() : nullptr;
        }
    }
    else if (is_a<InterfaceDefStmt>(owner))
    {
        const auto& it_type_metadata = user_types.find(owner);
        if (it_type_metadata == user_types.end())
            return nullptr;

        auto& member_vars  = it_type_metadata->second.member_vars;

        const auto& it_var = member_vars.find(name);
        if (it_var != member_vars.end())
            return &it_var->second;
    }

    return nullptr;
}

CallType SemanticContext::find_invoc_type(
    const FuncId& func_id,
    access::Qualifier quelifier
) const
{
    // todo static variables
    if ([[maybe_unused]] VarDefStmt* var_def
        = find_var(func_id.name, quelifier))
    {
        /* todo
         * this should probably be something else like a delegate invocation
         * since delegate variables dont have to be only lambdas
         */
        return CallType::Delegate;
    }

    MethodId method_id{
        .func_id   = func_id,
        .is_static = std::holds_alternative<access::Static>(quelifier),
    };

    util::Overloaded overloaded{
        [&](const access::None&) -> CallType
        {
            if ([[maybe_unused]] const FuncMetadata* metadata
                = find_func(func_id))
                return CallType::LocalFunc;

            const auto current_type = this->current_type();
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(method_id, current_type))
            {
                return CallType::Method;
            }

            method_id.is_static = true;
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(method_id, current_type))
            {
                return CallType::StaticMethod;
            }

            return CallType::Unknown;
        },
        [&](const access::Instance&) -> CallType
        {
            const auto current_type = this->current_type();
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(method_id, current_type))
            {
                return CallType::Method;
            }

            return CallType::Unknown;
        },
        [&](const access::Static& static_memb) -> CallType
        {
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(method_id, static_memb.owner))
                return CallType::StaticMethod;

            return CallType::Unknown;
        },
        [&](const access::Base& base) -> CallType
        {
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(method_id, base.parent_type))
            {
                return CallType::Method;
            }
            return CallType::Unknown;
        },
        [](const access::Unknown&) -> CallType
        {
            // todo this should be some universal invocation expression
            return CallType::Unknown;
        }
    };
    return std::visit(overloaded, quelifier);
}

} // namespace astfri::csharp
