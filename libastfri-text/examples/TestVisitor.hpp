#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <iostream>

struct TestVisitor : astfri::VisitorAdapter {

    const std::string odsadenie_ = "    ";
    int aktualnyLevelOds_ = 0;

    void visit (astfri::DynamicType const& /*type*/) override { std::cout << "dynamic"; }

    void visit (astfri::IntType const& /*type*/) override { std::cout << "int"; }

    void visit (astfri::FloatType const& /*type*/) override { std::cout << "float"; }

    void visit (astfri::CharType const& /*type*/) override { std::cout << "char"; }

    void visit (astfri::BoolType const& /*type*/) override { std::cout << "bool"; }

    void visit (astfri::VoidType const& /*type*/) override { std::cout << "void"; }

    void visit (astfri::UserType const& type) override { std::cout << type.name_; }

    void visit (astfri::IndirectionType const& type) override { type.indirect_->accept(*this); }

    void visit (astfri::UnknownType const& /*type*/) override { std::cout << "UNKNOWN_TYPE"; }





    void visit (astfri::IntLiteralExpr const& expr) override { std::cout << expr.val_; }

    void visit (astfri::FloatLiteralExpr const& expr) override { std::cout << expr.val_ << "f"; }

    void visit (astfri::CharLiteralExpr const& expr) override { std::cout << "'" << expr.val_ << "'"; }

    void visit (astfri::StringLiteralExpr const& expr) override { std::cout << "\"" << expr.val_ << "\""; }

    void visit (astfri::BoolLiteralExpr const& expr) override { expr.val_ ? std::cout << "true" : std::cout << "false"; }
    
    void visit (astfri::NullLiteralExpr const& /*expr*/) override { std::cout << "NULL"; }

    void visit (astfri::IfExpr const& expr) override {
        for (int i = 0; i < aktualnyLevelOds_; ++i) {
            std::cout << odsadenie_;
        }
        std::cout << "if (";
        expr.cond_->accept(*this);
        std::cout << ") {\n";
        ++aktualnyLevelOds_;
        expr.iftrue_->accept(*this);
        --aktualnyLevelOds_;
        for (int i = 0; i < aktualnyLevelOds_; ++i) {
            std::cout << odsadenie_;
        }
        std::cout << "\n} else {\n";
        ++aktualnyLevelOds_;
        expr.iffalse_->accept(*this);
        --aktualnyLevelOds_;
        for (int i = 0; i < aktualnyLevelOds_; ++i) {
            std::cout << odsadenie_;
        }
        std::cout << "\n}\n\n";
    }

    void visit (astfri::ParamVarRefExpr const& expr) override { std::cout << expr.param_; }

    void visit (astfri::LocalVarRefExpr const& expr) override { std::cout << expr.var_; }

    void visit (astfri::MemberVarRefExpr const& expr) override { std::cout << expr.member_; }

    void visit (astfri::GlobalVarRefExpr const& expr) override { std::cout << expr.global_; }

    void visit (astfri::ThisExpr const& /*expr*/) override { std::cout << "this"; }

    void visit (astfri::UnknownExpr const& /*expr*/) override { std::cout << "UNKNOWN_EXPRESSION"; }





    void visit (astfri::TranslationUnit const& stmt) override {
        for (auto* gl : stmt.globals_) {
            gl->accept(*this);
        }
        for (auto* fun : stmt.functions_) {
            fun->accept(*this);
        }
        for (auto* cl : stmt.classes_) {
            cl->accept(*this);
        }
    }

    void visit (astfri::ReturnStmt const& stmt) override {
        for (int i = 0; i < aktualnyLevelOds_; ++i) {
            std::cout << odsadenie_;
        }
        std::cout << "return ";
        stmt.val_->accept(*this);
        std::cout << "\n";
    }

    void visit (astfri::ExprStmt const& stmt) override { stmt.expr_->accept(*this); }

    void visit (astfri::ThrowStmt const& stmt) override {
        for (int i = 0; i < aktualnyLevelOds_; ++i) {
            std::cout << odsadenie_;
        }
        std::cout << "throw ";
        stmt.val_->accept(*this);
        std::cout << "\n";
    }

    void visit (astfri::UnknownStmt const& /*stmt*/) override {
        for (int i = 0; i < aktualnyLevelOds_; ++i) {
            std::cout << odsadenie_;
        }
        std::cout << "UNKNOWN_STATEMENT";
    }
};