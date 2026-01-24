#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/util/AstfriUtil.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/util/Utils.hpp>
#include <libastfri-cs/impl/visitor/SrcCodeVisitor.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace astfri::csharp
{
Stmt* SrcCodeVisitor::visit_class_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    static const TSSymbol sBaseList = util::symbol_for_name("base_list", true);
    static const TSSymbol sTypeParam
        = util::symbol_for_name("type_parameter_list", true);
    static const TSSymbol sTypeParamConstr
        = util::symbol_for_name("type_parameter_constraints_clause", true);

    const std::string_view src = self->src_str();
    const TSNode nClassName = util::child_by_field_name(node, "name");
    std::string className   = util::extract_text(nClassName, src);
    Scope scope             = util::create_scope(node, src);

    ClassDefStmt* classDef  = stmtFact_.mk_class_def(className, scope);
    classDef->name_         = std::move(className); // todo remove this
    // todo use move semantic for the string

    self->semanticContext_.enter_type(classDef);
    self->typeTrs_.set_current_namespace(std::move(scope));

    const TSNode nClassBody = util::child_by_field_name(node, "body");
    // handling of base class and interface implementations
    bool first           = true;
    auto processBaseList = [&](const TSNode& current) -> void
    {
        std::string name     = util::extract_text(current, self->src_str());
        const TypeHandler th = RegManager::get_type_handler(current);
        Type* type           = th(&self->typeTrs_, current);
        if (first)
        {
            first = false;
            if (const auto class_t = as_a<ClassType>(type))
                classDef->bases_.push_back(class_t->m_def);
            return;
        }

        if (const auto interface_t = as_a<InterfaceType>(type))
            classDef->interfaces_.push_back(interface_t->m_def);
        else if (util::is_interface_name(name))
            classDef->interfaces_.push_back(
                stmtFact_.mk_interface_def(std::move(name), {})
            );
        else
        {
            // todo incomplete type
        }
    };

    // auto processGenericConstraints = [](const TSNode&) -> void { };

    auto processClassHeader = [&](const TSNode& current) -> bool
    {
        if (ts_node_eq(current, nClassBody))
            return false;

        const TSSymbol sCurrent = ts_node_symbol(current);
        if (sCurrent == sBaseList) // base list handeling
        {
            util::for_each_child_node(current, processBaseList);
        }
        else if (sCurrent == sTypeParam)
        {
            classDef->tparams_ = util::make_generic_params(current, src);
        }
        else if (sCurrent == sTypeParamConstr)
        {
            // util::for_each_child_node(current, processGenericConstraints);
        }
        return true;
    };
    util::for_each_child_node(node, processClassHeader);

    // if its partial class doesn't have a body
    if (ts_node_is_null(nClassBody))
        return classDef;

    auto processMembs = [classDef, self](const TSNode& nMember) -> void
    {
        const StmtHandler hMemb = RegManager::get_stmt_handler(nMember);
        Stmt* membStmt          = hMemb(self, nMember);

        if (const auto varDef = as_a<MemberVarDefStmt>(membStmt))
            classDef->vars_.push_back(varDef);
        else if (const auto constr = as_a<ConstructorDefStmt>(membStmt))
            classDef->constructors_.push_back(constr);
        else if (const auto destr = as_a<DestructorDefStmt>(membStmt))
            classDef->destructors_.push_back(destr);
        else if (const auto method = as_a<MethodDefStmt>(membStmt))
            classDef->methods_.push_back(method);
    };
    util::for_each_child_node(nClassBody, processMembs);

    self->semanticContext_.leave_type();
    return classDef;
}

Stmt* SrcCodeVisitor::visit_interface_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    static const TSSymbol sBaseList = util::symbol_for_name("base_list", true);
    static const TSSymbol sTypeParam
        = util::symbol_for_name("type_parameter_list", true);
    static const TSSymbol sTypeParamConstr
        = util::symbol_for_name("type_parameter_constraints_clause", true);

    const std::string_view src = self->src_str();
    const TSNode nIntfName     = util::child_by_field_name(node, "name");
    std::string intfName       = util::extract_text(nIntfName, src);
    Scope scope                = util::create_scope(node, src);

    InterfaceDefStmt* intfDef  = stmtFact_.mk_interface_def(intfName, scope);
    intfDef->name_             = std::move(intfName); // todo remove this
    // todo use move semantic for the string

    self->semanticContext_.enter_type(intfDef);
    self->typeTrs_.set_current_namespace(std::move(scope));

    // handling of base class and interface implementations
    auto processBaseList = [&](const TSNode& current) -> void
    {
        const TypeHandler th = RegManager::get_type_handler(current);
        Type* type           = th(&self->typeTrs_, current);

        if (const auto interface_t = as_a<InterfaceType>(type))
            intfDef->bases_.push_back(interface_t->m_def);
        else
        {
            // todo incomplete type
        }
    };
    // auto processGenericConstraints = [](const TSNode&) -> void { };

    const TSNode nIntfBody  = util::child_by_field_name(node, "body");
    auto processClassHeader = [&](const TSNode& current) -> bool
    {
        if (ts_node_eq(current, nIntfBody))
            return false;

        const TSSymbol sCurrent = ts_node_symbol(current);
        if (sCurrent == sBaseList) // base list handeling
        {
            util::for_each_child_node(current, processBaseList);
        }
        else if (sCurrent == sTypeParam)
        {
            intfDef->tparams_ = util::make_generic_params(current, src);
        }
        else if (sCurrent == sTypeParamConstr)
        {
            // util::for_each_child_node(current, processGenericConstraints);
        }
        return true;
    };
    util::for_each_child_node(node, processClassHeader);

    // if its partial interface and doesn't have a body
    if (ts_node_is_null(nIntfBody))
        return intfDef;

    auto processMembs = [intfDef, self](const TSNode& nMember) -> void
    {
        const StmtHandler hMemb = RegManager::get_stmt_handler(nMember);
        Stmt* membStmt          = hMemb(self, nMember);

        if (const auto varDef = as_a<MemberVarDefStmt>(membStmt))
        {
            // intfDef->vars_.push_back(varDef); // todo static variables
        }
        else if (const auto method = as_a<MethodDefStmt>(membStmt))
            intfDef->methods_.push_back(method);
    };
    util::for_each_child_node(nIntfBody, processMembs);

    self->semanticContext_.leave_type();
    return intfDef;
}

Stmt* SrcCodeVisitor::visit_memb_var_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    return self->visit_var_def_stmt(node, util::VarDefType::Member);
}

Stmt* SrcCodeVisitor::visit_local_var_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    return self->visit_var_def_stmt(node, util::VarDefType::Local);
}

Stmt* SrcCodeVisitor::visit_global_var_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode nVarDef = ts_node_child(node, 0);
    return self->visit_var_def_stmt(nVarDef, util::VarDefType::Global);
}

Stmt* SrcCodeVisitor::visit_param_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode nName = util::child_by_field_name(node, "name");
    const TSNode nInit = ts_node_next_named_sibling(nName);

    ParamVarDefStmt* param
        = util::make_param_def(node, self->src_str(), self->typeTrs_);
    if (! ts_node_is_null(nInit))
    {
        const ExprHandler hInit = RegManager::get_expr_handler(nInit);
        param->initializer_     = hInit(self, nInit);
    }
    self->semanticContext_.reg_param(param);
    return param;
}

Stmt* SrcCodeVisitor::visit_constr_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    self->semanticContext_.enter_scope();
    self->semanticContext_.reg_return(typeFact_.mk_void());
    ConstructorDefStmt* constrDef = stmtFact_.mk_constructor_def();

    const auto currentType        = self->semanticContext_.current_type();
    if (! currentType)
        throw std::logic_error("Owner type not found");

    if (! is_a<ClassDefStmt>(currentType))
        throw std::logic_error(
            "Constructor can only be defined for class type"
        );

    const TSNode nParamList = util::child_by_field_name(node, "parameters");
    const TSNode nBody      = util::child_by_field_name(node, "body");
    const TSNode nInit      = ts_node_next_sibling(nParamList);

    constrDef->owner_       = as_a<ClassDefStmt>(currentType);
    constrDef->params_      = self->make_param_list(nParamList, false);

    const CSModifiers modifs
        = CSModifiers::handle_memb_modifs(node, self->src_str());
    constrDef->access_
        = modifs.get_access_mod().value_or(AccessModifier::Private);

    const StmtHandler hBody = RegManager::get_stmt_handler(nBody);
    constrDef->body_        = as_a<CompoundStmt>(hBody(self, nBody));

    if (! ts_node_is_null(nInit))
    {
        const StmtHandler hBaseInit = RegManager::get_stmt_handler(nInit);
        Stmt* initStmt              = hBaseInit(self, nInit);
        if (const auto baseInit = as_a<BaseInitializerStmt>(initStmt))
            constrDef->baseInit_.push_back(baseInit);
        else if (const auto selfInit = as_a<SelfInitializerStmt>(initStmt))
            constrDef->selfInitializers.push_back(selfInit);
    }

    self->semanticContext_.leave_scope();
    self->semanticContext_.unregister_return_type();
    return constrDef;
}

Stmt* SrcCodeVisitor::visit_construct_init(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    constexpr std::string_view thisInitSw = "this";
    std::string src      = util::extract_text(node, self->src_str());
    const auto bracketIt = std::ranges::find(src, '(');
    const auto thisIt    = std::search(
        src.begin(),
        bracketIt,
        thisInitSw.begin(),
        thisInitSw.end()
    );

    const TSNode nArgList   = ts_node_named_child(node, 0);
    std::vector<Expr*> args = self->visit_arg_list(nArgList);
    if (thisIt != bracketIt)
        return stmtFact_.mk_self_initializer(std::move(args));

    const auto currentType = self->semanticContext_.current_type();
    if (! currentType)
        throw std::logic_error("Owner type not found");
    const auto* owner = as_a<ClassDefStmt>(currentType);
    // todo add records
    if (! owner)
        throw std::logic_error(
            "Constructor can only be defined for class type"
        );

    // todo fix this for Incomplete types
    if (owner->bases_.empty())
        return stmtFact_.mk_uknown();
    const ClassDefStmt* base = owner->bases_.back();
    return stmtFact_.mk_base_initializer(base->type_, std::move(args));
}

Stmt* SrcCodeVisitor::visit_destr_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    self->semanticContext_.reg_return(typeFact_.mk_void());
    const TSNode nBody      = util::child_by_field_name(node, "body");
    const StmtHandler hBody = RegManager::get_stmt_handler(nBody);
    Stmt* body              = hBody(self, nBody);
    const auto currentType  = self->semanticContext_.current_type();

    if (! currentType)
        throw std::logic_error("Owner type not found");
    auto* owner = as_a<ClassDefStmt>(currentType);
    if (! owner)
        throw std::logic_error("Destructor can only be defined for class type");

    self->semanticContext_.unregister_return_type();
    return stmtFact_.mk_destructor_def(owner, as_a<CompoundStmt>(body));
}

Stmt* SrcCodeVisitor::visit_method_def_stmt(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const auto currentType = self->semanticContext_.current_type();
    if (! currentType)
        throw std::logic_error("Owner type not found");

    const TSNode nParams = util::child_by_field_name(node, "parameters");
    const CSModifiers modifs
        = CSModifiers::handle_memb_modifs(node, self->src_str());
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
        = self->semanticContext_.find_method(methodId, currentType);
    if (! methodMeta)
        throw std::logic_error("Method \'" + methodId.name + "\' not found");

    // if method could be resolved
    if (methodMeta->methodDef)
    {
        auto& methodDef = methodMeta->methodDef;
        self->semanticContext_.enter_scope();
        self->semanticContext_.reg_return(methodDef->func_->retType_);

        for (auto& [paramDef, nParam, nInit] : methodMeta->params)
        {
            if (! ts_node_is_null(nInit))
            {
                ExprHandler hInit      = RegManager::get_expr_handler(nInit);
                paramDef->initializer_ = hInit(self, nInit);
            }
            self->semanticContext_.reg_param(paramDef);
            // util::print_child_nodes_types(param_node, self->get_src_code());
        }

        const TSNode nBody
            = util::child_by_field_name(methodMeta->nMethod, "body");
        if (! ts_node_is_null(nBody))
        {
            const StmtHandler hBody = RegManager::get_stmt_handler(nBody);
            methodDef->func_->body_ = as_a<CompoundStmt>(hBody(self, nBody));
        }

        self->semanticContext_.leave_scope();

        return methodDef;
    }

    // if it wansn't able to resolve method
    MethodDefStmt* methodDef = stmtFact_.mk_method_def();
    methodDef->owner_        = currentType;
    methodDef->func_         = self->make_func_stmt(node, true);
    methodDef->access_
        = modifs.get_access_mod().value_or(AccessModifier::Internal);
    return methodDef;
}

Stmt* SrcCodeVisitor::visit_func_stmt(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode nName           = util::child_by_field_name(node, "name");
    const std::string name       = util::extract_text(nName, self->src_str());
    const FuncMetadata* funcMeta = self->semanticContext_.find_func(name);
    if (! funcMeta)
        throw std::logic_error("Local function \'" + name + "\' not found");

    if (! funcMeta->funcDef)
        return self->make_func_stmt(node, false);

    self->semanticContext_.enter_scope();
    self->semanticContext_.reg_return(funcMeta->funcDef->retType_);
    for (const auto& paramMeta : funcMeta->params)
    {
        const TSNode nInit = paramMeta.nInit;
        if (! ts_node_is_null(nInit))
        {
            ExprHandler hInit = RegManager::get_expr_handler(nInit);
            paramMeta.paramDef->initializer_ = hInit(self, nInit);
        }
        self->semanticContext_.reg_param(paramMeta.paramDef);
    }

    const TSNode nBody       = util::child_by_field_name(node, "body");
    const StmtHandler hBody  = RegManager::get_stmt_handler(nBody);
    funcMeta->funcDef->body_ = as_a<CompoundStmt>(hBody(self, nBody));
    self->semanticContext_.leave_scope();
    self->semanticContext_.unregister_return_type();
    return funcMeta->funcDef;
}

} // namespace astfri::csharp