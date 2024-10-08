#ifndef LIBASTFRI_VISITOR_HPP
#define LIBASTFRI_VISITOR_HPP

namespace astfri
{
// stmt
struct TranslationUnit;
struct CompoundStatement;
struct DeclarationStatement;
struct DeclarationAndAssigmentStatement;
struct ReturnStatement;
struct ExpressionStatement;
struct IfStatement;
struct WhileStatement;
struct DoWhileStatement;
struct ForStatement;
struct UnknownStatement;

// expr
struct IntLiteral;
struct FloatLiteral;
struct CharLiteral;
struct StringLiteral;
struct BoolLiteral;
struct BinaryExpression;
struct UnaryExpression;
struct ParamRefExpression;
struct VarRefExpression;
struct FunctionCallExpression;
struct UnknownExpression;

// decl
struct VariableDefintion;
struct ParameterDefinition;
struct FunctionDefinition;
struct UknownDeclaration;

// type
struct Type;
struct IntType;
struct FloatType;
struct CharType;
struct BoolType;
struct VoidType;
struct UserType;
} // namespace libastfri::structures

namespace astfri
{

// sem definovat len hlavicky, prehliadku nechat na implementacii
class IVisitor
{
public:
    // stmt
    virtual void Visit (TranslationUnit const& stmt)      = 0;
    virtual void Visit (CompoundStatement const& stmt)    = 0;
    virtual void Visit (DeclarationStatement const& stmt) = 0;
    virtual void Visit (DeclarationAndAssigmentStatement const& stmt
    )                                                                 = 0;
    virtual void Visit (ReturnStatement const& stmt)      = 0;
    virtual void Visit (ExpressionStatement const& stmt)  = 0;
    virtual void Visit (IfStatement const& stmt)          = 0;
    virtual void Visit (WhileStatement const& stmt)       = 0;
    virtual void Visit (DoWhileStatement const& stmt)     = 0;
    virtual void Visit (ForStatement const& stmt)         = 0;
    virtual void Visit (UnknownStatement const& stmt)     = 0;

    // expr
    virtual void Visit (IntLiteral const& expr)             = 0;
    virtual void Visit (FloatLiteral const& expr)           = 0;
    virtual void Visit (CharLiteral const& expr)            = 0;
    virtual void Visit (StringLiteral const& expr)          = 0;
    virtual void Visit (BoolLiteral const& expr)            = 0;
    virtual void Visit (BinaryExpression const& expr)       = 0;
    virtual void Visit (UnaryExpression const& expr)        = 0;
    virtual void Visit (ParamRefExpression const& expr)     = 0;
    virtual void Visit (VarRefExpression const& expr)       = 0;
    virtual void Visit (FunctionCallExpression const& expr) = 0;
    virtual void Visit (UnknownExpression const& expr)      = 0;

    // decl
    virtual void Visit (VariableDefintion const& decl)   = 0;
    virtual void Visit (ParameterDefinition const& decl) = 0;
    virtual void Visit (FunctionDefinition const& decl)  = 0;
    virtual void Visit (UknownDeclaration const& decl)   = 0;

    // type
    virtual void Visit (Type const& type)      = 0;
    virtual void Visit (IntType const& type)   = 0;
    virtual void Visit (FloatType const& type) = 0;
    virtual void Visit (CharType const& type)  = 0;
    virtual void Visit (BoolType const& type)  = 0;
    virtual void Visit (VoidType const& type)  = 0;
    virtual void Visit (UserType const& type)  = 0;

    virtual ~IVisitor()                              = default;
};

class VisitorAdapter : public IVisitor
{
public:
    VisitorAdapter() = default;

    // stmt
    virtual void Visit (TranslationUnit const&) override
    {
    }

    virtual void Visit (CompoundStatement const&) override
    {
    }

    virtual void Visit (DeclarationStatement const&) override
    {
    }

    virtual void Visit (DeclarationAndAssigmentStatement const&)
        override
    {
    }

    virtual void Visit (ReturnStatement const&) override
    {
    }

    virtual void Visit (ExpressionStatement const&) override
    {
    }

    virtual void Visit (IfStatement const&) override
    {
    }

    virtual void Visit (WhileStatement const&) override
    {
    }

    virtual void Visit (DoWhileStatement const&) override
    {
    }

    virtual void Visit (ForStatement const&) override
    {
    }

    virtual void Visit (UnknownStatement const&) override
    {
    }

    // expr
    virtual void Visit (IntLiteral const&) override
    {
    }

    virtual void Visit (FloatLiteral const&) override
    {
    }

    virtual void Visit (CharLiteral const&) override
    {
    }

    virtual void Visit (StringLiteral const&) override
    {
    }

    virtual void Visit (BoolLiteral const&) override
    {
    }

    virtual void Visit (BinaryExpression const&) override
    {
    }

    virtual void Visit (UnaryExpression const&) override
    {
    }

    virtual void Visit (ParamRefExpression const&) override
    {
    }

    virtual void Visit (VarRefExpression const&) override
    {
    }

    virtual void Visit (FunctionCallExpression const&) override
    {
    }

    virtual void Visit (UnknownExpression const&) override
    {
    }

    // decl
    virtual void Visit (VariableDefintion const&) override
    {
    }

    virtual void Visit (ParameterDefinition const&) override
    {
    }

    virtual void Visit (FunctionDefinition const&) override
    {
    }

    virtual void Visit (UknownDeclaration const&) override
    {
    }

    // type
    virtual void Visit (Type const&) override
    {
    }

    virtual void Visit (IntType const&) override
    {
    }

    virtual void Visit (FloatType const&) override
    {
    }

    virtual void Visit (CharType const&) override
    {
    }

    virtual void Visit (BoolType const&) override
    {
    }

    virtual void Visit (VoidType const&) override
    {
    }

    virtual void Visit (UserType const&) override
    {
    }
};

class IVisitable
{
public:
    virtual void accept (IVisitor& visitor) = 0;
    virtual ~IVisitable()                   = default;
};
} // namespace libastfri::utils

#endif