#include <libastfri-text/inc/ASTVisitor.hpp>

ASTVisitor::ASTVisitor(const Configurator* conf, std::stringstream* output) {
    config_ = conf;
    std::string format = config_->get_output_format()->str();
    if (format == "txt") {
        exporter_ = new TxtFileExporter(config_, output);
    } else if (format == "rtf") {
        exporter_ = new RtfFileExporter(config_, output);
    } else if (format == "html") {
        exporter_ = new HtmlFileExporter(config_, output);
    } else {
        exporter_ = new TxtFileExporter(config_, output);
    }
}

ASTVisitor::~ASTVisitor() {
    delete exporter_;
    config_ = nullptr;
}

void ASTVisitor::visit(astfri::IfExpr const& expr) {
    exporter_->write_if_word();
    exporter_->write_space();
    exporter_->write_round_bracket("(");
    if (expr.cond_) {
        expr.cond_->accept(*this);
    }
    exporter_->write_round_bracket(")");
    write_open_bracket();
    if (expr.iftrue_) {
        exporter_->increase_indentation();
        expr.iftrue_->accept(*this);
        exporter_->decrease_indentation();
        exporter_->write_new_line();
    }
    exporter_->write_curl_bracket("}");
    exporter_->write_space();
    exporter_->write_else_word();
    write_open_bracket();
    if (expr.iffalse_) {
        exporter_->increase_indentation();
        expr.iffalse_->accept(*this);
        exporter_->decrease_indentation();
        exporter_->write_new_line();
    }
    exporter_->write_curl_bracket("}");
}

void ASTVisitor::visit(astfri::BinOpExpr const& expr) {
    if (expr.left_ && expr.right_) {
        expr.left_->accept(*this);
        switch (expr.op_) {
            case astfri::BinOpType::Add: exporter_->write_word(" + "); break;
            case astfri::BinOpType::Assign:
                exporter_->write_space();
                exporter_->write_assign_word();
                exporter_->write_space();
                break;
            case astfri::BinOpType::Divide: exporter_->write_word(" / "); break;
            case astfri::BinOpType::Equal: exporter_->write_word(" == "); break;
            case astfri::BinOpType::GreaterEqual: exporter_->write_word(" >= "); break;
            case astfri::BinOpType::Greater: exporter_->write_word(" > "); break;
            case astfri::BinOpType::LessEqual: exporter_->write_word(" <= "); break;
            case astfri::BinOpType::Less: exporter_->write_word(" < "); break;
            case astfri::BinOpType::NotEqual: exporter_->write_word(" != "); break;
            case astfri::BinOpType::Modulo: exporter_->write_word(" % "); break;
            case astfri::BinOpType::Subtract: exporter_->write_word(" - "); break;
            case astfri::BinOpType::Multiply: exporter_->write_word(" * "); break;
        }
        expr.right_->accept(*this);
    }
}

void ASTVisitor::visit(astfri::UnaryOpExpr const& expr) {
    if (!expr.arg_) {
        return;
    }
    switch (expr.op_) {
        case astfri::UnaryOpType::AddressOf: exporter_->write_word("&"); break;
        case astfri::UnaryOpType::Dereference: exporter_->write_word("*"); break;
        case astfri::UnaryOpType::LogicalNot: exporter_->write_word("!"); break;
        case astfri::UnaryOpType::Minus: exporter_->write_word("-"); break;
        case astfri::UnaryOpType::Plus: exporter_->write_word("+"); break;
    }
    expr.arg_->accept(*this);
}

void ASTVisitor::visit(astfri::AssignExpr const& expr) {
    if (expr.lhs_ && expr.rhs_) {
        expr.lhs_->accept(*this);
        exporter_->write_space();
        exporter_->write_assign_word();
        exporter_->write_space();
        expr.rhs_->accept(*this);
    }
}

void ASTVisitor::visit(astfri::CompoundAssignExpr const& expr) {
    if (!expr.lhs_ || !expr.rhs_) {
        return;
    }
    expr.lhs_->accept(*this);
    switch (expr.op_) {
        case astfri::BinOpType::Add: exporter_->write_word(" += "); break;
        case astfri::BinOpType::Divide: exporter_->write_word(" /= "); break;
        case astfri::BinOpType::Modulo: exporter_->write_word(" %= "); break;
        case astfri::BinOpType::Subtract: exporter_->write_word(" -= "); break;
        case astfri::BinOpType::Multiply: exporter_->write_word(" *= "); break;
        default: exporter_->write_space(); break;
    }
    expr.rhs_->accept(*this);
}

void ASTVisitor::visit(astfri::FunctionCallExpr const& expr) {
    exporter_->write_function_name(expr.name_);
    exporter_->write_round_bracket("(");
    for (size_t i = 0; i < expr.args_.size(); ++i) {
        if (expr.args_.at(i)) {
            expr.args_.at(i)->accept(*this);
            if (i < expr.args_.size() - 1) {
                exporter_->write_word(", ");
            }
        }
    }
    exporter_->write_round_bracket(")");
}

void ASTVisitor::visit(astfri::MethodCallExpr const& expr) {
    exporter_->write_method_name(expr.name_);
    exporter_->write_round_bracket("(");
    for (size_t i = 0; i < expr.args_.size(); ++i) {
        if (expr.args_.at(i)) {
            expr.args_.at(i)->accept(*this);
            if (i < expr.args_.size() - 1) {
                exporter_->write_word(", ");
            }
        }
    }
    exporter_->write_round_bracket(")");
}

void ASTVisitor::visit(astfri::LambdaExpr const& expr) {
    exporter_->write_round_bracket("(");
    for (size_t i = 0; i < expr.params_.size(); ++i) {
        if (expr.params_.at(i)) {
            expr.params_.at(i)->accept(*this);
            if (i < expr.params_.size() - 1) {
                exporter_->write_word(", ");
            }
        }
    }
    exporter_->write_round_bracket(")");
    write_open_bracket();
    if (expr.body_) {
        exporter_->increase_indentation();
        expr.body_->accept(*this);
        exporter_->decrease_indentation();
        exporter_->write_new_line();
    }
    exporter_->write_curl_bracket("}");
}

void ASTVisitor::visit(astfri::TranslationUnit const& stmt) {
    bool predch = false;
    for (auto a : stmt.globals_) {
        if (a) {
            a->accept(*this);
            exporter_->write_new_line();
            predch = true;
        }
    }
    for (auto a : stmt.functions_) {
        if (a) {
            if (predch) {
                exporter_->write_new_line();
            }
            a->accept(*this);
            exporter_->write_new_line();
            predch = true;
        }
    }
    for (auto a : stmt.classes_) {
        if (a) {
            if (predch) {
                exporter_->write_new_line();
            }
            a->accept(*this);
            predch = true;
        }
    }
}

void ASTVisitor::visit(astfri::CompoundStmt const& stmt) {
    for (size_t i = 0; i < stmt.stmts_.size(); ++i) {
        if (stmt.stmts_.at(i)) {
            stmt.stmts_.at(i)->accept(*this);
            if (i < stmt.stmts_.size() - 1) {
                exporter_->write_new_line();
            }
        }
    }
}

void ASTVisitor::visit(astfri::ReturnStmt const& stmt) {
    if (stmt.val_) {
        exporter_->write_return_word();
        exporter_->write_space();
        stmt.val_->accept(*this);
    }
}

void ASTVisitor::visit(astfri::IfStmt const& stmt) {
    exporter_->write_if_word();
    exporter_->write_space();
    exporter_->write_round_bracket("(");
    if (stmt.cond_) {
        stmt.cond_->accept(*this);
    }
    exporter_->write_round_bracket(")");
    write_open_bracket();
    if (stmt.iftrue_) {
        exporter_->increase_indentation();
        stmt.iftrue_->accept(*this);
        exporter_->decrease_indentation();
        exporter_->write_new_line();
    }
    exporter_->write_curl_bracket("}");
    exporter_->write_space();
    exporter_->write_else_word();
    write_open_bracket();
    if (stmt.iffalse_) {
        exporter_->increase_indentation();
        stmt.iffalse_->accept(*this);
        exporter_->decrease_indentation();
        exporter_->write_new_line();
    }
    exporter_->write_curl_bracket("}");
}

void ASTVisitor::visit(astfri::CaseStmt const& stmt) {
    exporter_->write_case_word();
    exporter_->write_space();
    if (stmt.expr_) {
        stmt.expr_->accept(*this);
    }
    exporter_->write_word(":");
    if (stmt.body_) {
        exporter_->write_new_line();
        exporter_->increase_indentation();
        stmt.body_->accept(*this);
        exporter_->decrease_indentation();
    }
}

void ASTVisitor::visit(astfri::SwitchStmt const& stmt) {
    exporter_->write_switch_word();
    exporter_->write_space();
    exporter_->write_round_bracket("(");
    stmt.expr_ ? stmt.expr_->accept(*this) : void();
    exporter_->write_round_bracket(")");
    write_open_bracket();
    exporter_->increase_indentation();
    for (size_t i = 0; i < stmt.cases_.size(); ++i) {
        if (stmt.cases_.at(i)) {
            stmt.cases_.at(i)->accept(*this);
            if (i < stmt.cases_.size() - 1) {
                exporter_->write_new_line();
            }
        }
    }
    exporter_->decrease_indentation();
    exporter_->write_new_line();
    exporter_->write_curl_bracket("}");
}

void ASTVisitor::visit(astfri::WhileStmt const& stmt) {
    exporter_->write_while_word();
    exporter_->write_space();
    exporter_->write_round_bracket("(");
    stmt.cond_ ? stmt.cond_->accept(*this) : void();
    exporter_->write_round_bracket(")");
    write_open_bracket();
    if (stmt.body_) {
        exporter_->increase_indentation();
        stmt.body_->accept(*this);
        exporter_->decrease_indentation();
        exporter_->write_new_line();
    }
    exporter_->write_curl_bracket("}");
}

void ASTVisitor::visit(astfri::DoWhileStmt const& stmt) {
    exporter_->write_do_word();
    write_open_bracket();
    if (stmt.body_) {
        exporter_->increase_indentation();
        stmt.body_->accept(*this);
        exporter_->decrease_indentation();
        exporter_->write_new_line();
    }
    exporter_->write_curl_bracket("}");
    exporter_->write_space();
    exporter_->write_while_word();
    exporter_->write_space();
    exporter_->write_round_bracket("(");
    stmt.cond_ ? stmt.cond_->accept(*this) : void();
    exporter_->write_round_bracket(")");
}

void ASTVisitor::visit(astfri::ForStmt const& stmt) {
    exporter_->write_for_word();
    exporter_->write_space();
    exporter_->write_round_bracket("(");
    stmt.init_ ? stmt.init_->accept(*this) : void();
    exporter_->write_word("; ");
    stmt.cond_ ? stmt.cond_->accept(*this) : void();
    exporter_->write_word("; ");
    stmt.step_ ? stmt.step_->accept(*this) : void();
    exporter_->write_round_bracket(")");
    write_open_bracket();
    if (stmt.body_) {
        exporter_->increase_indentation();
        stmt.body_->accept(*this);
        exporter_->decrease_indentation();
        exporter_->write_new_line();
    }
    exporter_->write_curl_bracket("}");
}

void ASTVisitor::visit(astfri::ThrowStmt const& stmt) {
    if (stmt.val_) {
        exporter_->write_throw_word();
        exporter_->write_space();
        stmt.val_->accept(*this);
    }
}

void ASTVisitor::visit(astfri::LocalVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        exporter_->write_space();
        exporter_->write_local_var_name(stmt.name_);
        if (stmt.initializer_) {
            exporter_->write_space();
            exporter_->write_assign_word();
            exporter_->write_space();
            stmt.initializer_->accept(*this);
        }
    }
}

void ASTVisitor::visit(astfri::ParamVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        exporter_->write_space();
        exporter_->write_param_var_name(stmt.name_);
        if (stmt.initializer_) {
            exporter_->write_space();
            exporter_->write_assign_word();
            exporter_->write_space();
            stmt.initializer_->accept(*this);
        }
    }
}

void ASTVisitor::visit(astfri::MemberVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        exporter_->write_space();
        exporter_->write_member_var_name(stmt.name_);
        if (stmt.initializer_) {
            exporter_->write_space();
            exporter_->write_assign_word();
            exporter_->write_space();
            stmt.initializer_->accept(*this);
        }
    }
}

void ASTVisitor::visit(astfri::GlobalVarDefStmt const& stmt) {
    if (stmt.type_) {
        stmt.type_->accept(*this);
        exporter_->write_space();
        exporter_->write_global_var_name(stmt.name_);
        if (stmt.initializer_) {
            exporter_->write_space();
            exporter_->write_assign_word();
            exporter_->write_space();
            stmt.initializer_->accept(*this);
        }
    }
}

void ASTVisitor::visit(astfri::FunctionDefStmt const& stmt) {
    if (!stmt.retType_) {
        return;
    }
    stmt.retType_->accept(*this);
    exporter_->write_space();
    exporter_->write_function_name(stmt.name_);
    exporter_->write_round_bracket("(");
    for (size_t i = 0; i < stmt.params_.size(); ++i) {
        if (stmt.params_.at(i)) {
            stmt.params_.at(i)->accept(*this);
            if (i < stmt.params_.size() - 1) {
                exporter_->write_word(", ");
            }
        }
    }
    exporter_->write_round_bracket(")");
    write_open_bracket();
    if (stmt.body_ && config_->show_function_body()) {
        exporter_->increase_indentation();
        stmt.body_->accept(*this);
        exporter_->decrease_indentation();
        exporter_->write_new_line();
    }
    exporter_->write_curl_bracket("}");
}

void ASTVisitor::visit(astfri::MethodDefStmt const& stmt) {
    if (!stmt.func_ || !stmt.owner_ || !stmt.func_->retType_) {
        return;
    }
    if (stmt.owner_->name_ != stmt.func_->name_ && stmt.func_->name_.at(0) != '~') {
        stmt.func_->retType_->accept(*this);
        exporter_->write_space();
    }
    exporter_->write_class_name(stmt.owner_->name_);
    exporter_->write_word("::");
    exporter_->write_method_name(stmt.func_->name_);
    exporter_->write_round_bracket("(");
    for (size_t i = 0; i < stmt.func_->params_.size(); ++i) {
        if (stmt.func_->params_.at(i)) {
            stmt.func_->params_.at(i)->accept(*this);
            if (i < stmt.func_->params_.size() - 1) {
                exporter_->write_word(", ");
            }
        }
    }
    exporter_->write_round_bracket(")");
    write_open_bracket();
    if (stmt.func_->body_ && config_->show_method_body()) {
        exporter_->increase_indentation();
        stmt.func_->body_->accept(*this);
        exporter_->decrease_indentation();
        exporter_->write_new_line();
    }
    exporter_->write_curl_bracket("}");
}

void ASTVisitor::visit(astfri::ClassDefStmt const& stmt) {
    if (!stmt.tparams_.empty()) {//gen_params
        exporter_->write_word("<");
        for (size_t i = 0; i < stmt.tparams_.size(); ++i) {
            if (stmt.tparams_.at(i)) {
                exporter_->write_word(stmt.tparams_.at(i)->name_ + " : " + stmt.tparams_.at(i)->constraint_);
                if (i < stmt.tparams_.size() - 1) {
                    exporter_->write_word(", ");
                }
            }
        }
        exporter_->write_word(">");
        exporter_->write_new_line();
    }
    exporter_->write_class_word();
    exporter_->write_space();
    exporter_->write_class_name(stmt.name_);
    write_open_bracket();
    if (!stmt.vars_.empty() && config_->show_class_body()) {//atribs
        exporter_->write_private_word();
        exporter_->write_word(":");
        exporter_->write_new_line();
        exporter_->increase_indentation();
        for (size_t i = 0; i < stmt.vars_.size(); ++i) {
            if (stmt.vars_.at(i)) {
                stmt.vars_.at(i)->accept(*this);
                exporter_->write_new_line();
            }
        }
        exporter_->decrease_indentation();
    }
    if (!stmt.methods_.empty() && config_->show_class_body()) {//methods
        exporter_->write_public_word();
        exporter_->write_word(":");
        exporter_->write_new_line();
        exporter_->increase_indentation();
        for (size_t i = 0; i < stmt.methods_.size(); ++i) {
            if (stmt.methods_.at(i)) {
                if (stmt.methods_.at(i)->owner_->name_ != stmt.methods_.at(i)->func_->name_ && stmt.methods_.at(i)->func_->name_.at(0) != '~') {
                    stmt.methods_.at(i)->func_->retType_->accept(*this);
                    exporter_->write_space();
                }
                exporter_->write_method_name(stmt.methods_.at(i)->func_->name_);
                exporter_->write_round_bracket("(");
                if (!stmt.methods_.at(i)->func_->params_.empty()) {
                    for (size_t j = 0; j < stmt.methods_.at(i)->func_->params_.size(); ++j) {
                        if (stmt.methods_.at(i)->func_->params_.at(j)) {
                            stmt.methods_.at(i)->func_->params_.at(j)->accept(*this);
                            if (j < stmt.methods_.at(i)->func_->params_.size() - 1) {
                                exporter_->write_word(", ");
                            }
                        }
                    }
                }
                exporter_->write_round_bracket(")");
                exporter_->write_new_line();
            }
        }
        exporter_->decrease_indentation();
    }
    exporter_->write_curl_bracket("}");
    exporter_->write_new_line();
    if (!stmt.methods_.empty() && config_->show_method_definition()) {
        for (size_t i = 0; i < stmt.methods_.size(); ++i) {
            if (stmt.methods_.at(i)) {
                exporter_->write_new_line();
                stmt.methods_.at(i)->accept(*this);
                exporter_->write_new_line();
            }
        }
    }
}

void ASTVisitor::write_open_bracket() {
    if (config_->open_bracket_new_line()) {
        exporter_->write_new_line();
    } else {
        exporter_->write_space();
    }
    exporter_->write_curl_bracket("{");
    exporter_->write_new_line();
}