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
 * @brief Base class for expressions representing literal values of type T.
 */
template<typename SelfType, typename T>
struct Literal : MakeAnExpr<SelfType> {
    T val{};
};

/**
 * @brief Represents an integer literal expression.
 */
struct IntLiteralExpr : Literal<IntLiteralExpr, int> {
};

/**
 * @brief Represents a floating-point literal expression.
 */
struct FloatLiteralExpr : Literal<FloatLiteralExpr, float> {
};

/**
 * @brief Represents a character literal expression.
 */
struct CharLiteralExpr : Literal<CharLiteralExpr, char> {
};

/**
 * @brief Represents a string literal expression.
 */
struct StringLiteralExpr : Literal<StringLiteralExpr, std::string> {
};

/**
 * @brief Represents a boolean literal expression.
 */
struct BoolLiteralExpr : Literal<BoolLiteralExpr, bool> {
};

/**
 * @brief Represents a null literal expression.
 */
struct NullLiteralExpr : MakeAnExpr<NullLiteralExpr> {
};

/**
 * @brief Represents a conditional expression (ternary operator).
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
 * @brief Represents binary operator expression.
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
 * @brief Represents unary operator expression.
 */
struct UnaryOpExpr : MakeAnExpr<UnaryOpExpr> {
    Expr *arg{nullptr};
    UnaryOpType op{UnaryOpType::UNINITIALIZED};
};

/**
 * @brief Represents a reference to a function parameter.
 */
struct ParamVarRefExpr : MakeAnExpr<ParamVarRefExpr> {
    // TODO this should be Either<ReqPtr<Symbol>, ReqPtr<ParamVarDef>>
    std::string param;
};

/**
 * @brief Represents a reference to a local variable.
 */
struct LocalVarRefExpr : MakeAnExpr<LocalVarRefExpr> {
    // TODO this should be Either<Symbol, LocalVarDef>
    std::string var;
};

/**
 * @brief Represents a reference to a member variable of an object or class.
 */
struct MemberVarRefExpr : MakeAnExpr<MemberVarRefExpr> {
    Expr *owner{nullptr};
    // TODO this should be Either<Symbol, MemberVarDef>
    std::string member;
};

/**
 * @brief Represents a reference to a global variable.
 */
struct GlobalVarRefExpr : MakeAnExpr<GlobalVarRefExpr> {
    // TODO this should be Either<Symbol, GlobalVarDef>
    std::string global;
};

/**
 * @brief Represents a reference to a class object.
 *
 * This type is mainly used as @c owner for static method calls
 */
struct ClassRefExpr : MakeAnExpr<ClassRefExpr> {
    // TODO this should be Either<Symbol, ClassType>
    std::string name;
};

/**
 * @brief Represents a call to a standalone function.
 */
struct FunctionCallExpr : MakeAnExpr<FunctionCallExpr> {
    // TODO this should be Either<Symbol, FunctionDefStmt>
    std::string name;
    std::vector<Expr*> args;
};

/**
 * @brief Represents a call to a class or object method.
 */
struct MethodCallExpr : MakeAnExpr<MethodCallExpr> {
    Expr *owner{nullptr};
    // TODO this should be Either<Symbol, MethodDefStmt>
    std::string name;
    std::vector<Expr*> args{};
};

/**
 * @brief Represents an invocation of a lambda expression.
 */
struct LambdaCallExpr : MakeAnExpr<LambdaCallExpr> {
    Expr* lambda{nullptr};
    std::vector<Expr*> args{};
};

/**
 * @brief Represents the definition of a lambda expression.
 */
struct LambdaExpr : MakeAnExpr<LambdaExpr> {
    LambdaType *type{nullptr};
    Type *returnType{nullptr};
    Stmt *body{nullptr};
    std::vector<ParamVarDefStmt*> params{};
};

/**
 * @brief Represents a reference to the current instance e.g., `this` or `self` keywords.
 */
struct ThisExpr : MakeAnExpr<ThisExpr> {
};

/**
 * @brief Represents a reference to the base class e.g., `super` or `base` keywords.
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
 * @brief Operator delete that frees allocated memory.
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
 * @brief Represents an expression whose content or type is currently unknown.
 */
struct UnknownExpr : MakeAnExpr<UnknownExpr> { };


} // namespace astfri

#endif
