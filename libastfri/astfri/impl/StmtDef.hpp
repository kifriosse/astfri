#ifndef ASTFRI_IMPL_STMT_DEF_HPP
#define ASTFRI_IMPL_STMT_DEF_HPP

#include <astfri/impl/tools/ReqPtr.hpp>
#include <astfri/impl/tools/OptPtr.hpp>
#include <astfri/impl/ASTNode.hpp>
#include <astfri/impl/StmtKind.hpp>
#include <astfri/impl/Type.hpp>

#include <string>
#include <vector>


namespace astfri {


/**
 * @brief Base for all statements.
 */
struct Stmt : virtual ASTNode<StmtKind> {
};


/**
 * @brief Helper CRTP base. Indirectly inherits @c SelfType from @c Stmt.
 *
 * See @c MakeA doc for more details.
 *
 * @tparam SelfType child class type.
 */
template<typename SelfType>
using MakeAStmt = MakeA<Stmt, SelfType>;


/**
 * @brief Defines access modifier of a class member.
 */
enum class AccessModifier {
    UNINITIALIZED = 0,

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
    UNINITIALIZED = 0,
    NotVirtual,
    Virtual,
    PureVirtual
};


/**
 * @brief Marks a member either static or non-static.
 */
enum class Staticity {
    UNINITIALIZED = 0,
    NonStatic,
    Static
};


/**
 * @brief TODO
 */
template<typename SelfType>
struct VarDefStmt : MakeAStmt<SelfType> {
    std::string name;
    Type* type;
    Expr* initializer;
};

/**
 * @brief TODO
 */
struct LocalVarDefStmt : VarDefStmt<LocalVarDefStmt> {
};


/**
 * @brief TODO
 */
struct ParamVarDefStmt : VarDefStmt<ParamVarDefStmt> {
    ParamVarDefStmt(std::string name, Type* type, Expr* initializer);
};


/**
 * @brief TODO
 */
struct MemberVarDefStmt : VarDefStmt<MemberVarDefStmt> {
    AccessModifier access;
    Staticity staticity;
};


/**
 * @brief TODO
 */
struct GlobalVarDefStmt : VarDefStmt<GlobalVarDefStmt> {
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
struct MultiLocalVarDefStmt : MakeAStmt<MultiLocalVarDefStmt> {
    std::vector<LocalVarDefStmt*> defs{};
};


/**
 * @brief TODO
 */
struct FunctionDefStmt : MakeAStmt<FunctionDefStmt> {
    std::string name{};
    std::vector<ParamVarDefStmt*> params{};
    Type* retType{nullptr};
    CompoundStmt* body{nullptr};
};


/**
 * @brief TODO
 */
struct MethodDefStmt : MakeAStmt<MethodDefStmt> {
    UserTypeDefStmt* owner{nullptr}; // TODO ClassType or Interface, variant?
    FunctionDefStmt* func{nullptr};
    AccessModifier access{AccessModifier::Public};
    Virtuality virtuality{Virtuality::NotVirtual};
    Staticity staticity;
};


/**
 * @brief Initializer of a base class called at the begining of a constructor
 */
struct BaseInitializerStmt : MakeAStmt<BaseInitializerStmt> {
    [[deprecated]] std::string base_{}; // TODO type
    ClassType* type{nullptr};
    std::vector<Expr*> args{};
};


/**
 * @brief TODO
 */
struct SelfInitializerStmt : MakeAStmt<SelfInitializerStmt> {
    std::vector<Expr*> args{};
};


/**
 * @brief TODO
 */
struct MemberInitializerStmt : MakeAStmt<MemberInitializerStmt> {
    MemberVarDefStmt* member{nullptr};
    Expr* arg{nullptr};
};


/**
 * @brief TODO
 */
struct ConstructorDefStmt : MakeAStmt<ConstructorDefStmt> {
    ClassDefStmt* owner{nullptr};
    std::vector<ParamVarDefStmt*> params{};
    std::vector<BaseInitializerStmt*> baseInit{};
    std::vector<SelfInitializerStmt*> selfInitializers{};
    std::vector<MemberInitializerStmt*> memberInitializers{};
    CompoundStmt* body{nullptr};
    AccessModifier access{AccessModifier::UNINITIALIZED};
};


/**
 * @brief Definition of a descructor
 */
struct DestructorDefStmt : MakeAStmt<DestructorDefStmt> {
    ClassDefStmt* owner{nullptr};
    CompoundStmt* body{nullptr};
};


/**
 * @brief TODO
 */
struct GenericParam : MakeAStmt<GenericParam> {
    // TODO later, this could be pointer to a concept
    std::string constraint{};
    std::string name{};
};


/**
 * @deprecated
 * @brief Common base for Class and Interface
 * In the future, it could also be used for union or strong type alias
 */
template<typename SelfType>
struct UserTypeDefStmt : MakeAStmt<SelfType> {
    [[deprecated]] std::string name{};
};

/**
 * @brief TODO
 */
struct InterfaceDefStmt : UserTypeDefStmt<InterfaceDefStmt> {
    InterfaceType* type{nullptr};
    std::vector<MethodDefStmt*> methods{};
    std::vector<GenericParam*> tparams{};
    std::vector<InterfaceDefStmt*> bases{};
};

/**
 * @brief TODO
 */
struct ClassDefStmt : UserTypeDefStmt<ClassDefStmt> {
    ClassType* type{nullptr};
    std::vector<MemberVarDefStmt*> vars{};
    std::vector<ConstructorDefStmt*> constructors{};
    std::vector<DestructorDefStmt*> destructors{};
    std::vector<MethodDefStmt*> methods{};
    std::vector<GenericParam*> tparams{};
    std::vector<InterfaceDefStmt*> interfaces{}; // TODO IntefaceType
    std::vector<ClassDefStmt*> bases{};          // TODO ClassType
    // TODO incomplete bases
};

/**
 * @brief TODO
 */
struct CompoundStmt : MakeAStmt<CompoundStmt> {
    std::vector<Stmt*> stmts{};
};

/**
 * @brief TODO
 */
struct ReturnStmt : MakeAStmt<ReturnStmt> {
    Expr* val{nullptr};
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
