#include <astfri/impl/StmtFactory.hpp>
#include <astfri/impl/TypeFactory.hpp>

namespace astfri {

StmtFactory& StmtFactory::get_instance() {
    static StmtFactory instance;
    return instance;
}

StmtFactory::StmtFactory() :
    m_typeFactory(&TypeFactory::get_instance()) {
}

LocalVarDefStmt* StmtFactory::mk_local_var_def(std::string name, Type* type, Expr* initializer) {
    return details::emplace_get<LocalVarDefStmt>(m_otherStmts, std::move(name), type, initializer);
}

ParamVarDefStmt* StmtFactory::mk_param_var_def(std::string name, Type* type, Expr* initializer) {
    return details::emplace_get<ParamVarDefStmt>(m_otherStmts, std::move(name), type, initializer);
}

MemberVarDefStmt* StmtFactory::mk_member_var_def(
    std::string name,
    Type* type,
    Expr* initializer,
    AccessModifier access
) {
    return details::emplace_get<MemberVarDefStmt>(
        m_otherStmts,
        std::move(name),
        type,
        initializer,
        access
    );
}

GlobalVarDefStmt* StmtFactory::mk_global_var_def(std::string name, Type* type, Expr* initializer) {
    return details::emplace_get<GlobalVarDefStmt>(m_otherStmts, std::move(name), type, initializer);
}

DefStmt* StmtFactory::mk_def() {
    return details::emplace_get<DefStmt>(m_otherStmts, std::vector<VarDefStmt*>{});
}

DefStmt* StmtFactory::mk_def(std::vector<VarDefStmt*> defs) {
    return details::emplace_get<DefStmt>(m_otherStmts, std::move(defs));
}

FunctionDefStmt* StmtFactory::mk_function_def() {
    return details::emplace_get<FunctionDefStmt>(
        m_otherStmts,
        "",
        std::vector<ParamVarDefStmt*>{},
        TypeFactory::get_instance().mk_unknown(),
        nullptr
    );
}

FunctionDefStmt* StmtFactory::mk_function_def(
    std::string name,
    std::vector<ParamVarDefStmt*> params,
    Type* retType,
    CompoundStmt* body
) {
    return details::emplace_get<FunctionDefStmt>(
        m_otherStmts,
        std::move(name),
        std::move(params),
        retType,
        body
    );
}

MethodDefStmt* StmtFactory::mk_method_def() {
    return details::emplace_get<MethodDefStmt>(m_otherStmts);
}

MethodDefStmt* StmtFactory::mk_method_def(
    UserTypeDefStmt* owner,
    FunctionDefStmt* func,
    AccessModifier access,
    Virtuality virtuality
) {
    MethodDefStmt* m = this->mk_method_def();
    m->owner         = owner;
    m->func          = func;
    m->access        = access;
    m->virtuality    = virtuality;
    return m;
}

InterfaceDefStmt* StmtFactory::mk_interface_def() {
    return details::emplace_get<InterfaceDefStmt>(m_otherStmts);
}

InterfaceDefStmt* StmtFactory::mk_interface_def(std::string name) {
    InterfaceDefStmt* i = details::emplace_get<InterfaceDefStmt>(m_otherStmts);
    i->name             = std::move(name);
    return i;
}

InterfaceDefStmt* StmtFactory::mk_interface_def(std::string name, Scope scope) {
    InterfaceDefStmt* i
        = details::emplace_get<InterfaceDefStmt>(mk_fqn(scope, name), m_interfaceDefMap);
    i->type = m_typeFactory->mk_interface(name, scope, i);
    return i;
}

ClassDefStmt* StmtFactory::get_class_def(std::string_view name, const Scope& scope) {
    const auto it = m_classDefMap.find(mk_fqn(scope, name));
    return it != m_classDefMap.end() ? &it->second : nullptr;
}

ClassDefStmt* StmtFactory::mk_class_def(std::string name, Scope scope) {
    ClassDefStmt* c = details::emplace_get<ClassDefStmt>(mk_fqn(scope, name), m_classDefMap);
    c->type         = m_typeFactory->mk_class(name, scope, c);
    return c;
}

ConstructorDefStmt* StmtFactory::mk_constructor_def() {
    return details::emplace_get<ConstructorDefStmt>(
        m_otherStmts,
        nullptr,
        std::vector<ParamVarDefStmt*>{},
        std::vector<BaseInitializerStmt*>{},
        nullptr,
        AccessModifier::Public
    );
}

ConstructorDefStmt* StmtFactory::mk_constructor_def(
    ClassDefStmt* owner,
    std::vector<ParamVarDefStmt*> params,
    std::vector<BaseInitializerStmt*> baseInit,
    CompoundStmt* body,
    AccessModifier access
) {
    return details::emplace_get<ConstructorDefStmt>(
        m_otherStmts,
        owner,
        std::move(params),
        std::move(baseInit),
        body,
        access
    );
}

BaseInitializerStmt* StmtFactory::mak_base_initializer(std::string base, std::vector<Expr*> args) {
    return details::emplace_get<BaseInitializerStmt>(
        m_otherStmts,
        std::move(base),
        std::move(args)
    );
}

BaseInitializerStmt* StmtFactory::mk_base_initializer(std::string base, std::vector<Expr*> args) {
    return details::emplace_get<BaseInitializerStmt>(
        m_otherStmts,
        std::move(base),
        std::move(args)
    );
}

BaseInitializerStmt* StmtFactory::mk_base_initializer(ClassType* type, std::vector<Expr*> args) {
    return details::emplace_get<BaseInitializerStmt>(m_otherStmts, type, std::move(args));
}

SelfInitializerStmt* StmtFactory::mk_self_initializer(std::vector<Expr*> args) {
    return details::emplace_get<SelfInitializerStmt>(m_otherStmts, std::move(args));
}

MemberInitializerStmt* StmtFactory::mk_member_initializer(MemberVarDefStmt* member, Expr* arg) {
    return details::emplace_get<MemberInitializerStmt>(m_otherStmts, member, arg);
}

DestructorDefStmt* StmtFactory::mk_destructor_def(ClassDefStmt* owner, CompoundStmt* body) {

    return details::emplace_get<DestructorDefStmt>(m_otherStmts, owner, body);
}

GenericParam* StmtFactory::mk_generic_param(std::string constraint, std::string name) {
    return details::emplace_get<GenericParam>(m_otherStmts, std::move(constraint), std::move(name));
}

CompoundStmt* StmtFactory::mk_compound(std::vector<Stmt*> stmts) {
    return details::emplace_get<CompoundStmt>(m_otherStmts, std::move(stmts));
}

ReturnStmt* StmtFactory::mk_return(Expr* val) {
    return details::emplace_get<ReturnStmt>(m_otherStmts, val);
}

ExprStmt* StmtFactory::mk_expr(Expr* expr) {
    return details::emplace_get<ExprStmt>(m_otherStmts, expr);
}

IfStmt* StmtFactory::mk_if(Expr* cond, Stmt* iftrue, Stmt* iffalse) {
    return details::emplace_get<IfStmt>(m_otherStmts, cond, iftrue, iffalse);
}

CaseStmt* StmtFactory::mk_case(Expr* expr, Stmt* body) {
    return details::emplace_get<CaseStmt>(m_otherStmts, std::vector<Expr*>{expr}, body);
}

CaseStmt* StmtFactory::mk_case(std::vector<Expr*> exprs, Stmt* body) {
    return details::emplace_get<CaseStmt>(m_otherStmts, std::move(exprs), body);
}

DefaultCaseStmt* StmtFactory::mk_default_case(Stmt* body) {
    return details::emplace_get<DefaultCaseStmt>(m_otherStmts, body);
}

SwitchStmt* StmtFactory::mk_switch(Expr* expr, std::vector<CaseBaseStmt*> cases) {
    return details::emplace_get<SwitchStmt>(m_otherStmts, expr, std::move(cases));
}

WhileStmt* StmtFactory::mk_while(Expr* cond, Stmt* body) {
    return details::emplace_get<WhileStmt>(m_otherStmts, cond, body);
}

DoWhileStmt* StmtFactory::mk_do_while(Expr* cond, Stmt* body) {
    return details::emplace_get<DoWhileStmt>(m_otherStmts, cond, body);
}

ForStmt* StmtFactory::mk_for(Stmt* init, Expr* cond, Stmt* step, Stmt* body) {
    return details::emplace_get<ForStmt>(m_otherStmts, init, cond, step, body);
}

ForEachStmt* StmtFactory::mk_for_each(LocalVarDefStmt* var, Expr* container, Stmt* body) {
    return details::emplace_get<ForEachStmt>(m_otherStmts, var, container, body);
}

ThrowStmt* StmtFactory::mk_throw(Expr* val) {
    return details::emplace_get<ThrowStmt>(m_otherStmts, val);
}

CatchStmt* StmtFactory::mk_catch(LocalVarDefStmt* param, Stmt* body) {
    return details::emplace_get<CatchStmt>(m_otherStmts, param, body);
}

TryStmt* StmtFactory::mk_try(Stmt* body, Stmt* finally, std::vector<CatchStmt*> catches) {
    return details::emplace_get<TryStmt>(m_otherStmts, body, finally, std::move(catches));
}

ContinueStmt* StmtFactory::mk_continue() {
    return &m_continueStmt;
}

BreakStmt* StmtFactory::mk_break() {
    return &m_breakStmt;
}

UnknownStmt* StmtFactory::mk_uknown() {
    return &m_unknownStmt;
}

TranslationUnit* StmtFactory::mk_translation_unit() {
    return details::emplace_get<TranslationUnit>(m_otherStmts);
}

TranslationUnit* StmtFactory::mk_translation_unit(
    std::vector<ClassDefStmt*> classes,
    std::vector<InterfaceDefStmt*> interfaces,
    std::vector<FunctionDefStmt*> functions,
    std::vector<GlobalVarDefStmt*> globals
) {
    TranslationUnit* u = this->mk_translation_unit();
    u->classes         = std::move(classes);
    u->interfaces      = std::move(interfaces);
    u->functions       = std::move(functions);
    u->globals         = std::move(globals);
    return u;
}

} // namespace astfri
