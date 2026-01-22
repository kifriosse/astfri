#include <libastfri-cs/impl/data/AccessType.hpp>
#include <libastfri-cs/impl/data/Identifiers.hpp>
#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/util/Common.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <algorithm>
#include <optional>
#include <string>

namespace astfri::csharp
{

SemanticContext::SymbolTableKV::iterator SemanticContext::SymbolTableKV::
    begin() const
{
    return userTypesMetadata.begin();
}

SemanticContext::SymbolTableKV::iterator SemanticContext::SymbolTableKV::
    end() const
{
    return userTypesMetadata.end();
}

SemanticContext::SemanticContext(SymbolTable& symbTable) :
    symbTable_(symbTable)
{
}

SemanticContext::SymbolTableKV SemanticContext::get_user_types() const
{
    return SymbolTableKV{symbTable_.userTypeKeys};
}

TypeMetadata* SemanticContext::get_type_metadata(
    UserTypeDefStmt* userType
) const
{
    const auto& it = symbTable_.userTypeMetadata.find(userType);
    return it != symbTable_.userTypeMetadata.end() ? &it->second : nullptr;
}

void SemanticContext::enter_type(UserTypeDefStmt* def)
{
    typeContext_.typeStack.push(def);
    enter_scope();
}

void SemanticContext::enter_scope()
{
    scopeContext_.scopeStack.emplace();
}

void SemanticContext::reg_local_var(LocalVarDefStmt* varDef)
{
    scopeContext_.scopeStack.top().push_back(varDef);
    scopeContext_.localVars.emplace(varDef->name_, varDef);
}

void SemanticContext::reg_param(ParamVarDefStmt* varDef)
{
    scopeContext_.scopeStack.top().push_back(varDef);
    scopeContext_.params.emplace(varDef->name_, varDef);
}

void SemanticContext::reg_local_func(FuncMetadata funcMeta)
{
    const auto funcDef = funcMeta.funcDef;
    scopeContext_.scopeStack.top().push_back(funcDef);
    scopeContext_.functions.emplace(funcDef->name_, std::move(funcMeta));
}

void SemanticContext::reg_return(Type* returnType)
{
    retTypeContext_.push(returnType);
}

void SemanticContext::leave_type()
{
    if (typeContext_.typeStack.empty())
        typeContext_.typeStack.pop();
    leave_scope();
}

void SemanticContext::leave_scope()
{
    if (scopeContext_.scopeStack.empty())
        return;

    for (const auto scopeMemb : scopeContext_.scopeStack.top())
    {
        if (const auto param = as_a<ParamVarDefStmt>(scopeMemb))
            scopeContext_.params.erase(param->name_);
        else if (const auto var = as_a<LocalVarDefStmt>(scopeMemb))
            scopeContext_.localVars.erase(var->name_);
        else if (const auto func = as_a<FunctionDefStmt>(scopeMemb))
        {
            scopeContext_.functions.erase(func->name_);
        }
    }
    scopeContext_.scopeStack.pop();
}

void SemanticContext::unregister_return_type()
{
    if (! retTypeContext_.empty())
        retTypeContext_.pop();
}

UserTypeDefStmt* SemanticContext::current_type() const
{
    auto& typeStack = typeContext_.typeStack;
    return typeStack.empty() ? nullptr : typeStack.top();
}

Type* SemanticContext::current_return_type() const
{
    return retTypeContext_.empty() ? nullptr : retTypeContext_.top();
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
            const auto itLocal = scopeContext_.localVars.find(name);
            if (itLocal != scopeContext_.localVars.end())
                return itLocal->second;

            // parameters
            const auto itParam = scopeContext_.params.find(name);
            if (itParam != scopeContext_.params.end())
                return itParam->second;

            // member variables - includes both static and instance members
            UserTypeDefStmt* currentType = this->current_type();

            if (! currentType)
                return nullptr;

            const auto& metadata = symbTable_.userTypeMetadata;
            const auto itType    = metadata.find(currentType);
            if (itType != symbTable_.userTypeMetadata.end())
            {
                const auto& membVars = itType->second.memberVars;
                const auto& itMemb   = membVars.find(name);
                if (itMemb != membVars.end())
                    return itMemb->second.varDef;
            }
            return nullptr;
        },
        [&](const access::Instance&) -> VarDefStmt*
        {
            auto currentType = as_a<ClassDefStmt>(this->current_type());

            while (currentType)
            {
                if (const auto metadata = find_memb_var(name, currentType))
                {
                    return metadata->varDef;
                }
                currentType = ! currentType->bases_.empty()
                                ? currentType->bases_.front()
                                : nullptr;
            }
            // todo handle properties
            return nullptr;
        },
        [&](const access::Static& staticMemb) -> VarDefStmt*
        {
            const auto metadata = find_memb_var(name, staticMemb.owner);
            return metadata->varDef;
        },
        [&](const access::Base& base) -> VarDefStmt*
        {
            const auto metadata = find_memb_var(name, base.parent);
            return metadata->varDef;
        },
        [&](const access::Unknown&) -> VarDefStmt* { return nullptr; }
    };
    return std::visit(overloaded, qualifier);
}

const FuncMetadata* SemanticContext::find_func(
    const std::string_view funcName
) const
{
    auto& funcs       = scopeContext_.functions;
    const auto itFunc = funcs.find(funcName);
    return itFunc == funcs.end() ? nullptr : &itFunc->second;
}

const MethodMetadata* SemanticContext::find_method(
    const MethodId& methodId,
    UserTypeDefStmt* owner
) const
{

    auto& userTypes = symbTable_.userTypeMetadata;
    if (auto* current = as_a<ClassDefStmt>(owner))
    {
        while (current)
        {
            const auto itTypeMetadata = userTypes.find(current);
            if (itTypeMetadata == userTypes.end())
                return nullptr;

            auto& [_, typeMetadata] = *itTypeMetadata;
            const auto itMethod = typeMetadata.methods.find(methodId);
            if (itMethod != typeMetadata.methods.end())
                return &itMethod->second;

            current
                = ! current->bases_.empty() ? current->bases_.front() : nullptr;
        }
    }
    else if (is_a<InterfaceDefStmt>(owner))
    {
        const auto itTypeMeta = userTypes.find(owner);
        if (itTypeMeta == userTypes.end())
            return nullptr;

        auto& [_, typeMeta] = *itTypeMeta;
        const auto itMethod = typeMeta.methods.find(methodId);
        if (itMethod != typeMeta.methods.end())
            return &itMethod->second;
    }

    return nullptr;
}

MemberVarMetadata* SemanticContext::find_memb_var(
    const std::string_view name,
    UserTypeDefStmt* owner
) const
{
    auto& userTypes = symbTable_.userTypeMetadata;
    // todo add handling of records
    if (auto* current = as_a<ClassDefStmt>(owner))
    {
        while (current)
        {
            const auto& itTypeMeta = userTypes.find(current);
            if (itTypeMeta == userTypes.end())
                continue;

            auto& membVars     = itTypeMeta->second.memberVars;

            const auto& itVars = membVars.find(name);
            if (itVars != membVars.end())
                return &itVars->second;

            current
                = ! current->bases_.empty() ? current->bases_.front() : nullptr;
        }
    }
    else if (is_a<InterfaceDefStmt>(owner))
    {
        const auto& itTypeMeta = userTypes.find(owner);
        if (itTypeMeta == userTypes.end())
            return nullptr;

        auto& membVars    = itTypeMeta->second.memberVars;

        const auto& itVar = membVars.find(name);
        if (itVar != membVars.end())
            return &itVar->second;
    }

    return nullptr;
}

CallType SemanticContext::find_invoc_type(
    InvocationId id,
    access::Qualifier quelifier
) const
{
    // todo static variables
    if ([[maybe_unused]] VarDefStmt* varDef = find_var(id.name, quelifier))
    {
        /* todo
         * this should probably be something else like a delegate invocation
         * since delegate variables dont have to be only lambdas
         */
        return CallType::Delegate;
    }

    util::Overloaded overloaded{
        [&](const access::None&) -> CallType
        {
            if ([[maybe_unused]] const FuncMetadata* metadata
                = find_func(id.name))
                return CallType::LocalFunc;

            MethodId methodId{
                .name       = std::move(id.name),
                .paramCount = id.paramCount,
                .isStatic   = false
            };

            const auto currentType = this->current_type();
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(methodId, currentType))
            {
                return CallType::Method;
            }

            methodId.isStatic = true;
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(methodId, currentType))
            {
                return CallType::StaticMethod;
            }

            return CallType::Unknown;
        },
        [&](const access::Instance&) -> CallType
        {
            const auto currentType = this->current_type();
            const MethodId methodId{
                .name       = std::move(id.name),
                .paramCount = id.paramCount,
                .isStatic   = false
            };

            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(methodId, currentType))
            {
                return CallType::Method;
            }

            return CallType::Unknown;
        },
        [&](const access::Static& staticMemb) -> CallType
        {
            const MethodId methodId{
                .name       = std::move(id.name),
                .paramCount = id.paramCount,
                .isStatic   = false
            };
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(methodId, staticMemb.owner))
                return CallType::StaticMethod;

            return CallType::Unknown;
        },
        [&](const access::Base& base) -> CallType
        {
            const MethodId methodId{
                .name       = std::move(id.name),
                .paramCount = id.paramCount,
                .isStatic   = true
            };
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(methodId, base.parent))
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
