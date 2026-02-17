#ifndef CSHARP_NODE_TYPE_HPP
#define CSHARP_NODE_TYPE_HPP

#include <tree_sitter/api.h>

namespace astfri::csharp
{

enum class NodeType : TSSymbol
{
    Unknown,
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
    DestructorDecl,
    MemberVarDef,
    PropertyDecl,
    IndexerDecl,
    OprDecl,
    CastOprDecl,
    // Other Definitions
    LocalFuncDecl,
    ParameterDecl,
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
    SwitchStmt,
    SwitchSection,
    TryStmt,
    CatchClause,
    CatchDecl,
    FinallyClause,
    ExprStmt,
    ReturnStmt,
    ThrowStmt,
    // Literals
    IntLit,
    RealLit,
    BoolLit,
    CharLit,
    StrLit,
    VerbatimStrLit,
    RawStrLit,
    NullLit,
    // Operation expressions
    PrefixUnOpr,
    PostfixUnOpr,
    BinaryOpr,
    TernaryOpr,
    Assignment,
    // Patterns
    ConstPattern,
    // other expr
    This,
    RefExpr,
    ParenthesizedExpr,
    MemberAccess,
    Invocation,
    CastExpr,
    LambdaExpr,

    // Types
    PredefinedType,
    QualifName,
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
    TypeParamConstrClause,
    TypeParamConstraint,

    ERROR,
};

} // namespace astfri::csharp

#endif // CSHARP_NODE_TYPE_HPP

