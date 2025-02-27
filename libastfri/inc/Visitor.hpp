#ifndef LIBASTFRI_VISITOR_HPP
#define LIBASTFRI_VISITOR_HPP

#include <libastfri/impl/ExprFwd.hpp>
#include <libastfri/impl/StmtFwd.hpp>
#include <libastfri/impl/TypeFwd.hpp>

namespace astfri
{
/**
 * @brief TODO
 */
struct IVisitor
{
    virtual void visit (DynamicType const& type)         = 0;
    virtual void visit (IntType const& type)             = 0;
    virtual void visit (FloatType const& type)           = 0;
    virtual void visit (CharType const& type)            = 0;
    virtual void visit (BoolType const& type)            = 0;
    virtual void visit (VoidType const& type)            = 0;
    virtual void visit (UserType const& type)            = 0;
    virtual void visit (IndirectionType const& type)     = 0;
    virtual void visit (UnknownType const& type)         = 0;

    virtual void visit (IntLiteralExpr const& expr)      = 0;
    virtual void visit (FloatLiteralExpr const& expr)    = 0;
    virtual void visit (CharLiteralExpr const& expr)     = 0;
    virtual void visit (StringLiteralExpr const& expr)   = 0;
    virtual void visit (BoolLiteralExpr const& expr)     = 0;
    virtual void visit (NullLiteralExpr const& expr)     = 0;
    virtual void visit (IfExpr const& expr)              = 0;
    virtual void visit (BinOpExpr const& expr)           = 0;
    virtual void visit (UnaryOpExpr const& expr)         = 0;
    virtual void visit (AssignExpr const& expr)          = 0;
    virtual void visit (CompoundAssignExpr const& expr)  = 0;
    virtual void visit (ParamVarRefExpr const& expr)     = 0;
    virtual void visit (LocalVarRefExpr const& expr)     = 0;
    virtual void visit (MemberVarRefExpr const& expr)    = 0;
    virtual void visit (GlobalVarRefExpr const& expr)    = 0;
    virtual void visit (ClassRefExpr const& expr)        = 0;
    virtual void visit (FunctionCallExpr const& expr)    = 0;
    virtual void visit (MethodCallExpr const& expr)      = 0;
    virtual void visit (LambdaExpr const& expr)          = 0;
    virtual void visit (ThisExpr const& expr)            = 0;
    virtual void visit (ConstructorCallExpr const& expr) = 0;
    virtual void visit (NewExpr const& expr)             = 0;
    virtual void visit (DeleteExpr const& expr)          = 0;
    virtual void visit (UnknownExpr const& expr)         = 0;

    virtual void visit (TranslationUnit const& stmt)     = 0;
    virtual void visit (CompoundStmt const& stmt)        = 0;
    virtual void visit (ReturnStmt const& stmt)          = 0;
    virtual void visit (ExprStmt const& stmt)            = 0;
    virtual void visit (IfStmt const& stmt)              = 0;
    virtual void visit (CaseStmt const& stmt)            = 0;
    virtual void visit (SwitchStmt const& stmt)          = 0;
    virtual void visit (WhileStmt const& stmt)           = 0;
    virtual void visit (DoWhileStmt const& stmt)         = 0;
    virtual void visit (ForStmt const& stmt)             = 0;
    virtual void visit (ThrowStmt const& stmt)           = 0;
    virtual void visit (UnknownStmt const& stmt)         = 0;
    virtual void visit (LocalVarDefStmt const& stmt)     = 0;
    virtual void visit (ParamVarDefStmt const& stmt)     = 0;
    virtual void visit (MemberVarDefStmt const& stmt)    = 0;
    virtual void visit (GlobalVarDefStmt const& stmt)    = 0;
    virtual void visit (FunctionDefStmt const& stmt)     = 0;
    virtual void visit (DefStmt const& stmt)             = 0;
    virtual void visit (MethodDefStmt const& stmt)       = 0;
    virtual void visit (BaseInitializerStmt const& stmt) = 0;
    virtual void visit (ConstructorDefStmt const& stmt)  = 0;
    virtual void visit (DestructorDefStmt const& stmt)   = 0;
    virtual void visit (GenericParam const& stmt)        = 0;
    virtual void visit (ClassDefStmt const& stmt)        = 0;

    virtual ~IVisitor()                                  = default;
};

/**
 * @brief TODO
 */
struct IVisitable
{
    virtual void accept (IVisitor& visitor) = 0;
    virtual ~IVisitable()                   = default;
};

/**
 * @brief TODO
 */
struct VisitorAdapter : IVisitor
{
    void visit (DynamicType const& /*type*/) override
    {
    }

    void visit (IntType const& /*type*/) override
    {
    }

    void visit (FloatType const& /*type*/) override
    {
    }

    void visit (CharType const& /*type*/) override
    {
    }

    void visit (BoolType const& /*type*/) override
    {
    }

    void visit (VoidType const& /*type*/) override
    {
    }

    void visit (UserType const& /*type*/) override
    {
    }

    void visit (IndirectionType const& /*type*/) override
    {
    }

    void visit (UnknownType const& /*type*/) override
    {
    }

    void visit (IntLiteralExpr const& /*expr*/) override
    {
    }

    void visit (FloatLiteralExpr const& /*expr*/) override
    {
    }

    void visit (CharLiteralExpr const& /*expr*/) override
    {
    }

    void visit (StringLiteralExpr const& /*expr*/) override
    {
    }

    void visit (BoolLiteralExpr const& /*expr*/) override
    {
    }

    void visit (NullLiteralExpr const& /*expr*/) override
    {
    }

    void visit (IfExpr const& /*expr*/) override
    {
    }

    void visit (BinOpExpr const& /*expr*/) override
    {
    }

    void visit (UnaryOpExpr const& /*expr*/) override
    {
    }

    void visit (AssignExpr const& /*expr*/) override
    {
    }

    void visit (CompoundAssignExpr const& /*expr*/) override
    {
    }

    void visit (ParamVarRefExpr const& /*expr*/) override
    {
    }

    void visit (LocalVarRefExpr const& /*expr*/) override
    {
    }

    void visit (MemberVarRefExpr const& /*expr*/) override
    {
    }

    void visit (GlobalVarRefExpr const& /*expr*/) override
    {
    }

    void visit (ClassRefExpr const& /*expr*/) override
    {
    }

    void visit (FunctionCallExpr const& /*expr*/) override
    {
    }

    void visit (MethodCallExpr const& /*expr*/) override
    {
    }

    void visit (LambdaExpr const& /*expr*/) override
    {
    }

    void visit (ThisExpr const& /*expr*/) override
    {
    }

    void visit (ConstructorCallExpr const& /*expr*/) override
    {
    }

    void visit (NewExpr const& /*expr*/) override
    {
    }

    void visit (DeleteExpr const& /*expr*/) override
    {
    }

    void visit (UnknownExpr const& /*expr*/) override
    {
    }

    void visit (TranslationUnit const& /*stmt*/) override
    {
    }

    void visit (CompoundStmt const& /*stmt*/) override
    {
    }

    void visit (ReturnStmt const& /*stmt*/) override
    {
    }

    void visit (ExprStmt const& /*stmt*/) override
    {
    }

    void visit (IfStmt const& /*stmt*/) override
    {
    }

    void visit (CaseStmt const& /*stmt*/) override
    {
    }

    void visit (SwitchStmt const& /*stmt*/) override
    {
    }

    void visit (WhileStmt const& /*stmt*/) override
    {
    }

    void visit (DoWhileStmt const& /*stmt*/) override
    {
    }

    void visit (ForStmt const& /*stmt*/) override
    {
    }

    void visit (ThrowStmt const& /*stmt*/) override
    {
    }

    void visit (UnknownStmt const& /*stmt*/) override
    {
    }

    void visit (LocalVarDefStmt const& /*stmt*/) override
    {
    }

    void visit (ParamVarDefStmt const& /*stmt*/) override
    {
    }

    void visit (MemberVarDefStmt const& /*stmt*/) override
    {
    }

    void visit (GlobalVarDefStmt const& /*stmt*/) override
    {
    }

    void visit (FunctionDefStmt const& /*stmt*/) override
    {
    }

    void visit (DefStmt const& /*stmt*/) override
    {
    }

    void visit (MethodDefStmt const& /*stmt*/) override
    {
    }

    void visit (BaseInitializerStmt const& /*stmt*/) override
    {
    }

    void visit (ConstructorDefStmt const& /*stmt*/) override
    {
    }

    void visit (DestructorDefStmt const& /*stmt*/) override
    {
    }

    void visit (GenericParam const& /*stmt*/) override
    {
    }

    void visit (ClassDefStmt const& /*stmt*/) override
    {
    }
};
} // namespace astfri

#endif
