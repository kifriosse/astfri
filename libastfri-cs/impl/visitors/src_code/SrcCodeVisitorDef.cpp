#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/util/AstfriUtil.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/util/Utils.hpp>
#include <libastfri-cs/impl/visitors/src_code/SrcCodeVisitor.hpp>
#include <astfri/Astfri.hpp>

#include <tree_sitter/api.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace astfri::csharp {
Stmt* SrcCodeVisitor::visit_class_def(SrcCodeVisitor* self, const TSNode& node) {
    const std::string_view src  = self->src_str();
    const TSNode nClassName     = util::child_by_field_name(node, "name");
    const std::string className = util::extract_text(nClassName, src);
    const Scope scope           = util::mk_scope(node, *self->src());
    TypeBinding* tb             = self->typeTrs_.get_type(className, scope);
    auto* classDef              = as_a<ClassDefStmt>(tb->def);

    self->semanticContext_.enter_type(tb);

    const TSNode nClassBody = util::child_by_field_name(node, "body");
    // handling of base class and interface implementations
    bool first           = true;
    auto processBaseList = [&](const TSNode& current) -> void {
        std::string name    = util::extract_text(current, self->src_str());
        const TypeMapper th = MapManager::get_type_mapper(current);
        Type* type          = th(&self->typeTrs_, current);
        if (first) {
            first = false;
            if (const auto class_t = as_a<ClassType>(type))
                classDef->bases.push_back(class_t->def);
            if (const auto interface_t = as_a<InterfaceType>(type)) {
                classDef->interfaces.push_back(interface_t->def);
            }
            else if (util::is_interface_name(name)) {
                classDef->interfaces.push_back(stmtFact_.mk_interface_def(std::move(name), {}));
                // todo might be useless if external types are imported
            }
            return;
        }

        if (const auto interface_t = as_a<InterfaceType>(type)) {
            classDef->interfaces.push_back(interface_t->def);
        }
        else if (util::is_interface_name(name)) {
            classDef->interfaces.push_back(stmtFact_.mk_interface_def(std::move(name), {}));
            // todo might be useless if external types are imported
        }
        else {
            // todo incomplete type
        }
    };

    // auto processGenericConstraints = [](const TSNode&) -> void { };

    auto processClassHeader = [&](const TSNode& current) -> bool {
        using enum NodeType;
        if (ts_node_eq(current, nClassBody))
            return false;

        const TSSymbol sCurrent = ts_node_symbol(current);
        if (sCurrent == MapManager::get_symbol(BaseList)) {
            util::for_each_child_node(current, processBaseList);
        }
        else if (sCurrent == MapManager::get_symbol(TypeParamList)) {
            classDef->tparams = util::make_generic_params(current, src);
        }
        else if (sCurrent == MapManager::get_symbol(TypeParamConstrClause)) {
            // util::for_each_child_node(current, processGenericConstraints);
        }
        return true;
    };
    util::for_each_child_node(node, processClassHeader);

    // if its partial class doesn't have a body
    if (ts_node_is_null(nClassBody))
        return classDef;

    auto processMembs = [classDef, self](const TSNode& nMember) -> void {
        if (util::is_type_decl(nMember))
            return;
        const StmtMapper hMemb = MapManager::get_stmt_mapper(nMember);
        Stmt* membStmt         = hMemb(self, nMember);

        if (const auto varDef = as_a<MemberVarDefStmt>(membStmt))
            classDef->vars.push_back(varDef);
        else if (const auto constr = as_a<ConstructorDefStmt>(membStmt))
            classDef->constructors.push_back(constr);
        else if (const auto destr = as_a<DestructorDefStmt>(membStmt))
            classDef->destructors.push_back(destr);
        else if (const auto method = as_a<MethodDefStmt>(membStmt))
            classDef->methods.push_back(method);
    };
    util::for_each_child_node(nClassBody, processMembs);

    self->semanticContext_.leave_type();
    return classDef;
}

Stmt* SrcCodeVisitor::visit_interface_def(SrcCodeVisitor* self, const TSNode& node) {
    const std::string_view src = self->src_str();
    const TSNode nIntfName     = util::child_by_field_name(node, "name");
    const std::string intfName = util::extract_text(nIntfName, src);
    const Scope scope          = util::mk_scope(node, *self->src());
    TypeBinding* tb            = self->typeTrs_.get_type(intfName, scope);
    auto* intfDef              = as_a<InterfaceDefStmt>(tb->def);
    self->semanticContext_.enter_type(tb);

    // handling of interface implementations
    auto processBaseList = [&](const TSNode& current) -> void {
        const TypeMapper th = MapManager::get_type_mapper(current);
        Type* type          = th(&self->typeTrs_, current);

        if (const auto tInterface = as_a<InterfaceType>(type))
            intfDef->bases.push_back(tInterface->def);
        else {
            // todo incomplete type
        }
    };
    // auto processGenericConstraints = [](const TSNode&) -> void { };

    const TSNode nIntfBody  = util::child_by_field_name(node, "body");
    auto processClassHeader = [&](const TSNode& current) -> bool {
        using enum NodeType;
        if (ts_node_eq(current, nIntfBody))
            return false;

        const TSSymbol sCurrent = ts_node_symbol(current);
        if (sCurrent == MapManager::get_symbol(BaseList)) {
            util::for_each_child_node(current, processBaseList);
        }
        else if (sCurrent == MapManager::get_symbol(TypeParamList)) {
            intfDef->tparams = util::make_generic_params(current, src);
        }
        else if (sCurrent == MapManager::get_symbol(TypeParamConstrClause)) {
            // util::for_each_child_node(current, processGenericConstraints);
        }
        return true;
    };
    util::for_each_child_node(node, processClassHeader);

    // if its partial interface and doesn't have a body
    if (ts_node_is_null(nIntfBody))
        return intfDef;

    auto processMembs = [intfDef, self](const TSNode& nMember) -> void {
        const StmtMapper hMemb = MapManager::get_stmt_mapper(nMember);
        Stmt* membStmt         = hMemb(self, nMember);

        if ([[maybe_unused]] const auto varDef = as_a<MemberVarDefStmt>(membStmt)) {
            // intfDef->vars_.push_back(varDef); // todo static variables
        }
        else if (const auto method = as_a<MethodDefStmt>(membStmt))
            intfDef->methods.push_back(method);
    };
    util::for_each_child_node(nIntfBody, processMembs);

    self->semanticContext_.leave_type();
    return intfDef;
}

Stmt* SrcCodeVisitor::visit_memb_var_def(SrcCodeVisitor* self, const TSNode& node) {
    return self->visit_var_def_stmt(node, util::VarDefType::Member);
}

Stmt* SrcCodeVisitor::visit_local_var_def(SrcCodeVisitor* self, const TSNode& node) {
    return self->visit_var_def_stmt(node, util::VarDefType::Local);
}

Stmt* SrcCodeVisitor::visit_global_var_def_stmt(SrcCodeVisitor* self, const TSNode& node) {
    const TSNode nVarDef = ts_node_child(node, 0);
    return self->visit_var_def_stmt(nVarDef, util::VarDefType::Global);
}

Stmt* SrcCodeVisitor::visit_param_def(SrcCodeVisitor* self, const TSNode& node) {
    const TSNode nName     = util::child_by_field_name(node, "name");
    const TSNode nInit     = ts_node_next_named_sibling(nName);

    ParamVarDefStmt* param = util::mk_param_def(node, self->src_str(), self->typeTrs_);
    if (! ts_node_is_null(nInit)) {
        const ExprMapper hInit = MapManager::get_expr_mapper(nInit);
        param->initializer     = hInit(self, nInit);
    }
    self->semanticContext_.reg_param(param);
    return param;
}

Stmt* SrcCodeVisitor::visit_constr_def(SrcCodeVisitor* self, const TSNode& node) {
    self->semanticContext_.enter_scope();
    self->semanticContext_.reg_return(typeFact_.mk_void());
    ConstructorDefStmt* constrDef = stmtFact_.mk_constructor_def();

    const auto currentType        = self->semanticContext_.current_type();
    if (! currentType)
        return stmtFact_.mk_uknown();
    // throw std::logic_error("Owner type not found");

    const auto currentClass = as_a<ClassDefStmt>(currentType->def);
    if (! currentClass)
        return stmtFact_.mk_uknown();
    // throw std::logic_error(
    //     "Constructor can only be defined for class type"
    // );

    const TSNode nParamList  = util::child_by_field_name(node, "parameters");
    const TSNode nBody       = util::child_by_field_name(node, "body");
    const TSNode nInit       = ts_node_next_sibling(nParamList);

    constrDef->owner         = as_a<ClassDefStmt>(currentClass);
    constrDef->params        = self->make_param_list(nParamList, false);

    const CSModifiers modifs = CSModifiers::parser_method_modifs(node, self->src_str());
    constrDef->access        = modifs.get_access_mod().value_or(AccessModifier::Private);

    const StmtMapper hBody   = MapManager::get_stmt_mapper(nBody);
    constrDef->body          = as_a<CompoundStmt>(hBody(self, nBody));

    if (! ts_node_is_null(nInit)) {
        const StmtMapper hBaseInit = MapManager::get_stmt_mapper(nInit);
        Stmt* initStmt             = hBaseInit(self, nInit);
        if (const auto baseInit = as_a<BaseInitializerStmt>(initStmt))
            constrDef->baseInit.push_back(baseInit);
        else if (const auto selfInit = as_a<SelfInitializerStmt>(initStmt))
            constrDef->selfInitializers.push_back(selfInit);
    }

    self->semanticContext_.leave_scope();
    self->semanticContext_.unregister_return_type();
    return constrDef;
}

Stmt* SrcCodeVisitor::visit_constr_init(SrcCodeVisitor* self, const TSNode& node) {
    constexpr std::string_view thisInitSw = "this";
    std::string src                       = util::extract_text(node, self->src_str());
    const auto bracketIt                  = std::ranges::find(src, '(');
    const auto thisIt = std::search(src.begin(), bracketIt, thisInitSw.begin(), thisInitSw.end());

    const TSNode nArgList   = ts_node_named_child(node, 0);
    std::vector<Expr*> args = self->visit_arg_list(nArgList);
    if (thisIt != bracketIt)
        return stmtFact_.mk_self_initializer(std::move(args));

    const auto currentType = self->semanticContext_.current_type();
    if (! currentType)
        return stmtFact_.mk_uknown();
    // throw std::logic_error("Owner type not found");

    const auto* owner = as_a<ClassDefStmt>(currentType->def);
    // todo add records
    if (! owner)
        return stmtFact_.mk_uknown();
    // throw std::logic_error(
    //     "Constructor can only be defined for class type"
    // );

    // todo fix this for Incomplete types
    if (owner->bases.empty())
        return stmtFact_.mk_uknown();
    const ClassDefStmt* base = owner->bases.back();
    return stmtFact_.mk_base_initializer(base->type, std::move(args));
}

Stmt* SrcCodeVisitor::visit_destr_def(SrcCodeVisitor* self, const TSNode& node) {
    self->semanticContext_.reg_return(typeFact_.mk_void());
    const TSNode nBody     = util::child_by_field_name(node, "body");
    const StmtMapper hBody = MapManager::get_stmt_mapper(nBody);
    Stmt* body             = hBody(self, nBody);
    const auto currentType = self->semanticContext_.current_type();

    if (! currentType)
        // throw std::logic_error("Owner type not found");
        return stmtFact_.mk_uknown();
    auto* owner = as_a<ClassDefStmt>(currentType->def);
    if (! owner)
        // throw std::logic_error("Destructor can only be defined for class
        // type");
        return stmtFact_.mk_uknown();

    self->semanticContext_.unregister_return_type();
    return stmtFact_.mk_destructor_def(owner, as_a<CompoundStmt>(body));
}

Stmt* SrcCodeVisitor::visit_method_def(SrcCodeVisitor* self, const TSNode& node) {
    const auto currentType = self->semanticContext_.current_type();
    if (! currentType)
        return stmtFact_.mk_uknown();
    // throw std::logic_error("Owner type not found");

    const TSNode nParams        = util::child_by_field_name(node, "parameters");
    const CSModifiers modifs    = CSModifiers::parser_method_modifs(node, self->src_str());
    const TSNode nName          = util::child_by_field_name(node, "name");
    const bool isVariadic       = util::has_variadic_param(nParams, nullptr);
    const size_t cNamedChildren = ts_node_named_child_count(nParams);
    const size_t cParam         = cNamedChildren - (isVariadic ? 1 : 0);

    const MethodId methodId{
        .name       = util::extract_text(nName, self->src_str()),
        .paramCount = cParam,
        .isStatic   = modifs.has(CSModifier::Static)
    };

    const MethodMetadata* methodMeta
        = self->semanticContext_.find_method(methodId, currentType->def);

    // if method could be resolved
    if (methodMeta && methodMeta->methodDef) {
        auto& methodDef = methodMeta->methodDef;
        self->semanticContext_.enter_scope();
        self->semanticContext_.reg_return(methodDef->func->retType);

        for (auto& [paramDef, nParam, nInit] : methodMeta->params) {
            if (! ts_node_is_null(nInit)) {
                ExprMapper hInit      = MapManager::get_expr_mapper(nInit);
                paramDef->initializer = hInit(self, nInit);
            }
            self->semanticContext_.reg_param(paramDef);
            // util::print_child_nodes_types(param_node, self->get_src_code());
        }

        const TSNode nBody = util::child_by_field_name(methodMeta->nMethod, "body");
        if (! ts_node_is_null(nBody)) {
            const StmtMapper hBody = MapManager::get_stmt_mapper(nBody);
            methodDef->func->body  = as_a<CompoundStmt>(hBody(self, nBody));
        }

        self->semanticContext_.leave_scope();

        return methodDef;
    }

    // if it wansn't able to resolve method
    MethodDefStmt* methodDef = stmtFact_.mk_method_def();
    methodDef->owner         = currentType->def;
    methodDef->func          = self->make_func_stmt(node, true);
    methodDef->access        = modifs.get_access_mod().value_or(AccessModifier::Internal);
    return methodDef;
}

Stmt* SrcCodeVisitor::visit_func_stmt(SrcCodeVisitor* self, const TSNode& node) {
    const TSNode nName           = util::child_by_field_name(node, "name");
    const std::string name       = util::extract_text(nName, self->src_str());
    const FuncMetadata* funcMeta = self->semanticContext_.find_func(name);
    if (! funcMeta)
        return stmtFact_.mk_uknown();
    // throw std::logic_error("Local function \'" + name + "\' not found");

    if (! funcMeta->funcDef)
        return self->make_func_stmt(node, false);

    self->semanticContext_.enter_scope();
    self->semanticContext_.reg_return(funcMeta->funcDef->retType);
    for (const auto& paramMeta : funcMeta->params) {
        const TSNode nInit = paramMeta.nInit;
        if (! ts_node_is_null(nInit)) {
            ExprMapper hInit                = MapManager::get_expr_mapper(nInit);
            paramMeta.paramDef->initializer = hInit(self, nInit);
        }
        self->semanticContext_.reg_param(paramMeta.paramDef);
    }

    const TSNode nBody      = util::child_by_field_name(node, "body");
    const StmtMapper hBody  = MapManager::get_stmt_mapper(nBody);
    funcMeta->funcDef->body = as_a<CompoundStmt>(hBody(self, nBody));
    self->semanticContext_.leave_scope();
    self->semanticContext_.unregister_return_type();
    return funcMeta->funcDef;
}

} // namespace astfri::csharp