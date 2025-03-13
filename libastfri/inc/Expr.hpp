#ifndef LIBASTFRI_EXPRESSION_HPP
#define LIBASTFRI_EXPRESSION_HPP

#include <libastfri/impl/Utils.hpp>

#include <string>
#include <vector>

namespace astfri
{
/**
 * @brief TODO
 */
struct Expr : virtual IVisitable
{
    virtual ~Expr() = default;
};

/**
 * @brief TODO
 */
template<typename T>
struct Literal : Expr
{
    T val_;
    Literal(T val);
};

/**
 * @brief TODO
 */
struct IntLiteralExpr : Literal<int>, details::MkVisitable<IntLiteralExpr>
{
    explicit IntLiteralExpr(int val);
};

/**
 * @brief TODO
 */
struct FloatLiteralExpr : Literal<float>, details::MkVisitable<FloatLiteralExpr>
{
    explicit FloatLiteralExpr(float val);
};

/**
 * @brief TODO
 */
struct CharLiteralExpr : Literal<char>, details::MkVisitable<CharLiteralExpr>
{
    explicit CharLiteralExpr(char val);
};

/**
 * @brief TODO
 */
struct StringLiteralExpr :
    Literal<std::string>,
    details::MkVisitable<StringLiteralExpr>
{
    explicit StringLiteralExpr(std::string val);
};

/**
 * @brief TODO
 */
struct BoolLiteralExpr : Literal<bool>, details::MkVisitable<BoolLiteralExpr>
{
    explicit BoolLiteralExpr(bool val);
};

/**
 * @brief TODO
 */
struct NullLiteralExpr : Expr, details::MkVisitable<NullLiteralExpr>
{
};

/**
 * @brief TODO
 */
struct IfExpr : Expr, details::MkVisitable<IfExpr>
{
    Expr* cond_;
    Expr* iftrue_;
    Expr* iffalse_;

    IfExpr(Expr* cond, Expr* iftrue, Expr* iffalse);
};

/**
 * @brief List of binary operators
 */
enum class BinOpType
{
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
    BitXorAssign
};

/**
 * @brief TODO
 */
struct BinOpExpr : Expr, details::MkVisitable<BinOpExpr>
{
    Expr* left_;
    BinOpType op_;
    Expr* right_;

    BinOpExpr(Expr* left, BinOpType op, Expr* right);
};

/**
 * @brief List of unary operators
 */
enum class UnaryOpType
{
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
struct UnaryOpExpr : Expr, details::MkVisitable<UnaryOpExpr>
{
    UnaryOpType op_;
    Expr* arg_;

    UnaryOpExpr(UnaryOpType op, Expr* arg);
};

/**
 * @brief TODO
 */
struct ParamVarRefExpr : Expr, details::MkVisitable<ParamVarRefExpr>
{
    // TODO later this should be a pointer to ParamVarDef
    std::string param_;

    explicit ParamVarRefExpr(std::string param);
};

/**
 * @brief TODO
 */
struct LocalVarRefExpr : Expr, details::MkVisitable<LocalVarRefExpr>
{
    // TODO later this should be a pointer to LocalVarDef
    std::string var_;

    explicit LocalVarRefExpr(std::string var);
};

/**
 * @brief TODO
 */
struct MemberVarRefExpr : Expr, details::MkVisitable<MemberVarRefExpr>
{
    Expr* owner_;
    // TODO later this should be a pointer to MemberVarDef
    std::string member_;

    MemberVarRefExpr(Expr* expr, std::string member);
};

/**
 * @brief TODO
 */
struct GlobalVarRefExpr : Expr, details::MkVisitable<GlobalVarRefExpr>
{
    // TODO later this should be a pointer to GlobalVarDef
    std::string global_;

    explicit GlobalVarRefExpr(std::string global);
};

/**
 * @brief Reference to a class object
 * This type is mainly used as @c owner for static method calls
 */
struct ClassRefExpr : Expr, details::MkVisitable<ClassRefExpr>
{
    // TODO later this could point to ClassDefStmt
    std::string name_;

    ClassRefExpr(std::string name);
};

/**
 * @brief TODO
 */
struct FunctionCallExpr : Expr, details::MkVisitable<FunctionCallExpr>
{
    // TODO later this sould be a pointer to FunctionDecl
    std::string name_;
    std::vector<Expr*> args_;

    FunctionCallExpr(std::string name, std::vector<Expr*> args);
};

/**
 * @brief TODO
 */
struct MethodCallExpr : Expr, details::MkVisitable<MethodCallExpr>
{
    Expr* owner_;
    // TODO later this sould be a pointer to MethodDecl
    std::string name_;
    std::vector<Expr*> args_;

    MethodCallExpr(Expr* owner, std::string name, std::vector<Expr*> args);
};

/**
 * @brief TODO
 */
struct LambdaCallExpr : Expr, details::MkVisitable<LambdaCallExpr>
{
    Expr* lambda_{nullptr};
    std::vector<Expr*> args_;
};

/**
 * @brief TODO
 */
struct LambdaExpr : Expr, details::MkVisitable<LambdaExpr>
{
    std::vector<ParamVarDefStmt*> params_;
    Stmt* body_;

    LambdaExpr(std::vector<ParamVarDefStmt*> params, Stmt* body);
};

/**
 * @brief TODO
 */
struct ThisExpr : Expr, details::MkVisitable<ThisExpr>
{
};

/**
 * @brief Constructor call
 */
struct ConstructorCallExpr : Expr, details::MkVisitable<ConstructorCallExpr>
{
    Type* type_;
    std::vector<Expr*> args_;
    ConstructorCallExpr(Type* type, std::vector<Expr*> args);
};

/**
 * @brief Operator new that allocates and possibly initializes memory
 * If @c init is null, then the memory is just allocated for the type
 * and it is not initialized.
 */
struct NewExpr : Expr, details::MkVisitable<NewExpr>
{
    ConstructorCallExpr* init_;
    NewExpr(ConstructorCallExpr* init);
};

/**
 * @brief Operator delete that frees allocated memory
 */
struct DeleteExpr : Expr, details::MkVisitable<DeleteExpr>
{
    Expr* arg_;
    DeleteExpr(Expr* arg);
};

/**
 * @brief TODO
 */
struct UnknownExpr : Expr, details::MkVisitable<UnknownExpr>
{
};
} // namespace astfri

#include <libastfri/impl/Expr.inl>

#endif
