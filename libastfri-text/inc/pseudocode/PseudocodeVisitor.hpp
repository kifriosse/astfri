#ifndef LIBASTFRI_TEXT_PSEUDOCODE_VISITOR
#define LIBASTFRI_TEXT_PSEUDOCODE_VISITOR

#include <libastfri-text/inc/AbstractVisitor.hpp>
#include <libastfri-text/inc/pseudocode/AbstractTextBuilder.hpp>

namespace astfri::text
{
    class PseudocodeVisitor : public AbstractVisitor
    {
    private:
        AbstractTextBuilder* builder_;
        TextConfigurator* configurator_;
    private:
        bool isMethodCall_;
        bool isConstructorCall_;
    public:
        static PseudocodeVisitor& get_instance();
        PseudocodeVisitor(PseudocodeVisitor const&) = delete;
        PseudocodeVisitor(PseudocodeVisitor&&)      = delete;
        PseudocodeVisitor& operator=(PseudocodeVisitor const&) = delete;
        PseudocodeVisitor& operator=(PseudocodeVisitor&&)      = delete;
    private:
        PseudocodeVisitor() = delete;
        explicit PseudocodeVisitor(AbstractTextBuilder& builder);
        ~PseudocodeVisitor() = default;
    public:
        void export_pseudocode();
        void clear_builder();
        void append_text(std::string const& text);
        void append_new_line();
        void append_space();
        void update_configuration();
        void reload_configuration();
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
        void visit(IncompleteType const& type) override;
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
        void visit(BracketExpr const& expr) override;
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
        void visit(ForEachStmt const& /*stmt*/) override {} // TODO
        void visit(ThrowStmt const& stmt) override;
        void visit(CatchStmt const& /*stmt*/) override {} // TODO
        void visit(TryStmt const& /*stmt*/) override {} // TODO
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
    private:
        void process_var_def(VarDefStmt const& var, int vartype);
        void process_return_type(Type const* const& type);
        void process_generic_params_decl(std::vector<GenericParam*> const& vgeneric);
        void process_member_var_decl(std::vector<MemberVarDefStmt*> const& vmembervars);
        void process_member_var(std::vector<MemberVarDefStmt*>& vmembervars);
        void process_constructor_decl(std::vector<ConstructorDefStmt*> const& vconstructors);
        void process_constructor(std::vector<ConstructorDefStmt*>& constr);
        void process_destructor_decl(std::vector<DestructorDefStmt*> const& vdestructors);
        void process_method_decl(std::vector<MethodDefStmt*> const& vmethods);
        void process_method(std::vector<MethodDefStmt*>& meth);
        // -----
        template<typename VectorRelation>
        void process_relations(VectorRelation const& vrel, bool ispolym);
    };

    //
    // -----
    //

    template<typename VectorRelation>
    void PseudocodeVisitor::process_relations(VectorRelation const& vrel, bool ispolym)
    {
        for (size_t i = 0; i < vrel.size(); ++i)
        {
            builder_->append_new_line();
            builder_->append_space();
            builder_->write_separator("->");
            builder_->append_space();
            if (ispolym)
            {
                builder_->write_implement_word();
            }
            else
            {
                builder_->write_extend_word();
            }
            builder_->append_space();
            if (vrel.at(i))
            {
                if (ispolym)
                {
                    //builder_->write_interface_name(vrel.at(i)->name_);
                    //builder_->write_interface_name(vrel.at(i)->type->name_);
                    builder_->write_interface_name("TEMPORARY SOLUTION"); // TODO ask for change
                }
                else
                {
                    //builder_->write_class_name(vrel.at(i)->name_);
                    //builder_->write_class_name(vrel.at(i)->m_type_->name_);
                    builder_->write_class_name("TEMPORARY SOLUTION"); // TODO ask for change
                }
                if (!vrel.at(i)->tparams_.empty())
                {
                    process_pargs(vrel.at(i)->tparams_, false);
                }
            }
            else
            {
                builder_->write_invalid_expr();
            }
        }
    }
}

#endif