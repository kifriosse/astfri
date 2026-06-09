#ifndef ASTFRI_IMPL_STMT_DEF_HPP
#define ASTFRI_IMPL_STMT_DEF_HPP

#include <astfri/impl/tools/ReqPtr.hpp>
#include <astfri/impl/tools/OptPtr.hpp>
#include <astfri/impl/ASTNode.hpp>
#include <astfri/impl/StmtKind.hpp>
#include <astfri/impl/TypeDef.hpp>

#include <string>
#include <vector>


namespace astfri {


/**
 * @brief Base for all statements.
 */
struct Stmt : detail::ASTNode<StmtKind> {
};


/**
 * @brief Helper CRTP base. Indirectly inherits @c SelfType from @c Stmt.
 *
 * See @c MakeA doc for more details.
 *
 * @tparam SelfType child class type.
 */
template<typename SelfType>
using MakeAStmt = detail::MakeA<Stmt, SelfType>;


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
struct VarDefStmt : Stmt {
    std::string name{};
    Type *type{nullptr};
    Expr *initializer{nullptr};
};


/**
 * @brief TODO
 */
struct LocalVarDefStmt : VarDefStmt {
    ASTFRI_ADD_NODE_CONSTRUCTOR(LocalVarDefStmt)
};


/**
 * @brief TODO
 */
struct ParamVarDefStmt : VarDefStmt {
    ASTFRI_ADD_NODE_CONSTRUCTOR(ParamVarDefStmt)
};


/**
 * @brief TODO
 */
struct MemberVarDefStmt : VarDefStmt {
    AccessModifier access{AccessModifier::UNINITIALIZED};
    Staticity staticity{Staticity::UNINITIALIZED};
    ASTFRI_ADD_NODE_CONSTRUCTOR(MemberVarDefStmt)
};


/**
 * @brief TODO
 */
struct GlobalVarDefStmt : VarDefStmt {
    ASTFRI_ADD_NODE_CONSTRUCTOR(GlobalVarDefStmt)
};


/**
 * @brief Definition statement that may contain multiple variable definitions
 * Covers the following situations:
 * @code
 * int x = 10, y = 20;
 * @endcode
 * In this case, you would use:
 * @code
   MultiVarDefStmt
   |-LocalVarDefStmt
   | `-IntLiteralExpr(10)
   `-LocalVarDefStmt
     `-IntLiteralExpr(10)
 * @endcode
 */
struct MultiVarDefStmt : MakeAStmt<MultiVarDefStmt> {
    std::vector<VarDefStmt*> defs{};
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
    Stmt* owner{nullptr}; // TODO ClassType or Interface, variant?
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
    MemberVarDefStmt *member{nullptr};
    std::vector<Expr*> args{nullptr};
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
struct UserTypeDefStmt : Stmt {
    [[deprecated]] std::string name{};
};

/**
 * @brief TODO
 */
struct InterfaceDefStmt : UserTypeDefStmt {
    InterfaceType *type{nullptr};
    std::vector<MethodDefStmt*> methods{};
    std::vector<GenericParam*> tparams{};
    std::vector<InterfaceDefStmt*> bases{};
    ASTFRI_ADD_NODE_CONSTRUCTOR(InterfaceDefStmt)
};

/**
 * @brief TODO
 */
struct ClassDefStmt : UserTypeDefStmt {
    ClassType* type{nullptr};
    std::vector<MemberVarDefStmt*> vars{};
    std::vector<ConstructorDefStmt*> constructors{};
    std::vector<DestructorDefStmt*> destructors{};
    std::vector<MethodDefStmt*> methods{};
    std::vector<GenericParam*> tparams{};
    std::vector<InterfaceDefStmt*> interfaces{}; // TODO IntefaceType
    std::vector<ClassDefStmt*> bases{};          // TODO ClassType
    // TODO incomplete bases
    ASTFRI_ADD_NODE_CONSTRUCTOR(ClassDefStmt)
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
struct ExprStmt : MakeAStmt<ExprStmt> {
    Expr* expr {nullptr};
};

/**
 * @brief TODO
 */
struct IfStmt : MakeAStmt<IfStmt> {
    Expr *cond{nullptr};
    Stmt *iftrue{nullptr};
    Stmt *iffalse{nullptr};
    // ReqPtr<Expr> cond;
    // ReqPtr<Stmt> iftrue;
    // OptPtr<Stmt> iffalse;
};

/**
 * @brief Case option in switch statement
 */
struct CaseStmt : MakeAStmt<CaseStmt> {
    Stmt *body{nullptr};
    std::vector<Expr*> exprs{};
};

/**
 * @brief Default option in switch statement
 */
struct DefaultCaseStmt : MakeAStmt<DefaultCaseStmt> {
    Stmt *body{nullptr};
};

/**
 * @brief TODO
 */
struct SwitchStmt : MakeAStmt<SwitchStmt> {
    Expr *expr{nullptr};
    DefaultCaseStmt *defaultCase{nullptr};
    std::vector<CaseStmt*> cases{};
};

/**
 * @brief TODO
 */
struct WhileStmt : MakeAStmt<WhileStmt> {
    Expr *cond{nullptr};
    Stmt *body{nullptr};
};

/**
 * @brief TODO
 */
struct DoWhileStmt : MakeAStmt<DoWhileStmt> {
    Expr *cond{nullptr};
    Stmt *body{nullptr};
};

/**
 * @brief TODO
 */
struct ForStmt : MakeAStmt<ForStmt> {
    Stmt *init{nullptr};
    Expr *cond{nullptr};
    Stmt *step{nullptr};
    Stmt *body{nullptr};
};

/**
 * @brief TODO
 */
struct ForEachStmt : MakeAStmt<ForEachStmt> {
    LocalVarDefStmt *var{nullptr};
    Expr *container{nullptr};
    Stmt *body{nullptr};
};

/**
 * @brief TODO
 */
struct ThrowStmt : MakeAStmt<ThrowStmt> {
    Expr *val{nullptr};
};

/**
 * @brief TODO
 */
struct CatchStmt : MakeAStmt<CatchStmt> {
    LocalVarDefStmt *param{nullptr};
    Stmt *body{nullptr};
};

/**
 * @brief TODO
 */
struct TryStmt : MakeAStmt<TryStmt> {
    Stmt *body{nullptr};
    Stmt *finally{nullptr};
    std::vector<CatchStmt*> catches{};
};

/**
 * @brief Break statement used to terminate a loop
 */
struct BreakStmt : MakeAStmt<BreakStmt> {
};

/**
 * @brief Continue statement used to skip rest of a loop
 */
struct ContinueStmt : MakeAStmt<ContinueStmt> {
};

/**
 * @brief TODO
 */
struct UnknownStmt : MakeAStmt<UnknownStmt> {
};

/**
 * @brief TODO
 */
struct TranslationUnit : MakeAStmt<TranslationUnit> {
    std::vector<ClassDefStmt*> classes{};
    std::vector<InterfaceDefStmt*> interfaces{};
    std::vector<FunctionDefStmt*> functions{};
    std::vector<GlobalVarDefStmt*> globals{};
};


} // namespace astfri

#endif
