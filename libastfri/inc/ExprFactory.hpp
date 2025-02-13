#ifndef LIBASTFRI_EXPRESSION_FACTORY_HPP
#define LIBASTFRI_EXPRESSION_FACTORY_HPP

#include <libastfri/inc/Expr.hpp>

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
    static ExprFactory& get_instance ();

public:
    ~ExprFactory () = default;

    IntLiteralExpr* mk_int_literal (int val);
    FloatLiteralExpr* mk_float_literal (float val);
    CharLiteralExpr* mk_char_literal (char val);
    StringLiteralExpr* mk_string_literal (std::string const& val);
    BoolLiteralExpr* mk_bool_literal (bool val);
    NullLiteralExpr* mk_null_literal ();
    IfExpr* mk_if (Expr* cond, Expr* iftrue, Expr* iffalse);
    BinOpExpr* mk_bin_on (Expr* left, BinOpType op, Expr* right);
    UnaryOpExpr* mk_unary_op (UnaryOpType op, Expr* arg);
    AssignExpr* mk_assign (Expr* lhs, Expr* rhs);
    CompoundAssignExpr* mk_compound_assign (Expr* lhs, BinOpType op, Expr* rhs);
    ParamVarRefExpr* mk_param_var_ref (std::string param);
    LocalVarRefExpr* mk_local_var_ref (std::string var);
    MemberVarRefExpr* mk_member_var_ref (std::string member);
    GlobalVarRefExpr* mk_global_var_ref (std::string global);
    FunctionCallExpr* mk_function_call (
        std::string name,
        std::vector<Expr*> args
    );
    MethodCallExpr* mk_method_call (
        Expr* owner,
        std::string name,
        std::vector<Expr*> args
    );
    LambdaExpr* mk_lambda_expr (
        std::vector<ParamVarDefStmt*> params,
        Stmt* body
    );
    ThisExpr* mk_this ();
    UnknownExpr* mk_unknown ();

private:
    ExprFactory ()                      = default;
    ExprFactory (ExprFactory const&)    = delete;
    void operator= (ExprFactory const&) = delete;

private:
    std::vector<std::unique_ptr<Expr>> exprs_;

    std::map<int, IntLiteralExpr> ints_;
    std::map<char, CharLiteralExpr> chars_;
    std::map<std::string, StringLiteralExpr> strings_;

    NullLiteralExpr null_;
    BoolLiteralExpr false_ {false};
    BoolLiteralExpr true_ {true};

    ThisExpr this_;
    UnknownExpr unknown_;
};
} // namespace astfri

#endif
