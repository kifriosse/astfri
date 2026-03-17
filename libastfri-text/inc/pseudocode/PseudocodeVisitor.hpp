#ifndef LIBASTFRI_TEXT_PSEUDOCODE_VISITOR
#define LIBASTFRI_TEXT_PSEUDOCODE_VISITOR

#include <libastfri-text/inc/AbstractVisitor.hpp>
#include <libastfri-text/inc/pseudocode/AbstractTextBuilder.hpp>

namespace astfri::text {
class PseudocodeVisitor : public AbstractVisitor {
private:
    AbstractTextBuilder* builder_;
    TextConfigurator* configurator_;

private:
    bool isMethodCall_;
    bool isConstructorCall_;

public:
    static PseudocodeVisitor& get_instance();
    PseudocodeVisitor(const PseudocodeVisitor&)            = delete;
    PseudocodeVisitor(PseudocodeVisitor&&)                 = delete;
    PseudocodeVisitor& operator=(const PseudocodeVisitor&) = delete;
    PseudocodeVisitor& operator=(PseudocodeVisitor&&)      = delete;

private:
    PseudocodeVisitor() = delete;
    explicit PseudocodeVisitor(AbstractTextBuilder& builder);
    ~PseudocodeVisitor() = default;

public:
    void export_pseudocode();
    void clear_builder();
    void append_text(const std::string& text);
    void append_new_line();
    void append_space();
    void update_configuration();
    void reload_configuration();
    // -----
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
    void visit(const UnknownType& type) override;
    // -----
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
    void visit(const ConstructorCallExpr& expr) override;
    void visit(const NewExpr& expr) override;
    void visit(const DeleteExpr& expr) override;
    void visit(const BracketExpr& expr) override;
    void visit(const UnknownExpr& expr) override;
    // -----
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
    void visit(const MemberVarDefStmt& stmt) override;
    void visit(const GlobalVarDefStmt& stmt) override;
    void visit(const FunctionDefStmt& stmt) override;
    void visit(const DefStmt& stmt) override;
    void visit(const MethodDefStmt& stmt) override;
    void visit(const BaseInitializerStmt& stmt) override;
    void visit(const ConstructorDefStmt& stmt) override;
    void visit(const DestructorDefStmt& stmt) override;
    void visit(const GenericParam& stmt) override;
    void visit(const InterfaceDefStmt& stmt) override;
    void visit(const ClassDefStmt& stmt) override;
    void visit(const ContinueStmt& stmt) override;
    void visit(const BreakStmt& stmt) override;

private:
    void process_var_def(const VarDefStmt& var, int vartype);
    void process_return_type(const Type* const& type);
    void process_generic_params_decl(const std::vector<GenericParam*>& vgeneric);
    void process_member_var_decl(const std::vector<MemberVarDefStmt*>& vmembervars);
    void process_member_var(std::vector<MemberVarDefStmt*>& vmembervars);
    void process_constructor_decl(const std::vector<ConstructorDefStmt*>& vconstructors);
    void process_constructor(std::vector<ConstructorDefStmt*>& constr);
    void process_destructor_decl(const std::vector<DestructorDefStmt*>& vdestructors);
    void process_method_decl(const std::vector<MethodDefStmt*>& vmethods);
    void process_method(std::vector<MethodDefStmt*>& meth);
    // -----
    template<typename VectorRelation>
    void process_relations(const VectorRelation& vrel, bool ispolym);
};

//
// -----
//

template<typename VectorRelation>
void PseudocodeVisitor::process_relations(const VectorRelation& vrel, bool ispolym) {
    for (size_t i = 0; i < vrel.size(); ++i) {
        builder_->append_new_line();
        builder_->append_space();
        builder_->write_separator("->");
        builder_->append_space();
        if (ispolym) {
            builder_->write_implement_word();
        }
        else {
            builder_->write_extend_word();
        }
        builder_->append_space();
        if (vrel.at(i)) {
            if (ispolym) {
                // builder_->write_interface_name(vrel.at(i)->name_);
                // builder_->write_interface_name(vrel.at(i)->type->name_);
                builder_->write_interface_name("TEMPORARY SOLUTION"); // TODO ask for change
            }
            else {
                // builder_->write_class_name(vrel.at(i)->name_);
                // builder_->write_class_name(vrel.at(i)->m_type_->name_);
                builder_->write_class_name("TEMPORARY SOLUTION"); // TODO ask for change
            }
            if (! vrel.at(i)->tparams.empty()) {
                process_pargs(vrel.at(i)->tparams, false);
            }
        }
        else {
            builder_->write_invalid_expr();
        }
    }
}
} // namespace astfri::text

#endif