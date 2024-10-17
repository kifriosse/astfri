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
struct VarDefStmt : Stmt
{
    std::string name_;
    Type* type_;
    Expr* initializer_;

    VarDefStmt(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct LocalVarDefStmt : VarDefStmt, details::MkVisitable<LocalVarDefStmt>
{
    LocalVarDefStmt(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct ParamVarDefStmt : VarDefStmt, details::MkVisitable<ParamVarDefStmt>
{
    ParamVarDefStmt(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct MemberVarDefStmt : VarDefStmt, details::MkVisitable<MemberVarDefStmt>
{
    MemberVarDefStmt(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct GlobalVarDefStmt : VarDefStmt, details::MkVisitable<GlobalVarDefStmt>
{
    GlobalVarDefStmt(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct FunctionDefStmt : Stmt, details::MkVisitable<FunctionDefStmt>
{
    std::string name_;
    std::vector<ParamVarDefStmt*> params_;
    Type* retType_;
    CompoundStmt* body_;

    FunctionDefStmt(
        std::string name,
        std::vector<ParamVarDefStmt*> params,
        Type* retType,
        CompoundStmt* body
    );
};

/**
 * @brief TODO
 */
struct MethodDefStmt : Stmt, details::MkVisitable<MethodDefStmt>
{
    ClassDefStmt* owner_;
    FunctionDefStmt* func_;

    MethodDefStmt(ClassDefStmt* owner, FunctionDefStmt* func);
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
struct ClassDefStmt : Stmt, details::MkVisitable<ClassDefStmt>
{
    std::string name_;
    std::vector<MemberVarDefStmt*> vars_;
    std::vector<MethodDefStmt*> methods_;
    std::vector<GenericParam*> tparams_;

    ClassDefStmt(
        std::string name,
        std::vector<MemberVarDefStmt*> vars,
        std::vector<MethodDefStmt*> methods,
        std::vector<GenericParam*> tparams
    );
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
struct SwitchStmt : Stmt, details::MkVisitable<SwitchStmt>
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
};

/**
 * @brief TODO
 */
struct TranslationUnit : Stmt, details::MkVisitable<TranslationUnit>
{
    std::vector<ClassDefStmt*> classes_;
    std::vector<FunctionDefStmt*> functions_;
    std::vector<GlobalVarDefStmt*> globals_;

    TranslationUnit(
        std::vector<ClassDefStmt*> classes,
        std::vector<FunctionDefStmt*> functions,
        std::vector<GlobalVarDefStmt*> globals
    );
};
} // namespace astfri

#endif