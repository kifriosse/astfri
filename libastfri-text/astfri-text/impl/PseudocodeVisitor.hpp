#ifndef LIBASTFRI_TEXT_PSEUDOCODE_VISITOR
#define LIBASTFRI_TEXT_PSEUDOCODE_VISITOR

#include <astfri-text/impl/AbstractVisitor.hpp>
#include <astfri-text/impl/PseudocodeBuilder.hpp>

namespace astfri::text
{
    class PseudocodeVisitor : public AbstractVisitor
    {
        PseudocodeBuilder* m_builder;
        Config* m_config;
        bool m_isMethodCall = false;
        bool m_isConstructorCall = false;
    public:
        PseudocodeVisitor(PseudocodeBuilder* builder, Config* config);
        //
        void reset_visitor() override;
        void replace_builder(AbstractBuilder* builder) override;
        //
        void visit(const DynamicType& type) override;
        void visit(const IntType& type) override;
        void visit(const FloatType& type) override;
        void visit(const CharType& type) override;
        void visit(const BoolType& type) override;
        void visit(const VoidType& type) override;
        void visit(const IndirectionType& type) override;
        void visit(const ClassType& type) override;
        void visit(const InterfaceType& type) override;
        void visit(const LambdaType& type) override;
        void visit(const IncompleteType& type) override;
        void visit(const DeducedType& type) override;
        void visit(const UnknownType& type) override;
        //
        void visit(const IntLiteralExpr& expr) override;
        void visit(const FloatLiteralExpr& expr) override;
        void visit(const CharLiteralExpr& expr) override;
        void visit(const StringLiteralExpr& expr) override;
        void visit(const BoolLiteralExpr& expr) override;
        void visit(const NullLiteralExpr& expr) override;
        void visit(const IfExpr& expr) override;
        void visit(const BinOpExpr& expr) override;
        void visit(const UnaryOpExpr& expr) override;
        void visit(const ParamVarRefExpr& expr) override;
        void visit(const LocalVarRefExpr& expr) override;
        void visit(const MemberVarRefExpr& expr) override;
        void visit(const GlobalVarRefExpr& expr) override;
        void visit(const ClassRefExpr& expr) override;
        void visit(const FunctionCallExpr& expr) override;
        void visit(const MethodCallExpr& expr) override;
        void visit(const LambdaCallExpr& expr) override;
        void visit(const LambdaExpr& expr) override;
        void visit(const ThisExpr& expr) override;
        void visit(const BaseExpr& expr) override;
        void visit(const ConstructorCallExpr& expr) override;
        void visit(const NewExpr& expr) override;
        void visit(const DeleteExpr& expr) override;
        void visit(const BracketExpr& expr) override;
        void visit(const UnknownExpr& expr) override;
        //
        void visit(const TranslationUnit& stmt) override;
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
        void visit(const ForEachStmt& stmt) override;
        void visit(const ThrowStmt& stmt) override;
        void visit(const CatchStmt& stmt) override;
        void visit(const TryStmt& stmt) override;
        void visit(const UnknownStmt& stmt) override;
        void visit(const LocalVarDefStmt& stmt) override;
        void visit(const ParamVarDefStmt& stmt) override;
        void visit(const MemberVarDefStmt& stmt) override;
        void visit(const GlobalVarDefStmt& stmt) override;
        void visit(const FunctionDefStmt& stmt) override;
        void visit(const MultiVarDefStmt& stmt) override;
        void visit(const MethodDefStmt& stmt) override;
        void visit(const BaseInitializerStmt& stmt) override;
        void visit(const SelfInitializerStmt& stmt) override;
        void visit(const MemberInitializerStmt& stmt) override;
        void visit(const ConstructorDefStmt& stmt) override;
        void visit(const DestructorDefStmt& stmt) override;
        void visit(const GenericParam& stmt) override;
        void visit(const InterfaceDefStmt& stmt) override;
        void visit(const ClassDefStmt& stmt) override;
        void visit(const ContinueStmt& stmt) override;
        void visit(const BreakStmt& stmt) override;
    private:
        void process_var_def(const VarDefStmt& var, int vartype);
        void process_return_type(Type* const& type);
        void process_generic_params_decl(const std::vector<GenericParam*>& vgeneric);
        void process_member_var_decl(const std::vector<MemberVarDefStmt*>& vmembervars);
        void process_member_var(std::vector<MemberVarDefStmt*>& vmembervars);
        void process_constructor_decl(const std::vector<ConstructorDefStmt*>& vconstructors);
        void process_constructor(std::vector<ConstructorDefStmt*>& constr);
        void process_destructor_decl(const std::vector<DestructorDefStmt*>& vdestructors);
        void process_method_decl(const std::vector<MethodDefStmt*>& vmethods);
        void process_method(std::vector<MethodDefStmt*>& meth);
        //
        template<typename VectorRelation>
        void process_relations(const VectorRelation& vrel, bool ispolym);
    };

    template<typename VectorRelation>
    void PseudocodeVisitor::process_relations(const VectorRelation& vrel, bool ispolym) {
        for (size_t i = 0; i < vrel.size(); ++i) {
            m_builder->write_new_line();
            m_builder->write_space();
            m_builder->write_separator("->");
            m_builder->write_space();
            if (ispolym) {
                m_builder->write_implement_word();
            }
            else {
                m_builder->write_extend_word();
            }
            m_builder->write_space();
            if (vrel.at(i)) {
                if (ispolym) {
                    // m_builder->write_interface_name(vrel.at(i)->name_);
                    // m_builder->write_interface_name(vrel.at(i)->type->name_);
                    m_builder->write_interface_name("TEMPORARY SOLUTION"); // TODO ask for change
                }
                else {
                    // m_builder->write_class_name(vrel.at(i)->name_);
                    // m_builder->write_class_name(vrel.at(i)->m_type_->name_);
                    m_builder->write_class_name("TEMPORARY SOLUTION"); // TODO ask for change
                }
                if (! vrel.at(i)->tparams.empty()) {
                    process_params_or_args(vrel.at(i)->tparams, false);
                }
            }
        }
    }
}

#endif