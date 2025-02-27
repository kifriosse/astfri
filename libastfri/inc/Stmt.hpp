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
 * @brief Defines access modifier of class member
 */
enum class AccessModifier
{
    /**
     * Public
     */
    Public,

    /**
     * Private
     */
    Private,

    /**
     * Protected
     */
    Protected,

    /**
     * Internal -- package private | assembly private and similar
     */
    Internal
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
    AccessModifier access_;

    MemberVarDefStmt(
        std::string name,
        Type* type,
        Expr* initializer,
        AccessModifier access
    );
};

/**
 * @brief TODO
 */
struct GlobalVarDefStmt : VarDefStmt, details::MkVisitable<GlobalVarDefStmt>
{
    GlobalVarDefStmt(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief Definition statement that may contain multiple variable definitions
 * Covers the following situations:
 * @code
   int x = 10, y = 20;
 * @endcode
 * In this case, you would use:
 * @code
   DefStmt
   |-VarDefStmt
   | `-IntLiteralExpr(10)
   `-VarDefStmt
     `-IntLiteralExpr(10)
 * @endcode
 */
struct DefStmt : Stmt, details::MkVisitable<DefStmt>
{
    std::vector<VarDefStmt*> defs_ {};

    DefStmt() = default;
    explicit DefStmt(std::vector<VarDefStmt*> defs);
};

/**
 * @brief TODO
 */
struct FunctionDefStmt : Stmt, details::MkVisitable<FunctionDefStmt>
{
    std::string name_ {};
    std::vector<ParamVarDefStmt*> params_ {};
    Type* retType_ {nullptr};
    CompoundStmt* body_ {nullptr};

    FunctionDefStmt() = default;

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
    AccessModifier access_;

    MethodDefStmt(
        ClassDefStmt* owner,
        FunctionDefStmt* func,
        AccessModifier access
    );
};

/**
 * @brief Initializer of a base class called at the begining of a constructor
 */
struct BaseInitializerStmt : Stmt, details::MkVisitable<BaseInitializerStmt>
{
    std::string base_;
    std::vector<Expr*> args_;

    BaseInitializerStmt(std::string base, std::vector<Expr*> args);
};

/**
 * @brief TODO
 */
struct ConstructorDefStmt : Stmt, details::MkVisitable<ConstructorDefStmt>
{
    ClassDefStmt* owner_ {nullptr};
    std::vector<ParamVarDefStmt*> params_ {};
    std::vector<BaseInitializerStmt*> baseInit_ {};
    CompoundStmt* body_ {nullptr};
    AccessModifier access_ {AccessModifier::Public};

    ConstructorDefStmt() = default;

    ConstructorDefStmt(
        ClassDefStmt* owner,
        std::vector<ParamVarDefStmt*> params,
        std::vector<BaseInitializerStmt*> baseInit,
        CompoundStmt* body,
        AccessModifier access
    );
};

/**
 * @brief Definition of a descructor
 */
struct DestructorDefStmt : Stmt, details::MkVisitable<DestructorDefStmt>
{
    ClassDefStmt* owner_;
    CompoundStmt* body_;

    DestructorDefStmt(ClassDefStmt* owner, CompoundStmt* body);
};

/**
 * @brief TODO
 */
struct GenericParam : Stmt, details::MkVisitable<GenericParam>
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
    std::string name_ {};
    std::vector<MemberVarDefStmt*> vars_ {};
    std::vector<MethodDefStmt*> methods_ {};
    std::vector<GenericParam*> tparams_ {};

    ClassDefStmt() = default;

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

    explicit CompoundStmt(std::vector<Stmt*> stmts);
};

/**
 * @brief TODO
 */
struct ReturnStmt : Stmt, details::MkVisitable<ReturnStmt>
{
    Expr* val_;

    explicit ReturnStmt(Expr* val);
};

/**
 * @brief TODO
 */
struct ExprStmt : Stmt, details::MkVisitable<ExprStmt>
{
    Expr* expr_;

    explicit ExprStmt(Expr* expr);
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

    explicit ThrowStmt(Expr* val);
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
    std::vector<ClassDefStmt*> classes_ {};
    std::vector<FunctionDefStmt*> functions_ {};
    std::vector<GlobalVarDefStmt*> globals_ {};

    TranslationUnit() = default;

    TranslationUnit(
        std::vector<ClassDefStmt*> classes,
        std::vector<FunctionDefStmt*> functions,
        std::vector<GlobalVarDefStmt*> globals
    );
};
} // namespace astfri

#endif
