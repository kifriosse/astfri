#ifndef LIBASTFRI_TEXT_JAVA_CODE_VISITOR
#define LIBASTFRI_TEXT_JAVA_CODE_VISITOR

#include <libastfri-text/inc/code/AbstractCodeVisitor.hpp>

namespace astfri::text
{
    class JavaCodeVisitor : public AbstractCodeVisitor
    {
    public:
        static JavaCodeVisitor& get_instance();
        JavaCodeVisitor(JavaCodeVisitor const&) = delete;
        JavaCodeVisitor(JavaCodeVisitor&&)      = delete;
        JavaCodeVisitor& operator=(JavaCodeVisitor const&) = delete;
        JavaCodeVisitor& operator=(JavaCodeVisitor&&)      = delete;
    private:
        explicit JavaCodeVisitor();
        ~JavaCodeVisitor() = default;
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
        void visit(DeleteExpr const& /*expr*/) override { }
        // -----
        void visit(TranslationUnit const& stmt) override;
        void visit(MemberVarDefStmt const& stmt) override;
        void visit(GlobalVarDefStmt const& stmt) override;
        void visit(FunctionDefStmt const& stmt) override;
        void visit(DefStmt const& stmt) override;
        void visit(MethodDefStmt const& stmt) override;
        void visit(BaseInitializerStmt const& stmt) override;
        void visit(ConstructorDefStmt const& stmt) override;
        void visit(DestructorDefStmt const& /*stmt*/) override { }
        void visit(GenericParam const& stmt) override;
        void visit(InterfaceDefStmt const& stmt) override;
        void visit(ClassDefStmt const& stmt) override;
        // -----
    private:
        void process_unclassified(
            std::vector<FunctionDefStmt*> const& functions,
            std::vector<GlobalVarDefStmt*> const& vars
        );
        void process_package(Scope const& scope);
        void process_identifier(AccessModifier const& access);
        // -----
        template<v_astfri_nodes SuperTypes>
        void process_supertypes(SuperTypes const& types, bool useExtends);
    };

    //
    // -----
    //

    template<v_astfri_nodes SuperTypes>
    void JavaCodeVisitor::process_supertypes(SuperTypes const& types, bool useExtends)
    {
        if (types.empty())
        {
            return;
        }
        if (useExtends)
        {
            builder_->append_text("extends ");
        }
        else
        {
            builder_->append_text("implements ");
        }
        for (size_t i = 0; i < types.size(); ++i)
        {
            builder_->append_text(types.at(i)->name_);
            if (i < types.size() - 1)
            {
                builder_->append_text(", ");
            }
        }
        builder_->append_space();
    }
}

#endif