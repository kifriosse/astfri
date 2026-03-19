#ifndef LIBASTFRI_TEXT_ABSTRACT_CODE_VISITOR
#define LIBASTFRI_TEXT_ABSTRACT_CODE_VISITOR

#include <libastfri-text/inc/AbstractVisitor.hpp>
#include <libastfri-text/inc/code/AbstractCodeBuilder.hpp>

namespace astfri::text {
class AbstractCodeVisitor : public AbstractVisitor {
protected:
    AbstractCodeVisitor() = delete;
    explicit AbstractCodeVisitor(AbstractCodeBuilder& builder);
    virtual ~AbstractCodeVisitor() = default;

public:
    void visit(const DynamicType& type) override = 0;
    void visit(const IntType& type) override;
    void visit(const FloatType& type) override = 0;
    void visit(const CharType& type) override;
    void visit(const BoolType& type) override = 0;
    void visit(const VoidType& type) override;
    void visit(const IndirectionType& type) override = 0;
    void visit(const ClassType& type) override;
    void visit(const InterfaceType& type) override;
    void visit(const LambdaType& type) override = 0;
    void visit(const IncompleteType& type) override;
    void visit(const UnknownType& type) override;
    // -----
    void visit(const IntLiteralExpr& expr) override;
    void visit(const FloatLiteralExpr& expr) override;
    void visit(const CharLiteralExpr& expr) override;
    void visit(const StringLiteralExpr& expr) override;
    void visit(const BoolLiteralExpr& expr) override;
    void visit(const NullLiteralExpr& expr) override = 0;
    void visit(const IfExpr& expr) override;
    void visit(const BinOpExpr& expr) override;
    void visit(const UnaryOpExpr& expr) override;
    void visit(const ParamVarRefExpr& expr) override;
    void visit(const LocalVarRefExpr& expr) override;
    void visit(const MemberVarRefExpr& expr) override = 0;
    void visit(const GlobalVarRefExpr& expr) override = 0;
    void visit(const ClassRefExpr& expr) override;
    void visit(const FunctionCallExpr& expr) override = 0;
    void visit(const MethodCallExpr& expr) override   = 0;
    void visit(const LambdaCallExpr& expr) override   = 0;
    void visit(const LambdaExpr& expr) override       = 0;
    void visit(const ThisExpr& expr) override;
    void visit(const ConstructorCallExpr& expr) override;
    void visit(const NewExpr& expr) override;
    void visit(const DeleteExpr& expr) override = 0;
    void visit(const BracketExpr& expr) override;
    void visit(const UnknownExpr& expr) override;
    // -----
    void visit(const TranslationUnit& stmt) override = 0;
    void visit(const CompoundStmt& stmt) override;
    void visit(const ReturnStmt& stmt) override;
    void visit(const ExprStmt& stmt) override;
    void visit(const IfStmt& stmt) override;
    void visit(const CaseStmt& stmt) override;
    void visit(const DefaultCaseStmt& stmt) override;
    void visit(const SwitchStmt& stmt) override;
    void visit(const WhileStmt& stmt) override;
    void visit(const DoWhileStmt& stmt) override;
    void visit(const ForStmt& stmt) override;

    void visit(const ForEachStmt& /*stmt*/) override {
    } // TODO

    void visit(const ThrowStmt& stmt) override;

    void visit(const CatchStmt& /*stmt*/) override {
    } // TODO

    void visit(const TryStmt& /*stmt*/) override {
    } // TODO

    void visit(const UnknownStmt& stmt) override;
    void visit(const LocalVarDefStmt& stmt) override;
    void visit(const ParamVarDefStmt& stmt) override;
    void visit(const MemberVarDefStmt& stmt) override    = 0;
    void visit(const GlobalVarDefStmt& stmt) override    = 0;
    void visit(const FunctionDefStmt& stmt) override     = 0;
    void visit(const DefStmt& stmt) override             = 0;
    void visit(const MethodDefStmt& stmt) override       = 0;
    void visit(const BaseInitializerStmt& stmt) override = 0;
    void visit(const ConstructorDefStmt& stmt) override  = 0;
    void visit(const DestructorDefStmt& stmt) override   = 0;
    void visit(const GenericParam& stmt) override        = 0;
    void visit(const InterfaceDefStmt& stmt) override    = 0;
    void visit(const ClassDefStmt& stmt) override        = 0;
    void visit(const ContinueStmt& stmt) override;
    void visit(const BreakStmt& stmt) override;
};
} // namespace astfri::text

#endif