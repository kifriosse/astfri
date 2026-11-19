#ifndef ASTFRI_IMPL_STMT_DEF_HPP
#define ASTFRI_IMPL_STMT_DEF_HPP

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
 * @brief Base for all statements.
 */
struct Stmt : ASTNode {
};
ASTFRI_REGISTER_TYPE(Stmt,ASTNode)


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
ASTFRI_REGISTER_TYPE(VarDefStmt,Stmt)


/**
* @brief TODO
*/
struct LocalVarDefStmt : VarDefStmt {
    LocalVarDefStmt();
};
ASTFRI_REGISTER_TYPE(LocalVarDefStmt,VarDefStmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(LocalVarDefStmt)


/**
 * @brief TODO
 */
struct ParamVarDefStmt : VarDefStmt {
    ParamVarDefStmt();
};
ASTFRI_REGISTER_TYPE(ParamVarDefStmt,VarDefStmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(ParamVarDefStmt)


/**
 * @brief TODO
 */
struct MemberVarDefStmt : VarDefStmt {
    AccessModifier access{AccessModifier::UNINITIALIZED};
    Staticity staticity{Staticity::UNINITIALIZED};

    MemberVarDefStmt();
};
ASTFRI_REGISTER_TYPE(MemberVarDefStmt,VarDefStmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(MemberVarDefStmt)


/**
 * @brief TODO
 */
struct GlobalVarDefStmt : VarDefStmt {
    GlobalVarDefStmt();
};
ASTFRI_REGISTER_TYPE(GlobalVarDefStmt,VarDefStmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(GlobalVarDefStmt)


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
struct MultiVarDefStmt : Stmt {
    std::vector<VarDefStmt*> defs{};

    MultiVarDefStmt();
};
ASTFRI_REGISTER_TYPE(MultiVarDefStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(MultiVarDefStmt)


/**
 * @brief TODO
 */
struct FunctionDefStmt : Stmt {
    std::string name{};
    std::vector<ParamVarDefStmt*> params{};
    Type* retType{nullptr};
    CompoundStmt* body{nullptr};

    FunctionDefStmt();
};
ASTFRI_REGISTER_TYPE(FunctionDefStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(FunctionDefStmt)


/**
 * @brief TODO
 */
struct MethodDefStmt : Stmt {
    Stmt* owner{nullptr}; // TODO ClassType or Interface, variant?
    FunctionDefStmt* func{nullptr};
    AccessModifier access{AccessModifier::Public};
    Virtuality virtuality{Virtuality::NotVirtual};
    Staticity staticity{Staticity::NonStatic};

    MethodDefStmt();
};
ASTFRI_REGISTER_TYPE(MethodDefStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(MethodDefStmt)


/**
 * @brief Initializer of a base class called at the begining of a constructor
 */
struct BaseInitializerStmt : Stmt {
    ClassType* type{nullptr};
    std::vector<Expr*> args{};

    BaseInitializerStmt();
};
ASTFRI_REGISTER_TYPE(BaseInitializerStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(BaseInitializerStmt)


/**
 * @brief TODO
 */
struct SelfInitializerStmt : Stmt {
    std::vector<Expr*> args{};

    SelfInitializerStmt();
};
ASTFRI_REGISTER_TYPE(SelfInitializerStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(SelfInitializerStmt)


/**
 * @brief TODO
 */
struct MemberInitializerStmt : Stmt {
    MemberVarDefStmt *member{nullptr};
    std::vector<Expr*> args{};

    MemberInitializerStmt();
};
ASTFRI_REGISTER_TYPE(MemberInitializerStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(MemberInitializerStmt)


/**
 * @brief TODO
 */
struct ConstructorDefStmt : Stmt {
    ClassDefStmt* owner{nullptr};
    std::vector<ParamVarDefStmt*> params{};
    std::vector<BaseInitializerStmt*> baseInit{};
    std::vector<SelfInitializerStmt*> selfInitializers{};
    std::vector<MemberInitializerStmt*> memberInitializers{};
    CompoundStmt* body{nullptr};
    AccessModifier access{AccessModifier::UNINITIALIZED};

    ConstructorDefStmt();
};
ASTFRI_REGISTER_TYPE(ConstructorDefStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(ConstructorDefStmt)


/**
 * @brief Definition of a descructor
 */
struct DestructorDefStmt : Stmt {
    ClassDefStmt* owner{nullptr};
    CompoundStmt* body{nullptr};

    DestructorDefStmt();
};
ASTFRI_REGISTER_TYPE(DestructorDefStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(DestructorDefStmt)


/**
 * @brief TODO
 */
struct GenericParam : Stmt {
    // TODO later, this could be pointer to a concept
    std::string constraint{};
    std::string name{};

    GenericParam();
};
ASTFRI_REGISTER_TYPE(GenericParam,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(GenericParam)


/**
 * @deprecated
 * @brief Common base for Class and Interface
 * In the future, it could also be used for union or strong type alias
 */
struct [[deprecated]] UserTypeDefStmt : Stmt { };
ASTFRI_REGISTER_TYPE(UserTypeDefStmt,Stmt)


/**
 * @brief TODO
 */
struct InterfaceDefStmt : UserTypeDefStmt {
    InterfaceType *type{nullptr};
    std::vector<MethodDefStmt*> methods{};
    std::vector<GenericParam*> tparams{};
    [[deprecated]] std::vector<InterfaceDefStmt*> bases{}; // TODO remove this
    std::vector<InterfaceType*> baseTypes{};               // TODO remove this when bases are removed

    InterfaceDefStmt();
};
ASTFRI_REGISTER_TYPE(InterfaceDefStmt,UserTypeDefStmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(InterfaceDefStmt)


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
    [[deprecated]] std::vector<InterfaceDefStmt*> interfaces{}; // TODO remove this
    [[deprecated]] std::vector<ClassDefStmt*> bases{};          // TODO remove this
    std::vector<ClassType*> baseTypes{};                        // TODO rename this when bases are removed
    std::vector<InterfaceType*> interfaceTypes{};               // TODO rename this wehn interfaces are removed

    ClassDefStmt();
};
ASTFRI_REGISTER_TYPE(ClassDefStmt,UserTypeDefStmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(ClassDefStmt)


/**
 * @brief TODO
 */
struct CompoundStmt : Stmt {
    std::vector<Stmt*> stmts{};

    CompoundStmt();
};
ASTFRI_REGISTER_TYPE(CompoundStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(CompoundStmt)


/**
 * @brief TODO
 */
struct ReturnStmt : Stmt {
    Expr* val{nullptr};

    ReturnStmt();
};
ASTFRI_REGISTER_TYPE(ReturnStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(ReturnStmt)


/**
 * @brief TODO
 */
struct ExprStmt : Stmt {
    Expr* expr {nullptr};

    ExprStmt();
};
ASTFRI_REGISTER_TYPE(ExprStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(ExprStmt)


/**
 * @brief TODO
 */
struct IfStmt : Stmt {
    Expr *cond{nullptr};
    Stmt *iftrue{nullptr};
    Stmt *iffalse{nullptr};
    // ReqPtr<Expr> cond;
    // ReqPtr<Stmt> iftrue;
    // OptPtr<Stmt> iffalse;

    IfStmt();
};
ASTFRI_REGISTER_TYPE(IfStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(IfStmt)


/**
 * @brief Base class for all label statements
 */
struct LabelStmt : Stmt { };
ASTFRI_REGISTER_TYPE(LabelStmt,Stmt)


/**
 * @brief Standard goto label (also used for java labels)
 */
struct GotoLabelStmt : LabelStmt {
    std::string name;

    GotoLabelStmt();
};
ASTFRI_REGISTER_TYPE(GotoLabelStmt,LabelStmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(GotoLabelStmt)


/**
 * @brief Goto statement used to jump to a label
 */
struct GotoStmt : Stmt {
    LabelStmt *label{nullptr};

    GotoStmt();
};
ASTFRI_REGISTER_TYPE(GotoStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(GotoStmt)


/**
 * @brief Case option in switch statement
 */
struct CaseStmt : LabelStmt {
    [[deprecated]] Stmt *body{nullptr};
    std::vector<Expr*> exprs{};

    CaseStmt();
};
ASTFRI_REGISTER_TYPE(CaseStmt,LabelStmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(CaseStmt)


/**
 * @brief Default option in switch statement
 */
struct DefaultCaseStmt : LabelStmt {
    [[deprecated]] Stmt *body{nullptr};

    DefaultCaseStmt();
};
ASTFRI_REGISTER_TYPE(DefaultCaseStmt,LabelStmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(DefaultCaseStmt)


/**
 * @brief TODO
 */
struct SwitchStmt : Stmt {
    [[deprecated]] DefaultCaseStmt *defaultCase{nullptr};
    [[deprecated]] std::vector<CaseStmt*> cases{};
    Expr *expr{nullptr};
    CompoundStmt* body{nullptr};

    SwitchStmt();
};
ASTFRI_REGISTER_TYPE(SwitchStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(SwitchStmt)


/**
 * @brief TODO
 */
struct WhileStmt : Stmt {
    Expr *cond{nullptr};
    Stmt *body{nullptr};

    WhileStmt();
};
ASTFRI_REGISTER_TYPE(WhileStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(WhileStmt)


/**
 * @brief TODO
 */
struct DoWhileStmt : Stmt {
    Expr *cond{nullptr};
    Stmt *body{nullptr};

    DoWhileStmt();
};
ASTFRI_REGISTER_TYPE(DoWhileStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(DoWhileStmt)


/**
 * @brief TODO
 */
struct ForStmt : Stmt {
    Stmt *init{nullptr};
    Expr *cond{nullptr};
    Stmt *step{nullptr};
    Stmt *body{nullptr};

    ForStmt();
};
ASTFRI_REGISTER_TYPE(ForStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(ForStmt)


/**
 * @brief TODO
 */
struct ForEachStmt : Stmt {
    LocalVarDefStmt *var{nullptr};
    Expr *container{nullptr};
    Stmt *body{nullptr};

    ForEachStmt();
};
ASTFRI_REGISTER_TYPE(ForEachStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(ForEachStmt)


/**
 * @brief TODO
 */
struct ThrowStmt : Stmt {
    Expr *val{nullptr};

    ThrowStmt();
};
ASTFRI_REGISTER_TYPE(ThrowStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(ThrowStmt)


/**
 * @brief TODO
 */
struct CatchStmt : Stmt {
    LocalVarDefStmt *param{nullptr};
    Stmt *body{nullptr};

    CatchStmt();
};
ASTFRI_REGISTER_TYPE(CatchStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(CatchStmt)


/**
 * @brief TODO
 */
struct TryStmt : Stmt {
    Stmt *body{nullptr};
    Stmt *finally{nullptr};
    std::vector<CatchStmt*> catches{};

    TryStmt();
};
ASTFRI_REGISTER_TYPE(TryStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(TryStmt)


/**
 * @brief Break statement used to terminate a loop
 */
struct BreakStmt : Stmt {
    BreakStmt();
};
ASTFRI_REGISTER_TYPE(BreakStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(BreakStmt)


/**
 * @brief Continue statement used to skip rest of a loop
 */
struct ContinueStmt : Stmt {
    ContinueStmt();
};
ASTFRI_REGISTER_TYPE(ContinueStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(ContinueStmt)


/**
 * @brief TODO
 */
struct UnknownStmt : Stmt {
    UnknownStmt();
};
ASTFRI_REGISTER_TYPE(UnknownStmt,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(UnknownStmt)


/**
 * @brief TODO
 */
struct TranslationUnit : Stmt {
    std::vector<ClassDefStmt*> classes{};
    std::vector<InterfaceDefStmt*> interfaces{};
    std::vector<FunctionDefStmt*> functions{};
    std::vector<GlobalVarDefStmt*> globals{};
    std::vector<Stmt*> content{};

    TranslationUnit();
};
ASTFRI_REGISTER_TYPE(TranslationUnit,Stmt)
ASTFRI_ADD_NODE_CONSTRUCTOR(TranslationUnit)


} // namespace astfri

#endif
