#ifndef LIBASTFRI_EXPRESSION_FACTORY_HPP
#define LIBASTFRI_EXPRESSION_FACTORY_HPP

#include <libastfri/inc/Expr.hpp>
#include <libastfri/inc/TypeFactory.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace astfri
{
/**
 * @brief TODO
 */
class ExprFactory
{
public:
    static ExprFactory& get_instance();

public:
    ~ExprFactory() = default;

    IntLiteralExpr* mk_int_literal(int val);

    FloatLiteralExpr* mk_float_literal(float val);

    CharLiteralExpr* mk_char_literal(char val);

    StringLiteralExpr* mk_string_literal(std::string const& val);

    BoolLiteralExpr* mk_bool_literal(bool val);

    NullLiteralExpr* mk_null_literal();

    IfExpr* mk_if(Expr* cond, Expr* iftrue, Expr* iffalse);

    BinOpExpr* mk_bin_on(Expr* left, BinOpType op, Expr* right);

    UnaryOpExpr* mk_unary_op(UnaryOpType op, Expr* arg);

    ParamVarRefExpr* mk_param_var_ref(std::string param);

    LocalVarRefExpr* mk_local_var_ref(std::string var);

    MemberVarRefExpr* mk_member_var_ref(Expr* owner, std::string member);

    GlobalVarRefExpr* mk_global_var_ref(std::string global);

    ClassRefExpr* mk_class_ref(std::string name);

    FunctionCallExpr* mk_function_call(std::string name, std::vector<Expr*> args);

    MethodCallExpr* mk_method_call(Expr* owner, std::string name, std::vector<Expr*> args);

    LambdaCallExpr* mk_lambda_call();
    LambdaCallExpr* mk_lambda_call(Expr* lambda, std::vector<Expr*> args);

    LambdaExpr* mk_lambda_expr(std::vector<ParamVarDefStmt*> params, Stmt* body);
    LambdaExpr* mk_lambda_expr(std::vector<ParamVarDefStmt*> params, Stmt* body, std::string name);

    ThisExpr* mk_this();

    ConstructorCallExpr* mk_constructor_call(Type* type, std::vector<Expr*> args);

    NewExpr* mk_new(ConstructorCallExpr* init);

    DeleteExpr* mk_delete(Expr* arg);

    UnknownExpr* mk_unknown();

private:
    ExprFactory();
    ExprFactory(const ExprFactory &) = delete;
    ExprFactory(ExprFactory &&) = delete;
    void operator=(const ExprFactory &) = delete;
    void operator=(ExprFactory &&) = delete;

private:
    TypeFactory *m_types;

    std::vector<std::unique_ptr<Expr>> exprs_;

    std::map<int, IntLiteralExpr> ints_;
    std::map<char, CharLiteralExpr> chars_;
    std::map<std::string, StringLiteralExpr> strings_;

    NullLiteralExpr null_;
    BoolLiteralExpr false_{false};
    BoolLiteralExpr true_{true};

    ThisExpr this_;
    UnknownExpr unknown_;
};
} // namespace astfri

#endif
