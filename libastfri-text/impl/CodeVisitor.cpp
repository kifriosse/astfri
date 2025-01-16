#include "libastfri-text/inc/CodeVisitor.hpp"

void CodeVisitor::visit(astfri::IfExpr const& expr) {
    this->vypisVyraz("if (");
    if (expr.cond_) {
        expr.cond_->accept(*this);
    }
    this->vypisVyraz(") {\n", true);
    if (expr.iftrue_) {
        ++this->aktualnyStupenOdsadenia_;
        expr.iftrue_->accept(*this);
        --this->aktualnyStupenOdsadenia_;
        this->vypisVyraz("\n", true);
    }
    this->vypisVyraz("} else {\n", true);
    if (expr.iffalse_) {
        ++this->aktualnyStupenOdsadenia_;
        expr.iffalse_->accept(*this);
        --this->aktualnyStupenOdsadenia_;
        this->vypisVyraz("\n", true);
    }
    this->vypisVyraz("}");
}

void CodeVisitor::visit(astfri::BinOpExpr const& expr) {
    if (expr.left_ && expr.right_) {
        expr.left_->accept(*this);
        switch (expr.op_) {
            case astfri::BinOpType::Add: this->output_->append(" + "); break;
            case astfri::BinOpType::Assign: this->output_->append(" = "); break;
            case astfri::BinOpType::Divide: this->output_->append(" / "); break;
            case astfri::BinOpType::Equal: this->output_->append(" == "); break;
            case astfri::BinOpType::GreaterEqual: this->output_->append(" >= "); break;
            case astfri::BinOpType::Greater: this->output_->append(" > "); break;
            case astfri::BinOpType::LessEqual: this->output_->append(" <= "); break;
            case astfri::BinOpType::Less: this->output_->append(" < "); break;
            case astfri::BinOpType::NotEqual: this->output_->append(" != "); break;
            case astfri::BinOpType::Modulo: this->output_->append(" % "); break;
            case astfri::BinOpType::Subtract: this->output_->append(" - "); break;
            case astfri::BinOpType::Multiply: this->output_->append(" * "); break;
        }
        expr.right_->accept(*this);
    }
}

void CodeVisitor::visit(astfri::UnaryOpExpr const& expr) {
    if (!expr.arg_) {
        return;
    }
    switch (expr.op_) {
        case astfri::UnaryOpType::AddressOf: this->vypisVyraz("&"); break;
        case astfri::UnaryOpType::Dereference: this->vypisVyraz("*"); break;
        case astfri::UnaryOpType::LogicalNot: this->vypisVyraz("!"); break;
        case astfri::UnaryOpType::Minus: this->vypisVyraz("-"); break;
        case astfri::UnaryOpType::Plus: this->vypisVyraz("+"); break;
    }
    expr.arg_->accept(*this);
}

void CodeVisitor::visit(astfri::AssignExpr const& expr) {
    if (expr.lhs_ && expr.rhs_) {
        expr.lhs_->accept(*this);
        this->output_->append(" = ");
        expr.rhs_->accept(*this);
    }
}

void CodeVisitor::visit(astfri::CompoundAssignExpr const& expr) {
    if (!expr.lhs_ || !expr.rhs_) {
        return;
    }
    expr.lhs_->accept(*this);
    switch (expr.op_) {
        case astfri::BinOpType::Add: this->output_->append(" += "); break;
        case astfri::BinOpType::Divide: this->output_->append(" /= "); break;
        case astfri::BinOpType::Modulo: this->output_->append(" %= "); break;
        case astfri::BinOpType::Subtract: this->output_->append(" -= "); break;
        case astfri::BinOpType::Multiply: this->output_->append(" *= "); break;
        default: this->output_->append(" "); break;
    }
    expr.rhs_->accept(*this);
}

void CodeVisitor::visit(astfri::FunctionCallExpr const& expr) {
    this->vypisVyraz(expr.name_);
    this->output_->append("(");
    for (size_t i = 0; i < expr.args_.size(); ++i) {
        if (expr.args_.at(i)) {
            expr.args_.at(i)->accept(*this);
            if (i < expr.args_.size() - 1) {
                this->output_->append(", ");
            }
        }
    }
    this->output_->append(")");
}

void CodeVisitor::visit(astfri::MethodCallExpr const& expr) {
    if (expr.owner_) {
        expr.owner_->accept(*this);
        this->output_->append("::");
    }
    this->vypisVyraz(expr.name_ + "(");
    for (size_t i = 0; i < expr.args_.size(); ++i) {
        if (expr.args_.at(i)) {
            expr.args_.at(i)->accept(*this);
            if (i < expr.args_.size() - 1) {
                this->output_->append(", ");
            }
        }
    }
    this->output_->append(")");
}

void CodeVisitor::visit(astfri::LambdaExpr const& expr) {
    this->vypisVyraz("(");
    for (size_t i = 0; i < expr.params_.size(); ++i) {
        if (expr.params_.at(i)) {
            expr.params_.at(i)->accept(*this);
            if (i < expr.params_.size() - 1) {
                this->output_->append(", ");
            }
        }
    }
    this->vypisVyraz(") {\n", true);
    if (expr.body_) {
        ++this->aktualnyStupenOdsadenia_;
        expr.body_->accept(*this);
        --this->aktualnyStupenOdsadenia_;
        this->vypisVyraz("\n", true);
    }
    this->vypisVyraz("}");
}

void CodeVisitor::visit(astfri::TranslationUnit const& stmt) {
    bool predch = false;
    for (auto a : stmt.globals_) {
        if (a) {
            a->accept(*this);
            this->vypisVyraz("\n", true);
            predch = true;
        }
    }
    for (auto a : stmt.functions_) {
        if (a) {
            if (predch) {
                this->vypisVyraz("\n", true);
            }
            a->accept(*this);
            this->vypisVyraz("\n", true);
            predch = true;
        }
    }
    for (auto a : stmt.classes_) {
        if (a) {
            if (predch) {
                this->vypisVyraz("\n", true);
            }
            a->accept(*this);
            predch = true;
        }
    }
}

void CodeVisitor::visit(astfri::CompoundStmt const& stmt) {
    for (size_t i = 0; i < stmt.stmts_.size(); ++i) {
        if (stmt.stmts_.at(i)) {
            stmt.stmts_.at(i)->accept(*this);
            if (i < stmt.stmts_.size() - 1) {
                this->vypisVyraz("\n", true);
            }
        }
    }
}

void CodeVisitor::visit(astfri::ReturnStmt const& stmt) {
    if (stmt.val_) {
        this->vypisVyraz("return ");
        stmt.val_->accept(*this);
    }
}

void CodeVisitor::visit(astfri::IfStmt const& stmt) {
    this->vypisVyraz("if (");
    if (stmt.cond_) {
        stmt.cond_->accept(*this);
    }
    this->vypisVyraz(") {\n", true);
    if (stmt.iftrue_) {
        ++this->aktualnyStupenOdsadenia_;
        stmt.iftrue_->accept(*this);
        --this->aktualnyStupenOdsadenia_;
        this->vypisVyraz("\n", true);
    }
    this->vypisVyraz("} else {\n", true);
    if (stmt.iffalse_) {
        ++this->aktualnyStupenOdsadenia_;
        stmt.iffalse_->accept(*this);
        --this->aktualnyStupenOdsadenia_;
        this->vypisVyraz("\n", true);
    }
    this->vypisVyraz("}");
}

void CodeVisitor::visit(astfri::CaseStmt const& stmt) {
    this->vypisVyraz("case ");
    if (stmt.expr_) {
        stmt.expr_->accept(*this);
    }
    this->vypisVyraz(":");
    if (stmt.body_) {
        this->vypisVyraz("\n", true);
        ++this->aktualnyStupenOdsadenia_;
        stmt.body_->accept(*this);
        --this->aktualnyStupenOdsadenia_;
    }
}

void CodeVisitor::visit(astfri::SwitchStmt const& stmt) {
    this->vypisVyraz("switch (");
    stmt.expr_ ? stmt.expr_->accept(*this) : void();
    this->vypisVyraz(") {\n", true);
    ++this->aktualnyStupenOdsadenia_;
    for (size_t i = 0; i < stmt.cases_.size(); ++i) {
        if (stmt.cases_.at(i)) {
            stmt.cases_.at(i)->accept(*this);
            if (i < stmt.cases_.size() - 1) {
                this->vypisVyraz("\n", true);
            }
        }
    }
    --this->aktualnyStupenOdsadenia_;
    this->vypisVyraz("\n", true);
    this->vypisVyraz("}");
}

void CodeVisitor::visit(astfri::WhileStmt const& stmt) {
    this->vypisVyraz("while (");
    stmt.cond_ ? stmt.cond_->accept(*this) : void();
    this->vypisVyraz(") {\n", true);
    if (stmt.body_) {
        ++this->aktualnyStupenOdsadenia_;
        stmt.body_->accept(*this);
        --this->aktualnyStupenOdsadenia_;
        this->vypisVyraz("\n", true);
    }
    this->vypisVyraz("}");
}

void CodeVisitor::visit(astfri::DoWhileStmt const& stmt) {
    this->vypisVyraz("do {\n", true);
    if (stmt.body_) {
        ++this->aktualnyStupenOdsadenia_;
        stmt.body_->accept(*this);
        --this->aktualnyStupenOdsadenia_;
        this->vypisVyraz("\n", true);
    }
    this->vypisVyraz("} while (");
    stmt.cond_ ? stmt.cond_->accept(*this) : void();
    this->output_->append(")");
}

void CodeVisitor::visit(astfri::ForStmt const& stmt) {
    this->vypisVyraz("for (");
    stmt.init_ ? stmt.init_->accept(*this) : void();
    this->output_->append("; ");
    stmt.cond_ ? stmt.cond_->accept(*this) : void();
    this->output_->append("; ");
    stmt.step_ ? stmt.step_->accept(*this) : void();
    this->vypisVyraz(") {\n", true);
    if (stmt.body_) {
        ++this->aktualnyStupenOdsadenia_;
        stmt.body_->accept(*this);
        --this->aktualnyStupenOdsadenia_;
        this->vypisVyraz("\n", true);
    }
    this->vypisVyraz("}");
}

void CodeVisitor::visit(astfri::ThrowStmt const& stmt) {
    if (stmt.val_) {
        this->vypisVyraz("throw ");
        stmt.val_->accept(*this);
    }
}

void CodeVisitor::visit(astfri::LocalVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        this->output_->append(" " + stmt.name_);
        if (stmt.initializer_) {
            this->output_->append(" = ");
            stmt.initializer_->accept(*this);
        }
    }
}

void CodeVisitor::visit(astfri::ParamVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        this->output_->append(" " + stmt.name_);
        if (stmt.initializer_) {
            this->output_->append(" = ");
            stmt.initializer_->accept(*this);
        }
    }
}

void CodeVisitor::visit(astfri::MemberVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        this->output_->append(" " + stmt.name_);
        if (stmt.initializer_) {
            this->output_->append(" = ");
            stmt.initializer_->accept(*this);
        }
    }
}

void CodeVisitor::visit(astfri::GlobalVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        this->output_->append(" " + stmt.name_);
        if (stmt.initializer_) {
            this->output_->append(" = ");
            stmt.initializer_->accept(*this);
        }
    }
}

void CodeVisitor::visit(astfri::FunctionDefStmt const& stmt) {
    if (!stmt.retType_) {
        return;
    }
    stmt.retType_->accept(*this);
    this->output_->append(" " + stmt.name_ + "(");
    for (size_t i = 0; i < stmt.params_.size(); ++i) {
        if (stmt.params_.at(i)) {
            stmt.params_.at(i)->accept(*this);
            if (i < stmt.params_.size() - 1) {
                this->output_->append(", ");
            }
        }
    }
    this->vypisVyraz(") {\n", true);
    if (stmt.body_) {
        ++this->aktualnyStupenOdsadenia_;
        stmt.body_->accept(*this);
        --this->aktualnyStupenOdsadenia_;
        this->vypisVyraz("\n", true);
    }
    this->vypisVyraz("}");
}

void CodeVisitor::visit(astfri::MethodDefStmt const& stmt) {
    if (!stmt.func_ || !stmt.owner_ || !stmt.func_->retType_) {
        return;
    }
    stmt.func_->retType_->accept(*this);
    this->output_->append(" " + stmt.owner_->name_ + "::" + stmt.func_->name_ + "(");
    for (size_t i = 0; i < stmt.func_->params_.size(); ++i) {
        if (stmt.func_->params_.at(i)) {
            stmt.func_->params_.at(i)->accept(*this);
            if (i < stmt.func_->params_.size() - 1) {
                this->output_->append(", ");
            }
        }
    }
    this->vypisVyraz(") {\n", true);
    if (stmt.func_->body_) {
        ++this->aktualnyStupenOdsadenia_;
        stmt.func_->body_->accept(*this);
        --this->aktualnyStupenOdsadenia_;
        this->vypisVyraz("\n", true);
    }
    this->vypisVyraz("}");
}

void CodeVisitor::visit(astfri::ClassDefStmt const& stmt) {
    if (!stmt.tparams_.empty()) {
        this->vypisVyraz("<");
        for (size_t i = 0; i < stmt.tparams_.size(); ++i) {
            if (stmt.tparams_.at(i)) {
                this->vypisVyraz(stmt.tparams_.at(i)->name_ + " : " + stmt.tparams_.at(i)->constraint_);
                if (i < stmt.tparams_.size() - 1) {
                    this->output_->append(", ");
                }
            }
        }
        this->vypisVyraz(">\n", true);
    }
    this->vypisVyraz("class " + stmt.name_ + " {\n", true);
    if (!stmt.vars_.empty()) {
        this->vypisVyraz("private:\n", true);
        ++this->aktualnyStupenOdsadenia_;
        for (size_t i = 0; i < stmt.vars_.size(); ++i) {
            if (stmt.vars_.at(i)) {
                stmt.vars_.at(i)->accept(*this);
                this->vypisVyraz("\n", true);
            }
        }
        --this->aktualnyStupenOdsadenia_;
    }
    if (!stmt.methods_.empty()) {
        this->vypisVyraz("public:\n", true);
        ++this->aktualnyStupenOdsadenia_;
        for (size_t i = 0; i < stmt.methods_.size(); ++i) {
            if (stmt.methods_.at(i)) {
                if (stmt.methods_.at(i)->owner_->name_ != stmt.methods_.at(i)->func_->name_ && stmt.methods_.at(i)->func_->name_.at(0) != '~') {
                    stmt.methods_.at(i)->func_->retType_->accept(*this);
                    this->vypisVyraz(" ");
                }
                this->vypisVyraz(stmt.methods_.at(i)->func_->name_ + "(");
                if (!stmt.methods_.at(i)->func_->params_.empty()) {
                    for (size_t j = 0; j < stmt.methods_.at(i)->func_->params_.size(); ++j) {
                        if (stmt.methods_.at(i)->func_->params_.at(j)) {
                            stmt.methods_.at(i)->func_->params_.at(j)->accept(*this);
                            if (j < stmt.methods_.at(i)->func_->params_.size() - 1) {
                                this->vypisVyraz(", ");
                            }
                        }
                    }
                }
                this->vypisVyraz(")\n", true);
            }
        }
        --this->aktualnyStupenOdsadenia_;
    }
    this->vypisVyraz("}\n", true);
    if (!stmt.methods_.empty()) {
        for (size_t i = 0; i < stmt.methods_.size(); ++i) {
            if (stmt.methods_.at(i)) {
                this->vypisVyraz("\n", true);
                stmt.methods_.at(i)->accept(*this);
                this->vypisVyraz("\n", true);
            }
        }
    }
}