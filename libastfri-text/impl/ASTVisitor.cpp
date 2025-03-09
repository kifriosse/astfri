#include <libastfri-text/inc/ASTVisitor.hpp>
#include <libastfri/inc/Expr.hpp>
#include <libastfri/inc/Type.hpp>

ASTVisitor::ASTVisitor() {
    configurator_ = std::make_shared<TextConfigurator>();
    if (configurator_->get_output_file_format()->view() == std::move("html")) {
        exporter_ = std::make_unique<HtmlFileExporter>(configurator_);
    } else {
        exporter_ = std::make_unique<TxtFileExporter>(configurator_);
    }
    currClassName_ = std::make_unique<std::stringstream>();
    currInterfName_ = std::make_unique<std::stringstream>();
    currGenParams_ = std::make_unique<std::vector<std::stringstream>>();
    isMethodCall_ = false;
    isConstructorCall_ = false;
}

void ASTVisitor::write_file() {
    exporter_->make_export();
}

void ASTVisitor::visit(DynamicType const& /*type*/) {
    exporter_->write_dynamic_type();
}

void ASTVisitor::visit(IntType const& /*type*/) {
    exporter_->write_int_type();
}

void ASTVisitor::visit(FloatType const& /*type*/) {
    exporter_->write_float_type();
}

void ASTVisitor::visit(CharType const& /*type*/) {
    exporter_->write_char_type();
}

void ASTVisitor::visit(BoolType const& /*type*/) {
    exporter_->write_bool_type();
}

void ASTVisitor::visit(VoidType const& /*type*/) {
    exporter_->write_void_type();
}

void ASTVisitor::visit(UserType const& type) {
    std::string name = type.name_;
    size_t st = std::move(name.find(std::move('<')));
    size_t en = std::move(name.find(std::move('>')));
    if (st != std::move(std::string::npos) && en != std::move(std::string::npos) && st < en) {
        std::string cl = name.substr(0, st);
        std::string genPar = name.substr(st + 1, en - st - 1);
        exporter_->write_class_name(std::move(cl));
        exporter_->write_separator_sign(std::move("<"));
        exporter_->write_gen_param_name(std::move(genPar));
        exporter_->write_separator_sign(std::move(">"));
    } else {
        if (!currGenParams_->empty()) {
            for (auto& a : *currGenParams_) {
                if (std::move(a.str()) == name) {
                    exporter_->write_gen_param_name(std::move(name));
                    return;
                }
            }
        }
        if (name == std::move(currClassName_->str())) {
            exporter_->write_class_name(std::move(name));
        } else if (name == std::move(currInterfName_->str())) {
            exporter_->write_interface_name(std::move(name));
        } else if (name[0] >= std::move('A') && name[0] <= std::move('Z')) {
            exporter_->write_class_name(std::move(name));
        } else {
            exporter_->write_user_type(std::move(name));
        }
    }
}

void ASTVisitor::visit(IndirectionType const& type) {
    type.indirect_ ? type.indirect_->accept(*this) : exporter_->write_invalid_word();
    exporter_->write_pointer_word();
}

void ASTVisitor::visit(UnknownType const& /*type*/) {
    exporter_->write_unknown_word();
}

void ASTVisitor::visit(IntLiteralExpr const& expr) {
    exporter_->write_int_val(expr.val_);
}

void ASTVisitor::visit(FloatLiteralExpr const& expr) {
    exporter_->write_float_val(expr.val_);
}

void ASTVisitor::visit(CharLiteralExpr const& expr) {
    exporter_->write_char_val(expr.val_);
}

void ASTVisitor::visit(StringLiteralExpr const& expr) {
    exporter_->write_string_val(expr.val_);
}

void ASTVisitor::visit(BoolLiteralExpr const& expr) {
    exporter_->write_bool_val(expr.val_);
}

void ASTVisitor::visit(NullLiteralExpr const& /*expr*/) {
    exporter_->write_null_val();
}

void ASTVisitor::visit(IfExpr const& expr) {
    exporter_->write_if_word();
    write_cond(expr.cond_);
    exporter_->write_new_line();
    exporter_->increase_indentation();
    exporter_->write_do_word();
    exporter_->write_space();
    expr.iftrue_ ? expr.iftrue_->accept(*this) : void();
    exporter_->write_new_line();
    exporter_->write_else_word();
    exporter_->write_space();
    expr.iffalse_ ? expr.iffalse_->accept(*this) : void();
    exporter_->decrease_indentation();
}

void ASTVisitor::visit(BinOpExpr const& expr) {
    exporter_->write_round_bracket(std::move("("));
    expr.left_ ? expr.left_->accept(*this) : exporter_->write_invalid_word();
    exporter_->write_space();
    switch (expr.op_) {
        case BinOpType::Assign: exporter_->write_assign_op_word(); break;
        case BinOpType::Add: exporter_->write_operator_sign(std::move("+")); break;
        case BinOpType::Subtract: exporter_->write_operator_sign(std::move("-")); break;
        case BinOpType::Multiply: exporter_->write_operator_sign(std::move("*")); break;
        case BinOpType::Divide: exporter_->write_operator_sign(std::move("/")); break;
        case BinOpType::FloorDivide: exporter_->write_separator_sign(std::move("div")); break;
        case BinOpType::Modulo: exporter_->write_operator_sign(std::move("%")); break;
        case BinOpType::Exponentiation: exporter_->write_separator_sign(std::move("**")); break;
        case BinOpType::Equal: exporter_->write_operator_sign(std::move("==")); break;
        case BinOpType::NotEqual: exporter_->write_operator_sign(std::move("!=")); break;
        case BinOpType::Less: exporter_->write_operator_sign(std::move("<")); break;
        case BinOpType::LessEqual: exporter_->write_operator_sign(std::move("<=")); break;
        case BinOpType::Greater: exporter_->write_operator_sign(std::move(">")); break;
        case BinOpType::GreaterEqual: exporter_->write_operator_sign(std::move(">=")); break;
        case BinOpType::LogicalAnd: exporter_->write_operator_sign(std::move("&&")); break;
        case BinOpType::LogicalOr: exporter_->write_operator_sign(std::move("||")); break;
        case BinOpType::BitShiftRight: exporter_->write_operator_sign(std::move(">>")); break;
        case BinOpType::BitShiftLeft: exporter_->write_operator_sign(std::move("<<")); break;
        case BinOpType::BitAnd: exporter_->write_operator_sign(std::move("&")); break;
        case BinOpType::BitOr: exporter_->write_operator_sign(std::move("|")); break;
        case BinOpType::BitXor: exporter_->write_operator_sign(std::move("^")); break;
        case BinOpType::Comma: exporter_->write_separator_sign(std::move(",")); break;
        case BinOpType::AddAssign: exporter_->write_separator_sign(std::move("+=")); break;
        case BinOpType::SubtractAssign: exporter_->write_separator_sign(std::move("-=")); break;
        case BinOpType::MultiplyAssign: exporter_->write_separator_sign(std::move("*=")); break;
        case BinOpType::DivideAssign: exporter_->write_separator_sign(std::move("/=")); break;
        case BinOpType::FloorDivideAssign: exporter_->write_separator_sign(std::move("div=")); break;
        case BinOpType::ModuloAssign: exporter_->write_separator_sign(std::move("%=")); break;
        case BinOpType::ExponentiationAssign: exporter_->write_separator_sign(std::move("**=")); break;
        case BinOpType::BitShiftRightAssign: exporter_->write_separator_sign(std::move(">>=")); break;
        case BinOpType::BitShiftLeftAssign: exporter_->write_separator_sign(std::move("<<=")); break;
        case BinOpType::BitAndAssign: exporter_->write_separator_sign(std::move("&=")); break;
        case BinOpType::BitOrAssign: exporter_->write_separator_sign(std::move("|=")); break;
        case BinOpType::BitXorAssign: exporter_->write_separator_sign(std::move("^=")); break;
    }
    exporter_->write_space();
    expr.right_ ? expr.right_->accept(*this) : exporter_->write_invalid_word();
    exporter_->write_round_bracket(std::move(")"));
}

void ASTVisitor::visit(UnaryOpExpr const& expr) {
    exporter_->write_round_bracket(std::move("("));
    switch (expr.op_) {
        case UnaryOpType::LogicalNot:
            exporter_->write_operator_sign(std::move("!"));
            expr.arg_ ? expr.arg_->accept(*this) : exporter_->write_invalid_word();
            break;
        case UnaryOpType::Minus:
            exporter_->write_operator_sign(std::move("-"));
            expr.arg_ ? expr.arg_->accept(*this) : exporter_->write_invalid_word();
            break;
        case UnaryOpType::Plus:
            exporter_->write_operator_sign(std::move("+"));
            expr.arg_ ? expr.arg_->accept(*this) : exporter_->write_invalid_word();
            break;
        case UnaryOpType::Dereference:
            exporter_->write_operator_sign(std::move("*"));
            expr.arg_ ? expr.arg_->accept(*this) : exporter_->write_invalid_word();
            break;
        case UnaryOpType::AddressOf:
            exporter_->write_operator_sign(std::move("&"));
            expr.arg_ ? expr.arg_->accept(*this) : exporter_->write_invalid_word();
            break;
        case UnaryOpType::PreIncrement:
            exporter_->write_operator_sign(std::move("++"));
            expr.arg_ ? expr.arg_->accept(*this) : exporter_->write_invalid_word();
            break;
        case UnaryOpType::PostIncrement:
            expr.arg_ ? expr.arg_->accept(*this) : exporter_->write_invalid_word();
            exporter_->write_operator_sign(std::move("++"));
            break;
        case UnaryOpType::PreDecrement:
            exporter_->write_operator_sign(std::move("--"));
            expr.arg_ ? expr.arg_->accept(*this) : exporter_->write_invalid_word();
            break;
        case UnaryOpType::PostDecrement:
            expr.arg_ ? expr.arg_->accept(*this) : exporter_->write_invalid_word();
            exporter_->write_operator_sign(std::move("--"));
            break;
        case UnaryOpType::BitFlip:
            exporter_->write_operator_sign(std::move("~"));
            expr.arg_ ? expr.arg_->accept(*this) : exporter_->write_invalid_word();
            break;
    }
    exporter_->write_round_bracket(std::move(")"));
}

void ASTVisitor::visit(ParamVarRefExpr const& expr) {
    exporter_->write_param_var_name(expr.param_);
}

void ASTVisitor::visit(LocalVarRefExpr const& expr) {
    exporter_->write_local_var_name(expr.var_);
}

void ASTVisitor::visit(MemberVarRefExpr const& expr) {
    expr.owner_ ? (expr.owner_->accept(*this), exporter_->write_separator_sign(std::move("."))) : void();
    exporter_->write_member_var_name(expr.member_);
}

void ASTVisitor::visit(GlobalVarRefExpr const& expr) {
    exporter_->write_global_var_name(expr.global_);
}

void ASTVisitor::visit(ClassRefExpr const& expr) {
    exporter_->write_class_name(expr.name_);
}

void ASTVisitor::visit(FunctionCallExpr const& expr) {
    configurator_->sh_other_expr() ? (exporter_->write_call_word(), exporter_->write_space()) : void();
    exporter_->write_function_name(expr.name_);
    write_parameters(expr.args_);
}

void ASTVisitor::visit(MethodCallExpr const& expr) {
    if (configurator_->sh_other_expr() && !isMethodCall_) {
        exporter_->write_call_word();
        exporter_->write_space();
        isMethodCall_ = true;
    }
    expr.owner_ ? (expr.owner_->accept(*this), exporter_->write_separator_sign(std::move("."))) : void();
    isMethodCall_ = false;
    exporter_->write_method_name(expr.name_);
    write_parameters(expr.args_);
}

void ASTVisitor::visit(LambdaExpr const& expr) {
    if (!configurator_->sh_func_dec()) {
        return;
    }
    exporter_->write_lambda_word();
    write_parameters(expr.params_);
    if (configurator_->sh_func_def()) {
        write_body(expr.body_);
        return;
    }
    exporter_->write_space();
    exporter_->write_curl_bracket(std::move("{"));
    exporter_->write_curl_bracket(std::move("}"));
}

void ASTVisitor::visit(ThisExpr const& /*expr*/) {
    exporter_->write_this_word();
}

void ASTVisitor::visit(ConstructorCallExpr const& expr) {
    if (configurator_->sh_other_expr() && !isConstructorCall_) {
        exporter_->write_call_word();
        exporter_->write_space();
        exporter_->write_constr_word();
        exporter_->write_space();
    }
    expr.type_ ? expr.type_->accept(*this) : exporter_->write_invalid_word();
    isConstructorCall_ = false;
    write_parameters(expr.args_);
}

void ASTVisitor::visit(NewExpr const& expr) {
    exporter_->write_new_word();
    exporter_->write_space();
    isConstructorCall_ = true;
    expr.init_ ? expr.init_->accept(*this) : exporter_->write_invalid_word();
    isConstructorCall_ = false;
}

void ASTVisitor::visit(DeleteExpr const& expr) {
    exporter_->write_delete_word();
    exporter_->write_space();
    expr.arg_ ? expr.arg_->accept(*this) : exporter_->write_invalid_word();
}

void ASTVisitor::visit(UnknownExpr const& /*expr*/) {
    exporter_->write_unknown_word();
}

void ASTVisitor::visit(TranslationUnit const& stmt) {
    bool predch = false;
    if (configurator_->sh_glob_par()) {
        for (GlobalVarDefStmt* a : stmt.globals_) {
            if (a) {
                a->accept(*this);
                exporter_->write_new_line();
                predch = true;
            }
        }
    }
    if (configurator_->sh_func_dec()) {
        for (FunctionDefStmt* a : stmt.functions_) {
            if (a) {
                predch ? exporter_->write_new_line() : void();
                a->accept(*this);
                exporter_->write_new_line();
                predch = true;
            }
        }
    }
    for (InterfaceDefStmt* a : stmt.interfaces_) {
        if (a) {
            predch ? exporter_->write_new_line() : void();
            a->accept(*this);
            exporter_->write_new_line();
            predch = true;
        }
    }
    for (ClassDefStmt* a : stmt.classes_) {
        if (a) {
            predch ? exporter_->write_new_line() : void();
            a->accept(*this);
            exporter_->write_new_line();
            predch = true;
        }
    }
}

void ASTVisitor::visit(CompoundStmt const& stmt) {
    for (size_t i = 0; i < stmt.stmts_.size(); ++i) {
        if (stmt.stmts_.at(i)) {
            stmt.stmts_.at(i)->accept(*this);
            (i < stmt.stmts_.size() - 1) ? exporter_->write_new_line() : void();
        }
    }
}

void ASTVisitor::visit(ReturnStmt const& stmt) {
    exporter_->write_return_word();
    stmt.val_ ? (exporter_->write_space(), stmt.val_->accept(*this)) : void();
}

void ASTVisitor::visit(ExprStmt const& stmt) {
    stmt.expr_ ? stmt.expr_->accept(*this) : void();
}

void ASTVisitor::visit(IfStmt const& stmt) {
    exporter_->write_if_word();
    write_cond(stmt.cond_);
    exporter_->write_space();
    exporter_->write_do_word();
    write_body(stmt.iftrue_);
    if (stmt.iffalse_) {
        exporter_->write_space();
        exporter_->write_else_word();
        write_body(stmt.iffalse_);
    }
}

void ASTVisitor::visit(CaseStmt const& stmt) {
    exporter_->write_case_word();
    exporter_->write_space();
    if (stmt.exprs_.empty()) {
        exporter_->write_invalid_word();
    } else {
        for (size_t i = 0; i < stmt.exprs_.size(); ++i) {
            stmt.exprs_.at(i) ? stmt.exprs_.at(i)->accept(*this) : exporter_->write_invalid_word();
            (i < stmt.exprs_.size() - 1) ? (exporter_->write_separator_sign(std::move(",")), exporter_->write_space()) : void();
        }
    }
    exporter_->write_space();
    exporter_->write_do_word();
    write_body(stmt.body_);
}

void ASTVisitor::visit(DefaultCaseStmt const& stmt) {
    exporter_->write_default_word();
    exporter_->write_space();
    exporter_->write_do_word();
    write_body(stmt.body_);
}

void ASTVisitor::visit(SwitchStmt const& stmt) {
    exporter_->write_switch_word();
    write_cond(stmt.expr_);
    write_open_curl_bracket();
    exporter_->increase_indentation();
    for (size_t i = 0; i < stmt.cases_.size(); ++i) {
        stmt.cases_.at(i) ? stmt.cases_.at(i)->accept(*this) : exporter_->write_invalid_word();
        (i < stmt.cases_.size() - 1) ? exporter_->write_new_line() : void();
    }
    exporter_->decrease_indentation();
    exporter_->write_new_line();
    exporter_->write_curl_bracket(std::move("}"));
}

void ASTVisitor::visit(WhileStmt const& stmt) {
    exporter_->write_while_word();
    write_cond(stmt.cond_);
    exporter_->write_space();
    exporter_->write_repeat_word();
    write_body(stmt.body_);
}

void ASTVisitor::visit(DoWhileStmt const& stmt) {
    exporter_->write_repeat_word();
    write_body(stmt.body_);
    exporter_->write_space();
    exporter_->write_while_word();
    write_cond(stmt.cond_);
}

void ASTVisitor::visit(ForStmt const& stmt) {
    exporter_->write_for_word();
    exporter_->write_space();
    exporter_->write_round_bracket(std::move("("));
    stmt.init_ ? stmt.init_->accept(*this) : void();
    exporter_->write_separator_sign(std::move(";"));
    exporter_->write_space();
    stmt.cond_ ? stmt.cond_->accept(*this) : void();
    exporter_->write_separator_sign(std::move(";"));
    exporter_->write_space();
    stmt.step_ ? stmt.step_->accept(*this) : void();
    exporter_->write_round_bracket(std::move(")"));
    exporter_->write_space();
    exporter_->write_repeat_word();
    write_body(stmt.body_);
}

void ASTVisitor::visit(ThrowStmt const& stmt) {
    exporter_->write_throw_word();
    exporter_->write_space();
    stmt.val_ ? stmt.val_->accept(*this) : exporter_->write_invalid_word();
}

void ASTVisitor::visit(UnknownStmt const& /*stmt*/) {
    exporter_->write_unknown_word();
}

void ASTVisitor::visit(LocalVarDefStmt const& stmt) {
    configurator_->sh_other_expr() ? (exporter_->write_define_word(), exporter_->write_space()) : void();
    stmt.type_ ? stmt.type_->accept(*this) : exporter_->write_invalid_word();
    exporter_->write_space();
    exporter_->write_local_var_name(stmt.name_);
    stmt.initializer_ ? write_initialization(&stmt) : void();
}

void ASTVisitor::visit(ParamVarDefStmt const& stmt) {
    stmt.type_ ? stmt.type_->accept(*this) : exporter_->write_invalid_word();
    exporter_->write_space();
    exporter_->write_param_var_name(stmt.name_);
    stmt.initializer_ ? write_initialization(&stmt) : void();
}

void ASTVisitor::visit(MemberVarDefStmt const& stmt) {
    configurator_->sh_other_expr() ? (exporter_->write_define_word(), exporter_->write_space()) : void();
    stmt.type_ ? stmt.type_->accept(*this) : exporter_->write_invalid_word();
    exporter_->write_space();
    exporter_->write_member_var_name(stmt.name_);
    stmt.initializer_ ? write_initialization(&stmt) : void();
}

void ASTVisitor::visit(GlobalVarDefStmt const& stmt) {
    configurator_->sh_other_expr() ? (exporter_->write_define_word(), exporter_->write_space()) : void();
    stmt.type_ ? stmt.type_->accept(*this) : exporter_->write_invalid_word();
    exporter_->write_space();
    exporter_->write_global_var_name(stmt.name_);
    stmt.initializer_ ? write_initialization(&stmt) : void();
}

void ASTVisitor::visit(FunctionDefStmt const& stmt) {
    if (!configurator_->sh_func_dec()) {
        return;
    }
    configurator_->sh_other_expr() ? (exporter_->write_function_word(), exporter_->write_space()) : void();
    exporter_->write_function_name(stmt.name_);
    write_parameters(stmt.params_);
    write_return_type(stmt.retType_);
    if (configurator_->sh_func_def()) {
        write_body(stmt.body_);
        return;
    }
    exporter_->write_space();
    exporter_->write_curl_bracket(std::move("{"));
    exporter_->write_curl_bracket(std::move("}"));
}

void ASTVisitor::visit(DefStmt const& stmt) {
    if (stmt.defs_.empty()) {
        exporter_->write_invalid_word();
        return;
    }
    configurator_->sh_other_expr() ? (exporter_->write_define_word(), exporter_->write_space()) : void();
    stmt.defs_.at(0)->type_ ? stmt.defs_.at(0)->type_->accept(*this) : exporter_->write_invalid_word();
    for (size_t i = 0; i < stmt.defs_.size(); ++i) {
        exporter_->write_space();
        exporter_->write_local_var_name(stmt.defs_.at(i)->name_);
        stmt.defs_.at(i)->initializer_ ? write_initialization(stmt.defs_.at(i)) : void();
        (i < stmt.defs_.size() - 1) ? (exporter_->write_separator_sign(std::move(",")), exporter_->write_space()) : void();
    }
}

void ASTVisitor::visit(MethodDefStmt const& stmt) {
    if (!configurator_->sh_meth_def() ||
        (stmt.access_ != AccessModifier::Public && configurator_->get_view()->view() != std::move("inner")) ||
        stmt.virtuality_ == Virtuality::Virtual
    ) {
        return;
    }
    configurator_->sh_other_expr() ? (exporter_->write_method_word(), exporter_->write_space()) : void();
    if (configurator_->sh_meth_owner() && stmt.owner_) {
        exporter_->write_class_name(stmt.owner_->name_);
        if (configurator_->sh_gener_par() && !stmt.owner_->tparams_.empty()) {
            write_gen_params(stmt.owner_->tparams_);
        }
        exporter_->write_separator_sign(std::move("::"));
    }
    exporter_->write_method_name(stmt.func_->name_);
    write_parameters(stmt.func_->params_);
    write_return_type(stmt.func_->retType_);
    write_body(stmt.func_->body_);
}

void ASTVisitor::visit(BaseInitializerStmt const& stmt) {
    if (configurator_->sh_other_expr()) {
        exporter_->write_call_word();
        exporter_->write_space();
        exporter_->write_constr_word();
        exporter_->write_space();
    }
    std::string name = stmt.base_;
    size_t st = name.find(std::move('<'));
    size_t en = name.find(std::move('>'));
    if (st != std::string::npos && en != std::string::npos && st < en) {
        std::string cl = name.substr(0, st);
        std::string genPar = name.substr(st + 1, en - st - 1);
        exporter_->write_class_name(std::move(cl));
        exporter_->write_separator_sign(std::move("<"));
        exporter_->write_gen_param_name(std::move(genPar));
        exporter_->write_separator_sign(std::move(">"));
    } else {
        exporter_->write_class_name(std::move(name));
    }
    write_parameters(stmt.args_);
}

void ASTVisitor::visit(ConstructorDefStmt const& stmt) {
    if (!configurator_->sh_meth_def() || (stmt.access_ != AccessModifier::Public && configurator_->get_view()->view() != std::move("inner"))) {
        return;
    }
    configurator_->sh_other_expr() ? (exporter_->write_constr_word(), exporter_->write_space()) : void();
    if (configurator_->sh_meth_owner() && stmt.owner_) {
        exporter_->write_class_name(stmt.owner_->name_);
        if (configurator_->sh_gener_par() && !stmt.owner_->tparams_.empty()) {
            write_gen_params(stmt.owner_->tparams_);
        }
        exporter_->write_separator_sign(std::move("::"));
    }
    stmt.owner_ ? exporter_->write_class_name(stmt.owner_->name_) : exporter_->write_invalid_word();
    write_parameters(stmt.params_);
    if (!stmt.baseInit_.empty()) {
        for (size_t i = 0; i < stmt.baseInit_.size(); ++i) {
            exporter_->write_new_line();
            write_arrow();
            stmt.baseInit_.at(i) ? stmt.baseInit_.at(i)->accept(*this) : exporter_->write_invalid_word();
        }
    }
    write_body(stmt.body_);
}

void ASTVisitor::visit(DestructorDefStmt const& stmt) {
    if (!configurator_->sh_meth_def()) {
        return;
    }
    configurator_->sh_other_expr() ? (exporter_->write_destr_word(), exporter_->write_space()) : void();
    if (configurator_->sh_meth_owner() && stmt.owner_) {
        exporter_->write_class_name(stmt.owner_->name_);
        if (configurator_->sh_gener_par() && !stmt.owner_->tparams_.empty()) {
            write_gen_params(stmt.owner_->tparams_);
        }
        exporter_->write_separator_sign(std::move("::"));
    }
    stmt.owner_ ? exporter_->write_class_name(std::move("~") + stmt.owner_->name_) : exporter_->write_invalid_word();
    exporter_->write_round_bracket(std::move("("));
    exporter_->write_round_bracket(std::move(")"));
    write_body(stmt.body_);
}

void ASTVisitor::visit(GenericParam const& stmt) {
    exporter_->write_gen_param_name(stmt.name_);
    currGenParams_->push_back(std::stringstream(stmt.name_));
}

void ASTVisitor::visit(InterfaceDefStmt const& stmt) {
    if (!configurator_->sh_interf_dec()) {
        return;
    }
    currInterfName_ = std::make_unique<std::stringstream>(stmt.name_);
    if (configurator_->sh_gener_par() && !stmt.tparams_.empty()) {
        write_gen_params(stmt.tparams_);
        exporter_->write_new_line();
    }
    exporter_->write_interface_word();
    exporter_->write_space();
    exporter_->write_interface_name(stmt.name_);
    if (!stmt.bases_.empty()) {
        write_implementations(stmt.bases_);
    }
    write_open_curl_bracket();
    if (configurator_->sh_interf_def() && configurator_->sh_meth_dec() && !stmt.methods_.empty()) {
        std::vector<MethodDefStmt*> meth {};
        if (has_acc_mod(stmt.methods_, meth, AccessModifier::Public)) {
            exporter_->write_public_word();
            write_methods(meth);
        }
        if (configurator_->get_view()->view() == std::move("inner")) {
            if (has_acc_mod(stmt.methods_, meth, AccessModifier::Protected)) {
                exporter_->write_protected_word();
                write_methods(meth);
            }
        }
    }
    exporter_->write_curl_bracket(std::move("}"));
    exporter_->write_new_line();
    currInterfName_ = std::make_unique<std::stringstream>();
}

void ASTVisitor::visit(ClassDefStmt const& stmt) {
    currClassName_ = std::make_unique<std::stringstream>(stmt.name_);
    if (configurator_->sh_class_dec()) {
        if (configurator_->sh_gener_par() && !stmt.tparams_.empty()) {
            write_gen_params(stmt.tparams_);
            exporter_->write_new_line();
        }
        exporter_->write_class_word();
        exporter_->write_space();
        exporter_->write_class_name(stmt.name_);
        if (!stmt.interfaces_.empty()) {
            write_implementations(stmt.interfaces_);
        }
        write_open_curl_bracket();
        if (configurator_->sh_class_def()) {
            if (configurator_->sh_attrib() && !stmt.vars_.empty()) {
                std::vector<MemberVarDefStmt*> vars {};
                if (has_acc_mod(stmt.vars_, vars, AccessModifier::Public)) {
                    exporter_->write_public_word();
                    write_member_vars(vars);
                }
                if (configurator_->get_view()->view() == std::move("inner")) {
                    if (has_acc_mod(stmt.vars_, vars, AccessModifier::Private)) {
                        exporter_->write_private_word();
                        write_member_vars(vars);
                    }
                    if (has_acc_mod(stmt.vars_, vars, AccessModifier::Protected)) {
                        exporter_->write_protected_word();
                        write_member_vars(vars);
                    }
                }
            }
            if (configurator_->sh_meth_dec()) {
                if (!stmt.constructors_.empty()) {
                    std::vector<ConstructorDefStmt*> con {};
                    if (has_acc_mod(stmt.constructors_, con, AccessModifier::Public)) {
                        exporter_->write_public_word();
                        write_constructors(con);
                    }
                    if (configurator_->get_view()->view() == std::move("inner")) {
                        if (has_acc_mod(stmt.constructors_, con, AccessModifier::Private)) {
                            exporter_->write_private_word();
                            write_constructors(con);
                        }
                        if (has_acc_mod(stmt.constructors_, con, AccessModifier::Protected)) {
                            exporter_->write_protected_word();
                            write_constructors(con);
                        }
                    }
                }
                if (!stmt.destructors_.empty()) {
                    exporter_->write_destrs_word();
                    exporter_->write_new_line();
                    exporter_->increase_indentation();
                    for (size_t i = 0; i < stmt.destructors_.size(); ++i) {
                        if (stmt.destructors_.at(i) && stmt.destructors_.at(i)->owner_) {
                            exporter_->write_class_name(std::move("~") + stmt.destructors_.at(i)->owner_->name_);
                            exporter_->write_round_bracket(std::move("("));
                            exporter_->write_round_bracket(std::move(")"));
                        } else {
                            exporter_->write_invalid_word();
                        }
                        exporter_->write_new_line();
                    }
                    exporter_->decrease_indentation();
                }
                if (!stmt.methods_.empty()) {
                    std::vector<MethodDefStmt*> meth {};
                    if (has_acc_mod(stmt.methods_, meth, AccessModifier::Public)) {
                        exporter_->write_public_word();
                        write_methods(meth);
                    }
                    if (configurator_->get_view()->view() == std::move("inner")) {
                        if (has_acc_mod(stmt.methods_, meth, AccessModifier::Private)) {
                            exporter_->write_private_word();
                            write_methods(meth);
                        }
                        if (has_acc_mod(stmt.methods_, meth, AccessModifier::Protected)) {
                            exporter_->write_protected_word();
                            write_methods(meth);
                        }
                    }
                }
            }
        }
        exporter_->write_curl_bracket(std::move("}"));
        exporter_->write_new_line();
    }
    if (configurator_->sh_meth_def()) {
        for (size_t i = 0; i < stmt.constructors_.size(); ++i) {
            if (stmt.constructors_.at(i)) {
                exporter_->write_new_line();
                stmt.constructors_.at(i)->accept(*this);
                exporter_->write_new_line();
            }
        }
        for (size_t i = 0; i < stmt.destructors_.size(); ++i) {
            if (stmt.destructors_.at(i)) {
                exporter_->write_new_line();
                stmt.destructors_.at(i)->accept(*this);
                exporter_->write_new_line();
            }
        }
        for (size_t i = 0; i < stmt.methods_.size(); ++i) {
            if (stmt.methods_.at(i) && stmt.methods_.at(i)->virtuality_ == Virtuality::NotVirtual) {
                exporter_->write_new_line();
                stmt.methods_.at(i)->accept(*this);
                exporter_->write_new_line();
            }
        }
    }
    currClassName_ = std::make_unique<std::stringstream>();
}

void ASTVisitor::visit(ContinueStmt const& /*stmt*/) {
    exporter_->write_continue_word();
}

void ASTVisitor::visit(BreakStmt const& /*stmt*/) {
    exporter_->write_break_word();
}

//---------------------------------------------------------------------------------------

void ASTVisitor::write_open_curl_bracket() {
    configurator_->use_cpp_br() ? exporter_->write_new_line() : exporter_->write_space();
    exporter_->write_curl_bracket(std::move("{"));
    exporter_->write_new_line();
}

void ASTVisitor::write_arrow() {
    exporter_->write_space();
    exporter_->write_separator_sign(std::move("->"));
    exporter_->write_space();
}

void ASTVisitor::write_initialization(const VarDefStmt* init) {
    exporter_->write_space();
    exporter_->write_assign_op_word();
    exporter_->write_space();
    init->initializer_->accept(*this);
}

void ASTVisitor::write_member_vars(std::vector<MemberVarDefStmt*>& vars) {
    exporter_->write_space();
    exporter_->write_attribs_word();
    exporter_->write_new_line();
    exporter_->increase_indentation();
    bool once = false;
    for (size_t i = 0; i < vars.size(); ++i) {
        if (vars.at(i)) {
            vars.at(i)->accept(*this);
            exporter_->write_new_line();
            once = true;
        }
    }
    once ? void() : exporter_->write_new_line();
    exporter_->decrease_indentation();
    vars.clear();
}

void ASTVisitor::write_constructors(std::vector<ConstructorDefStmt*>& constr) {
    exporter_->write_space();
    exporter_->write_constrs_word();
    exporter_->write_new_line();
    exporter_->increase_indentation();
    for (size_t i = 0; i < constr.size(); ++i) {
        if (constr.at(i) && constr.at(i)->owner_) {
            exporter_->write_class_name(constr.at(i)->owner_->name_);
            write_parameters(constr.at(i)->params_);
        } else {
            exporter_->write_invalid_word();
        }
        exporter_->write_new_line();
    }
    exporter_->decrease_indentation();
    constr.clear();
}

void ASTVisitor::write_methods(std::vector<MethodDefStmt*>& meth) {
    exporter_->write_space();
    exporter_->write_meths_word();
    exporter_->write_new_line();
    exporter_->increase_indentation();
    bool once = false;
    for (size_t i = 0; i < meth.size(); ++i) {
        if (meth.at(i) && meth.at(i)->func_) {
            if (meth.at(i)->virtuality_ == Virtuality::Virtual) {
                exporter_->write_virtual_word();
                write_arrow();
            }
            exporter_->write_method_name(meth.at(i)->func_->name_);
            write_parameters(meth.at(i)->func_->params_);
            write_return_type(meth.at(i)->func_->retType_);
            exporter_->write_new_line();
            once = true;
        }
    }
    once ? void() : exporter_->write_new_line();
    exporter_->decrease_indentation();
    meth.clear();
}

void ASTVisitor::write_return_type(Type* type) {
    write_arrow();
    configurator_->sh_other_expr() ? (exporter_->write_returns_word(), exporter_->write_space()) : void();
    type ? type->accept(*this) : exporter_->write_invalid_word();
}

void ASTVisitor::write_body(Stmt* body) {
    write_open_curl_bracket();
    exporter_->increase_indentation();
    body ? (body->accept(*this), exporter_->write_new_line()) : void();
    exporter_->decrease_indentation();
    exporter_->write_curl_bracket(std::move("}"));
}

void ASTVisitor::write_cond(Expr* cond) {
    exporter_->write_space();
    exporter_->write_round_bracket(std::move("("));
    cond ? cond->accept(*this) : void();
    exporter_->write_round_bracket(std::move(")"));
}

void ASTVisitor::write_gen_params(const std::vector<GenericParam*>& params) {
    exporter_->write_separator_sign(std::move("<"));
    for (size_t i = 0; i < params.size(); ++i) {
        params.at(i) ? params.at(i)->accept(*this) : exporter_->write_invalid_word();
        (i < params.size() - 1) ? (exporter_->write_separator_sign(std::move(",")), exporter_->write_space()) : void();
    }
    exporter_->write_separator_sign(std::move(">"));
}

void ASTVisitor::write_implementations(const std::vector<InterfaceDefStmt*>& interfaces) {
    for (size_t i = 0; i < interfaces.size(); ++i) {
        exporter_->write_new_line();
        write_arrow();
        exporter_->write_implement_word();
        exporter_->write_space();
        exporter_->write_interface_word();
        exporter_->write_space();
        if (interfaces.at(i)) {
            exporter_->write_interface_name(interfaces.at(i)->name_);
            interfaces.at(i)->tparams_.empty() ? void() : write_gen_params(interfaces.at(i)->tparams_);
        } else {
            exporter_->write_invalid_word();
        }
    }
}