#include <astfri/impl/Utils.hpp>
#include <astfri/impl/ExprFactory.hpp>


namespace astfri {


ExprFactory& ExprFactory::get_instance() {
    static ExprFactory instance;
    return instance;
}

ExprFactory::ExprFactory() :
    m_typeFactory(&TypeFactory::get_instance()) {
}

IntLiteralExpr* ExprFactory::mk_int_literal(const int val) {
    return details::emplace_get<IntLiteralExpr>(
        m_intLiterals,
        val,
        [val](){
            IntLiteralExpr l;
            l.val = val;
            return l;
        });
}

FloatLiteralExpr* ExprFactory::mk_float_literal(float val) {
    return details::emplace_get<FloatLiteralExpr>(
        m_intLiterals,
        val,
        [val](){
            FloatLiteralExpr l;
            l.val = val;
            return l;
        });
}

CharLiteralExpr* ExprFactory::mk_char_literal(const char val) {
    return details::emplace_get<CharLiteralExpr>(
        m_intLiterals,
        val,
        [val](){
            CharLiteralExpr l;
            l.val = val;
            return l;
        });
}

StringLiteralExpr* ExprFactory::mk_string_literal(const std::string& val) {
    return details::emplace_get<StringLiteralExpr>(
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
    return details::create_store_get<IfExpr>(
        m_otherExpressions,
        [=](IfExpr &i){
            i.cond = cond;
            i.iftrue = iftrue;
            i.iffalse = iffalse;
        });
}

BinOpExpr* ExprFactory::mk_bin_on(Expr* left, BinOpType op, Expr* right) {
    return details::emplace_get<BinOpExpr>(m_otherExpressions, left, op, right);
}

UnaryOpExpr* ExprFactory::mk_unary_op(UnaryOpType op, Expr* arg) {
    return details::emplace_get<UnaryOpExpr>(m_otherExpressions, op, arg);
}

ParamVarRefExpr* ExprFactory::mk_param_var_ref(std::string param) {
    return details::emplace_get<ParamVarRefExpr>(m_otherExpressions, std::move(param));
}

LocalVarRefExpr* ExprFactory::mk_local_var_ref(std::string var) {
    return details::emplace_get<LocalVarRefExpr>(m_otherExpressions, std::move(var));
}

MemberVarRefExpr* ExprFactory::mk_member_var_ref(Expr* owner, std::string member) {
    return details::emplace_get<MemberVarRefExpr>(m_otherExpressions, owner, std::move(member));
}

GlobalVarRefExpr* ExprFactory::mk_global_var_ref(std::string global) {
    return details::emplace_get<GlobalVarRefExpr>(m_otherExpressions, std::move(global));
}

ClassRefExpr* ExprFactory::mk_class_ref(std::string name) {
    return details::emplace_get<ClassRefExpr>(m_otherExpressions, std::move(name));
}

FunctionCallExpr* ExprFactory::mk_function_call(std::string name, std::vector<Expr*> args) {
    return details::emplace_get<FunctionCallExpr>(
        m_otherExpressions,
        std::move(name),
        std::move(args)
    );
}

MethodCallExpr* ExprFactory::mk_method_call(
    Expr* owner,
    std::string name,
    std::vector<Expr*> args
) {
    return details::emplace_get<MethodCallExpr>(
        m_otherExpressions,
        owner,
        std::move(name),
        std::move(args)
    );
}

LambdaCallExpr* ExprFactory::mk_lambda_call() {
    return details::emplace_get<LambdaCallExpr>(m_otherExpressions);
}

LambdaCallExpr* ExprFactory::mk_lambda_call(Expr* lambda, std::vector<Expr*> args) {
    LambdaCallExpr* c = this->mk_lambda_call();
    c->lambda         = lambda;
    c->args           = std::move(args);
    return c;
}

LambdaExpr* ExprFactory::mk_lambda_expr() {
    LambdaExpr* e = details::emplace_get<LambdaExpr>(m_otherExpressions);
    e->type       = m_typeFactory->mk_lambda("", e);
    return e;
}

LambdaExpr* ExprFactory::mk_lambda_expr(std::vector<ParamVarDefStmt*> params, Stmt* body) {
    return mk_lambda_expr(std::move(params), body, "");
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
    LambdaExpr* e = details::emplace_get<LambdaExpr>(name, m_lambdaExprs, std::move(params), body);
    e->type       = m_typeFactory->mk_lambda(std::move(name), e);
    return e;
}

ThisExpr* ExprFactory::mk_this() {
    return &m_thisExpr;
}

BaseExpr* ExprFactory::mk_base() {
    return &m_baseExpr;
}

ConstructorCallExpr* ExprFactory::mk_constructor_call(Type* type, std::vector<Expr*> args) {
    return details::emplace_get<ConstructorCallExpr>(m_otherExpressions, type, std::move(args));
}

NewExpr* ExprFactory::mk_new(ConstructorCallExpr* init) {
    return details::emplace_get<NewExpr>(m_otherExpressions, init);
}

DeleteExpr* ExprFactory::mk_delete(Expr* arg) {
    return details::emplace_get<DeleteExpr>(m_otherExpressions, arg);
}

BracketExpr* ExprFactory::mk_bracket(Expr* expr) {
    return details::emplace_get<BracketExpr>(m_otherExpressions, expr);
}

UnknownExpr* ExprFactory::mk_unknown() {
    return &m_unknownExpr;
}


} // namespace astfri
