#ifndef ASTFRI_IMPL_EXPR_DEF_HPP
#define ASTFRI_IMPL_EXPR_DEF_HPP

#include <astfri/impl/ASTNode.hpp>
#include <astfri/impl/ExprKind.hpp>

#include <string>
#include <vector>


namespace astfri {


/**
 * @brief Base for all expressions.
 */
struct Expr : ASTNode<ExprKind> {
};

/**
 * @brief Helper CRTP base. Indirectly inherits @c SelfType from @c Expr.
 *
 * See @c MakeA doc for more details.
 *
 * @tparam SelfType child class type.
 */
template<typename SelfType>
using MakeAnExpr = MakeA<Expr, SelfType>;

/**
 * @brief TODO
 */
template<typename SelfType, typename T>
struct Literal : MakeAnExpr<SelfType> {
    T val{};
};

/**
 * @brief TODO
 */
struct IntLiteralExpr : Literal<IntLiteralExpr, int> {
};

/**
 * @brief TODO
 */
struct FloatLiteralExpr : Literal<FloatLiteralExpr, float> {
};

/**
 * @brief TODO
 */
struct CharLiteralExpr : Literal<CharLiteralExpr, char> {
};

/**
 * @brief TODO
 */
struct StringLiteralExpr : Literal<StringLiteralExpr, std::string> {
};

/**
 * @brief TODO
 */
struct BoolLiteralExpr : Literal<BoolLiteralExpr, bool> {
};

/**
 * @brief TODO
 */
struct NullLiteralExpr : MakeAnExpr<NullLiteralExpr> {
};

/**
 * @brief TODO
 */
struct IfExpr : MakeAnExpr<IfExpr> {
    Expr *cond{nullptr};
    Expr *iftrue{nullptr};
    Expr *iffalse{nullptr};
};

/**
 * @brief List of binary operators.
 */
enum class BinOpType {
    UNINITIALIZED = 0,

    // {lhs} = {rhs}, {lhs} := {rhs}, {lhs} <- {rhs}
    Assign,

    // {lhs} + {rhs}
    Add,

    // {lhs} - {rhs}
    Subtract,

    // {lhs} * {rhs}
    Multiply,

    // {lhs} / {rhs}
    Divide,

    // {lhs} // {rhs}, {lhs} div {rhs}
    FloorDivide,

    // {lhs} % {rhs}, {lhs} mod {rhs}
    Modulo,

    // {lhs} ** {rhs}
    Exponentiation,

    // {lhs} == {rhs}
    Equal,

    // {lhs} != {rhs}, {lhs} <> {rhs}
    NotEqual,

    // {lhs} < {rhs}
    Less,

    // {lhs} <= {rhs}
    LessEqual,

    // {lhs} > {rhs}
    Greater,

    // {lhs} >= {rhs}
    GreaterEqual,

    // {lhs} && {rhs}
    LogicalAnd,

    // {lhs} || {rhs}
    LogicalOr,

    // {lhs} >> {rhs}
    BitShiftRight,

    // {lhs} >>> {rhs}
    BitShiftRightUnsigned,

    // {lhs} << {rhs}
    BitShiftLeft,

    // {lhs} & {rhs}
    BitAnd,

    // {lhs} | {rhs}
    BitOr,

    // {lhs} ^ {rhs}
    BitXor,

    // {lhs} , {rhs}
    Comma,

    // {lhs} += {rhs}
    AddAssign,

    // {lhs} -= {rhs}
    SubtractAssign,

    // {lhs} *= {rhs}
    MultiplyAssign,

    // {lhs} /= {rhs}
    DivideAssign,

    // {lhs} //= {rhs}
    FloorDivideAssign,

    // {lhs} %= {rhs}
    ModuloAssign,

    // {lhs} **= {rhs}
    ExponentiationAssign,

    // {lhs} >>= {rhs}
    BitShiftRightAssign,

    // {lhs} <<= {rhs}
    BitShiftLeftAssign,

    // {lhs} &= {rhs}
    BitAndAssign,

    // {lhs} |= {rhs}
    BitOrAssign,

    // {lhs} ^= {rhs}
    BitXorAssign,
};

/**
 * @brief TODO
 */
struct BinOpExpr : MakeAnExpr<BinOpExpr> {
    Expr *left{nullptr};
    Expr *right{nullptr};
    BinOpType op{BinOpType::UNINITIALIZED};
};

/**
 * @brief List of unary operators.
 */
enum class UnaryOpType {
    UNINITIALIZED = 0,

    // !arg, not arg
    LogicalNot,

    // -arg
    Minus,

    // +arg
    Plus,

    // *arg
    Dereference,

    // &arg
    AddressOf,

    // ++arg
    PreIncrement,

    // arg++
    PostIncrement,

    // --arg
    PreDecrement,

    // arg--
    PostDecrement,

    // ~arg
    BitFlip
};

/**
 * @brief TODO
 */
struct UnaryOpExpr : MakeAnExpr<UnaryOpExpr> {
    Expr *arg{nullptr};
    UnaryOpType op{UnaryOpType::UNINITIALIZED};
};

/**
 * @brief TODO
 */
struct ParamVarRefExpr : MakeAnExpr<ParamVarRefExpr> {
    // TODO this should be Either<ReqPtr<Symbol>, ReqPtr<ParamVarDef>>
    std::string param;
};

/**
 * @brief TODO
 */
struct LocalVarRefExpr : MakeAnExpr<LocalVarRefExpr> {
    // TODO this should be Either<Symbol, LocalVarDef>
    std::string var;
};

/**
 * @brief TODO
 */
struct MemberVarRefExpr : MakeAnExpr<MemberVarRefExpr> {
    Expr *owner{nullptr};
    // TODO this should be Either<Symbol, MemberVarDef>
    std::string member;
};

/**
 * @brief TODO
 */
struct GlobalVarRefExpr : MakeAnExpr<GlobalVarRefExpr> {
    // TODO this should be Either<Symbol, GlobalVarDef>
    std::string global;
};

/**
 * @brief Reference to a class object
 *
 * This type is mainly used as @c owner for static method calls
 */
struct ClassRefExpr : MakeAnExpr<ClassRefExpr> {
    // TODO this should be Either<Symbol, ClassType>
    std::string name;
};

/**
 * @brief TODO
 */
struct FunctionCallExpr : MakeAnExpr<FunctionCallExpr> {
    // TODO this should be Either<Symbol, FunctionDefStmt>
    std::string name;
    std::vector<Expr*> args;
};

/**
 * @brief TODO
 */
struct MethodCallExpr : MakeAnExpr<MethodCallExpr> {
    Expr *owner{nullptr};
    // TODO this should be Either<Symbol, MethodDefStmt>
    std::string name;
    std::vector<Expr*> args{};
};

/**
 * @brief TODO
 */
struct LambdaCallExpr : MakeAnExpr<LambdaCallExpr> {
    Expr* lambda{nullptr};
    std::vector<Expr*> args{};
};

/**
 * @brief TODO
 */
struct LambdaExpr : MakeAnExpr<LambdaExpr> {
    LambdaType *type{nullptr};
    Type *returnType{nullptr};
    Stmt *body{nullptr};
    std::vector<ParamVarDefStmt*> params{};
};

/**
 * @brief Reference to the current instance e.g., `this` or `self` keywords.
 */
struct ThisExpr : MakeAnExpr<ThisExpr> {
};

/**
 * @brief Reference to the base class e.g., `super` or `base` keywords.
 */
struct BaseExpr : MakeAnExpr<BaseExpr> {
};

/**
 * @brief Constructor call
 */
struct ConstructorCallExpr : MakeAnExpr<ConstructorCallExpr> {
    Type* type{nullptr};
    std::vector<Expr*> args{};
};

/**
 * @brief Operator new that allocates and initializes it using a constructor
 */
struct NewExpr : MakeAnExpr<NewExpr> {
    ConstructorCallExpr* init{nullptr};
};

/**
 * @brief Operator delete that frees allocated memory
 */
struct DeleteExpr : MakeAnExpr<DeleteExpr> {
    Expr* arg{nullptr};
};

/**
 * @brief Brackets prioritizing part of an expression.
 */
struct BracketExpr : MakeAnExpr<BracketExpr> {
    Expr* expr{nullptr};
};


// TODO
struct SwitchExpr
{
};

// TODO
struct Sink
{
    // _
};

/**
 * @brief TODO, tento bude moct ist prec, pozri UnknownType
 */
struct UnknownExpr : Expr, details::MkVisitable<UnknownExpr> { };


} // namespace astfri

#endif
