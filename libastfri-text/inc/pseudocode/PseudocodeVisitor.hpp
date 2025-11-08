#ifndef LIBASTFRI_TEXT_PSEUDOCODE_VISITOR
#define LIBASTFRI_TEXT_PSEUDOCODE_VISITOR

#include <libastfri-text/inc/AbstractVisitor.hpp>
#include <libastfri-text/inc/pseudocode/AbstractTextBuilder.hpp>

namespace astfri::text
{
    class PseudocodeVisitor : public AbstractVisitor
    {
    private:
        TextConfigurator* configurator_;
    public:
        static PseudocodeVisitor& get_instance();
        PseudocodeVisitor(PseudocodeVisitor const&) = delete;
        PseudocodeVisitor(PseudocodeVisitor&&)      = delete;
        PseudocodeVisitor& operator=(PseudocodeVisitor const&) = delete;
        PseudocodeVisitor& operator=(PseudocodeVisitor&&)      = delete;
    private:
        PseudocodeVisitor() = delete;
        explicit PseudocodeVisitor(AbstractTextBuilder* const& builder);
        ~PseudocodeVisitor() = default;
    public:
        // -----
        void visit(DynamicType const& type) override;
        void visit(IntType const& type) override;
        void visit(FloatType const& type) override;
        void visit(CharType const& type) override;
        void visit(BoolType const& type) override;
        void visit(VoidType const& type) override;
        void visit(IndirectionType const& type) override;
        void visit(ClassType const& type) override;
        void visit(InterfaceType const& type) override;
        void visit(LambdaType const& type) override;
        void visit(UnknownType const& type) override;
        // -----
        void visit(IntLiteralExpr const& expr) override;
        void visit(FloatLiteralExpr const& expr) override;
        void visit(CharLiteralExpr const& expr) override;
        void visit(StringLiteralExpr const& expr) override;
        void visit(BoolLiteralExpr const& expr) override;
        void visit(NullLiteralExpr const& expr) override;
        void visit(IfExpr const& expr) override;
        void visit(BinOpExpr const& expr) override;
        void visit(UnaryOpExpr const& expr) override;
        void visit(ParamVarRefExpr const& expr) override;
        void visit(LocalVarRefExpr const& expr) override;
        void visit(MemberVarRefExpr const& expr) override;
        void visit(GlobalVarRefExpr const& expr) override;
        void visit(ClassRefExpr const& expr) override;
        void visit(FunctionCallExpr const& expr) override;
        void visit(MethodCallExpr const& expr) override;
        void visit(LambdaCallExpr const& expr) override;
        void visit(LambdaExpr const& expr) override;
        void visit(ThisExpr const& expr) override;
        void visit(ConstructorCallExpr const& expr) override;
        void visit(NewExpr const& expr) override;
        void visit(DeleteExpr const& expr) override;
        void visit(UnknownExpr const& expr) override;
        // -----
        void visit(TranslationUnit const& stmt) override;
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
        void visit(ThrowStmt const& stmt) override;
        void visit(UnknownStmt const& stmt) override;
        void visit(LocalVarDefStmt const& stmt) override;
        void visit(ParamVarDefStmt const& stmt) override;
        void visit(MemberVarDefStmt const& stmt) override;
        void visit(GlobalVarDefStmt const& stmt) override;
        void visit(FunctionDefStmt const& stmt) override;
        void visit(DefStmt const& stmt) override;
        void visit(MethodDefStmt const& stmt) override;
        void visit(BaseInitializerStmt const& stmt) override;
        void visit(ConstructorDefStmt const& stmt) override;
        void visit(DestructorDefStmt const& stmt) override;
        void visit(GenericParam const& stmt) override;
        void visit(InterfaceDefStmt const& stmt) override;
        void visit(ClassDefStmt const& stmt) override;
        void visit(ContinueStmt const& stmt) override;
        void visit(BreakStmt const& stmt) override;
        // -----
    };
}

#endif