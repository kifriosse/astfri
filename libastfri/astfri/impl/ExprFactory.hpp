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
    /**
     * @brief Returns per @p val unique instance of @c IntLiteralExpr.
     * @param val Lieteral value.
     * @return per @p val unique instance of @c IntLiteralExpr.
     */
    IntLiteralExpr* mk_int_literal(int val);

    /**
     * @brief Returns new instance of @c FloatLiteralExpr.
     * @param val Literal value.
     * @return New instalce of @c FloatLiteralExpr.
     */
    FloatLiteralExpr* mk_float_literal(float val);

    /**
     * @brief Returns per @p val unique instance of @c CharLiteralExpr.
     * @param val Lieteral value.
     * @return per @p val unique instance of @c CharLiteralExpr.
     */
    CharLiteralExpr* mk_char_literal(char val);

    /**
     * @brief Returns per @p val unique instance of @c StringLiteralExpr.
     * @param val Lieteral value.
     * @return per @p val unique instance of @c StringLiteralExpr.
     */
    StringLiteralExpr* mk_string_literal(const std::string& val);

    /**
     * @brief Returns per @p val unique instance of @c BoolLiteralExpr.
     * @param val Lieteral value.
     * @return per @p val unique instance of @c BoolLiteralExpr.
     */
    BoolLiteralExpr* mk_bool_literal(bool val);

    /**
     * @brief Returns singleton instance of @c NullLiteralExpr.
     * @return Singleton instance of @c NullLiteralExpr.
     */
    NullLiteralExpr* mk_null_literal();

    /**
     * @brief Returns new instance of @c IfExpr (ternary operator).
     * @param cond If condition.
     * @param iftrue Expression if @p cond is true.
     * @param iffalse Expression if @p cond is false.
     * @return New instance if @c IfExpr (ternary operator).
     */
    IfExpr* mk_if(Expr* cond, Expr* iftrue, Expr* iffalse);

    /**
     * @brief Returns new instance of @c BinOpExpr.
     * @param left Left argument.
     * @param op Operator type.
     * @param right Right argument.
     * @return New instance of @c BinOpExpr.
     */
    BinOpExpr* mk_bin_on(Expr* left, BinOpType op, Expr* right);

    /**
     * @brief Returns new instance of @c UnaryOpExpr.
     * @param op Operator type.
     * @param arg Operator argument.
     * @return New instance of @c UnaryOpExpr.
     */
    UnaryOpExpr* mk_unary_op(UnaryOpType op, Expr* arg);

    /**
     * @brief TODO
     * @param
     * @return
     */
    ParamVarRefExpr* mk_param_var_ref(std::string param);

    /**
     * @brief
     * @param
     * @return
     */
    LocalVarRefExpr* mk_local_var_ref(std::string var);

    /**
     * @brief
     * @param
     * @return
     */
    MemberVarRefExpr* mk_member_var_ref(Expr* owner, std::string member);

    /**
     * @brief
     * @param
     * @return
     */
    GlobalVarRefExpr* mk_global_var_ref(std::string global);

    /**
     * @brief
     * @param
     * @return
     */
    ClassRefExpr* mk_class_ref(std::string name);

    /**
     * @brief TODO
     * @param
     * @return
     */
    FunctionCallExpr* mk_function_call(std::string name, std::vector<Expr*> args);

    /**
     * @brief
     * @param
     * @return
     */
    MethodCallExpr* mk_method_call(Expr* owner, std::string name, std::vector<Expr*> args);

    /**
     * @deprecated
     */
    [[deprecated]] LambdaCallExpr* mk_lambda_call();

    /**
     * @brief Returns new instance of @c LambdaCallExpr.
     * The parameter @p lambda can be a @c LocalVarRef, @c ParamVarRef, ...,
     * or @c LambdaExpr if it is an imediatelly invoked lambda.
     * @param lambda Expression providing the lambda function.
     * @param args Call arguments.
     * @return New instance of @c LambdaCallExpr.
     */
    LambdaCallExpr* mk_lambda_call(Expr* lambda, std::vector<Expr*> args);

    /**
     * @deprecated
     */
    [[deprecated]] LambdaExpr* mk_lambda_expr();

    /**
     * @deprecated
     */
    [[deprecated]] LambdaExpr* mk_lambda_expr(std::vector<ParamVarDefStmt*> params, Stmt* body);

    /**
     * @brief Returns an existing instace of @c LambdaExpr with name @p name or @c nullptr.
     * The instance must have been created before using @c mk_lambda_expr with the same @p name.
     * Otherwise, @c nullptr is returned.
     * @param name Lambda name.
     * @return an existing instace of @c LambdaExpr with name @p name or @c nullptr.
     */
    LambdaExpr* get_lambda_expr(std::string_view name);

    /**
     * @brief Returns new per @p name unique instance of @c LambdaExpr.
     * @param params Function parameters.
     * @param body Function body.
     * @param name Name of the lambda functions (e.g. generated from its position in the code).
     * @return Per @p name unique instance of @c LambdaExpr.
     */
    LambdaExpr* mk_lambda_expr(std::vector<ParamVarDefStmt*> params, Stmt* body, std::string name);

    /**
     * @brief Returns singleton instance of @c ThisExpr.
     * @return Singleton instance of @c ThisExpr.
     */
    ThisExpr* mk_this();

    /**
     * @brief Returns singleton instance of @c BaseExpr.
     * @return Singleton instance of @c BaseExpr.
     */
    BaseExpr* mk_base();

    /**
     * @brief TODO
     * @param
     * @return
     */
    ConstructorCallExpr* mk_constructor_call(Type* type, std::vector<Expr*> args);

    /**
     * @brief TODO
     * @param
     * @return
     */
    NewExpr* mk_new(ConstructorCallExpr* init);

    /**
     * @brief Returns new instance of @c DeleteExpr.
     * @param arg Argument of the delete operator.
     * @return New instance of @c DeleteExpr.
     */
    DeleteExpr* mk_delete(Expr* arg);

    /**
     * @brief Returns new instance of @c BracketExpr.
     * @param expr Expression inside brackets.
     * @return New instance of @c BracketExpr.
     */
    BracketExpr* mk_bracket(Expr* expr);

    /**
     * @brief Returns singleton instance of @c UnknownExpr.
     * @return Singleton instance of @c UnknownExpr.
     */
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
    /**
     * @brief Pointer to the cooperating type factory.
     * TypeFactory is singleton and could be accessed directly.
     * But we keep a dedicated pointer on purpose to explicitly express the dependency.
     * If, in the future, TypeFactory is, for some reason, no longer singleton,
     * nothing will change here.
     */
    TypeFactory* m_typeFactory;

    /** @brief RAII holder for all other expressions. */
    std::vector<std::unique_ptr<Expr>> m_otherExpressions;

    /** @brief Map for per-value unique IntLiteralExpr instances. */
    std::map<int, IntLiteralExpr> m_intLiterals;

    /** @brief Map for per-value unique CharLiteralExpr instances. */
    std::map<char, CharLiteralExpr> m_charLiterals;

    /**
     * @brief Map for per-value unique StringLiteralExpr instances.
     * Transparent std::less<> comparator to allow lookup by std::string_view and similar.
     */
    std::map<std::string, StringLiteralExpr, std::less<>> m_stringLiterals;

    /**
     * @brief Map for per-name unique LambdaExpr instances.
     * Transparent std::less<> comparator to allow lookup by std::string_view and similar.
     */
    std::map<std::string, LambdaExpr, std::less<>> m_lambdaExprs;

    /** @brief Single NullLiteralExpr instance. */
    NullLiteralExpr m_nullLiteral;

    /** @brief Single BoolLiteralExpr instance for value @c false. */
    BoolLiteralExpr m_falseLiteral{false};

    /** @brief Single BoolLiteralExpr instance for value @c true. */
    BoolLiteralExpr m_trueLiteral{true};

    /** @brief Single ThisExpr instance. */
    ThisExpr m_thisExpr;

    /** @brief Single BaseExpr instance. */
    BaseExpr m_baseExpr;

    /** @brief Single UnknownExpr instance. */
    UnknownExpr m_unknownExpr;
};

} // namespace astfri

#endif
