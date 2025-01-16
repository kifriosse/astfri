#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

struct CodeVisitor : astfri::IVisitor {
private:
    int aktualnyStupenOdsadenia_ = 0;
    bool zacatyRiadok_ = false;
    std::string*& output_;
public:
    inline CodeVisitor(std::string*& output) : output_(output) {};
    void visit(astfri::TranslationUnit const& stmt) override;
private:
    inline void visit(astfri::DynamicType const& /*type*/) override { this->vypisVyraz("dynamic"); }
    inline void visit(astfri::IntType const& /*type*/) override { this->vypisVyraz("int"); }
    inline void visit(astfri::FloatType const& /*type*/) override { this->vypisVyraz("float"); }
    inline void visit(astfri::CharType const& /*type*/) override { this->vypisVyraz("char"); }
    inline void visit(astfri::BoolType const& /*type*/) override { this->vypisVyraz("bool"); }
    inline void visit(astfri::VoidType const& /*type*/) override { this->vypisVyraz("void"); }
    inline void visit(astfri::UserType const& type) override { this->vypisVyraz(type.name_); }
    inline void visit(astfri::IndirectionType const& type) override { type.indirect_->accept(*this); }
    inline void visit(astfri::UnknownType const& /*type*/) override { this->vypisVyraz("UNKNOWN_TYPE"); }

    inline void visit(astfri::IntLiteralExpr const& expr) override { this->vypisVyraz(std::to_string(expr.val_)); }
    inline void visit(astfri::FloatLiteralExpr const& expr) override { this->vypisVyraz(std::to_string(expr.val_) + "f"); }
    inline void visit(astfri::CharLiteralExpr const& expr) override { this->vypisVyraz("'" + std::to_string(expr.val_) + "'"); }
    inline void visit(astfri::StringLiteralExpr const& expr) override { this->vypisVyraz("\"" + expr.val_ + "\""); }
    inline void visit(astfri::BoolLiteralExpr const& expr) override { expr.val_ ? this->vypisVyraz("true") : this->vypisVyraz("false"); }
    inline void visit(astfri::NullLiteralExpr const& /*expr*/) override { this->vypisVyraz("NULL"); }
    void visit(astfri::IfExpr const& expr) override;
    void visit(astfri::BinOpExpr const& expr) override;
    void visit(astfri::UnaryOpExpr const& expr) override;
    void visit(astfri::AssignExpr const& expr) override;
    void visit(astfri::CompoundAssignExpr const& expr) override;
    inline void visit(astfri::ParamVarRefExpr const& expr) override { this->vypisVyraz(expr.param_); }
    inline void visit(astfri::LocalVarRefExpr const& expr) override { this->vypisVyraz(expr.var_); }
    inline void visit(astfri::MemberVarRefExpr const& expr) override { this->vypisVyraz(expr.member_); }
    inline void visit(astfri::GlobalVarRefExpr const& expr) override { this->vypisVyraz(expr.global_); }
    void visit(astfri::FunctionCallExpr const& expr) override;
    void visit(astfri::MethodCallExpr const& expr) override;
    void visit(astfri::LambdaExpr const& expr) override;
    inline void visit(astfri::ThisExpr const& /*expr*/) override { this->vypisVyraz("this"); }
    inline void visit(astfri::UnknownExpr const& /*expr*/) override { this->vypisVyraz("UNKNOWN_EXPRESSION"); }

    void visit(astfri::CompoundStmt const& stmt) override;
    void visit(astfri::ReturnStmt const& stmt) override;
    inline void visit(astfri::ExprStmt const& stmt) override { stmt.expr_->accept(*this); }
    void visit(astfri::IfStmt const& stmt) override;
    void visit(astfri::CaseStmt const& stmt) override;
    void visit(astfri::SwitchStmt const& stmt) override;
    void visit(astfri::WhileStmt const& stmt) override;
    void visit(astfri::DoWhileStmt const& stmt) override;
    void visit(astfri::ForStmt const& stmt) override;
    void visit(astfri::ThrowStmt const& stmt) override;
    inline void visit(astfri::UnknownStmt const& /*stmt*/) override { this->vypisVyraz("UNKNOWN_STATEMENT"); }
    void visit(astfri::LocalVarDefStmt const& stmt) override;
    void visit(astfri::ParamVarDefStmt const& stmt) override;
    void visit(astfri::MemberVarDefStmt const& stmt) override;
    void visit(astfri::GlobalVarDefStmt const& stmt) override;
    void visit(astfri::FunctionDefStmt const& stmt) override;
    void visit(astfri::MethodDefStmt const& stmt) override;
    void visit(astfri::ClassDefStmt const& stmt) override;
private:
    inline void odsad() {
        for (int i = 0; i < this->aktualnyStupenOdsadenia_; ++i) {
            this->output_->append("    ");
            this->zacatyRiadok_ = true;
        }
    }
    inline void vypisVyraz(const std::string& s, bool mamNovyRiadok = false) {
        if (!this->zacatyRiadok_) {
            this->odsad();
        } 
        this->output_->append(s);
        if (mamNovyRiadok) {
            this->zacatyRiadok_ = false;
        }
    }
};