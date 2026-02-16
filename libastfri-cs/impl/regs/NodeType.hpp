#ifndef CSHARP_NODE_TYPE_HPP
#define CSHARP_NODE_TYPE_HPP

namespace astfri::csharp
{

enum class NodeType
{
    // Definitions
    // Type Definitions
    ClassDecl,
    StructDecl,
    InterfaceDecl,
    EnumDecl,
    DelegateDecl,
    RecordDecl,
    //Member Definitions
    ConstructorDecl,
    ConstructorInit,
    MethodDecl,
    DecstructorDecl,
    FieldDecl,
    PropertyDecl,
    IndexerDecl,
    OperatorDecl,
    // Other Definitions
    LocalFunctionDecl,
    Parameter,
    LocalVarDef,
    // Statements
    Block,
    ArrowExprClaus,
    DoWhileLoop,
    WhileLoop,
    ForLoop,
    ForeachLoop,
    BreakStmt,
    ContinueStmt,
    IfStmt,
    TryStmt,
    CatchClause,
    FinallyClause,
    CatchDecl,
    SwitchStmt,
    SwitchSection,
    ExprStmt,
    ReturnStmt,
    ThrowStmt,
    // Literals
    IntLit,
    RealLit,
    BoolLit,
    CharLit,
    StrLit,
    NullLit,
    VerbatimStrLit,
    RawStrLit,
    // Operation expressions
    PrefixUnOpr,
    SuffixUnOpr,
    BinaryOpr,
    TernaryOpr,
    Assignment,
    // Patterns
    ConstPattern,
    // other expr
    ThisExpr,
    This,
    RefExpr,
    ParenthesizedExpr,
    MemberAccess,
    Invocation,
    CastExpr,
    LambdaExpr,

    // Types
    PredefinedType,
    QualifiedName,
    ImplicitType,
    NullableType,
    PointerType,
    RefType,
    ArrayType,
    GenericType,
    TupleType,
    FuncPointerType,
    ScopedType,

    // Other
    Modifier,
    Identifier,
    Global,
    /**
     * @brief Node for static keyword inside static using directives
     */
    Static,
    AliasQualifName,
    /**
     * @brief Node for list of bases and implemented interfaces
     */
    BaseList,
    /**
     * @brief Node for list generic parameters
     */
    TypeParamList,
    /**
     * @brief Node for list of constrains for generic parameters
     */
    TypeParamConstraints,

    ERROR,
};

} // namespace astfri::csharp

#endif // CSHARP_NODE_TYPE_HPP

