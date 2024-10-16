#ifndef LIBASTFRI_VISITOR_HPP
#define LIBASTFRI_VISITOR_HPP

#include <libastfri/impl/TypeFwd.hpp>
#include <libastfri/impl/ExpressionFwd.hpp>
#include <libastfri/impl/StatementFwd.hpp>

namespace astfri
{
/**
 * @brief TODO
 */
struct IVisitor
{
    virtual void visit(const DynamicType& type) = 0;
    virtual void visit(const IntType& type) = 0;
    virtual void visit(const FloatType& type) = 0;
    virtual void visit(const CharType& type) = 0;
    virtual void visit(const BoolType& type) = 0;
    virtual void visit(const VoidType& type) = 0;
    virtual void visit(const UserType& type) = 0;
    virtual void visit(const IndirectionType& type) = 0;
    virtual void visit(const UnknownType& type) = 0;

    virtual void visit(const IntLiteralExpr& expr) = 0;
    virtual void visit(const FloatLiteralExpr& expr) = 0;
    virtual void visit(const CharLiteralExpr& expr) = 0;
    virtual void visit(const StringLiteralExpr& expr) = 0;
    virtual void visit(const BoolLiteralExpr& expr) = 0;
    virtual void visit(const NullLiteralExpr& expr) = 0;
    virtual void visit(const IfExpr& expr) = 0;
    virtual void visit(const BinOpExpr& expr) = 0;
    virtual void visit(const UnaryOpExpr& expr) = 0;
    virtual void visit(const AssignExpr& expr) = 0;
    virtual void visit(const CompoundAssignExpr& expr) = 0;
    virtual void visit(const ParamVarRefExpr& expr) = 0;
    virtual void visit(const LocalVarRefExpr& expr) = 0;
    virtual void visit(const MemberVarRefExpr& expr) = 0;
    virtual void visit(const FunctionCallExpr& expr) = 0;
    virtual void visit(const MethodCallExpr& expr) = 0;
    virtual void visit(const LambdaExpr& expr) = 0;
    virtual void visit(const ThisExpr& expr) = 0;
    virtual void visit(const UnknownExpr& expr) = 0;

    virtual void visit(const TranslationUnit& stmt) = 0;
    virtual void visit(const CompoundStmt& stmt) = 0;
    virtual void visit(const DeclarationStmt& stmt) = 0;
    virtual void visit(const DeclarationAndAssigmentStmt& stmt) = 0;
    virtual void visit(const ReturnStmt& stmt) = 0;
    virtual void visit(const ExprStmt& stmt) = 0;
    virtual void visit(const IfStmt& stmt) = 0;
    virtual void visit(const CaseStmt& stmt) = 0;
    virtual void visit(const SwitchStmt& stmt) = 0;
    virtual void visit(const WhileStmt& stmt) = 0;
    virtual void visit(const DoWhileStmt& stmt) = 0;
    virtual void visit(const ForStmt& stmt) = 0;
    virtual void visit(const ThrowStmt& stmt) = 0;
    virtual void visit(const UnknownStmt& stmt) = 0;
    virtual void visit(const LocalVarDefStmt& stmt) = 0;
    virtual void visit(const ParamVarDefStmt& stmt) = 0;
    virtual void visit(const MemberVarDefStmt& stmt) = 0;
    virtual void visit(const GlobalVarDefStmt& stmt) = 0;
    virtual void visit(const FunctionDefStmt& stmt) = 0;
    virtual void visit(const MethodDefStmt& stmt) = 0;
    virtual void visit(const ClassDefStmt& stmt) = 0;
    virtual void visit(const UknownDeclaration& stmt) = 0;
    virtual void visit(const GenericParam& stmt) = 0;

    virtual ~IVisitor()                        = default;
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
    void visit(const DynamicType& /*type*/) override {}
    void visit(const IntType& /*type*/) override {}
    void visit(const FloatType& /*type*/) override {}
    void visit(const CharType& /*type*/) override {}
    void visit(const BoolType& /*type*/) override {}
    void visit(const VoidType& /*type*/) override {}
    void visit(const UserType& /*type*/) override {}
    void visit(const IndirectionType& /*type*/) override {}
    void visit(const UnknownType& /*type*/) override {}
    void visit(const IntLiteralExpr& /*expr*/) override {}
    void visit(const FloatLiteralExpr& /*expr*/) override {}
    void visit(const CharLiteralExpr& /*expr*/) override {}
    void visit(const StringLiteralExpr& /*expr*/) override {}
    void visit(const BoolLiteralExpr& /*expr*/) override {}
    void visit(const NullLiteralExpr& /*expr*/) override {}
    void visit(const IfExpr& /*expr*/) override {}
    void visit(const BinOpExpr& /*expr*/) override {}
    void visit(const UnaryOpExpr& /*expr*/) override {}
    void visit(const AssignExpr& /*expr*/) override {}
    void visit(const CompoundAssignExpr& /*expr*/) override {}
    void visit(const ParamVarRefExpr& /*expr*/) override {}
    void visit(const LocalVarRefExpr& /*expr*/) override {}
    void visit(const MemberVarRefExpr& /*expr*/) override {}
    void visit(const FunctionCallExpr& /*expr*/) override {}
    void visit(const MethodCallExpr& /*expr*/) override {}
    void visit(const LambdaExpr& /*expr*/) override {}
    void visit(const ThisExpr& /*expr*/) override {}
    void visit(const UnknownExpr& /*expr*/) override {}
    void visit(const TranslationUnit& /*stmt*/) override {}
    void visit(const CompoundStmt& /*stmt*/) override {}
    void visit(const DeclarationStmt& /*stmt*/) override {}
    void visit(const DeclarationAndAssigmentStmt& /*stmt*/) override {}
    void visit(const ReturnStmt& /*stmt*/) override {}
    void visit(const ExprStmt& /*stmt*/) override {}
    void visit(const IfStmt& /*stmt*/) override {}
    void visit(const CaseStmt& /*stmt*/) override {}
    void visit(const SwitchStmt& /*stmt*/) override {}
    void visit(const WhileStmt& /*stmt*/) override {}
    void visit(const DoWhileStmt& /*stmt*/) override {}
    void visit(const ForStmt& /*stmt*/) override {}
    void visit(const ThrowStmt& /*stmt*/) override {}
    void visit(const UnknownStmt& /*stmt*/) override {}
    void visit(const LocalVarDefStmt& /*stmt*/) override {}
    void visit(const ParamVarDefStmt& /*stmt*/) override {}
    void visit(const MemberVarDefStmt& /*stmt*/) override {}
    void visit(const GlobalVarDefStmt& /*stmt*/) override {}
    void visit(const FunctionDefStmt& /*stmt*/) override {}
    void visit(const MethodDefStmt& /*stmt*/) override {}
    void visit(const ClassDefStmt& /*stmt*/) override {}
    void visit(const UknownDeclaration& /*stmt*/) override {}
    void visit(const GenericParam& /*stmt*/) override {}
};
} // namespace astfri

#endif