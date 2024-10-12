#ifndef LIBASTFRI_STATEMENT_HPP
#define LIBASTFRI_STATEMENT_HPP

#include <libastfri/Visitor.hpp>
#include <libastfri/impl/Utils.hpp>

#include <string>
#include <vector>

namespace astfri
{
struct Type;
struct Expr;

/**
 * @brief TODO
 */
struct Stmt
{
    virtual ~Stmt() = default;
};

/**
 * @brief TODO
 */
struct VarDef : Stmt
{
    std::string name_;
    Type* type_;
    Expr* initializer_;

    VarDef(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct LocalVarDef : VarDef, details::MakeVisitable<LocalVarDef>
{
    LocalVarDef(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct ParamVarDef : VarDef, details::MakeVisitable<ParamVarDef>
{
    ParamVarDef(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct MemberVarDef : VarDef, details::MakeVisitable<MemberVarDef>
{
    MemberVarDef(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct GlobalVarDef : VarDef, details::MakeVisitable<GlobalVarDef>
{
    GlobalVarDef(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct FunctionDef : Stmt, details::MakeVisitable<FunctionDef>
{
    std::string name_;
    std::vector<ParamVarDef*> params_;
    Type* retType_;
    CompoundStmt* body_;

    FunctionDef(
        std::string name,
        std::vector<ParamVarDef*> params,
        Type* retType,
        CompoundStmt* body
    );
};

/**
 * @brief TODO
 */
struct MethodDef : Stmt, details::MakeVisitable<MethodDef>
{
    ClassDef* owner_;
    FunctionDef* func_;

    MethodDef(
        ClassDef* owner,
        FunctionDef* func
    );
};

/**
 * @brief TODO
 */
struct ClassDef : Stmt, details::MakeVisitable<ClassDef>
{
    std::string name_;
    std::vector<MemberVarDef*> vars_;
    std::vector<std::string> tparams_;
};

/**
 * @brief TODO
 */
struct CompoundStmt : Stmt, details::MakeVisitable<CompoundStmt>
{
    std::vector<Stmt*> stmts_;

    CompoundStmt(std::vector<Stmt*> stmts);
};

/**
 * @brief TODO
 */
struct ReturnStmt : Stmt, details::MakeVisitable<ReturnStmt>
{
    Expr* val_;

    ReturnStmt(Expr* val);
};

/**
 * @brief TODO
 */
struct ExprStmt : Stmt, details::MakeVisitable<ExprStmt>
{
    Expr* expr_;

    ExprStmt(Expr* expression);
};

/**
 * @brief TODO
 */
struct IfStmt : Stmt, details::MakeVisitable<IfStmt>
{
    Expr* cond_;
    Stmt* iftrue_;
    Stmt* iffalse_;

    IfStmt(Expr* cond, Stmt* iftrue, Stmt* iffalse);
};


// TODO continue here


// Loop statement
struct LoopStatement : Stmt
{
    Expr* condition;
    CompoundStmt* body;

    LoopStatement(Expr* condition, CompoundStmt* body);
};

struct WhileStatement : LoopStatement, details::MakeVisitable<WhileStatement>
{
    WhileStatement(Expr* condition, CompoundStmt* body);
};

struct DoWhileStatement :
    LoopStatement,
    details::MakeVisitable<DoWhileStatement>
{
    DoWhileStatement(Expr* condition, CompoundStmt* body);
};

struct ForStatement : LoopStatement, details::MakeVisitable<ForStatement>
{
    Stmt* init;
    Expr* step;

    ForStatement(Stmt* init, Expr* condition, Expr* step, CompoundStmt* body);
};

struct UnknownStatement : Stmt, details::MakeVisitable<UnknownStatement>
{
    std::string message;

    UnknownStatement(std::string message);
};

/**
 * @brief TODO
 */
struct TranslationUnit : details::MakeVisitable<TranslationUnit>
{
    std::vector<ClassDef*> classes_;
    std::vector<FunctionDef*> functions_;
    std::vector<GlobalVarDef*> globals_;

    TranslationUnit(
        std::vector<ClassDef*> classes,
        std::vector<FunctionDef*> functions,
        std::vector<GlobalVarDef*> globals
    );
};
} // namespace astfri

#endif