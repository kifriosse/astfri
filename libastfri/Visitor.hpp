#ifndef LIBASTFRI_VISITOR_HPP
#define LIBASTFRI_VISITOR_HPP

namespace astfri
{
// Stmt TODO
struct TranslationUnit;
struct CompoundStmt;
struct DeclarationStmt;
struct DeclarationAndAssigmentStmt;
struct ReturnStmt;
struct ExpressionStmt;
struct IfStmt;
struct WhileStmt;
struct DoWhileStmt;
struct ForStmt;
struct UnknownStmt;

struct IntLiteral;
struct FloatLiteral;
struct CharLiteral;
struct StringLiteral;
struct BoolLiteral;
struct BinOpExpr;
struct UnaryOpExpr;
struct ParamVarRefExpr;
struct LocalVarRefExpr;
struct MemberVarRefExpr;
struct FunctionCallExpr;
struct MethodCallExpr;
struct UnknownExpr;

// Decl TODO
struct VariableDefintion;
struct ParameterDefinition;
struct FunctionDef;
struct ClassDef;
struct UknownDeclaration;
struct GenericParam;

struct IntType;
struct FloatType;
struct CharType;
struct BoolType;
struct VoidType;
struct UserType;
struct IndirectionType;
struct UnknownType;

/**
 * @brief TODO
 */
struct IVisitor
{
    // stmt
    virtual void visit (TranslationUnit const& stmt)                  = 0;
    virtual void visit (CompoundStmt const& stmt)                = 0;
    virtual void visit (DeclarationStmt const& stmt)             = 0;
    virtual void visit (DeclarationAndAssigmentStmt const& stmt) = 0;
    virtual void visit (ReturnStmt const& stmt)                  = 0;
    virtual void visit (ExpressionStmt const& stmt)              = 0;
    virtual void visit (IfStmt const& stmt)                      = 0;
    virtual void visit (WhileStmt const& stmt)                   = 0;
    virtual void visit (DoWhileStmt const& stmt)                 = 0;
    virtual void visit (ForStmt const& stmt)                     = 0;
    virtual void visit (UnknownStmt const& stmt)                 = 0;

    virtual void visit (IntLiteral const& expr) = 0;
    virtual void visit (FloatLiteral const& expr) = 0;
    virtual void visit (CharLiteral const& expr) = 0;
    virtual void visit (StringLiteral const& expr) = 0;
    virtual void visit (BoolLiteral const& expr) = 0;
    virtual void visit (BinOpExpr const& expr) = 0;
    virtual void visit (UnaryOpExpr const& expr) = 0;
    virtual void visit (ParamVarRefExpr const& expr) = 0;
    virtual void visit (LocalVarRefExpr const& expr) = 0;
    virtual void visit (MemberVarRefExpr const& expr) = 0;
    virtual void visit (FunctionCallExpr const& expr) = 0;
    virtual void visit (MethodCallExpr const& expr) = 0;
    virtual void visit (UnknownExpr const& expr) = 0;

    // decl

    virtual void visit (IntType const& type) = 0;
    virtual void visit (FloatType const& type) = 0;
    virtual void visit (CharType const& type) = 0;
    virtual void visit (BoolType const& type) = 0;
    virtual void visit (VoidType const& type) = 0;
    virtual void visit (UserType const& type) = 0;
    virtual void visit (IndirectionType const& type) = 0;
    virtual void visit (UnknownType const& type) = 0;

    virtual ~IVisitor()                        = default;
};

/**
 * @brief TODO
 */
struct VisitorAdapter : IVisitor
{
    // TODO empty impls
};

/**
 * @brief TODO
 */
struct IVisitable
{
    virtual void accept (IVisitor& visitor) = 0;
    virtual ~IVisitable()                   = default;
};
} // namespace astfri

#endif