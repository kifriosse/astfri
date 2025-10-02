#ifndef LIBASTFRI_TEXT_AST_VISITOR
#define LIBASTFRI_TEXT_AST_VISITOR

#include <libastfri-text/inc/HtmlFileExporter.hpp>
#include <libastfri-text/inc/TxtFileExporter.hpp>
#include <libastfri/inc/Stmt.hpp>

using namespace astfri;

class ASTVisitor : public IVisitor
{
    std::shared_ptr<TextConfigurator> configurator_;
    std::unique_ptr<Exporter> exporter_;
    std::unique_ptr<std::stringstream> currClassName_;
    std::unique_ptr<std::stringstream> currInterfName_;
    std::unique_ptr<std::vector<std::stringstream>> currGenParams_;
    bool isMethodCall_;
    bool isConstructorCall_;

public:
    ASTVisitor();
    void write_file();
    void reset_exporter();
    void reset_configurator();

public:
    void visit(DynamicType const& type) override;
    void visit(IntType const& type) override;
    void visit(FloatType const& type) override;
    void visit(CharType const& type) override;
    void visit(BoolType const& type) override;
    void visit(VoidType const& type) override;
    void visit(UserType const& type) override;
    void visit(IndirectionType const& type) override;
    void visit(UnknownType const& type) override;
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

private:
    void write_open_curl_bracket();
    void write_arrow();
    void write_body(Stmt* body);
    void write_cond(Expr* cond);
    void write_return_type(Type* type);
    void write_identifier_from_string(std::string classname);
    void write_initialization(VarDefStmt const* init);
    void write_generic_params_decl(std::vector<GenericParam*> const& vgeneric);
    void write_generic_params(std::vector<GenericParam*> const& vgeneric);
    void write_member_var_decl(std::vector<MemberVarDefStmt*> const& vmembervars);
    void write_member_var(std::vector<MemberVarDefStmt*>& vmembervars);
    void write_constructor_decl(std::vector<ConstructorDefStmt*> const& vconstructors);
    void write_constructor(std::vector<ConstructorDefStmt*>& vconstructors);
    void write_destructor_decl(std::vector<DestructorDefStmt*> const& vdestructors);
    void write_method_decl(std::vector<MethodDefStmt*> const& vmethods);
    void write_method(std::vector<MethodDefStmt*>& vmethods);
    template<typename VParams>
    void write_parameters(VParams const& vparams);
    template<typename VMembers>
    bool has_acc_mod(VMembers const& vstmts, VMembers& vfound, AccessModifier const& accmod);
    template<typename VRelations>
    void write_relations(VRelations const& vrel, bool const& ispolym);
};

template<typename VParams>
void ASTVisitor::write_parameters(VParams const& vparams)
{
    exporter_->write_round_bracket("(");
    for (size_t i = 0; i < vparams.size(); ++i)
    {
        vparams.at(i) ? vparams.at(i)->accept(*this) : exporter_->write_invalid_word();
        (i < vparams.size() - 1)     ? (exporter_->write_separator_sign(",")),
            exporter_->write_space() : void();
    }
    exporter_->write_round_bracket(")");
}

template<typename VMembers>
bool ASTVisitor::has_acc_mod(VMembers const& vstmts, VMembers& vfound, AccessModifier const& accmod)
{
    bool hasMod = false;
    for (size_t i = 0; i < vstmts.size(); ++i)
    {
        if (vstmts.at(i)->access_ == accmod)
        {
            vfound.push_back(vstmts.at(i));
            hasMod = true;
        }
    }
    return hasMod;
}

template<typename VRelations>
void ASTVisitor::write_relations(VRelations const& vrel, bool const& ispolym)
{
    for (size_t i = 0; i < vrel.size(); ++i)
    {
        exporter_->write_new_line();
        write_arrow();
        ispolym ? exporter_->write_implement_word() : exporter_->write_extend_word();
        exporter_->write_space();
        if (vrel.at(i))
        {
            ispolym ? exporter_->write_interface_name(vrel.at(i)->name_)
                    : exporter_->write_class_name(vrel.at(i)->name_);
            vrel.at(i)->tparams_.empty() ? void() : write_generic_params(vrel.at(i)->tparams_);
        }
        else
        {
            exporter_->write_invalid_word();
        }
    }
}

#endif