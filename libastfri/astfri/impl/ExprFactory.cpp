#include <astfri/impl/Utils.hpp>
#include <astfri/impl/ExprFactory.hpp>


namespace astfri {


ExprFactory& ExprFactory::get_instance() {
    static ExprFactory instance;
    return instance;
}

ExprFactory::ExprFactory() :
    m_typeFactory(&TypeFactory::get_instance()) {
    m_trueLiteral.val = true;
    m_falseLiteral.val = false;
}

IntLiteralExpr* ExprFactory::mk_int_literal(const int val) {
    return details::get_or_emplace<IntLiteralExpr>(
        m_intLiterals,
        val,
        [val](){
            IntLiteralExpr l;
            l.val = val;
            return l;
        });
}

FloatLiteralExpr* ExprFactory::mk_float_literal(float val) {
    return details::get_or_emplace<FloatLiteralExpr>(
        m_intLiterals,
        val,
        [val](){
            FloatLiteralExpr l;
            l.val = val;
            return l;
        });
}

CharLiteralExpr* ExprFactory::mk_char_literal(const char val) {
    return details::get_or_emplace<CharLiteralExpr>(
        m_intLiterals,
        val,
        [val](){
            CharLiteralExpr l;
            l.val = val;
            return l;
        });
}

StringLiteralExpr* ExprFactory::mk_string_literal(const std::string& val) {
    return details::get_or_emplace<StringLiteralExpr>(
        m_intLiterals,
        val,
        [val](){
            StringLiteralExpr l;
            l.val = val;
            return l;
        });
}

BoolLiteralExpr* ExprFactory::mk_bool_literal(const bool val) {
    return val ? &m_trueLiteral : &m_falseLiteral;
}

NullLiteralExpr* ExprFactory::mk_null_literal() {
    return &m_nullLiteral;
}

IfExpr* ExprFactory::mk_if(Expr* cond, Expr* iftrue, Expr* iffalse) {
    return details::create_store_get<IfExpr>(m_otherExpressions, [=](IfExpr &i){
        i.cond = cond;
        i.iftrue = iftrue;
        i.iffalse = iffalse;
    });
}

BinOpExpr* ExprFactory::mk_bin_on(Expr* left, BinOpType op, Expr* right) {
    return details::create_store_get<BinOpExpr>(m_otherExpressions, [=](BinOpExpr &e){
        e.left = left;
        e.op = op;
        e.right = right;
    });
}

UnaryOpExpr* ExprFactory::mk_unary_op(UnaryOpType op, Expr* arg) {
    return details::create_store_get<UnaryOpExpr>(m_otherExpressions, [=](UnaryOpExpr &u){
        u.op = op;
        u.arg = arg;
    });
}

ParamVarRefExpr* ExprFactory::mk_param_var_ref(std::string param) {
    return details::create_store_get<ParamVarRefExpr>(m_otherExpressions, [&](ParamVarRefExpr &e){
        e.param = std::move(param);
    });
}

LocalVarRefExpr* ExprFactory::mk_local_var_ref(std::string var) {
    return details::create_store_get<LocalVarRefExpr>(m_otherExpressions, [&](LocalVarRefExpr &r){
        r.var = std::move(var);
    });
}

MemberVarRefExpr* ExprFactory::mk_member_var_ref(Expr* owner, std::string member) {
    return details::create_store_get<MemberVarRefExpr>(m_otherExpressions, [&](MemberVarRefExpr &r){
        r.owner = owner;
        r.member = std::move(member);
    });
}

GlobalVarRefExpr* ExprFactory::mk_global_var_ref(std::string global) {
    return details::create_store_get<GlobalVarRefExpr>(m_otherExpressions, [&](GlobalVarRefExpr &r){
        r.global = std::move(global);
    });
}

ClassRefExpr* ExprFactory::mk_class_ref(std::string name) {
    return details::create_store_get<ClassRefExpr>(m_otherExpressions, [&](ClassRefExpr &r){
        r.name = std::move(name);
    });
}

FunctionCallExpr* ExprFactory::mk_function_call(std::string name, std::vector<Expr*> args) {
    return details::create_store_get<FunctionCallExpr>(m_otherExpressions, [&](FunctionCallExpr &r){
        r.name = std::move(name);
        r.args = std::move(args);
    });
}

MethodCallExpr* ExprFactory::mk_method_call(Expr* owner, std::string name, std::vector<Expr*> args){
    return details::create_store_get<MethodCallExpr>(m_otherExpressions, [&](MethodCallExpr &c){
        c.owner = owner;
        c.name = std::move(name);
        c.args = std::move(args);
    });
}

LambdaCallExpr* ExprFactory::mk_lambda_call() {
    return details::create_store_get<LambdaCallExpr>(m_otherExpressions, [](LambdaCallExpr &l){
        (void)l;
    });
}

LambdaCallExpr* ExprFactory::mk_lambda_call(Expr* lambda, std::vector<Expr*> args) {
    return details::create_store_get<LambdaCallExpr>(m_otherExpressions, [&](LambdaCallExpr &l){
        l.lambda = lambda;
        l.args = std::move(args);
    });
}

LambdaExpr* ExprFactory::get_lambda_expr(std::string_view name) {
    const auto it = m_lambdaExprs.find(name);
    return it != m_lambdaExprs.end() ? &it->second : nullptr;
}

LambdaExpr* ExprFactory::mk_lambda_expr(
    std::vector<ParamVarDefStmt*> params,
    Stmt* body,
    std::string name
) {
    LambdaExpr* e = details::get_or_emplace<LambdaExpr>(m_lambdaExprs, name, [&](){
        LambdaExpr l;
        l.body = body;
        l.params = std::move(params);
        l.returnType = m_typeFactory->mk_deduced(nullptr); // TODO
    });
    e->type = m_typeFactory->mk_lambda(std::move(name), e);
    return e;
}

ThisExpr* ExprFactory::mk_this() {
    return &m_thisExpr;
}

BaseExpr* ExprFactory::mk_base() {
    return &m_baseExpr;
}

ConstructorCallExpr* ExprFactory::mk_constructor_call(Type* type, std::vector<Expr*> args) {
    return details::create_store_get<ConstructorCallExpr>(m_otherExpressions,
    [&](ConstructorCallExpr &c){
        c.type = type;
        c.args = std::move(args);
    });
}

NewExpr* ExprFactory::mk_new(ConstructorCallExpr* init) {
    return details::create_store_get<NewExpr>(m_otherExpressions, [=](NewExpr &n){
        n.init = init;
    });
}

DeleteExpr* ExprFactory::mk_delete(Expr* arg) {
    return details::create_store_get<DeleteExpr>(m_otherExpressions, [=](DeleteExpr &d){
        d.arg = arg;
    });
}

BracketExpr* ExprFactory::mk_bracket(Expr* expr) {
    return details::create_store_get<BracketExpr>(m_otherExpressions, [=](BracketExpr &b){
        b.expr = expr;
    });
}

UnknownExpr* ExprFactory::mk_unknown() {
    return &m_unknownExpr;
}


} // namespace astfri
