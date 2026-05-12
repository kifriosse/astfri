#include <astfri/impl/StmtFactory.hpp>
#include <astfri/impl/TypeFactory.hpp>
#include <astfri/impl/Utils.hpp>


namespace astfri {


StmtFactory &StmtFactory::get_instance() {
    static StmtFactory instance;
    return instance;
}

StmtFactory::StmtFactory() :
    m_typeFactory(&TypeFactory::get_instance()) {
}

LocalVarDefStmt *StmtFactory::mk_local_var_def(std::string name, Type *type, Expr *initializer) {
    return details::create_store_get<LocalVarDefStmt>(
        m_otherStmts,
        [&name, type, initializer](){
            LocalVarDefStmt v;
            v.name = std::move(name);
            v.type = type;
            v.initializer = initializer;
            return v;
        });
}

ParamVarDefStmt *StmtFactory::mk_param_var_def(std::string name, Type *type, Expr *initializer) {
    return details::create_store_get<ParamVarDefStmt>(
        m_otherStmts,
        [&name, type, initializer](){
            ParamVarDefStmt p;
            p.name = std::move(name);
            p.type = type;
            p.initializer = initializer;
            return p;
        });
}

MemberVarDefStmt *StmtFactory::mk_member_var_def(
    std::string name,
    Type *type,
    Expr *initializer,
    AccessModifier access,
    Staticity staticity
) {
    return details::create_store_get<MemberVarDefStmt>(
        m_otherStmts,
        [=, &name](){
            MemberVarDefStmt m;
            m.name = std::move(name);
            m.type = type;
            m.initializer = initializer;
            m.access = access;
            m.staticity = staticity;
            return m;
        });
}

GlobalVarDefStmt* StmtFactory::mk_global_var_def(std::string name, Type* type, Expr* initializer) {
    return details::create_store_get<GlobalVarDefStmt>(
        m_otherStmts,
        [&name, type, initializer](){
            GlobalVarDefStmt g;
            g.name = std::move(name);
            g.type = type;
            g.initializer = initializer;
            return g;
        });
}

MultiLocalVarDefStmt *StmtFactory::mk_def() {
    return details::create_store_get<MultiLocalVarDefStmt>(
        m_otherStmts,
        [](){
            MultiLocalVarDefStmt m;
            return m;
        });
}

MultiLocalVarDefStmt *StmtFactory::mk_def(std::vector<LocalVarDefStmt*> defs) {
    return details::create_store_get<MultiLocalVarDefStmt>(
        m_otherStmts,
        [&defs](){
            MultiLocalVarDefStmt m;
            m.defs = std::move(defs);
            return m;
        });
}

FunctionDefStmt *StmtFactory::mk_function_def() {
    return details::create_store_get<FunctionDefStmt>(
        m_otherStmts,
        [](){
            FunctionDefStmt f;
            return f;
        });
}

FunctionDefStmt *StmtFactory::mk_function_def(
    std::string name,
    std::vector<ParamVarDefStmt*> params,
    Type *retType,
    CompoundStmt *body
) {
    return details::create_store_get<FunctionDefStmt>(
        m_otherStmts,
        [&name, &params, retType, body](){
            FunctionDefStmt f;
            f.name = std::move(name);
            f.params = std::move(params);
            f.retType = retType;
            f.body = body;
            return f;
        });
}

MethodDefStmt *StmtFactory::mk_method_def() {
    return details::create_store_get<MethodDefStmt>(
        m_otherStmts,
        [](){
            MethodDefStmt m;
            return m;
        });
}

MethodDefStmt *StmtFactory::mk_method_def(
    Stmt *owner,
    FunctionDefStmt *func,
    AccessModifier access,
    Virtuality virtuality,
    Staticity staticity
) {
    return details::create_store_get<MethodDefStmt>(
        m_otherStmts,
        [=](){
            MethodDefStmt m;
            m.owner = owner;
            m.func = func;
            m.access = access;
            m.virtuality = virtuality;
            m.staticity = staticity;
            return m;
        });
}

InterfaceDefStmt* StmtFactory::mk_interface_def(const std::string &name, const Scope &scope) {
    auto *i = details::get_or_emplace<InterfaceDefStmt>(
        m_interfaceDefMap,
        mk_fqn(scope, name),
        [&name](InterfaceDefStmt &i){
            i.name = std::move(name);
        });
    i->type = m_typeFactory->mk_interface(name, scope, i);
    return i;
}

ClassDefStmt *StmtFactory::get_class_def(std::string_view name, const Scope &scope) {
    const auto it = m_classDefMap.find(mk_fqn(scope, name));
    return it != m_classDefMap.end() ? &it->second : nullptr;
}

ClassDefStmt *StmtFactory::mk_class_def(const std::string &name, const Scope &scope) {
    auto *c = details::get_or_emplace<ClassDefStmt>(
        m_classDefMap,
        mk_fqn(scope, name),
        [&name](ClassDefStmt &c){
            c.name = name;
        });
    c->type = m_typeFactory->mk_class(name, scope, c);
    return c;
}

ConstructorDefStmt *StmtFactory::mk_constructor_def() {
    return details::create_store_get<ConstructorDefStmt>(
        m_otherStmts,
        [](){
            ConstructorDefStmt c;
            return c;
        });
}

ConstructorDefStmt *StmtFactory::mk_constructor_def(
    ClassDefStmt *owner,
    std::vector<ParamVarDefStmt*> params,
    std::vector<BaseInitializerStmt*> baseInit,
    CompoundStmt *body,
    AccessModifier access
) {
    return details::create_store_get<ConstructorDefStmt>(
        m_otherStmts,
        [=, &params, &baseInit](){
            ConstructorDefStmt c;
            c.owner = owner;
            c.params = std::move(params);
            c.baseInit = std::move(baseInit);
            c.body = body;
            c.access = access;
            return c;
        });
}

BaseInitializerStmt *StmtFactory::mk_base_initializer(ClassType *type, std::vector<Expr*> args) {
    return details::create_store_get<BaseInitializerStmt>(
        m_otherStmts,
        [type, &args](){
            BaseInitializerStmt b;
            b.type = type;
            b.args = std::move(args);
            return b;
        });
}

SelfInitializerStmt *StmtFactory::mk_self_initializer(std::vector<Expr*> args) {
    return details::create_store_get<SelfInitializerStmt>(
        m_otherStmts,
        [&args](){
            SelfInitializerStmt i;
            i.args = std::move(args);
            return i;
        });
}

MemberInitializerStmt *StmtFactory::mk_member_initializer(
    MemberVarDefStmt *member,
    std::vector<Expr*> args
) {
    return details::create_store_get<MemberInitializerStmt>(
        m_otherStmts,
        [member, &args](){
            MemberInitializerStmt i;
            i.member = member;
            i.args = std::move(args);
            return i;
        });
}

DestructorDefStmt *StmtFactory::mk_destructor_def(ClassDefStmt *owner, CompoundStmt *body) {

    return details::create_store_get<DestructorDefStmt>(
        m_otherStmts,
        [owner, body](){
            DestructorDefStmt d;
            d.owner = owner;
            d.body = body;
            return d;
        });
}

GenericParam *StmtFactory::mk_generic_param(std::string constraint, std::string name) {
    return details::create_store_get<GenericParam>(
        m_otherStmts,
        [&constraint, &name](){
            GenericParam p;
            p.constraint = std::move(constraint);
            p.name = std::move(name);
            return p;
        });
}

CompoundStmt *StmtFactory::mk_compound(std::vector<Stmt*> stmts) {
    return details::create_store_get<CompoundStmt>(
        m_otherStmts,
        [&stmts](){
            CompoundStmt s;
            s.stmts = std::move(stmts);
            return s;
        });
}

ReturnStmt *StmtFactory::mk_return(Expr *val) {
    return details::create_store_get<ReturnStmt>(
        m_otherStmts,
        [val](){
            ReturnStmt r;
            r.val = val;
            return r;
        });
}

ExprStmt *StmtFactory::mk_expr(Expr *expr) {
    return details::create_store_get<ExprStmt>(
        m_otherStmts,
        [expr](){
            ExprStmt s;
            s.expr = expr;
            return s;
        });
}

IfStmt *StmtFactory::mk_if(Expr *cond, Stmt *iftrue, Stmt *iffalse) {
    return details::create_store_get<IfStmt>(
        m_otherStmts,
        [cond, iftrue, iffalse](){
            IfStmt s;
            s.cond = cond;
            s.iftrue = iftrue;
            s.iffalse = iffalse;
            return s;
        });
}

CaseStmt *StmtFactory::mk_case(Expr *expr, Stmt *body) {
    return details::create_store_get<CaseStmt>(
        m_otherStmts,
        [expr, body](){
            CaseStmt c;
            c.exprs = {expr};
            c.body = body;
            return c;
        });
}

CaseStmt *StmtFactory::mk_case(std::vector<Expr*> exprs, Stmt *body) {
    return details::create_store_get<CaseStmt>(
        m_otherStmts,
        [&exprs, body](){
            CaseStmt c;
            c.exprs = std::move(exprs);
            c.body = body;
            return c;
        });
}

DefaultCaseStmt *StmtFactory::mk_default_case(Stmt *body) {
    return details::create_store_get<DefaultCaseStmt>(
        m_otherStmts,
        [body](){
            DefaultCaseStmt d;
            d.body = body;
            return d;
        });
}

SwitchStmt *StmtFactory::mk_switch(Expr *expr, std::vector<CaseStmt*> cases) {
    return details::create_store_get<SwitchStmt>(
        m_otherStmts,
        [expr, &cases](){
            SwitchStmt s;
            s.expr = expr;
            s.cases = std::move(cases);
            s.defaultCase = nullptr;
            return s;
        });
}

SwitchStmt *StmtFactory::mk_switch(
    Expr *expr,
    std::vector<CaseStmt*> cases,
    DefaultCaseStmt *defaultStmt
) {
    return details::create_store_get<SwitchStmt>(
        m_otherStmts,
        [expr, &cases, defaultStmt](){
            SwitchStmt s;
            s.expr = expr;
            s.cases = std::move(cases);
            s.defaultCase = defaultStmt;
            return s;
        });
}

WhileStmt *StmtFactory::mk_while(Expr *cond, Stmt *body) {
    return details::create_store_get<WhileStmt>(
        m_otherStmts,
        [cond, body](){
            WhileStmt w;
            w.cond = cond;
            w.body = body;
            return w;
        });
}

DoWhileStmt *StmtFactory::mk_do_while(Expr *cond, Stmt *body) {
    return details::create_store_get<DoWhileStmt>(
        m_otherStmts,
        [cond, body](){
            DoWhileStmt w;
            w.cond = cond;
            w.body = body;
            return w;
        });
}

ForStmt *StmtFactory::mk_for(Stmt *init, Expr *cond, Stmt *step, Stmt *body) {
    return details::create_store_get<ForStmt>(
        m_otherStmts,
        [=](){
            ForStmt f;
            f.init = init;
            f.cond = cond;
            f.step = step;
            f.body = body;
            return f;
        });
}

ForEachStmt *StmtFactory::mk_for_each(LocalVarDefStmt *var, Expr *container, Stmt *body) {
    return details::create_store_get<ForEachStmt>(
        m_otherStmts,
        [var, container, body](){
            ForEachStmt f;
            f.var = var;
            f.container = container;
            f.body = body;
            return f;
        });
}

ThrowStmt *StmtFactory::mk_throw(Expr *val) {
    return details::create_store_get<ThrowStmt>(
        m_otherStmts,
        [val](){
            ThrowStmt t;
            t.val = val;
            return t;
        });
}

CatchStmt *StmtFactory::mk_catch(LocalVarDefStmt *param, Stmt *body) {
    return details::create_store_get<CatchStmt>(
        m_otherStmts,
        [param, body](){
            CatchStmt c;
            c.param = param;
            c.body = body;
            return c;
        });
}

TryStmt *StmtFactory::mk_try(Stmt *body, Stmt *finally, std::vector<CatchStmt*> catches) {
    return details::create_store_get<TryStmt>(
        m_otherStmts,
        [body, finally, &catches](){
            TryStmt t;
            t.body = body;
            t.finally = finally;
            t.catches = std::move(catches);
            return t;
        });
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
    return details::create_store_get<TranslationUnit>(
        m_otherStmts,
        [](){
            TranslationUnit t;
            return t;
        });
}


} // namespace astfri
