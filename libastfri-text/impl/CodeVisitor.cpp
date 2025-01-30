#include "libastfri-text/inc/CodeVisitor.hpp"

void CodeVisitor::visit(astfri::IfExpr const& expr) {
    exp_->write_word("if (");
    if (expr.cond_) {
        expr.cond_->accept(*this);
    }
    exp_->write_word(") {\n", true);
    if (expr.iftrue_) {
        exp_->increase_indentation();
        expr.iftrue_->accept(*this);
        exp_->decrease_indentation();
        exp_->write_word("\n", true);
    }
    exp_->write_word("} else {\n", true);
    if (expr.iffalse_) {
        exp_->increase_indentation();
        expr.iffalse_->accept(*this);
        exp_->decrease_indentation();
        exp_->write_word("\n", true);
    }
    exp_->write_word("}");
}

void CodeVisitor::visit(astfri::BinOpExpr const& expr) {
    if (expr.left_ && expr.right_) {
        expr.left_->accept(*this);
        switch (expr.op_) {
            case astfri::BinOpType::Add: exp_->write_word(" + "); break;
            case astfri::BinOpType::Assign: exp_->write_word(" = "); break;
            case astfri::BinOpType::Divide: exp_->write_word(" / "); break;
            case astfri::BinOpType::Equal: exp_->write_word(" == "); break;
            case astfri::BinOpType::GreaterEqual: exp_->write_word(" >= "); break;
            case astfri::BinOpType::Greater: exp_->write_word(" > "); break;
            case astfri::BinOpType::LessEqual: exp_->write_word(" <= "); break;
            case astfri::BinOpType::Less: exp_->write_word(" < "); break;
            case astfri::BinOpType::NotEqual: exp_->write_word(" != "); break;
            case astfri::BinOpType::Modulo: exp_->write_word(" % "); break;
            case astfri::BinOpType::Subtract: exp_->write_word(" - "); break;
            case astfri::BinOpType::Multiply: exp_->write_word(" * "); break;
        }
        expr.right_->accept(*this);
    }
}

void CodeVisitor::visit(astfri::UnaryOpExpr const& expr) {
    if (!expr.arg_) {
        return;
    }
    switch (expr.op_) {
        case astfri::UnaryOpType::AddressOf: exp_->write_word("&"); break;
        case astfri::UnaryOpType::Dereference: exp_->write_word("*"); break;
        case astfri::UnaryOpType::LogicalNot: exp_->write_word("!"); break;
        case astfri::UnaryOpType::Minus: exp_->write_word("-"); break;
        case astfri::UnaryOpType::Plus: exp_->write_word("+"); break;
    }
    expr.arg_->accept(*this);
}

void CodeVisitor::visit(astfri::AssignExpr const& expr) {
    if (expr.lhs_ && expr.rhs_) {
        expr.lhs_->accept(*this);
        exp_->write_word(" = ");
        expr.rhs_->accept(*this);
    }
}

void CodeVisitor::visit(astfri::CompoundAssignExpr const& expr) {
    if (!expr.lhs_ || !expr.rhs_) {
        return;
    }
    expr.lhs_->accept(*this);
    switch (expr.op_) {
        case astfri::BinOpType::Add: exp_->write_word(" += "); break;
        case astfri::BinOpType::Divide: exp_->write_word(" /= "); break;
        case astfri::BinOpType::Modulo: exp_->write_word(" %= "); break;
        case astfri::BinOpType::Subtract: exp_->write_word(" -= "); break;
        case astfri::BinOpType::Multiply: exp_->write_word(" *= "); break;
        default: exp_->write_word(" "); break;
    }
    expr.rhs_->accept(*this);
}

void CodeVisitor::visit(astfri::FunctionCallExpr const& expr) {
    exp_->write_word(expr.name_);
    exp_->write_word("(");
    for (size_t i = 0; i < expr.args_.size(); ++i) {
        if (expr.args_.at(i)) {
            expr.args_.at(i)->accept(*this);
            if (i < expr.args_.size() - 1) {
                exp_->write_word(", ");
            }
        }
    }
    exp_->write_word(")");
}

void CodeVisitor::visit(astfri::MethodCallExpr const& expr) {
    if (expr.owner_) {
        expr.owner_->accept(*this);
        exp_->write_word("::");
    }
    exp_->write_word(expr.name_ + "(");
    for (size_t i = 0; i < expr.args_.size(); ++i) {
        if (expr.args_.at(i)) {
            expr.args_.at(i)->accept(*this);
            if (i < expr.args_.size() - 1) {
                exp_->write_word(", ");
            }
        }
    }
    exp_->write_word(")");
}

void CodeVisitor::visit(astfri::LambdaExpr const& expr) {
    exp_->write_word("(");
    for (size_t i = 0; i < expr.params_.size(); ++i) {
        if (expr.params_.at(i)) {
            expr.params_.at(i)->accept(*this);
            if (i < expr.params_.size() - 1) {
                exp_->write_word(", ");
            }
        }
    }
    exp_->write_word(") {\n", true);
    if (expr.body_) {
        exp_->increase_indentation();
        expr.body_->accept(*this);
        exp_->decrease_indentation();
        exp_->write_word("\n", true);
    }
    exp_->write_word("}");
}

void CodeVisitor::visit(astfri::TranslationUnit const& stmt) {
    bool predch = false;
    for (auto a : stmt.globals_) {
        if (a) {
            a->accept(*this);
            exp_->write_word("\n", true);
            predch = true;
        }
    }
    for (auto a : stmt.functions_) {
        if (a) {
            if (predch) {
                exp_->write_word("\n", true);
            }
            a->accept(*this);
            exp_->write_word("\n", true);
            predch = true;
        }
    }
    for (auto a : stmt.classes_) {
        if (a) {
            if (predch) {
                exp_->write_word("\n", true);
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
                exp_->write_word("\n", true);
            }
        }
    }
}

void CodeVisitor::visit(astfri::ReturnStmt const& stmt) {
    if (stmt.val_) {
        exp_->write_word("return ");
        stmt.val_->accept(*this);
    }
}

void CodeVisitor::visit(astfri::IfStmt const& stmt) {
    exp_->write_word("if (");
    if (stmt.cond_) {
        stmt.cond_->accept(*this);
    }
    exp_->write_word(") {\n", true);
    if (stmt.iftrue_) {
        exp_->increase_indentation();
        stmt.iftrue_->accept(*this);
        exp_->decrease_indentation();
        exp_->write_word("\n", true);
    }
    exp_->write_word("} else {\n", true);
    if (stmt.iffalse_) {
        exp_->increase_indentation();
        stmt.iffalse_->accept(*this);
        exp_->decrease_indentation();
        exp_->write_word("\n", true);
    }
    exp_->write_word("}");
}

void CodeVisitor::visit(astfri::CaseStmt const& stmt) {
    exp_->write_word("case ");
    if (stmt.expr_) {
        stmt.expr_->accept(*this);
    }
    exp_->write_word(":");
    if (stmt.body_) {
        exp_->write_word("\n", true);
        exp_->increase_indentation();
        stmt.body_->accept(*this);
        exp_->decrease_indentation();
    }
}

void CodeVisitor::visit(astfri::SwitchStmt const& stmt) {
    exp_->write_word("switch (");
    stmt.expr_ ? stmt.expr_->accept(*this) : void();
    exp_->write_word(") {\n", true);
    exp_->increase_indentation();
    for (size_t i = 0; i < stmt.cases_.size(); ++i) {
        if (stmt.cases_.at(i)) {
            stmt.cases_.at(i)->accept(*this);
            if (i < stmt.cases_.size() - 1) {
                exp_->write_word("\n", true);
            }
        }
    }
    exp_->decrease_indentation();
    exp_->write_word("\n", true);
    exp_->write_word("}");
}

void CodeVisitor::visit(astfri::WhileStmt const& stmt) {
    exp_->write_word("while (");
    stmt.cond_ ? stmt.cond_->accept(*this) : void();
    exp_->write_word(") {\n", true);
    if (stmt.body_) {
        exp_->increase_indentation();
        stmt.body_->accept(*this);
        exp_->decrease_indentation();
        exp_->write_word("\n", true);
    }
    exp_->write_word("}");
}

void CodeVisitor::visit(astfri::DoWhileStmt const& stmt) {
    exp_->write_word("do {\n", true);
    if (stmt.body_) {
        exp_->increase_indentation();
        stmt.body_->accept(*this);
        exp_->decrease_indentation();
        exp_->write_word("\n", true);
    }
    exp_->write_word("} while (");
    stmt.cond_ ? stmt.cond_->accept(*this) : void();
    exp_->write_word(")");
}

void CodeVisitor::visit(astfri::ForStmt const& stmt) {
    exp_->write_word("for (");
    stmt.init_ ? stmt.init_->accept(*this) : void();
    exp_->write_word("; ");
    stmt.cond_ ? stmt.cond_->accept(*this) : void();
    exp_->write_word("; ");
    stmt.step_ ? stmt.step_->accept(*this) : void();
    exp_->write_word(") {\n", true);
    if (stmt.body_) {
        exp_->increase_indentation();
        stmt.body_->accept(*this);
        exp_->decrease_indentation();
        exp_->write_word("\n", true);
    }
    exp_->write_word("}");
}

void CodeVisitor::visit(astfri::ThrowStmt const& stmt) {
    if (stmt.val_) {
        exp_->write_word("throw ");
        stmt.val_->accept(*this);
    }
}

void CodeVisitor::visit(astfri::LocalVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        exp_->write_word(" " + stmt.name_);
        if (stmt.initializer_) {
            exp_->write_word(" = ");
            stmt.initializer_->accept(*this);
        }
    }
}

void CodeVisitor::visit(astfri::ParamVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        exp_->write_word(" " + stmt.name_);
        if (stmt.initializer_) {
            exp_->write_word(" = ");
            stmt.initializer_->accept(*this);
        }
    }
}

void CodeVisitor::visit(astfri::MemberVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        exp_->write_word(" " + stmt.name_);
        if (stmt.initializer_) {
            exp_->write_word(" = ");
            stmt.initializer_->accept(*this);
        }
    }
}

void CodeVisitor::visit(astfri::GlobalVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        exp_->write_word(" " + stmt.name_);
        if (stmt.initializer_) {
            exp_->write_word(" = ");
            stmt.initializer_->accept(*this);
        }
    }
}

void CodeVisitor::visit(astfri::FunctionDefStmt const& stmt) {
    if (!stmt.retType_) {
        return;
    }
    stmt.retType_->accept(*this);
    exp_->write_word(" " + stmt.name_ + "(");
    for (size_t i = 0; i < stmt.params_.size(); ++i) {
        if (stmt.params_.at(i)) {
            stmt.params_.at(i)->accept(*this);
            if (i < stmt.params_.size() - 1) {
                exp_->write_word(", ");
            }
        }
    }
    exp_->write_word(") {\n", true);
    if (stmt.body_) {
        exp_->increase_indentation();
        stmt.body_->accept(*this);
        exp_->decrease_indentation();
        exp_->write_word("\n", true);
    }
    exp_->write_word("}");
}

void CodeVisitor::visit(astfri::MethodDefStmt const& stmt) {
    if (!stmt.func_ || !stmt.owner_ || !stmt.func_->retType_) {
        return;
    }
    stmt.func_->retType_->accept(*this);
    exp_->write_word(" " + stmt.owner_->name_ + "::" + stmt.func_->name_ + "(");
    for (size_t i = 0; i < stmt.func_->params_.size(); ++i) {
        if (stmt.func_->params_.at(i)) {
            stmt.func_->params_.at(i)->accept(*this);
            if (i < stmt.func_->params_.size() - 1) {
                exp_->write_word(", ");
            }
        }
    }
    exp_->write_word(") {\n", true);
    if (stmt.func_->body_) {
        exp_->increase_indentation();
        stmt.func_->body_->accept(*this);
        exp_->decrease_indentation();
        exp_->write_word("\n", true);
    }
    exp_->write_word("}");
}

void CodeVisitor::visit(astfri::ClassDefStmt const& stmt) {
    if (!stmt.tparams_.empty()) {
        exp_->write_word("<");
        for (size_t i = 0; i < stmt.tparams_.size(); ++i) {
            if (stmt.tparams_.at(i)) {
                exp_->write_word(stmt.tparams_.at(i)->name_ + " : " + stmt.tparams_.at(i)->constraint_);
                if (i < stmt.tparams_.size() - 1) {
                    exp_->write_word(", ");
                }
            }
        }
        exp_->write_word(">\n", true);
    }
    exp_->write_word("class " + stmt.name_ + " {\n", true);
    if (!stmt.vars_.empty()) {
        exp_->write_word("private:\n", true);
        exp_->increase_indentation();
        for (size_t i = 0; i < stmt.vars_.size(); ++i) {
            if (stmt.vars_.at(i)) {
                stmt.vars_.at(i)->accept(*this);
                exp_->write_word("\n", true);
            }
        }
        exp_->decrease_indentation();
    }
    if (!stmt.methods_.empty()) {
        exp_->write_word("public:\n", true);
        exp_->increase_indentation();
        for (size_t i = 0; i < stmt.methods_.size(); ++i) {
            if (stmt.methods_.at(i)) {
                if (stmt.methods_.at(i)->owner_->name_ != stmt.methods_.at(i)->func_->name_ && stmt.methods_.at(i)->func_->name_.at(0) != '~') {
                    stmt.methods_.at(i)->func_->retType_->accept(*this);
                    exp_->write_word(" ");
                }
                exp_->write_word(stmt.methods_.at(i)->func_->name_ + "(");
                if (!stmt.methods_.at(i)->func_->params_.empty()) {
                    for (size_t j = 0; j < stmt.methods_.at(i)->func_->params_.size(); ++j) {
                        if (stmt.methods_.at(i)->func_->params_.at(j)) {
                            stmt.methods_.at(i)->func_->params_.at(j)->accept(*this);
                            if (j < stmt.methods_.at(i)->func_->params_.size() - 1) {
                                exp_->write_word(", ");
                            }
                        }
                    }
                }
                exp_->write_word(")\n", true);
            }
        }
        exp_->decrease_indentation();
    }
    exp_->write_word("}\n", true);
    if (!stmt.methods_.empty()) {
        for (size_t i = 0; i < stmt.methods_.size(); ++i) {
            if (stmt.methods_.at(i)) {
                exp_->write_word("\n", true);
                stmt.methods_.at(i)->accept(*this);
                exp_->write_word("\n", true);
            }
        }
    }
}