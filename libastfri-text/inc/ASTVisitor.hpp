#ifndef LIBASTFRI_TEXT_INC_AST_VISITOR
#define LIBASTFRI_TEXT_INC_AST_VISITOR

#include <libastfri-text/inc/HtmlFileExporter.hpp>
#include <libastfri-text/inc/RtfFileExporter.hpp>
#include <libastfri-text/inc/TxtFileExporter.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

struct ASTVisitor : astfri::IVisitor {
private:
    std::shared_ptr<TextConfigurator> config_;
    std::unique_ptr<Exporter> exporter_;
public:
    ASTVisitor();
    void write_file() { exporter_->make_export(); };
    void visit(astfri::DynamicType const& /*type*/) override { exporter_->write_dynamic_type(); };
    void visit(astfri::IntType const& /*type*/) override { exporter_->write_int_type(); };
    void visit(astfri::FloatType const& /*type*/) override { exporter_->write_float_type(); };
    void visit(astfri::CharType const& /*type*/) override { exporter_->write_char_type(); };
    void visit(astfri::BoolType const& /*type*/) override { exporter_->write_bool_type(); };
    void visit(astfri::VoidType const& /*type*/) override { exporter_->write_void_type(); };
    void visit(astfri::UserType const& type) override { exporter_->write_user_type(type.name_); };
    void visit(astfri::IndirectionType const& type) override { type.indirect_->accept(*this); };
    void visit(astfri::UnknownType const& /*type*/) override { exporter_->write_unknown_word(); };
    void visit(astfri::IntLiteralExpr const& expr) override { exporter_->write_int_val(expr.val_); };
    void visit(astfri::FloatLiteralExpr const& expr) override { exporter_->write_float_val(expr.val_); };
    void visit(astfri::CharLiteralExpr const& expr) override { exporter_->write_char_val(expr.val_); };
    void visit(astfri::StringLiteralExpr const& expr) override { exporter_->write_string_val(expr.val_); };
    void visit(astfri::BoolLiteralExpr const& expr) override { exporter_->write_bool_val(expr.val_); };
    void visit(astfri::NullLiteralExpr const& /*expr*/) override { exporter_->write_null_val(); };
    void visit(astfri::IfExpr const& expr) override;
    void visit(astfri::BinOpExpr const& expr) override;
    void visit(astfri::UnaryOpExpr const& expr) override;
    void visit(astfri::AssignExpr const& expr) override;
    void visit(astfri::CompoundAssignExpr const& expr) override;
    void visit(astfri::ParamVarRefExpr const& expr) override { exporter_->write_param_var_name(expr.param_); };
    void visit(astfri::LocalVarRefExpr const& expr) override { exporter_->write_local_var_name(expr.var_); };
    void visit(astfri::MemberVarRefExpr const& expr) override { exporter_->write_member_var_name(expr.member_); };
    void visit(astfri::GlobalVarRefExpr const& expr) override { exporter_->write_global_var_name(expr.global_); };
    void visit(astfri::ClassRefExpr const& expr) override;
    void visit(astfri::FunctionCallExpr const& expr) override;
    void visit(astfri::MethodCallExpr const& expr) override;
    void visit(astfri::LambdaExpr const& expr) override;
    void visit(astfri::ThisExpr const& /*expr*/) override { exporter_->write_this_word(); };
    void visit(astfri::ConstructorCallExpr const& expr) override;
    void visit(astfri::NewExpr const& expr) override;
    void visit(astfri::DeleteExpr const& expr) override;
    void visit(astfri::UnknownExpr const& /*expr*/) override { exporter_->write_unknown_word(); };
    void visit(astfri::TranslationUnit const& stmt) override;
    void visit(astfri::CompoundStmt const& stmt) override;
    void visit(astfri::ReturnStmt const& stmt) override;
    void visit(astfri::ExprStmt const& stmt) override { stmt.expr_->accept(*this); };
    void visit(astfri::IfStmt const& stmt) override;
    void visit(astfri::CaseStmt const& stmt) override;
    void visit(astfri::SwitchStmt const& stmt) override;
    void visit(astfri::WhileStmt const& stmt) override;
    void visit(astfri::DoWhileStmt const& stmt) override;
    void visit(astfri::ForStmt const& stmt) override;
    void visit(astfri::ThrowStmt const& stmt) override;
    void visit(astfri::UnknownStmt const& /*stmt*/) override { exporter_->write_unknown_word(); };
    void visit(astfri::LocalVarDefStmt const& stmt) override;
    void visit(astfri::ParamVarDefStmt const& stmt) override;
    void visit(astfri::MemberVarDefStmt const& stmt) override;
    void visit(astfri::GlobalVarDefStmt const& stmt) override;
    void visit(astfri::FunctionDefStmt const& stmt) override;
    void visit(astfri::DefStmt const& stmt) override;
    void visit(astfri::MethodDefStmt const& stmt) override;
    void visit(astfri::BaseInitializerStmt const& stmt) override;
    void visit(astfri::ConstructorDefStmt const& stmt) override;
    void visit(astfri::DestructorDefStmt const& stmt) override;
    void visit(astfri::GenericParam const& stmt) override;
    void visit(astfri::ClassDefStmt const& stmt) override;
private:
    void write_open_bracket();
};

#endif