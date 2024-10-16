#ifndef LIBASTFRI_EXPRESSION_FACTORY_HPP
#define LIBASTFRI_EXPRESSION_FACTORY_HPP

#include <libastfri/Expression.hpp>

#include <map>
#include <string>
#include <vector>

namespace astfri
{
class ExpressionFactory
{
public:
    static ExpressionFactory& getInstance();

public:
    ~ExpressionFactory();

    IntLiteralExpr* mk_int_literal(int val);
    FloatLiteralExpr* mk_float_literal(float val);
    CharLiteralExpr* mk_char_literal(char val);
    StringLiteralExpr* mk_string_literal(const std::string& val);
    BoolLiteralExpr* mk_bool_literal(bool val);
    NullLiteralExpr* mk_null_literal();
    IfExpr* mk_if();
    BinOpExpr* mk_bin_on();
    UnaryOpExpr* mk_unary_op();
    AssignExpr* mk_assign();
    CompoundAssignExpr* mk_compound_assign();
    ParamVarRefExpr* mk_param_var_ref();
    LocalVarRefExpr* mk_local_var_ref();
    MemberVarRefExpr* mk_member_var_ref();
    FunctionCallExpr* mk_function_call();
    MethodCallExpr* mk_method_call();
    LambdaExpr* mk_lambda_expr();
    ThisExpr* mk_this();
    UnknownExpr* mk_unknown();

private:
    ExpressionFactory() = default;
    ExpressionFactory(ExpressionFactory const&) = delete;
    void operator= (ExpressionFactory const&)   = delete;

private:
    std::vector<Expr*> exprs_;

    std::map<int, IntLiteralExpr> ints_;
    std::map<char, CharLiteralExpr> chars_;
    std::map<std::string, StringLiteralExpr> strings_;

    NullLiteralExpr null_;
    BoolLiteralExpr false_{false};
    BoolLiteralExpr true_{true};
};
} // namespace astfri

#endif