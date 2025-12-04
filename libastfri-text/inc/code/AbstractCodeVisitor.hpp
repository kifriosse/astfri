#ifndef LIBASTFRI_TEXT_ABSTRACT_CODE_VISITOR
#define LIBASTFRI_TEXT_ABSTRACT_CODE_VISITOR

#include <libastfri-text/inc/AbstractVisitor.hpp>
#include <libastfri-text/inc/code/AbstractCodeBuilder.hpp>
#include "libastfri/inc/Expr.hpp"
#include "libastfri/inc/Stmt.hpp"

namespace astfri::text
{
    class AbstractCodeVisitor : public AbstractVisitor
    {
    public:
        explicit AbstractCodeVisitor(AbstractCodeBuilder& builder);
        virtual ~AbstractCodeVisitor() = default;
    public:
        void visit(DynamicType const& type) override = 0;
        void visit(IntType const& type) override;
        void visit(FloatType const& type) override = 0;
        void visit(CharType const& type) override;
        void visit(BoolType const& type) override = 0;
        void visit(VoidType const& type) override;
        void visit(IndirectionType const& type) override = 0;
        void visit(ClassType const& type) override;
        void visit(InterfaceType const& type) override;
        void visit(LambdaType const& type) override = 0;
        void visit(UnknownType const& type) override;
        // -----
        void visit(IntLiteralExpr const& expr) override;
        void visit(FloatLiteralExpr const& expr) override;
        void visit(CharLiteralExpr const& expr) override;
        void visit(StringLiteralExpr const& expr) override;
        void visit(BoolLiteralExpr const& expr) override;
        void visit(NullLiteralExpr const& expr) override = 0;
        void visit(IfExpr const& expr) override;
        void visit(BinOpExpr const& expr) override;
        void visit(UnaryOpExpr const& expr) override;
        void visit(ParamVarRefExpr const& expr) override;
        void visit(LocalVarRefExpr const& expr) override;
        void visit(MemberVarRefExpr const& expr) override = 0;
        void visit(GlobalVarRefExpr const& expr) override = 0;
        void visit(ClassRefExpr const& expr) override;
        void visit(FunctionCallExpr const& expr) override = 0;
        void visit(MethodCallExpr const& expr) override = 0;
        void visit(LambdaCallExpr const& expr) override = 0;
        void visit(LambdaExpr const& expr) override = 0;
        void visit(ThisExpr const& expr) override;
        void visit(ConstructorCallExpr const& expr) override;
        void visit(NewExpr const& expr) override;
        void visit(DeleteExpr const& expr) override = 0;
        void visit(BracketExpr const& expr) override {} // MM: TODO
        void visit(UnknownExpr const& expr) override;
        // -----
        void visit(TranslationUnit const& stmt) override = 0;
        void visit(CompoundStmt const& stmt) override;
        void visit(ReturnStmt const& stmt) override;
        void visit(ExprStmt const& stmt) override;
        void visit(IfStmt const& stmt) override;
        void visit(CaseStmt const& stmt) override;
        void visit(DefaultCaseStmt const& stmt) override;
        void visit(SwitchStmt const& stmt) override;
        void visit(WhileStmt const& stmt) override;
        void visit(DoWhileStmt const& stmt) override;
        void visit(ForStmt const& stmt) override;
        void visit(ForEachStmt const& stmt) override {} // MM: TODO
        void visit(ThrowStmt const& stmt) override;
        void visit(CatchStmt const& stmt) override {} // MM: TODO
        void visit(TryStmt const& stmt) override {} // MM: TODO
        void visit(UnknownStmt const& stmt) override;
        void visit(LocalVarDefStmt const& stmt) override;
        void visit(ParamVarDefStmt const& stmt) override;
        void visit(MemberVarDefStmt const& stmt) override = 0;
        void visit(GlobalVarDefStmt const& stmt) override = 0;
        void visit(FunctionDefStmt const& stmt) override = 0;
        void visit(DefStmt const& stmt) override = 0;
        void visit(MethodDefStmt const& stmt) override = 0;
        void visit(BaseInitializerStmt const& stmt) override = 0;
        void visit(ConstructorDefStmt const& stmt) override = 0;
        void visit(DestructorDefStmt const& stmt) override = 0;
        void visit(GenericParam const& stmt) override = 0;
        void visit(InterfaceDefStmt const& stmt) override = 0;
        void visit(ClassDefStmt const& stmt) override = 0;
        void visit(ContinueStmt const& stmt) override;
        void visit(BreakStmt const& stmt) override;
    };
}

#endif