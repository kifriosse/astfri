#include <libastfri-text/inc/code/AbstractCodeVisitor.hpp>

using namespace astfri::text;

AbstractCodeVisitor::AbstractCodeVisitor(AbstractCodeBuilder& builder) :
    AbstractVisitor(builder) {
}

//
// -----
//

void AbstractCodeVisitor::visit(const IntType& /*type*/) {
    builder_->append_text("int");
}

void AbstractCodeVisitor::visit(const CharType& /*type*/) {
    builder_->append_text("char");
}

void AbstractCodeVisitor::visit(const VoidType& /*type*/) {
    builder_->append_text("void");
}

void AbstractCodeVisitor::visit(const ClassType& type) {
    builder_->append_text(type.name);
}

void AbstractCodeVisitor::visit(const InterfaceType& type) {
    builder_->append_text(type.name);
}

void AbstractCodeVisitor::visit(const IncompleteType& type) {
    builder_->append_text(type.name);
}

void AbstractCodeVisitor::visit(const UnknownType& /*type*/) {
    builder_->write_unknown_type();
}

// -----

void AbstractCodeVisitor::visit(const IntLiteralExpr& expr) {
    builder_->write_int_val(expr.val);
}

void AbstractCodeVisitor::visit(const FloatLiteralExpr& expr) {
    builder_->write_float_val(expr.val);
}

void AbstractCodeVisitor::visit(const CharLiteralExpr& expr) {
    builder_->write_char_val(expr.val);
}

void AbstractCodeVisitor::visit(const StringLiteralExpr& expr) {
    builder_->write_string_val(expr.val);
}

void AbstractCodeVisitor::visit(const BoolLiteralExpr& expr) {
    builder_->write_bool_val(expr.val);
}

void AbstractCodeVisitor::visit(const IfExpr& expr) {
    if (! try_accept_node(expr.cond)) {
        builder_->write_invalid_expr();
    }
    builder_->append_text(" ? ");
    if (! try_accept_node(expr.iftrue)) {
        builder_->write_invalid_expr();
    }
    builder_->append_text(" : ");
    if (! try_accept_node(expr.iffalse)) {
        builder_->write_invalid_expr();
    }
    builder_->append_text(";");
}

void AbstractCodeVisitor::visit(const BinOpExpr& expr) {
    if (! try_accept_node(expr.left)) {
        builder_->write_invalid_expr();
    }
    builder_->append_space();
    switch (expr.op) {
    case BinOpType::Assign:
        builder_->append_text("=");
        break;
    case BinOpType::Add:
        builder_->append_text("+");
        break;
    case BinOpType::Subtract:
        builder_->append_text("-");
        break;
    case BinOpType::Multiply:
        builder_->append_text("*");
        break;
    case BinOpType::Divide:
        builder_->append_text("/");
        break;
    case BinOpType::FloorDivide:
        builder_->append_text("div");
        break;
    case BinOpType::Modulo:
        builder_->append_text("%");
        break;
    case BinOpType::Exponentiation:
        builder_->append_text("**");
        break;
    case BinOpType::Equal:
        builder_->append_text("==");
        break;
    case BinOpType::NotEqual:
        builder_->append_text("!=");
        break;
    case BinOpType::Less:
        builder_->append_text("<");
        break;
    case BinOpType::LessEqual:
        builder_->append_text("<=");
        break;
    case BinOpType::Greater:
        builder_->append_text(">");
        break;
    case BinOpType::GreaterEqual:
        builder_->append_text(">=");
        break;
    case BinOpType::LogicalAnd:
        builder_->append_text("&&");
        break;
    case BinOpType::LogicalOr:
        builder_->append_text("||");
        break;
    case BinOpType::BitShiftRight:
        builder_->append_text(">>");
        break;
    case BinOpType::BitShiftLeft:
        builder_->append_text("<<");
        break;
    case BinOpType::BitAnd:
        builder_->append_text("&");
        break;
    case BinOpType::BitOr:
        builder_->append_text("|");
        break;
    case BinOpType::BitXor:
        builder_->append_text("^");
        break;
    case BinOpType::Comma:
        builder_->append_text(",");
        break;
    case BinOpType::AddAssign:
        builder_->append_text("+=");
        break;
    case BinOpType::SubtractAssign:
        builder_->append_text("-=");
        break;
    case BinOpType::MultiplyAssign:
        builder_->append_text("*=");
        break;
    case BinOpType::DivideAssign:
        builder_->append_text("/=");
        break;
    case BinOpType::FloorDivideAssign:
        builder_->append_text("div=");
        break;
    case BinOpType::ModuloAssign:
        builder_->append_text("%=");
        break;
    case BinOpType::ExponentiationAssign:
        builder_->append_text("**=");
        break;
    case BinOpType::BitShiftRightAssign:
        builder_->append_text(">>=");
        break;
    case BinOpType::BitShiftLeftAssign:
        builder_->append_text("<<=");
        break;
    case BinOpType::BitAndAssign:
        builder_->append_text("&=");
        break;
    case BinOpType::BitOrAssign:
        builder_->append_text("|=");
        break;
    case BinOpType::BitXorAssign:
        builder_->append_text("^=");
        break;
    }
    builder_->append_space();
    if (! try_accept_node(expr.right)) {
        builder_->write_invalid_expr();
    }
}

void AbstractCodeVisitor::visit(const UnaryOpExpr& expr) {
    switch (expr.op) {
    case UnaryOpType::LogicalNot:
        builder_->append_text("!");
        if (! try_accept_node(expr.arg)) {
            builder_->write_invalid_expr();
        }
        break;
    case UnaryOpType::Minus:
        builder_->append_text("-");
        if (! try_accept_node(expr.arg)) {
            builder_->write_invalid_expr();
        }
        break;
    case UnaryOpType::Plus:
        if (! try_accept_node(expr.arg)) {
            builder_->write_invalid_expr();
        }
        break;
    case UnaryOpType::Dereference:
        builder_->append_text("*");
        if (! try_accept_node(expr.arg)) {
            builder_->write_invalid_expr();
        }
        break;
    case UnaryOpType::AddressOf:
        builder_->append_text("&");
        if (! try_accept_node(expr.arg)) {
            builder_->write_invalid_expr();
        }
        break;
    case UnaryOpType::PreIncrement:
        builder_->append_text("++");
        if (! try_accept_node(expr.arg)) {
            builder_->write_invalid_expr();
        }
        break;
    case UnaryOpType::PostIncrement:
        if (! try_accept_node(expr.arg)) {
            builder_->write_invalid_expr();
        }
        builder_->append_text("++");
        break;
    case UnaryOpType::PreDecrement:
        builder_->append_text("--");
        if (! try_accept_node(expr.arg)) {
            builder_->write_invalid_expr();
        }
        break;
    case UnaryOpType::PostDecrement:
        if (! try_accept_node(expr.arg)) {
            builder_->write_invalid_expr();
        }
        builder_->append_text("--");
        break;
    case UnaryOpType::BitFlip:
        builder_->append_text("~");
        if (! try_accept_node(expr.arg)) {
            builder_->write_invalid_expr();
        }
        break;
    }
}

void AbstractCodeVisitor::visit(const ParamVarRefExpr& expr) {
    builder_->append_text(expr.param);
}

void AbstractCodeVisitor::visit(const LocalVarRefExpr& expr) {
    builder_->append_text(expr.var);
}

void AbstractCodeVisitor::visit(const ClassRefExpr& expr) {
    builder_->append_text(expr.name);
}

void AbstractCodeVisitor::visit(const ThisExpr& /*expr*/) {
    builder_->append_text("this");
}

void AbstractCodeVisitor::visit(const ConstructorCallExpr& expr) {
    if (! try_accept_node(expr.type)) {
        builder_->write_invalid_type();
    }
    process_pargs(expr.args, false);
}

void AbstractCodeVisitor::visit(const NewExpr& expr) {
    builder_->append_text("new ");
    if (! try_accept_node(expr.init)) {
        builder_->write_invalid_expr();
    }
}

void AbstractCodeVisitor::visit(const BracketExpr& expr) {
    builder_->write_left_bracket("(");
    if (! try_accept_node(expr.expr)) {
        builder_->write_invalid_expr();
    }
    builder_->write_right_bracket(")");
}

void AbstractCodeVisitor::visit(const UnknownExpr& /*expr*/) {
    builder_->write_unknown_expr();
}

// -----

void AbstractCodeVisitor::visit(const CompoundStmt& stmt) {
    for (size_t i = 0; i < stmt.stmts.size(); ++i) {
        if (! try_accept_node(stmt.stmts.at(i))) {
            builder_->write_invalid_expr();
        }
        if (i < stmt.stmts.size() - 1) {
            builder_->append_new_line();
        }
    }
}

void AbstractCodeVisitor::visit(const ReturnStmt& stmt) {
    builder_->append_text("return");
    if (stmt.val) {
        builder_->append_space();
        try_accept_node(stmt.val);
    }
    builder_->append_text(";");
}

void AbstractCodeVisitor::visit(const ExprStmt& stmt) {
    if (! try_accept_node(stmt.expr)) {
        builder_->write_invalid_expr();
    }
}

void AbstractCodeVisitor::visit(const IfStmt& stmt) {
    builder_->append_text("if");
    process_condition(stmt.cond);
    builder_->write_opening_curl_bracket();
    if (! try_accept_node(stmt.iftrue)) {
        builder_->write_invalid_stmt();
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
    if (stmt.iffalse) {
        builder_->write_opening_else_word();
        try_accept_node(stmt.iffalse);
        builder_->append_new_line();
        builder_->decrease_indentation();
        builder_->append_text("}");
    }
}

void AbstractCodeVisitor::visit(const CaseStmt& stmt) {
    builder_->append_text("case ");
    if (stmt.exprs.empty()) {
        builder_->write_invalid_expr();
    }
    else {
        for (size_t i = 0; i < stmt.exprs.size(); ++i) {
            if (! try_accept_node(stmt.exprs.at(i))) {
                builder_->write_invalid_expr();
            }
            if (i < stmt.exprs.size() - 1) {
                builder_->append_text(", ");
            }
        }
    }
    builder_->append_text(":");
    builder_->append_new_line();
    builder_->increase_indentation();
    if (! try_accept_node(stmt.body)) {
        builder_->write_invalid_stmt();
    }
    builder_->decrease_indentation();
}

void AbstractCodeVisitor::visit(const DefaultCaseStmt& stmt) {
    builder_->append_text("default:");
    builder_->append_new_line();
    builder_->increase_indentation();
    if (! try_accept_node(stmt.body)) {
        builder_->write_invalid_stmt();
    }
    builder_->decrease_indentation();
}

void AbstractCodeVisitor::visit(const SwitchStmt& stmt) {
    builder_->append_text("switch");
    process_condition(stmt.expr_);
    builder_->write_opening_curl_bracket();
    for (size_t i = 0; i < stmt.cases.size(); ++i) {
        if (! try_accept_node(stmt.cases.at(i))) {
            builder_->write_invalid_stmt();
        }
        if (i < stmt.cases.size() - 1) {
            builder_->append_new_line();
        }
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
}

void AbstractCodeVisitor::visit(const WhileStmt& stmt) {
    builder_->append_text("while");
    process_condition(stmt.cond);
    builder_->write_opening_curl_bracket();
    if (! try_accept_node(stmt.body)) {
        builder_->write_invalid_stmt();
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
}

void AbstractCodeVisitor::visit(const DoWhileStmt& stmt) {
    builder_->append_text("do");
    builder_->write_opening_curl_bracket();
    if (! try_accept_node(stmt.body)) {
        builder_->write_invalid_stmt();
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("} while");
    process_condition(stmt.cond);
}

void AbstractCodeVisitor::visit(const ForStmt& stmt) {
    builder_->append_text("for (");
    if (! try_accept_node(stmt.init)) {
        builder_->write_invalid_stmt();
    }
    builder_->append_text("; ");
    if (! try_accept_node(stmt.cond)) {
        builder_->write_invalid_stmt();
    }
    builder_->append_text("; ");
    if (! try_accept_node(stmt.step)) {
        builder_->write_invalid_stmt();
    }
    builder_->append_text(")");
    builder_->write_opening_curl_bracket();
    if (! try_accept_node(stmt.body)) {
        builder_->write_invalid_stmt();
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
}

void AbstractCodeVisitor::visit(const ThrowStmt& stmt) {
    builder_->append_text("throw ");
    if (! try_accept_node(stmt.val)) {
        builder_->write_invalid_stmt();
    }
    builder_->append_text(";");
}

void AbstractCodeVisitor::visit(const UnknownStmt& /*stmt*/) {
    builder_->write_unknown_stmt();
}

void AbstractCodeVisitor::visit(const LocalVarDefStmt& stmt) {
    if (! try_accept_node(stmt.type)) {
        builder_->write_invalid_type();
    }
    builder_->append_space();
    builder_->append_text(stmt.name);
    if (stmt.initializer) {
        builder_->append_text(" = ");
        try_accept_node(stmt.initializer);
    }
    builder_->append_text(";");
}

void AbstractCodeVisitor::visit(const ParamVarDefStmt& stmt) {
    if (! try_accept_node(stmt.type)) {
        builder_->write_invalid_type();
    }
    builder_->append_space();
    builder_->append_text(stmt.name);
    if (stmt.initializer) {
        builder_->append_text(" = ");
        try_accept_node(stmt.initializer);
    }
}

void AbstractCodeVisitor::visit(const ContinueStmt& /*stmt*/) {
    builder_->append_text("continue;");
}

void AbstractCodeVisitor::visit(const BreakStmt& /*stmt*/) {
    builder_->append_text("break;");
}