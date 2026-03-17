#include <libastfri-cs/impl/data/AccessType.hpp>
#include <libastfri-cs/impl/data/Identifiers.hpp>
#include <libastfri-cs/impl/data/Metadata.hpp>
#include <libastfri-cs/impl/data/SymbolTable.hpp>
#include <libastfri-cs/impl/SemanticContext.hpp>
#include <libastfri-cs/impl/util/Common.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <algorithm>
#include <string>

namespace astfri::csharp {

SemanticContext::SemanticContext(SymbolTable& symbTable) :
    symbTable_(symbTable) {
}

void SemanticContext::enter_type(TypeBinding* tb) {
    typeContext_.typeStack.push_back(tb);
    enter_scope();
}

void SemanticContext::enter_scope() {
    scopeContext_.scopeStack.emplace_back();
}

void SemanticContext::reg_local_var(LocalVarDefStmt* varDef) {
    scopeContext_.scopeStack.back().push_back(varDef);
    scopeContext_.localVars.emplace(varDef->name, varDef);
}

void SemanticContext::reg_param(ParamVarDefStmt* varDef) {
    scopeContext_.scopeStack.back().push_back(varDef);
    scopeContext_.params.emplace(varDef->name, varDef);
}

void SemanticContext::reg_local_func(FuncMetadata funcMeta) {
    const auto funcDef = funcMeta.funcDef;
    scopeContext_.scopeStack.back().push_back(funcDef);
    scopeContext_.functions.emplace(funcDef->name, std::move(funcMeta));
}

void SemanticContext::reg_return(Type* returnType) {
    retTypeContext_.push_back(returnType);
}

void SemanticContext::leave_type() {
    if (! typeContext_.typeStack.empty())
        typeContext_.typeStack.pop_back();
    leave_scope();
}

void SemanticContext::leave_scope() {
    if (scopeContext_.scopeStack.empty())
        return;

    for (const auto scopeMemb : scopeContext_.scopeStack.back()) {
        if (const auto param = as_a<ParamVarDefStmt>(scopeMemb))
            scopeContext_.params.erase(param->name);
        else if (const auto var = as_a<LocalVarDefStmt>(scopeMemb))
            scopeContext_.localVars.erase(var->name);
        else if (const auto func = as_a<FunctionDefStmt>(scopeMemb)) {
            scopeContext_.functions.erase(func->name);
        }
    }
    scopeContext_.scopeStack.pop_back();
}

void SemanticContext::unregister_return_type() {
    if (! retTypeContext_.empty())
        retTypeContext_.pop_back();
}

TypeBinding* SemanticContext::current_type() const {
    auto& typeStack = typeContext_.typeStack;
    return typeStack.empty() ? nullptr : typeStack.back();
}

Type* SemanticContext::current_return_type() const {
    return retTypeContext_.empty() ? nullptr : retTypeContext_.back();
}

VarDefStmt* SemanticContext::find_var(
    const std::string_view name,
    const access::Qualifier qualifier
) const {
    util::Overloaded overloaded{
        [&](const access::None&) -> VarDefStmt* {
            // local variables
            const auto itLocal = scopeContext_.localVars.find(name);
            if (itLocal != scopeContext_.localVars.end())
                return itLocal->second;

            // parameters
            const auto itParam = scopeContext_.params.find(name);
            if (itParam != scopeContext_.params.end())
                return itParam->second;

            // member variables - includes both static and instance members
            const TypeBinding* currentType = current_type();

            if (! currentType)
                return nullptr;

            if (auto* typeMeta = symbTable_.get_type_metadata(currentType->def)) {
                if (const auto* varMeta = typeMeta->find_memb_var(name))
                    return varMeta->varDef;
            }
            return nullptr;
        },
        [&](const access::Instance&) -> VarDefStmt* {
            auto currentType = as_a<ClassDefStmt>(current_type()->def);

            while (currentType) {
                if (const auto metadata = find_memb_var(name, currentType)) {
                    return metadata->varDef;
                }
                currentType = ! currentType->bases.empty() ? currentType->bases.front() : nullptr;
            }
            // todo handle properties
            return nullptr;
        },
        [&](const access::Static& staticMemb) -> VarDefStmt* {
            const auto metadata = find_memb_var(name, staticMemb.owner);
            return metadata->varDef;
        },
        [&](const access::Base& base) -> VarDefStmt* {
            const auto metadata = find_memb_var(name, base.parent);
            return metadata->varDef;
        },
        [&](const access::Unknown&) -> VarDefStmt* { return nullptr; }
    };
    return std::visit(overloaded, qualifier);
}

const FuncMetadata* SemanticContext::find_func(const std::string_view funcName) const {
    auto& funcs       = scopeContext_.functions;
    const auto itFunc = funcs.find(funcName);
    return itFunc == funcs.end() ? nullptr : &itFunc->second;
}

const MethodMetadata* SemanticContext::find_method(const MethodId& methodId, UserTypeDefStmt* owner)
    const {
    if (auto* classDef = as_a<ClassDefStmt>(owner)) {
        ClassDefStmt* current = classDef;
        while (current) {
            TypeMetadata* typeMeta = symbTable_.get_type_metadata(current);
            if (! typeMeta)
                return nullptr;

            if (const auto* methodMeta = typeMeta->find_method(methodId))
                return methodMeta;

            current = ! current->bases.empty() ? current->bases.front() : nullptr;
        }
    }
    else if (auto* intDef = as_a<InterfaceDefStmt>(owner)) {
        TypeMetadata* typeMeta = symbTable_.get_type_metadata(intDef);
        if (! typeMeta)
            return nullptr;

        if (const auto* methodMeta = typeMeta->find_method(methodId))
            return methodMeta;
    }

    return nullptr;
}

MemberVarMetadata* SemanticContext::find_memb_var(
    const std::string_view name,
    UserTypeDefStmt* owner
) const {
    // todo add handling of records
    if (auto* current = as_a<ClassDefStmt>(owner)) {
        while (current) {
            TypeMetadata* typeMeta = symbTable_.get_type_metadata(current);
            if (! typeMeta)
                return nullptr;

            if (auto* varMeta = typeMeta->find_memb_var(name))
                return varMeta;

            current = ! current->bases.empty() ? current->bases.front() : nullptr;
        }
    }
    else if (is_a<InterfaceDefStmt>(owner)) {
        TypeMetadata* typeMeta = symbTable_.get_type_metadata(current);
        if (! typeMeta)
            return nullptr;
        return typeMeta->find_memb_var(name);
    }

    return nullptr;
}

InvocationType SemanticContext::find_invoc_type(InvocationId id, access::Qualifier quelifier)
    const {
    // todo static variables
    if ([[maybe_unused]] VarDefStmt* varDef = find_var(id.name, quelifier)) {
        /* todo
         * this should probably be something else like a delegate invocation
         * since delegate variables dont have to be only lambdas
         */
        return InvocationType::Delegate;
    }

    util::Overloaded overloaded{
        [&](const access::None&) -> InvocationType {
            if ([[maybe_unused]] const FuncMetadata* metadata = find_func(id.name))
                return InvocationType::LocalFunc;

            MethodId methodId{
                .name       = std::move(id.name),
                .paramCount = id.paramCount,
                .isStatic   = false
            };

            const auto currentType = this->current_type();
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(methodId, currentType->def)) {
                return InvocationType::Method;
            }

            methodId.isStatic = true;
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(methodId, currentType->def)) {
                return InvocationType::StaticMethod;
            }

            return InvocationType::Unknown;
        },
        [&](const access::Instance&) -> InvocationType {
            const auto currentType = this->current_type();
            const MethodId methodId{
                .name       = std::move(id.name),
                .paramCount = id.paramCount,
                .isStatic   = false
            };

            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(methodId, currentType->def)) {
                return InvocationType::Method;
            }

            return InvocationType::Unknown;
        },
        [&](const access::Static& staticMemb) -> InvocationType {
            const MethodId methodId{
                .name       = std::move(id.name),
                .paramCount = id.paramCount,
                .isStatic   = false
            };
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(methodId, staticMemb.owner))
                return InvocationType::StaticMethod;

            return InvocationType::Unknown;
        },
        [&](const access::Base& base) -> InvocationType {
            const MethodId methodId{
                .name       = std::move(id.name),
                .paramCount = id.paramCount,
                .isStatic   = true
            };
            if ([[maybe_unused]] const MethodMetadata* metadata
                = find_method(methodId, base.parent)) {
                return InvocationType::Method;
            }
            return InvocationType::Unknown;
        },
        [](const access::Unknown&) -> InvocationType {
            // todo this should be some universal invocation expression
            return InvocationType::Unknown;
        }
    };
    return std::visit(overloaded, quelifier);
}

} // namespace astfri::csharp
