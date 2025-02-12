#include <libastfri-text/inc/TxtFileExporter.hpp>
#include <libastfri-text/inc/RtfFileExporter.hpp>
#include <libastfri-text/inc/HtmlFileExporter.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

struct ASTVisitor : astfri::IVisitor {
private:
    const Configurator* config_;
    Exporter* exporter_;
public:
    ASTVisitor(const Configurator* conf, std::stringstream* output);
    ~ASTVisitor();
    void write_file() { exporter_->make_export(); };
    void visit(astfri::DynamicType const& /*type*/) override { exporter_->write_word("dynamic"); };
    void visit(astfri::IntType const& /*type*/) override { exporter_->write_int_type(); };
    void visit(astfri::FloatType const& /*type*/) override { exporter_->write_float_type(); };
    void visit(astfri::CharType const& /*type*/) override { exporter_->write_char_type(); };
    void visit(astfri::BoolType const& /*type*/) override { exporter_->write_bool_type(); };
    void visit(astfri::VoidType const& /*type*/) override { exporter_->write_void_type(); };
    void visit(astfri::UserType const& type) override { exporter_->write_word(type.name_); };
    void visit(astfri::IndirectionType const& type) override { type.indirect_->accept(*this); };
    void visit(astfri::UnknownType const& /*type*/) override { exporter_->write_word("UNKNOWN_TYPE"); };
    void visit(astfri::IntLiteralExpr const& expr) override { exporter_->write_word(std::to_string(expr.val_)); };
    void visit(astfri::FloatLiteralExpr const& expr) override { exporter_->write_word(std::to_string(expr.val_) + "f"); };
    void visit(astfri::CharLiteralExpr const& expr) override { exporter_->write_word("'" + std::to_string(expr.val_) + "'"); };
    void visit(astfri::StringLiteralExpr const& expr) override { exporter_->write_word("\"" + expr.val_ + "\""); };
    void visit(astfri::BoolLiteralExpr const& expr) override { expr.val_ ? exporter_->write_word("true") : exporter_->write_word("false"); };
    void visit(astfri::NullLiteralExpr const& /*expr*/) override { exporter_->write_word("NULL"); };
    void visit(astfri::IfExpr const& expr) override;
    void visit(astfri::BinOpExpr const& expr) override;
    void visit(astfri::UnaryOpExpr const& expr) override;
    void visit(astfri::AssignExpr const& expr) override;
    void visit(astfri::CompoundAssignExpr const& expr) override;
    void visit(astfri::ParamVarRefExpr const& expr) override { exporter_->write_word(expr.param_); };
    void visit(astfri::LocalVarRefExpr const& expr) override { exporter_->write_word(expr.var_); };
    void visit(astfri::MemberVarRefExpr const& expr) override { exporter_->write_word(expr.member_); };
    void visit(astfri::GlobalVarRefExpr const& expr) override { exporter_->write_word(expr.global_); };
    void visit(astfri::FunctionCallExpr const& expr) override;
    void visit(astfri::MethodCallExpr const& expr) override;
    void visit(astfri::LambdaExpr const& expr) override;
    void visit(astfri::ThisExpr const& /*expr*/) override { exporter_->write_this_word(); };
    void visit(astfri::UnknownExpr const& /*expr*/) override { exporter_->write_word("UNKNOWN_EXPRESSION"); };
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
    void visit(astfri::UnknownStmt const& /*stmt*/) override { exporter_->write_word("UNKNOWN_STATEMENT"); };
    void visit(astfri::LocalVarDefStmt const& stmt) override;
    void visit(astfri::ParamVarDefStmt const& stmt) override;
    void visit(astfri::MemberVarDefStmt const& stmt) override;
    void visit(astfri::GlobalVarDefStmt const& stmt) override;
    void visit(astfri::FunctionDefStmt const& stmt) override;
    void visit(astfri::MethodDefStmt const& stmt) override;
    void visit(astfri::ClassDefStmt const& stmt) override;
};