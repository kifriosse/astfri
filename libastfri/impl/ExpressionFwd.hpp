#ifndef LIBASTFRI_IMPL_EXPRESSION_FWD_HPP
#define LIBASTFRI_IMPL_EXPRESSION_FWD_HPP

namespace astfri
{
struct Expr;
struct IntLiteralExpr;
struct FloatLiteralExpr;
struct CharLiteralExpr;
struct StringLiteralExpr;
struct BoolLiteralExpr;
struct NullLiteralExpr;
struct IfExpr;
struct BinOpExpr;
struct UnaryOpExpr;
struct AssignExpr;
struct CompoundAssignExpr;
struct ParamVarRefExpr;
struct LocalVarRefExpr;
struct MemberVarRefExpr;
struct FunctionCallExpr;
struct MethodCallExpr;
struct LambdaExpr;
struct ThisExpr;
struct UnknownExpr;
}

#endif