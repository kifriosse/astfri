#include <libastfri-text/inc/code/AbstractCodeVisitor.hpp>

using namespace astfri::text;

AbstractCodeVisitor::AbstractCodeVisitor(AbstractCodeBuilder* const& builder) :
    AbstractVisitor(builder)
{
}

void AbstractCodeVisitor::visit(IntType const& /*type*/)
{
    builder_->append_text("int");
}

void AbstractCodeVisitor::visit(CharType const& /*type*/)
{
    builder_->append_text("char");
}

void AbstractCodeVisitor::visit(VoidType const& /*type*/)
{
    builder_->append_text("void");
}

void AbstractCodeVisitor::visit(ClassType const& type)
{
    builder_->append_text(type.name_);
}

void AbstractCodeVisitor::visit(InterfaceType const& type)
{
    builder_->append_text(type.name_);
}

void AbstractCodeVisitor::visit(UnknownType const& /*type*/)
{
    builder_->write_unknown_type();
}

void AbstractCodeVisitor::visit(IntLiteralExpr const& expr)
{
    builder_->write_int_val(expr.val_);
}

void AbstractCodeVisitor::visit(FloatLiteralExpr const& expr)
{
    builder_->write_float_val(expr.val_);
}

void AbstractCodeVisitor::visit(CharLiteralExpr const& expr)
{
    builder_->write_char_val(expr.val_);
}

void AbstractCodeVisitor::visit(StringLiteralExpr const& expr)
{
    builder_->write_string_val(expr.val_);
}

void AbstractCodeVisitor::visit(BoolLiteralExpr const& expr)
{
    builder_->write_bool_val(expr.val_);
}

void AbstractCodeVisitor::visit(IfExpr const& expr)
{
    check_and_accept_pointer_w_error(expr.cond_);
    builder_->append_text(" ? ");
    check_and_accept_pointer_w_error(expr.iftrue_);
    builder_->append_text(" : ");
    check_and_accept_pointer_w_error(expr.iffalse_);
    builder_->append_text(";");
}

void AbstractCodeVisitor::visit(BinOpExpr const& expr)
{
    bool usebr = false;
    switch (expr.op_)
    {
        case BinOpType::Add:
        case BinOpType::Subtract:
        case BinOpType::Multiply:
        case BinOpType::Divide:
        case BinOpType::FloorDivide:
        case BinOpType::Modulo:
        case BinOpType::Exponentiation:
        case BinOpType::LogicalAnd:
        case BinOpType::LogicalOr:
        case BinOpType::BitShiftRight:
        case BinOpType::BitShiftLeft:
        case BinOpType::BitAnd:
        case BinOpType::BitOr:
        case BinOpType::BitXor:
            usebr = true;
        default:
            break;
    }
    if (usebr)
    {
        builder_->append_text("(");
    }
    check_and_accept_pointer_w_error(expr.left_);
    builder_->append_space();
    switch (expr.op_)
    {
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
    check_and_accept_pointer_w_error(expr.right_);
    if (usebr)
    {
        builder_->append_text(")");
    }
}

void AbstractCodeVisitor::visit(UnaryOpExpr const& expr)
{
    switch (expr.op_)
    {
        case UnaryOpType::LogicalNot:
            builder_->append_text("!");
            check_and_accept_pointer_w_error(expr.arg_);
            break;
        case UnaryOpType::Minus:
            builder_->append_text("(");
            builder_->append_text("-");
            check_and_accept_pointer_w_error(expr.arg_);
            builder_->append_text(")");
            break;
        case UnaryOpType::Plus:
            check_and_accept_pointer_w_error(expr.arg_);
            break;
        case UnaryOpType::Dereference:
            builder_->append_text("*");
            builder_->append_text("(");
            check_and_accept_pointer_w_error(expr.arg_);
            builder_->append_text(")");
            break;
        case UnaryOpType::AddressOf:
            builder_->append_text("&");
            builder_->append_text("(");
            check_and_accept_pointer_w_error(expr.arg_);
            builder_->append_text(")");
            break;
        case UnaryOpType::PreIncrement:
            builder_->append_text("++");
            check_and_accept_pointer_w_error(expr.arg_);
            break;
        case UnaryOpType::PostIncrement:
            check_and_accept_pointer_w_error(expr.arg_);
            builder_->append_text("++");
            break;
        case UnaryOpType::PreDecrement:
            builder_->append_text("--");
            check_and_accept_pointer_w_error(expr.arg_);
            break;
        case UnaryOpType::PostDecrement:
            check_and_accept_pointer_w_error(expr.arg_);
            builder_->append_text("--");
            break;
        case UnaryOpType::BitFlip:
            builder_->append_text("~");
            check_and_accept_pointer_w_error(expr.arg_);
            break;
    }
}

void AbstractCodeVisitor::visit(ParamVarRefExpr const& expr)
{
    builder_->append_text(expr.param_);
}

void AbstractCodeVisitor::visit(LocalVarRefExpr const& expr)
{
    builder_->append_text(expr.var_);
}

void AbstractCodeVisitor::visit(GlobalVarRefExpr const& expr)
{
    builder_->append_text(expr.global_);
}

void AbstractCodeVisitor::visit(ClassRefExpr const& expr)
{
    builder_->append_text(expr.name_);
}

void AbstractCodeVisitor::visit(ThisExpr const& /*expr*/)
{
    builder_->append_text("this");
}

void AbstractCodeVisitor::visit(ConstructorCallExpr const& expr)
{
    check_and_accept_pointer_w_error(expr.type_);
    write_params_or_args(expr.args_, false);
}

void AbstractCodeVisitor::visit(NewExpr const& expr)
{
    builder_->append_text("new ");
    check_and_accept_pointer_w_error(expr.init_);
}

void AbstractCodeVisitor::visit(UnknownExpr const& /*expr*/)
{
    builder_->write_unknown_expr();
}

void AbstractCodeVisitor::visit(CompoundStmt const& stmt)
{
    for (size_t i = 0; i < stmt.stmts_.size(); ++i) {
        check_and_accept_pointer_w_error(stmt.stmts_.at(i));
        if (i < stmt.stmts_.size() - 1) {
            builder_->append_new_line();
        }
    }
}

void AbstractCodeVisitor::visit(ReturnStmt const& stmt)
{
    builder_->append_text("return");
    if (stmt.val_) {
        builder_->append_space();
        check_and_accept_pointer_w_error(stmt.val_);
    }
    builder_->append_text(";");
}

void AbstractCodeVisitor::visit(ExprStmt const& stmt)
{
    check_and_accept_pointer_w_error(stmt.expr_);
}

void AbstractCodeVisitor::visit(IfStmt const& stmt)
{
    builder_->append_text("if");
    write_condition(stmt.cond_);
    builder_->write_opening_curl_bracket();
    check_and_accept_pointer_w_error(stmt.iftrue_);
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
    if (stmt.iffalse_) {
        builder_->write_opening_else_word();
        check_and_accept_pointer_w_error(stmt.iffalse_);
        builder_->append_new_line();
        builder_->decrease_indentation();
        builder_->append_text("}");
    }
}

void AbstractCodeVisitor::visit(CaseStmt const& stmt)
{
    builder_->append_text("case ");
    if (stmt.exprs_.empty()) {
        builder_->write_invalid_expr();
    } else {
        for (size_t i = 0; i < stmt.exprs_.size(); ++i) {
            check_and_accept_pointer_w_error(stmt.exprs_.at(i));
            if (i < stmt.exprs_.size() - 1) {
                builder_->append_text(", ");
            }
        }
    }
    builder_->append_text(":");
    builder_->append_new_line();
    builder_->increase_indentation();
    check_and_accept_pointer_w_error(stmt.body_);
    builder_->decrease_indentation();
}

void AbstractCodeVisitor::visit(DefaultCaseStmt const& stmt)
{
    builder_->append_text("default:");
    builder_->append_new_line();
    builder_->increase_indentation();
    check_and_accept_pointer_w_error(stmt.body_);
    builder_->decrease_indentation();
}

void AbstractCodeVisitor::visit(SwitchStmt const& stmt)
{
    builder_->append_text("switch");
    write_condition(stmt.expr_);
    builder_->write_opening_curl_bracket();
    for (size_t i = 0; i < stmt.cases_.size(); ++i) {
        check_and_accept_pointer_w_error(stmt.cases_.at(i));
        if (i < stmt.cases_.size() - 1) {
            builder_->append_new_line();
        }
    }
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");    
}

void AbstractCodeVisitor::visit(WhileStmt const& stmt)
{
    builder_->append_text("while");
    write_condition(stmt.cond_);
    builder_->write_opening_curl_bracket();
    check_and_accept_pointer_w_error(stmt.body_);
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
}

void AbstractCodeVisitor::visit(DoWhileStmt const& stmt)
{
    builder_->append_text("do");
    builder_->write_opening_curl_bracket();
    check_and_accept_pointer_w_error(stmt.body_);
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("} while");
    write_condition(stmt.cond_);
}

void AbstractCodeVisitor::visit(ForStmt const& stmt)
{
    builder_->append_text("for (");
    check_and_accept_pointer_w_error(stmt.init_);
    builder_->append_text("; ");
    check_and_accept_pointer_w_error(stmt.cond_);
    builder_->append_text("; ");
    check_and_accept_pointer_w_error(stmt.step_);
    builder_->append_text(")");
    builder_->write_opening_curl_bracket();
    check_and_accept_pointer_w_error(stmt.body_);
    builder_->append_new_line();
    builder_->decrease_indentation();
    builder_->append_text("}");
}

void AbstractCodeVisitor::visit(ThrowStmt const& stmt)
{
    builder_->append_text("throw ");
    check_and_accept_pointer_w_error(stmt.val_);
    builder_->append_text(";");
}

void AbstractCodeVisitor::visit(UnknownStmt const& /*stmt*/)
{
    builder_->write_unknown_stmt();
}

void AbstractCodeVisitor::visit(LocalVarDefStmt const& stmt)
{
    check_and_accept_pointer_w_error(stmt.type_);
    builder_->append_space();
    builder_->append_text(stmt.name_);
    if (stmt.initializer_) {
        builder_->append_text(" = ");
        check_and_accept_pointer_w_error(stmt.initializer_);
    }
    builder_->append_text(";");
}

void AbstractCodeVisitor::visit(ParamVarDefStmt const& stmt)
{
    check_and_accept_pointer_w_error(stmt.type_);
    builder_->append_space();
    builder_->append_text(stmt.name_);
    if (stmt.initializer_) {
        builder_->append_text(" = ");
        check_and_accept_pointer_w_error(stmt.initializer_);
    }
}

void AbstractCodeVisitor::visit(GlobalVarDefStmt const& stmt)
{
    check_and_accept_pointer_w_error(stmt.type_);
    builder_->append_space();
    builder_->append_text(stmt.name_);
    if (stmt.initializer_) {
        builder_->append_text(" = ");
        check_and_accept_pointer_w_error(stmt.initializer_);
    }
    builder_->append_text(";");
}

void AbstractCodeVisitor::visit(ContinueStmt const& /*stmt*/)
{
    builder_->append_text("continue;");
}

void AbstractCodeVisitor::visit(BreakStmt const& /*stmt*/)
{
    builder_->append_text("break;");
}