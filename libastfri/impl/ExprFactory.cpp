#include <libastfri/impl/Utils.hpp>
#include <libastfri/inc/ExprFactory.hpp>

namespace astfri
{

ExprFactory& ExprFactory::get_instance()
{
    static ExprFactory instance;
    return instance;
}

IntLiteralExpr* ExprFactory::mk_int_literal(int const val)
{
    return details::emplace_get<IntLiteralExpr>(val, ints_, val);
}

FloatLiteralExpr* ExprFactory::mk_float_literal(float val)
{
    return details::emplace_get<FloatLiteralExpr>(exprs_, val);
}

CharLiteralExpr* ExprFactory::mk_char_literal(char const val)
{
    return details::emplace_get<CharLiteralExpr>(val, chars_, val);
}

StringLiteralExpr* ExprFactory::mk_string_literal(std::string const& val)
{
    return details::emplace_get<StringLiteralExpr>(val, strings_, val);
}

BoolLiteralExpr* ExprFactory::mk_bool_literal(bool const val)
{
    return val ? &true_ : &false_;
}

NullLiteralExpr* ExprFactory::mk_null_literal()
{
    return &null_;
}

IfExpr* ExprFactory::mk_if(Expr* cond, Expr* iftrue, Expr* iffalse)
{
    return details::emplace_get<IfExpr>(exprs_, cond, iftrue, iffalse);
}

BinOpExpr* ExprFactory::mk_bin_on(Expr* left, BinOpType op, Expr* right)
{
    return details::emplace_get<BinOpExpr>(exprs_, left, op, right);
}

UnaryOpExpr* ExprFactory::mk_unary_op(UnaryOpType op, Expr* arg)
{
    return details::emplace_get<UnaryOpExpr>(exprs_, op, arg);
}

AssignExpr* ExprFactory::mk_assign(Expr* lhs, Expr* rhs)
{
    return details::emplace_get<AssignExpr>(exprs_, lhs, rhs);
}

CompoundAssignExpr* ExprFactory::mk_compound_assign(
    Expr* lhs,
    BinOpType op,
    Expr* rhs
)
{
    return details::emplace_get<CompoundAssignExpr>(exprs_, lhs, op, rhs);
}

ParamVarRefExpr* ExprFactory::mk_param_var_ref(std::string param)
{
    return details::emplace_get<ParamVarRefExpr>(exprs_, std::move(param));
}

LocalVarRefExpr* ExprFactory::mk_local_var_ref(std::string var)
{
    return details::emplace_get<LocalVarRefExpr>(exprs_, std::move(var));
}

MemberVarRefExpr* ExprFactory::mk_member_var_ref(std::string member)
{
    return details::emplace_get<MemberVarRefExpr>(exprs_, std::move(member));
}

GlobalVarRefExpr* ExprFactory::mk_global_var_ref(std::string global)
{
    return details::emplace_get<GlobalVarRefExpr>(exprs_, std::move(global));
}

ClassRefExpr* ExprFactory::mk_class_ref(std::string name)
{
    return details::emplace_get<ClassRefExpr>(exprs_, std::move(name));
}

FunctionCallExpr* ExprFactory::mk_function_call(
    std::string name,
    std::vector<Expr*> args
)
{
    return details::emplace_get<FunctionCallExpr>(
        exprs_,
        std::move(name),
        std::move(args)
    );
}

MethodCallExpr* ExprFactory::mk_method_call(
    Expr* owner,
    std::string name,
    std::vector<Expr*> args
)
{
    return details::emplace_get<MethodCallExpr>(
        exprs_,
        owner,
        std::move(name),
        std::move(args)
    );
}

LambdaExpr* ExprFactory::mk_lambda_expr(
    std::vector<ParamVarDefStmt*> params,
    Stmt* body
)
{
    return details::emplace_get<LambdaExpr>(exprs_, std::move(params), body);
}

ThisExpr* ExprFactory::mk_this()
{
    return &this_;
}

ConstructorCallExpr* ExprFactory::mk_constructor_call(
    Type* type,
    std::vector<Expr*> args
)
{
    return details::emplace_get<ConstructorCallExpr>(
        exprs_,
        type,
        std::move(args)
    );
}

NewExpr* ExprFactory::mk_new(ConstructorCallExpr* init)
{
    return details::emplace_get<NewExpr>(exprs_, init);
}

DeleteExpr* ExprFactory::mk_delete(Expr* arg)
{
    return details::emplace_get<DeleteExpr>(exprs_, arg);
}

UnknownExpr* ExprFactory::mk_unknown()
{
    return &unknown_;
}

} // namespace astfri
