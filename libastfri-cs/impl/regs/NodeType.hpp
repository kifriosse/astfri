#ifndef CSHARP_NODE_TYPE_HPP
#define CSHARP_NODE_TYPE_HPP

#include <tree_sitter/api.h>

namespace astfri::csharp {

/**
 * @brief Enum for representing tree-sitter node type
 */
enum class NodeType : TSSymbol {
    Unknown,
    CompilationUnit,
    // Definitions
    NamespaceDecl,
    // Type Definitions
    ClassDecl,
    StructDecl,
    InterfaceDecl,
    EnumDecl,
    DelegateDecl,
    RecordDecl,
    // Member Definitions
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
    // Operators
    // Binary
    Assign,
    Minus,
    Plus,
    /**
     * @brief Used for both multiplication and dereference
     */
    Asterisk,
    Divide,
    Modulo,
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    LogicalAnd,
    LogicalOr,
    BitShiftRight,
    BitShiftRightUnsigned,
    BitShiftLeft,
    /**
     * @brief Used for both BitWiseAnd and AddressOff operator
     */
    Ampersand,
    BitOr,
    BitXor,
    AddAssign,
    SubtractAssign,
    MultiplyAssign,
    DivideAssign,
    ModuloAssign,
    BitShiftRightAssign,
    BitShiftRightUnsignedAssign,
    BitShiftLeftAssign,
    BitAndAssign,
    BitOrAssign,
    BitXorAssign,
    // Unary
    LogicalNot,
    Increment,
    Decrement,
    BitFlip,
    // Patterns
    /**
     * @brief Node for constant patterns in switch, e.g. \code case 5: \endcode
     */
    ConstPattern,
    // other expr
    This,
    Base,
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
    /**
     * @brief Node for \code global\endcode keyword inside global using
     * directives
     */
    Global,
    /**
     * @brief Node for \code static\endcode keyword inside static using
     * directives
     */
    Static,
    /**
     * @brief Node for explicit alias in qualified name, e.g.
     * \code global::System.Console\endcode
     */
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
    /**
     * @brief Node for single generic parameter constraint
     */
    TypeParamConstraint,

    ERROR,
};

} // namespace astfri::csharp

#endif // CSHARP_NODE_TYPE_HPP
