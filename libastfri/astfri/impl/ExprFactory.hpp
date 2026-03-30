#ifndef ASTFRI_IMPL_EXPR_FACTORY_HPP
#define ASTFRI_IMPL_EXPR_FACTORY_HPP

#include <astfri/impl/Expr.hpp>
#include <astfri/impl/TypeFactory.hpp>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace astfri {

/**
 * @brief Singleton expression factory.
 */
class ExprFactory {
public:
    /**
     * @brief Returns the singleton instance.
     * @return Singleton instance.
     */
    static ExprFactory& get_instance();

public:
    IntLiteralExpr* mk_int_literal(int val);

    FloatLiteralExpr* mk_float_literal(float val);

    CharLiteralExpr* mk_char_literal(char val);

    StringLiteralExpr* mk_string_literal(const std::string& val);

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

    [[deprecated]] LambdaExpr* mk_lambda_expr();
    [[deprecated]] LambdaExpr* mk_lambda_expr(std::vector<ParamVarDefStmt*> params, Stmt* body);

    LambdaExpr* get_lambda_expr(std::string_view name);
    LambdaExpr* mk_lambda_expr(std::vector<ParamVarDefStmt*> params, Stmt* body, std::string name);

    ThisExpr* mk_this();

    ConstructorCallExpr* mk_constructor_call(Type* type, std::vector<Expr*> args);

    NewExpr* mk_new(ConstructorCallExpr* init);

    DeleteExpr* mk_delete(Expr* arg);

    BracketExpr* mk_bracket(Expr* expr);

    UnknownExpr* mk_unknown();

public:
    /**
     * @brief Deleted copy constructor.
     */
    ExprFactory(const ExprFactory& other) = delete;

    /**
     * @brief Explicitly deleted move constructor.
     */
    ExprFactory(ExprFactory&& other) = delete;

    /**
     * @brief Deleted copy-assignment.
     */
    void operator=(const ExprFactory& other) = delete;

    /**
     * @brief Explicitly deleted copy-assignment.
     */
    void operator=(ExprFactory&& other) = delete;

private:
    /**
     * @brief Private deafult constructor.
     */
    ExprFactory();

private:
    TypeFactory* m_typeFactory;
    std::vector<std::unique_ptr<Expr>> m_otherExpressions;
    std::map<int, IntLiteralExpr> m_intLiterals;
    std::map<char, CharLiteralExpr> m_charLiterals;
    std::map<std::string, StringLiteralExpr, std::less<>> m_stringLiterals;
    std::map<std::string, LambdaExpr, std::less<>> m_lambdaExprs;
    NullLiteralExpr m_nullLiteral;
    BoolLiteralExpr m_falseLiteral{false};
    BoolLiteralExpr m_trueLiteral{true};
    ThisExpr m_thisExpr;
    UnknownExpr m_unknownExpr;
};

} // namespace astfri

#endif
