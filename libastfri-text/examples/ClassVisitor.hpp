#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri/inc/Visitor.hpp>
#include <iostream>

struct ClassVisitor : astfri::VisitorAdapter {
    void visit(astfri::ClassDefStmt const& stmt) override {
        std::cout << "class " << stmt.name_ << " {";
        if (!stmt.vars_.empty()) {
            std::cout << "\nprivate:";
            for (astfri::MemberVarDefStmt* var : stmt.vars_) {
                this->visit(*var);
            }
        }
        if (!stmt.methods_.empty()) {
            std::cout << "\npublic:";
            for (astfri::MethodDefStmt* meth : stmt.methods_) {
                std::cout << "\n    ";
                if (meth->owner_->name_ == meth->func_->name_ || meth->func_->name_.at(0) == '~') {
                    std::cout << meth->func_->name_ << "(";
                } else {
                    meth->func_->retType_->accept(*this);
                    std::cout << " " << meth->func_->name_ << "(";
                }
                if (!meth->func_->params_.empty()) {
                    auto st = meth->func_->params_.begin();
                    auto en = meth->func_->params_.end();
                    while (st != en) {
                        this->visit(*(*st));
                        ++st;
                        if (st != en) {
                            std::cout << ", ";
                        }
                    }
                }
                std::cout << ")" ;
            }
        }
        std::cout << "\n}\n";
        if (!stmt.methods_.empty()) {
            for (astfri::MethodDefStmt* meth : stmt.methods_) {
                std::cout << "\n";
                this->visit(*meth);
            }
            std::cout << "\n";
        }
    }
private:
    void visit(astfri::MemberVarDefStmt const& stmt) override {
        std::cout << "\n    ";
        stmt.type_->accept(*this);
        std::cout << " " << stmt.name_;
        if (stmt.initializer_) {
            std::cout << " <- ";
            stmt.initializer_->accept(*this);
        }
    }
    void visit(astfri::IntType const&) override {
        std::cout << "int";
    }
    void visit(astfri::UserType const& type) override {
        std::cout << type.name_;
    }
    void visit(astfri::IntLiteralExpr const& expr) override {
        std::cout << expr.val_;
    }
    void visit(astfri::StringLiteralExpr const& expr) override {
        std::cout << "\"" << expr.val_ << "\"";
    }
    void visit(astfri::MethodDefStmt const& stmt) override {
        std::cout << stmt.owner_->name_ << "::" << stmt.func_->name_ << "(";
        auto st = stmt.func_->params_.begin();
        auto en = stmt.func_->params_.end();
        while (st != en) {
            this->visit(*(*st));
            ++st;
            if (st != en) {
                std::cout << ", ";
            }
        }
        std::cout << ")" << " -> ";
        stmt.func_->retType_->accept(*this);
        std::cout << " {";
        if (stmt.func_->body_) {
            this->visit(*stmt.func_->body_);
            std::cout << "\n";
        }
        std::cout << "}";
    }
    void visit(astfri::ParamVarDefStmt const& stmt) override {
        stmt.type_->accept(*this);
        std::cout << " " << stmt.name_;
        if (stmt.initializer_) {
            std::cout << " <- ";
            stmt.initializer_->accept(*this);
        }
    }
    void visit(astfri::CompoundStmt const& stmt) override {
        for (astfri::Stmt* st : stmt.stmts_) {
            st->accept(*this);
        }
    }
    void visit(astfri::ReturnStmt const& stmt) override {
        std::cout << "\n    return ";
        stmt.val_->accept(*this);
    }
    void visit(astfri::BinOpExpr const& expr) override {
        std::cout << "(";
        expr.left_->accept(*this);
        std::cout << " ";
        switch (expr.op_) {
            case astfri::BinOpType::Assign:
                std::cout << "=";
                break;
            case astfri::BinOpType::Add:
                std::cout << "+";
                break;
            case astfri::BinOpType::Subtract:
                std::cout << "-";
                break;
            case astfri::BinOpType::Multiply:
                std::cout << "*";
                break;
            case astfri::BinOpType::Divide:
                std::cout << "/";
                break;
            case astfri::BinOpType::Modulo:
                std::cout << "%";
                break;
            case astfri::BinOpType::Equal:
                std::cout << "==";
                break;
            case astfri::BinOpType::NotEqual:
                std::cout << "!=";
                break;
            case astfri::BinOpType::Less:
                std::cout << "<";
                break;
            case astfri::BinOpType::LessEqual:
                std::cout << "<=";
                break;
            case astfri::BinOpType::Greater:
                std::cout << ">";
                break;
            case astfri::BinOpType::GreaterEqual:
                std::cout << ">=";
                break;
        }
        std::cout << " ";
        expr.right_->accept(*this);
        std::cout << ")";
    }
    void visit(astfri::MemberVarRefExpr const& expr) override {
        std::cout << expr.member_;
    }
    void visit(astfri::ParamVarRefExpr const& expr) override {
        std::cout << expr.param_;
    }
    void visit(astfri::ExprStmt const& stmt) override {
        stmt.expr_->accept(*this);
    }
    void visit(astfri::AssignExpr const& expr) override {
        std::cout << "\n    ";
        expr.lhs_->accept(*this);
        std::cout << " = ";
        expr.rhs_->accept(*this);
    }
};