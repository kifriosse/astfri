#ifndef ASTFRI_IMPL_AST_NODE_TYPE_HPP
#define ASTFRI_IMPL_AST_NODE_TYPE_HPP


namespace astfri {


/**
 * @brief Type tags for each ASTNode type.
 */
enum class ASTNodeType {

    UNINITIALIZED = 0,

    ASTNode,

    Expr,
    LiteralExpr,
    IntLiteralExpr,
    FloatLiteralExpr,
    CharLiteralExpr,
    StringLiteralExpr,
    BoolLiteralExpr,
    NullLiteralExpr,
    IfExpr,
    BinOpExpr,
    UnaryOpExpr,
    ParamVarRefExpr,
    LocalVarRefExpr,
    MemberVarRefExpr,
    GlobalVarRefExpr,
    ClassRefExpr,
    FunctionCallExpr,
    MethodCallExpr,
    LambdaCallExpr,
    LambdaExpr,
    ThisExpr,
    BaseExpr,
    ConstructorCallExpr,
    NewExpr,
    DeleteExpr,
    BracketExpr,
    NamedExpr,
    UnknownExpr,

    Stmt,
    VarDefStmt,
    LocalVarDefStmt,
    ParamVarDefStmt,
    MemberVarDefStmt,
    GlobalVarDefStmt,
    MultiVarDefStmt,
    FunctionDefStmt,
    MethodDefStmt,
    BaseInitializerStmt,
    SelfInitializerStmt,
    MemberInitializerStmt,
    ConstructorDefStmt,
    DestructorDefStmt,
    GenericParam, // deprecated
    UserTypeDefStmt, // deprecated
    InterfaceDefStmt,
    ClassDefStmt,
    CompoundStmt,
    ReturnStmt,
    ExprStmt,
    IfStmt,
    LabelStmt,
    GotoLabelStmt,
    GotoStmt,
    CaseStmt,
    DefaultCaseStmt,
    SwitchStmt,
    WhileStmt,
    DoWhileStmt,
    ForStmt,
    ForEachStmt,
    ThrowStmt,
    CatchStmt,
    TryStmt,
    ContinueStmt,
    BreakStmt,
    UnknownStmt,
    TranslationUnit, //deprecated

    Type,
    DynamicType,
    IntType,
    FloatType,
    CharType,
    BoolType,
    VoidType,
    IndirectionType,
    ScopedType,
    ClassType,
    InterfaceType,
    LambdaType,
    DeducedType,
    IncompleteType,
    UnknownType,

    NO_TYPE,

    COUNT,
};


} // namespace astfri

#endif
