#ifndef LIBASTFRI_STATEMENT_HPP
#define LIBASTFRI_STATEMENT_HPP

#include <libastfri/impl/Utils.hpp>

#include <string>
#include <vector>

namespace astfri
{
/**
 * @brief TODO
 */
struct Stmt : virtual IVisitable
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
struct LocalVarDef : VarDef, details::MkVisitable<LocalVarDef>
{
    LocalVarDef(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct ParamVarDef : VarDef, details::MkVisitable<ParamVarDef>
{
    ParamVarDef(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct MemberVarDef : VarDef, details::MkVisitable<MemberVarDef>
{
    MemberVarDef(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct GlobalVarDef : VarDef, details::MkVisitable<GlobalVarDef>
{
    GlobalVarDef(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct FunctionDef : Stmt, details::MkVisitable<FunctionDef>
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
struct MethodDef : Stmt, details::MkVisitable<MethodDef>
{
    ClassDef* owner_;
    FunctionDef* func_;

    MethodDef(ClassDef* owner, FunctionDef* func);
};

/**
 * @brief TODO
 */
struct ClassDef : Stmt, details::MkVisitable<ClassDef>
{
    std::string name_;
    std::vector<MemberVarDef*> vars_;
    std::vector<GenericParam*> tparams_;

    ClassDef(
        std::string name,
        std::vector<MemberVarDef*> vars,
        std::vector<GenericParam*> tparams
    );
};

/**
 * @brief TODO
 */
struct GenericParam
{
    // TODO later, this could be pointer to a concept
    std::string constraint_;
    std::string name_;

    GenericParam(std::string constraint, std::string name);
};

/**
 * @brief TODO
 */
struct CompoundStmt : Stmt, details::MkVisitable<CompoundStmt>
{
    std::vector<Stmt*> stmts_;

    CompoundStmt(std::vector<Stmt*> stmts);
};

/**
 * @brief TODO
 */
struct ReturnStmt : Stmt, details::MkVisitable<ReturnStmt>
{
    Expr* val_;

    ReturnStmt(Expr* val);
};

/**
 * @brief TODO
 */
struct ExprStmt : Stmt, details::MkVisitable<ExprStmt>
{
    Expr* expr_;

    ExprStmt(Expr* expr);
};

/**
 * @brief TODO
 */
struct IfStmt : Stmt, details::MkVisitable<IfStmt>
{
    Expr* cond_;
    Stmt* iftrue_;
    Stmt* iffalse_;

    IfStmt(Expr* cond, Stmt* iftrue, Stmt* iffalse);
};

/**
 * @brief TODO
 */
struct CaseStmt : Stmt, details::MkVisitable<CaseStmt>
{
    Expr* expr_;
    Stmt* body_;

    CaseStmt(Expr* expr, Stmt* body);
};

/**
 * @brief TODO
 */
struct SwitchStmt : Stmt, details::MkVisitable<Stmt>
{
    Expr* expr_;
    std::vector<CaseStmt*> cases_;

    SwitchStmt(Expr* expr, std::vector<CaseStmt*> cases);
};

/**
 * @brief TODO
 */
struct LoopStmt : Stmt
{
    Expr* cond_;
    CompoundStmt* body_;

    LoopStmt(Expr* cond, CompoundStmt* body);
};

/**
 * @brief TODO
 */
struct WhileStmt : LoopStmt, details::MkVisitable<WhileStmt>
{
    WhileStmt(Expr* cond, CompoundStmt* body);
};

/**
 * @brief TODO
 */
struct DoWhileStmt : LoopStmt, details::MkVisitable<DoWhileStmt>
{
    DoWhileStmt(Expr* cond, CompoundStmt* body);
};

/**
 * @brief TODO
 */
struct ForStmt : LoopStmt, details::MkVisitable<ForStmt>
{
    Stmt* init_;
    Stmt* step_;

    ForStmt(Stmt* init, Expr* cond, Stmt* step, CompoundStmt* body);
};

/**
 * @brief TODO
 */
struct ThrowStmt : Stmt, details::MkVisitable<ThrowStmt>
{
    Expr* val_;

    ThrowStmt(Expr* val);
};

/**
 * @brief TODO
 */
struct UnknownStmt : Stmt, details::MkVisitable<UnknownStmt>
{
    std::string message_;

    UnknownStmt(std::string message);
};

/**
 * @brief TODO
 */
struct TranslationUnit : details::MkVisitable<TranslationUnit>
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