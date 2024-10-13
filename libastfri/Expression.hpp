#ifndef LIBASTFRI_EXPRESSION_HPP
#define LIBASTFRI_EXPRESSION_HPP

#include <libastfri/Visitor.hpp>
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
struct IntLiteral : Literal<int>, details::MkVisitable<IntLiteral>
{
    IntLiteral(int val);
};

/**
 * @brief TODO
 */
struct FloatLiteral : Literal<float>, details::MkVisitable<FloatLiteral>
{
    FloatLiteral(float val);
};

/**
 * @brief TODO
 */
struct CharLiteral : Literal<char>, details::MkVisitable<CharLiteral>
{
    CharLiteral(char val);
};

/**
 * @brief TODO
 */
struct StringLiteral : Literal<std::string>, details::MkVisitable<StringLiteral>
{
    StringLiteral(std::string val);
};

/**
 * @brief TODO
 */
struct BoolLiteral : Literal<bool>, details::MkVisitable<BoolLiteral>
{
    BoolLiteral(bool val);
};

/**
 * @brief TODO
 */
 struct NullLiteral : Expr, details::MkVisitable<NullLiteral>
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

    IfExpr(
        Expr* cond,
        Expr* iftrue,
        Expr* iffalse
    );
};

/**
 * @brief TODO
 */
enum class BinOpType
{
    Assign,
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual
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
 * @brief TODO
 */
enum class UnaryOpType
{
    LogicalNot,
    Minus,
    Plus,
    Dereference,
    AddressOf
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
struct AssignExpr : Expr, details::MkVisitable<AssignExpr>
{
    Expr* lhs_;
    Expr* rhs_;

    AssignExpr(Expr* lhs, Expr* rhs);
};

/**
 * @brief TODO
 */
struct CompoundAssignExpr : Expr, details::MkVisitable<CompoundAssignExpr>
{
    Expr* lhs_;
    Expr* rhs_;
    BinOpType op_;

    CompoundAssignExpr(Expr* lhs, Expr* rhs);
};

/**
 * @brief TODO
 */
struct RefExpr : Expr
{
};

/**
 * @brief TODO
 */
struct ParamVarRefExpr : RefExpr, details::MkVisitable<ParamVarRefExpr>
{
    // TODO later this should be a pointer to ParamVarDef
    std::string param_;

    ParamVarRefExpr(std::string param);
};

/**
 * @brief TODO
 */
struct LocalVarRefExpr : RefExpr, details::MkVisitable<LocalVarRefExpr>
{
    // TODO later this should be a pointer to LocalVarDef
    std::string var_;

    LocalVarRefExpr(std::string var);
};

/**
 * @brief TODO
 */
struct MemberVarRefExpr : RefExpr, details::MkVisitable<MemberVarRefExpr>
{
    // TODO later this should be a pointer to MemberVarDef
    std::string member_;

    MemberVarRefExpr(std::string member);
};

/**
 * @brief TODO
 */
struct FunctionCallExpr : RefExpr, details::MkVisitable<FunctionCallExpr>
{
    // TODO later this sould be a pointer to FunctionDecl
    std::string name_;
    std::vector<Expr*> args_;

    FunctionCallExpr(std::string name, std::vector<Expr*> args);
};

/**
 * @brief TODO
 */
struct MethodCallExpr : RefExpr, details::MkVisitable<MethodCallExpr>
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
struct UnknownExpr : Expr, details::MkVisitable<UnknownExpr>
{
    std::string message_;

    UnknownExpr(std::string message);
};
} // namespace astfri

#include <libastfri/impl/Expression.inl>

#endif