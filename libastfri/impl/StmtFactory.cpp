#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include "libastfri/impl/Scope.hpp"
#include "libastfri/inc/Stmt.hpp"

namespace astfri
{
StmtFactory& StmtFactory::get_instance()
{
    static StmtFactory instance;
    return instance;
}

StmtFactory::StmtFactory() :
    m_types(&TypeFactory::get_instance())
{
}

LocalVarDefStmt* StmtFactory::mk_local_var_def(std::string name, Type* type, Expr* initializer)
{
    return details::emplace_get<LocalVarDefStmt>(stmts_, std::move(name), type, initializer);
}

ParamVarDefStmt* StmtFactory::mk_param_var_def(std::string name, Type* type, Expr* initializer)
{
    return details::emplace_get<ParamVarDefStmt>(stmts_, std::move(name), type, initializer);
}

MemberVarDefStmt* StmtFactory::mk_member_var_def(
    std::string name,
    Type* type,
    Expr* initializer,
    AccessModifier access
)
{
    return details::emplace_get<MemberVarDefStmt>(
        stmts_,
        std::move(name),
        type,
        initializer,
        access
    );
}

GlobalVarDefStmt* StmtFactory::mk_global_var_def(std::string name, Type* type, Expr* initializer)
{
    return details::emplace_get<GlobalVarDefStmt>(stmts_, std::move(name), type, initializer);
}

DefStmt* StmtFactory::mk_def()
{
    return details::emplace_get<DefStmt>(stmts_, std::vector<VarDefStmt*>{});
}

DefStmt* StmtFactory::mk_def(std::vector<VarDefStmt*> defs)
{
    return details::emplace_get<DefStmt>(stmts_, std::move(defs));
}

FunctionDefStmt* StmtFactory::mk_function_def()
{
    return details::emplace_get<FunctionDefStmt>(
        stmts_,
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
)
{
    return details::emplace_get<FunctionDefStmt>(
        stmts_,
        std::move(name),
        std::move(params),
        retType,
        body
    );
}

MethodDefStmt* StmtFactory::mk_method_def()
{
    return details::emplace_get<MethodDefStmt>(stmts_);
}

MethodDefStmt* StmtFactory::mk_method_def(
    UserTypeDefStmt* owner,
    FunctionDefStmt* func,
    AccessModifier access,
    Virtuality virtuality
)
{
    MethodDefStmt* m = this->mk_method_def();
    m->owner_        = owner;
    m->func_         = func;
    m->access_       = access;
    m->virtuality_   = virtuality;
    return m;
}

InterfaceDefStmt* StmtFactory::mk_interface_def()
{
    return details::emplace_get<InterfaceDefStmt>(stmts_);
}

InterfaceDefStmt* StmtFactory::mk_interface_def(std::string name)
{
    InterfaceDefStmt* i = details::emplace_get<InterfaceDefStmt>(stmts_);
    i->name_            = std::move(name);
    return i;
}

InterfaceDefStmt* StmtFactory::mk_interface_def(std::string name, Scope scope) {
    InterfaceDefStmt*i = details::emplace_get<InterfaceDefStmt>(mk_fqn(scope, name), m_interfaces);
    i->m_type = m_types->mk_interface(name, scope, i);
    return i;
}

ClassDefStmt *StmtFactory::get_class_def(std::string_view name, const Scope &scope) {
    const auto it = m_classes.find(mk_fqn(scope, name));
    return it != m_classes.end() ? &it->second : nullptr;
}

ClassDefStmt* StmtFactory::mk_class_def(std::string name, Scope scope)
{
    ClassDefStmt *c = details::emplace_get<ClassDefStmt>(mk_fqn(scope, name), m_classes);
    c->type_ = m_types->mk_class(name, scope, c);
    return c;
}

ConstructorDefStmt* StmtFactory::mk_constructor_def()
{
    return details::emplace_get<ConstructorDefStmt>(
        stmts_,
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
)
{
    return details::emplace_get<ConstructorDefStmt>(
        stmts_,
        owner,
        std::move(params),
        std::move(baseInit),
        body,
        access
    );
}

BaseInitializerStmt* StmtFactory::mak_base_initializer(std::string base, std::vector<Expr*> args)
{
    return details::emplace_get<BaseInitializerStmt>(stmts_, std::move(base), std::move(args));
}

BaseInitializerStmt* StmtFactory::mk_base_initializer(std::string base, std::vector<Expr*> args)
{
    return details::emplace_get<BaseInitializerStmt>(stmts_, std::move(base), std::move(args));
}

BaseInitializerStmt* StmtFactory::mk_base_initializer(ClassType *type, std::vector<Expr*> args)
{
    return details::emplace_get<BaseInitializerStmt>(stmts_, type, std::move(args));
}

SelfInitializerStmt* StmtFactory::mk_self_initializer(std::vector<Expr*> args)
{
    return details::emplace_get<SelfInitializerStmt>(stmts_, std::move(args));
}

MemberInitializerStmt* StmtFactory::mk_member_initializer(MemberVarDefStmt *member, Expr *arg)
{
    return details::emplace_get<MemberInitializerStmt>(stmts_, member, arg);
}

DestructorDefStmt* StmtFactory::mk_destructor_def(ClassDefStmt* owner, CompoundStmt* body)
{

    return details::emplace_get<DestructorDefStmt>(stmts_, owner, body);
}

GenericParam* StmtFactory::mk_generic_param(std::string constraint, std::string name)
{
    return details::emplace_get<GenericParam>(stmts_, std::move(constraint), std::move(name));
}

CompoundStmt* StmtFactory::mk_compound(std::vector<Stmt*> stmts)
{
    return details::emplace_get<CompoundStmt>(stmts_, std::move(stmts));
}

ReturnStmt* StmtFactory::mk_return(Expr* val)
{
    return details::emplace_get<ReturnStmt>(stmts_, val);
}

ExprStmt* StmtFactory::mk_expr(Expr* expr)
{
    return details::emplace_get<ExprStmt>(stmts_, expr);
}

IfStmt* StmtFactory::mk_if(Expr* cond, Stmt* iftrue, Stmt* iffalse)
{
    return details::emplace_get<IfStmt>(stmts_, cond, iftrue, iffalse);
}

CaseStmt* StmtFactory::mk_case(Expr* expr, Stmt* body)
{
    return details::emplace_get<CaseStmt>(stmts_, std::vector<Expr*>{expr}, body);
}

CaseStmt* StmtFactory::mk_case(std::vector<Expr*> exprs, Stmt* body)
{
    return details::emplace_get<CaseStmt>(stmts_, std::move(exprs), body);
}

DefaultCaseStmt* StmtFactory::mk_default_case(Stmt* body)
{
    return details::emplace_get<DefaultCaseStmt>(stmts_, body);
}

SwitchStmt* StmtFactory::mk_switch(Expr* expr, std::vector<CaseBaseStmt*> cases)
{
    return details::emplace_get<SwitchStmt>(stmts_, expr, std::move(cases));
}

WhileStmt* StmtFactory::mk_while(Expr* cond, Stmt* body)
{
    return details::emplace_get<WhileStmt>(stmts_, cond, body);
}

DoWhileStmt* StmtFactory::mk_do_while(Expr* cond, Stmt* body)
{
    return details::emplace_get<DoWhileStmt>(stmts_, cond, body);
}

ForStmt* StmtFactory::mk_for(Stmt* init, Expr* cond, Stmt* step, Stmt* body)
{
    return details::emplace_get<ForStmt>(stmts_, init, cond, step, body);
}

ForEachStmt* StmtFactory::mk_for_each(LocalVarDefStmt *var, Expr *container, Stmt *body)
{
    return details::emplace_get<ForEachStmt>(stmts_, var, container, body);
}

ThrowStmt* StmtFactory::mk_throw(Expr* val)
{
    return details::emplace_get<ThrowStmt>(stmts_, val);
}

CatchStmt *StmtFactory::mk_catch(LocalVarDefStmt *param, Stmt *body) {
    return details::emplace_get<CatchStmt>(stmts_, param, body);
}

TryStmt *StmtFactory::mk_try(Stmt *body, Stmt *finally, std::vector<CatchStmt*> catches) {
    return details::emplace_get<TryStmt>(stmts_, body, finally, std::move(catches));
}

ContinueStmt* StmtFactory::mk_continue()
{
    return &continue_;
}

BreakStmt* StmtFactory::mk_break()
{
    return &break_;
}

UnknownStmt* StmtFactory::mk_uknown()
{
    return &unknown_;
}

TranslationUnit* StmtFactory::mk_translation_unit()
{
    return details::emplace_get<TranslationUnit>(stmts_);
}

TranslationUnit* StmtFactory::mk_translation_unit(
    std::vector<ClassDefStmt*> classes,
    std::vector<InterfaceDefStmt*> interfaces,
    std::vector<FunctionDefStmt*> functions,
    std::vector<GlobalVarDefStmt*> globals
)
{
    TranslationUnit* u = this->mk_translation_unit();
    u->classes_        = std::move(classes);
    u->interfaces_     = std::move(interfaces);
    u->functions_      = std::move(functions);
    u->globals_        = std::move(globals);
    return u;
}

} // namespace astfri
