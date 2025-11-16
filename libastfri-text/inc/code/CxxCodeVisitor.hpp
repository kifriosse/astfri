#ifndef LIBASTFRI_TEXT_CXX_CODE_VISITOR
#define LIBASTFRI_TEXT_CXX_CODE_VISITOR

#include <libastfri-text/inc/code/AbstractCodeVisitor.hpp>

namespace astfri::text
{
    class CxxCodeVisitor : public AbstractCodeVisitor
    {
    public:
        static CxxCodeVisitor& get_instance();
        CxxCodeVisitor(CxxCodeVisitor const&) = delete;
        CxxCodeVisitor(CxxCodeVisitor&&)      = delete;
        CxxCodeVisitor& operator=(CxxCodeVisitor const&) = delete;
        CxxCodeVisitor& operator=(CxxCodeVisitor&&)      = delete;
    private:
        explicit CxxCodeVisitor();
        ~CxxCodeVisitor() = default;
    public:
        // -----
        void visit(DynamicType const& type) override;
        void visit(FloatType const& type) override;
        void visit(BoolType const& type) override;
        void visit(IndirectionType const& type) override;
        void visit(LambdaType const& type) override;
        // -----
        void visit(NullLiteralExpr const& expr) override;
        void visit(MemberVarRefExpr const& expr) override;
        void visit(GlobalVarRefExpr const& expr) override;
        void visit(FunctionCallExpr const& expr) override;
        void visit(MethodCallExpr const& expr) override;
        void visit(LambdaCallExpr const& expr) override;
        void visit(LambdaExpr const& expr) override;
        void visit(DeleteExpr const& expr) override;
        // -----
        void visit(TranslationUnit const& stmt) override;
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
        // -----
    };
}

#endif