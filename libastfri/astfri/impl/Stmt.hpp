#ifndef ASTFRI_IMPL_STMT_HPP
#define ASTFRI_IMPL_STMT_HPP

#include <astfri/impl/tools/Ptr.hpp>
#include <astfri/impl/Scope.hpp>
#include <astfri/impl/Utils.hpp>
#include <astfri/impl/Type.hpp>

#include <string>
#include <vector>

namespace astfri {

/**
 * @brief Base class for all statements.
 */
struct Stmt : virtual Visitable {
    /**
     * @brief Virtual destructor.
     * Statements are simple structs with no virtual functions.
     * Having the virtual destructor add vtable which allows us to use dynamic_cast.
     * If we move to our own type info later, the destructor may be removed.
     */
    virtual ~Stmt() = default;
};

/**
 * @brief Defines access modifier of a class member.
 */
enum class AccessModifier {
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
 * @brief Marks method as virtual or not virtual.
 */
enum class Virtuality {
    NotVirtual,
    Virtual,
    PureVirtual
};
// TODO Abstractity?

/**
 * @brief Marks a member either static or non-static.
 */
enum class Staticity {
    NonStatic,
    Static
};

/**
 * @brief TODO
 */
struct VarDefStmt :
    Stmt // TODO visitable here?
{
    std::string name;
    Type* type;
    Expr* initializer;

    VarDefStmt(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct LocalVarDefStmt : VarDefStmt, details::MkVisitable<LocalVarDefStmt> {
    LocalVarDefStmt(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct ParamVarDefStmt : VarDefStmt, details::MkVisitable<ParamVarDefStmt> {
    ParamVarDefStmt(std::string name, Type* type, Expr* initializer);
};

/**
 * @brief TODO
 */
struct MemberVarDefStmt : VarDefStmt, details::MkVisitable<MemberVarDefStmt> {
    AccessModifier access;
    Staticity staticity;

    MemberVarDefStmt(
        std::string name,
        Type* type,
        Expr* initializer,
        AccessModifier access,
        Staticity staticity);
};

/**
 * @brief TODO
 */
struct GlobalVarDefStmt : VarDefStmt, details::MkVisitable<GlobalVarDefStmt> {
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
   |-LocalVarDefStmt
   | `-IntLiteralExpr(10)
   `-LocalVarDefStmt
     `-IntLiteralExpr(10)
 * @endcode
 */
struct DefStmt : Stmt, details::MkVisitable<DefStmt> { // TODO this needs a better name, this is terrible
    std::vector<VarDefStmt*> defs;

    explicit DefStmt(std::vector<VarDefStmt*> defs);
};

/**
 * @brief TODO
 */
struct FunctionDefStmt : Stmt, details::MkVisitable<FunctionDefStmt> {
    std::string name;
    std::vector<ParamVarDefStmt*> params;
    Type* retType;
    CompoundStmt* body;

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
struct MethodDefStmt : Stmt, details::MkVisitable<MethodDefStmt> {
    UserTypeDefStmt* owner{nullptr}; // TODO ClassType or Interface, variant?
    FunctionDefStmt* func{nullptr};
    AccessModifier access{AccessModifier::Public};
    Virtuality virtuality{Virtuality::NotVirtual};
    Staticity staticity;
};

/**
 * @brief Initializer of a base class called at the begining of a constructor
 */
struct BaseInitializerStmt : Stmt, details::MkVisitable<BaseInitializerStmt> {
    [[deprecated]] std::string base_; // TODO type
    ClassType* type;
    std::vector<Expr*> args;

    [[deprecated]] BaseInitializerStmt(std::string base, std::vector<Expr*> args);
    BaseInitializerStmt(ClassType* type, std::vector<Expr*> args);
};

/**
 * @brief TODO
 */
struct SelfInitializerStmt : Stmt, details::MkVisitable<SelfInitializerStmt> {
    std::vector<Expr*> args;
    SelfInitializerStmt(std::vector<Expr*> args);
};

/**
 * @brief TODO
 */
struct MemberInitializerStmt : Stmt, details::MkVisitable<MemberInitializerStmt> {
    MemberVarDefStmt* member;
    Expr* arg;
    MemberInitializerStmt(MemberVarDefStmt* member, Expr* arg);
};

/**
 * @brief TODO
 */
struct ConstructorDefStmt : Stmt, details::MkVisitable<ConstructorDefStmt> {
    ClassDefStmt* owner;
    std::vector<ParamVarDefStmt*> params;
    std::vector<BaseInitializerStmt*> baseInit;
    std::vector<SelfInitializerStmt*> selfInitializers;
    std::vector<MemberInitializerStmt*> memberInitializers;
    CompoundStmt* body;
    AccessModifier access;

    ConstructorDefStmt();
    [[deprecated]] ConstructorDefStmt(
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
struct DestructorDefStmt : Stmt, details::MkVisitable<DestructorDefStmt> {
    ClassDefStmt* owner;
    CompoundStmt* body;

    DestructorDefStmt(ClassDefStmt* owner, CompoundStmt* body);
};

/**
 * @brief TODO
 */
struct GenericParam : Stmt, details::MkVisitable<GenericParam> {
    // TODO later, this could be pointer to a concept
    std::string constraint;
    std::string name;

    GenericParam(std::string constraint, std::string name);
};

/**
 * @deprecated
 * @brief Common base for Class and Interface
 * In the future, it could also be used for union or strong type alias
 */
struct UserTypeDefStmt : Stmt {
    [[deprecated]] std::string name;
};

/**
 * @brief TODO
 */
struct InterfaceDefStmt : UserTypeDefStmt, details::MkVisitable<InterfaceDefStmt> {
    InterfaceType* type;
    std::vector<MethodDefStmt*> methods;
    std::vector<GenericParam*> tparams;
    std::vector<InterfaceDefStmt*> bases;
};

/**
 * @brief TODO
 */
struct ClassDefStmt : UserTypeDefStmt, details::MkVisitable<ClassDefStmt> {
    ClassType* type;
    std::vector<MemberVarDefStmt*> vars;
    std::vector<ConstructorDefStmt*> constructors;
    std::vector<DestructorDefStmt*> destructors;
    std::vector<MethodDefStmt*> methods;
    std::vector<GenericParam*> tparams;
    std::vector<InterfaceDefStmt*> interfaces; // TODO IntefaceType
    std::vector<ClassDefStmt*> bases;          // TODO ClassType
    // TODO incomplete bases
};

/**
 * @brief TODO
 */
struct CompoundStmt : Stmt, details::MkVisitable<CompoundStmt> {
    std::vector<Stmt*> stmts;

    explicit CompoundStmt(std::vector<Stmt*> stmts);
};

/**
 * @brief TODO
 */
struct ReturnStmt : Stmt, details::MkVisitable<ReturnStmt> {
    Expr* val;

    explicit ReturnStmt(Expr* val);
};

/**
 * @brief TODO
 */
struct ExprStmt : Stmt, details::MkVisitable<ExprStmt> {
    Expr* expr;

    explicit ExprStmt(Expr* expr);
};

/**
 * @brief TODO
 */
struct IfStmt : Stmt, details::MkVisitable<IfStmt> {
    Expr *cond;
    Stmt *iftrue;
    Stmt *iffalse;
    // ReqPtr<Expr> cond;
    // ReqPtr<Stmt> iftrue;
    // OptPtr<Stmt> iffalse;

    IfStmt(Expr* cond, Stmt* iftrue, Stmt* iffalse);
};

/**
 * @brief Base for case options in switch statement
 */
struct CaseBaseStmt :
    Stmt // variant alebo common base? rovnako pri method ownerovi
{
    Stmt* body;

    explicit CaseBaseStmt(Stmt* body);
};

/**
 * @brief Case option in switch statement
 */
struct CaseStmt : CaseBaseStmt, details::MkVisitable<CaseStmt> {
    std::vector<Expr*> exprs;

    CaseStmt(std::vector<Expr*> exprs, Stmt* body);
};

/**
 * @brief Default option in switch statement
 */
struct DefaultCaseStmt : CaseBaseStmt, details::MkVisitable<DefaultCaseStmt> {
    explicit DefaultCaseStmt(Stmt* body);
};

/**
 * @brief TODO
 */
struct SwitchStmt : Stmt, details::MkVisitable<SwitchStmt> {
    Expr* expr_;
    std::vector<CaseBaseStmt*> cases;

    SwitchStmt(Expr* expr, std::vector<CaseBaseStmt*> cases);
};

/**
 * @brief TODO
 */
struct LoopStmt : Stmt {
    Expr* cond;
    Stmt* body;

    LoopStmt(Expr* cond, Stmt* body);
};

/**
 * @brief TODO
 */
struct WhileStmt : LoopStmt, details::MkVisitable<WhileStmt> {
    WhileStmt(Expr* cond, Stmt* body);
};

/**
 * @brief TODO
 */
struct DoWhileStmt : LoopStmt, details::MkVisitable<DoWhileStmt> {
    DoWhileStmt(Expr* cond, Stmt* body);
};

/**
 * @brief TODO
 */
struct ForStmt : LoopStmt, details::MkVisitable<ForStmt> {
    Stmt* init;
    Stmt* step;

    ForStmt(Stmt* init, Expr* cond, Stmt* step, Stmt* body);
};

/**
 * @brief TODO
 */
struct ForEachStmt : Stmt, details::MkVisitable<ForEachStmt> {
    LocalVarDefStmt* var;
    Expr* container;
    Stmt* body;
    ForEachStmt(LocalVarDefStmt* var, Expr* container, Stmt* body);
};

/**
 * @brief TODO
 */
struct ThrowStmt : Stmt, details::MkVisitable<ThrowStmt> {
    Expr* val;

    explicit ThrowStmt(Expr* val);
};

/**
 * @brief TODO
 */
struct CatchStmt : Stmt, details::MkVisitable<CatchStmt> {
    LocalVarDefStmt* param;
    Stmt* body;
    CatchStmt(LocalVarDefStmt* param, Stmt* body);
};

/**
 * @brief TODO
 */
struct TryStmt : Stmt, details::MkVisitable<TryStmt> {
    Stmt* body;
    Stmt* finally;
    std::vector<CatchStmt*> catches;
    TryStmt(Stmt* body, Stmt* finally, std::vector<CatchStmt*> catches);
};

/**
 * @brief Break statement used to terminate a loop
 */
struct BreakStmt : Stmt, details::MkVisitable<BreakStmt> { };

/**
 * @brief Continue statement used to skip rest of a loop
 */
struct ContinueStmt : Stmt, details::MkVisitable<ContinueStmt> { };

/**
 * @brief TODO
 */
struct UnknownStmt : Stmt, details::MkVisitable<UnknownStmt> { };

/**
 * @brief TODO
 */
struct TranslationUnit : Stmt, details::MkVisitable<TranslationUnit> {
    std::vector<ClassDefStmt*> classes;
    std::vector<InterfaceDefStmt*> interfaces;
    std::vector<FunctionDefStmt*> functions;
    std::vector<GlobalVarDefStmt*> globals;
};

} // namespace astfri

#endif
