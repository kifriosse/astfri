#ifndef ASTFRI_IMPL_EXPR_DEF_HPP
#define ASTFRI_IMPL_EXPR_DEF_HPP

#include <astfri/impl/ASTNode.hpp>
#include <astfri/impl/ExprFwd.hpp>
#include <astfri/impl/StmtFwd.hpp>
#include <astfri/impl/TypeFwd.hpp>
#include <astfri/impl/TypeInfo.hpp>
#include <astfri/impl/Visitor.hpp>

#include <string>
#include <vector>


namespace astfri {


/**
 * @brief Base for all expressions.
 */
struct Expr : ASTNode {
};
ASTFRI_REGISTER_TYPE(Expr,ASTNode)


/**
 * @brief Base class for expressions representing literal values.
 */
struct LiteralExpr : Expr {
};
ASTFRI_REGISTER_TYPE(LiteralExpr,Expr)


/**
 * @brief Represents an integer literal expression.
 */
struct IntLiteralExpr : LiteralExpr {
    int val;

    IntLiteralExpr();
};
ASTFRI_REGISTER_TYPE(IntLiteralExpr,LiteralExpr)
ASTFRI_ADD_NODE_CONSTRUCTOR(IntLiteralExpr)


/**
 * @brief Represents a floating-point literal expression.
 */
struct FloatLiteralExpr : LiteralExpr {
    float val;

    FloatLiteralExpr();
};
ASTFRI_REGISTER_TYPE(FloatLiteralExpr,LiteralExpr)
ASTFRI_ADD_NODE_CONSTRUCTOR(FloatLiteralExpr)


/**
 * @brief Represents a character literal expression.
 */
struct CharLiteralExpr : LiteralExpr {
    char val;

    CharLiteralExpr();
};
ASTFRI_REGISTER_TYPE(CharLiteralExpr,LiteralExpr)
ASTFRI_ADD_NODE_CONSTRUCTOR(CharLiteralExpr)

/**
 * @brief Represents a string literal expression.
 */
struct StringLiteralExpr : LiteralExpr {
    std::string val;

    StringLiteralExpr();
};
ASTFRI_REGISTER_TYPE(StringLiteralExpr,LiteralExpr)
ASTFRI_ADD_NODE_CONSTRUCTOR(StringLiteralExpr)


/**
 * @brief Represents a boolean literal expression.
 */
struct BoolLiteralExpr : LiteralExpr {
    bool val;

    BoolLiteralExpr();
};
ASTFRI_REGISTER_TYPE(BoolLiteralExpr,LiteralExpr)
ASTFRI_ADD_NODE_CONSTRUCTOR(BoolLiteralExpr)


/**
 * @brief Represents a null literal expression.
 */
struct NullLiteralExpr : LiteralExpr {
    NullLiteralExpr();
};
ASTFRI_REGISTER_TYPE(NullLiteralExpr,LiteralExpr)
ASTFRI_ADD_NODE_CONSTRUCTOR(NullLiteralExpr)


/**
 * @brief Represents a conditional expression (ternary operator).
 */
struct IfExpr : Expr {
    Expr *cond{nullptr};
    Expr *iftrue{nullptr};
    Expr *iffalse{nullptr};

    IfExpr();
};
ASTFRI_REGISTER_TYPE(IfExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(IfExpr)


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

    // {lhs} >>>= {rhs}
    BitShiftRightUnsignedAssign,

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
struct BinOpExpr : Expr {
    Expr *left{nullptr};
    Expr *right{nullptr};
    BinOpType op{BinOpType::UNINITIALIZED};

    BinOpExpr();
};
ASTFRI_REGISTER_TYPE(BinOpExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(BinOpExpr)


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
struct UnaryOpExpr : Expr {
    Expr *arg{nullptr};
    UnaryOpType op{UnaryOpType::UNINITIALIZED};

    UnaryOpExpr();
};
ASTFRI_REGISTER_TYPE(UnaryOpExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(UnaryOpExpr)


/**
 * @brief Represents a reference to a function parameter.
 */
struct ParamVarRefExpr : Expr {
    // TODO this should be SumPtr<Symbol, ParamVarDef>
    std::string param;

    ParamVarRefExpr();
};
ASTFRI_REGISTER_TYPE(ParamVarRefExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(ParamVarRefExpr)


/**
 * @brief Represents a reference to a local variable.
 */
struct LocalVarRefExpr : Expr {
    // TODO this should be SumPtr<Symbol, LocalVarDef>
    std::string var;

    LocalVarRefExpr();
};
ASTFRI_REGISTER_TYPE(LocalVarRefExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(LocalVarRefExpr)


/**
 * @brief Represents a reference to a member variable of an object or class.
 */
struct MemberVarRefExpr : Expr {
    Expr *owner{nullptr};
    // TODO this should be SumPtr<Symbol, MemberVarDef>
    std::string member;

    MemberVarRefExpr();
};
ASTFRI_REGISTER_TYPE(MemberVarRefExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(MemberVarRefExpr)


/**
 * @brief Represents a reference to a global variable.
 */
struct GlobalVarRefExpr : Expr {
    // TODO this should be SumPtr<Symbol, GlobalVarDef>
    std::string global;

    GlobalVarRefExpr();
};
ASTFRI_REGISTER_TYPE(GlobalVarRefExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(GlobalVarRefExpr)


/**
 * @brief Represents a reference to a class object.
 *
 * This type is mainly used as @c owner for static method calls
 */
struct ClassRefExpr : Expr {
    // TODO this should be SumPtr<Symbol, ClassType>
    std::string name;

    ClassRefExpr();
};
ASTFRI_REGISTER_TYPE(ClassRefExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(ClassRefExpr)


/**
 * @brief Represents a call to a standalone function.
 */
struct FunctionCallExpr : Expr {
    // TODO this should be SumPtr<Symbol, FunctionDefStmt>
    std::string name;
    std::vector<Expr*> args;

    FunctionCallExpr();
};
ASTFRI_REGISTER_TYPE(FunctionCallExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(FunctionCallExpr)


/**
 * @brief Represents a call to a class or object method.
 */
struct MethodCallExpr : Expr {
    Expr *owner{nullptr}; // TODO addressee
    // TODO this should be SumPtr<Symbol, MethodDefStmt>
    std::string name;
    std::vector<Expr*> args{};

    MethodCallExpr();
};
ASTFRI_REGISTER_TYPE(MethodCallExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(MethodCallExpr)


/**
 * @brief Represents an invocation of a lambda expression.
 */
struct LambdaCallExpr : Expr {
    Expr* lambda{nullptr};
    std::vector<Expr*> args{};

    LambdaCallExpr();
};
ASTFRI_REGISTER_TYPE(LambdaCallExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(LambdaCallExpr)


/**
 * @brief Represents the definition of a lambda expression.
 */
struct LambdaExpr : Expr{
    LambdaType *type{nullptr};
    Type *returnType{nullptr};
    Stmt *body{nullptr};
    std::vector<ParamVarDefStmt*> params{};

    LambdaExpr();
};
ASTFRI_REGISTER_TYPE(LambdaExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(LambdaExpr)


/**
 * @brief Represents a reference to the current instance e.g., `this` or `self` keywords.
 */
struct ThisExpr : Expr {

    ThisExpr();
};
ASTFRI_REGISTER_TYPE(ThisExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(ThisExpr)


/**
 * @brief Represents a reference to the base class e.g., `super` or `base` keywords.
 */
struct BaseExpr : Expr {
    BaseExpr();
};
ASTFRI_REGISTER_TYPE(BaseExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(BaseExpr)


/**
 * @brief Constructor call
 */
struct ConstructorCallExpr : Expr {
    Type* type{nullptr};
    std::vector<Expr*> args{};

    ConstructorCallExpr();
};
ASTFRI_REGISTER_TYPE(ConstructorCallExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(ConstructorCallExpr)


/**
 * @brief Operator new that allocates and initializes it using a constructor
 */
struct NewExpr : Expr {
    ConstructorCallExpr* init{nullptr};

    NewExpr();
};
ASTFRI_REGISTER_TYPE(NewExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(NewExpr)


/**
 * @brief Operator delete that frees allocated memory.
 */
struct DeleteExpr : Expr {
    Expr* arg{nullptr};

    DeleteExpr();
};
ASTFRI_REGISTER_TYPE(DeleteExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(DeleteExpr)


/**
 * @brief Brackets prioritizing part of an expression.
 */
struct BracketExpr : Expr {
    Expr* expr{nullptr};

    BracketExpr();
};
ASTFRI_REGISTER_TYPE(BracketExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(BracketExpr)


/**
 * @brief Expression associated with a specific parameter name in CallExpression.
 *
 * C# example:
 * @code{.cs}
 *  void CreateUser(string username, int age, bool admin)
 *  { ... }
 * CreateUser(username: "alice", age: 30, isAdmin: true);
 * @endcode
 */
struct NamedExpr : Expr {
    std::string name{};
    Expr* expr{nullptr};

    NamedExpr();
};
ASTFRI_REGISTER_TYPE(NamedExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(NamedExpr)


// TODO
struct SwitchExpr // : Expr
{
    // ASTFRI_ADD_NODE_CONSTRUCTOR(SwitchExpr)
};


// TODO
struct SinkExpr // : Expr
{
    // ASTFRI_ADD_NODE_CONSTRUCTOR(Sink)
    // _
};


/**
 * @brief Represents an expression whose content or type is currently unknown.
 */
struct UnknownExpr : Expr {
    UnknownExpr();
};
ASTFRI_REGISTER_TYPE(UnknownExpr,Expr)
ASTFRI_ADD_NODE_CONSTRUCTOR(UnknownExpr)


} // namespace astfri

#endif
