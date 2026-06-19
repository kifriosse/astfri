#include <astfri-text/impl/PseudocodeVisitor.hpp>

using namespace astfri::text;

PseudocodeVisitor::PseudocodeVisitor(PseudocodeBuilder& builder, Config const& config) :
    AbstractVisitor(builder),
    m_builder(builder),
    m_config(config),
    m_isMethodCall(false),
    m_isConstructorCall(false)
{
}

void PseudocodeVisitor::reset_visitor()
{
    m_isMethodCall = false;
    m_isConstructorCall = false;
}

void PseudocodeVisitor::visit(const DynamicType& /*type*/)
{
    m_builder.write_dynamic_type_word();
}

void PseudocodeVisitor::visit(const IntType& /*type*/)
{
    m_builder.write_int_type_word();
}

void PseudocodeVisitor::visit(const FloatType& /*type*/)
{
    m_builder.write_float_type_word();
}

void PseudocodeVisitor::visit(const CharType& /*type*/)
{
    m_builder.write_char_type_word();
}

void PseudocodeVisitor::visit(const BoolType& /*type*/)
{
    m_builder.write_bool_type_word();
}

void PseudocodeVisitor::visit(const VoidType& /*type*/)
{
    m_builder.write_void_type_word();
}

void PseudocodeVisitor::visit(const IndirectionType& type)
{
    accept_node(type.indirect);
    m_builder.write_pointer_operator();
}

void PseudocodeVisitor::visit(const ClassType& type)
{
    m_builder.write_class_name(type.name);
}

void PseudocodeVisitor::visit(const InterfaceType& type)
{
    m_builder.write_interface_name(type.name);
}

void PseudocodeVisitor::visit(const LambdaType& type)
{
    m_builder.write_system_type(type.name);
}

void PseudocodeVisitor::visit(const IncompleteType& type)
{
    m_builder.write_system_type(type.name);
}

void PseudocodeVisitor::visit(const DeducedType& type)
{
    accept_node(type.realType); // TODO: auto / var ?
}

void PseudocodeVisitor::visit(const UnknownType& /*type*/)
{
    m_builder.write_unknown_type();
}

void PseudocodeVisitor::visit(const IntLiteralExpr& expr)
{
    m_builder.write_int_val(expr.val);
}

void PseudocodeVisitor::visit(const FloatLiteralExpr& expr)
{
    m_builder.write_float_val(expr.val);
}

void PseudocodeVisitor::visit(const CharLiteralExpr& expr)
{
    m_builder.write_char_val(expr.val);
}

void PseudocodeVisitor::visit(const StringLiteralExpr& expr)
{
    m_builder.write_string_val(expr.val);
}

void PseudocodeVisitor::visit(const BoolLiteralExpr& expr)
{
    m_builder.write_bool_val(expr.val);
}

void PseudocodeVisitor::visit(const NullLiteralExpr& /*expr*/)
{
    m_builder.write_null_val();
}

void PseudocodeVisitor::visit(const IfExpr& expr)
{
    m_builder.write_if_word();
    process_condition(expr.cond);
    m_builder.write_space();
    m_builder.write_separator("?");
    m_builder.write_new_line();
    m_builder.increase_indentation();
    accept_node(expr.iftrue);
    m_builder.write_space();
    m_builder.write_separator(":");
    m_builder.write_new_line();
    accept_node(expr.iffalse);
    m_builder.decrease_indentation();
}

void PseudocodeVisitor::visit(const BinOpExpr& expr)
{
    accept_node(expr.left);
    m_builder.write_space();
    switch (expr.op)
    {
        case BinOpType::UNINITIALIZED:
            m_builder.write_operator("?");
            break;
        case BinOpType::Assign:
            m_builder.write_assign_operator();
            break;
        case BinOpType::Add:
            m_builder.write_operator("+");
            break;
        case BinOpType::Subtract:
            m_builder.write_operator("-");
            break;
        case BinOpType::Multiply:
            m_builder.write_operator("*");
            break;
        case BinOpType::Divide:
            m_builder.write_operator("/");
            break;
        case BinOpType::FloorDivide:
            m_builder.write_operator("div");
            break;
        case BinOpType::Modulo:
            m_builder.write_modulo_operator();
            break;
        case BinOpType::Exponentiation:
            m_builder.write_operator("**");
            break;
        case BinOpType::Equal:
            m_builder.write_operator("==");
            break;
        case BinOpType::NotEqual:
            m_builder.write_operator("!=");
            break;
        case BinOpType::Less:
            m_builder.write_operator("<");
            break;
        case BinOpType::LessEqual:
            m_builder.write_operator("<=");
            break;
        case BinOpType::Greater:
            m_builder.write_operator(">");
            break;
        case BinOpType::GreaterEqual:
            m_builder.write_operator(">=");
            break;
        case BinOpType::LogicalAnd:
            m_builder.write_operator("&&");
            break;
        case BinOpType::LogicalOr:
            m_builder.write_operator("||");
            break;
        case BinOpType::BitShiftRight:
            m_builder.write_operator(">>");
            break;
        case BinOpType::BitShiftRightUnsigned:
            m_builder.write_operator(">>>");
            break;
        case BinOpType::BitShiftLeft:
            m_builder.write_operator("<<");
            break;
        case BinOpType::BitAnd:
            m_builder.write_operator("&");
            break;
        case BinOpType::BitOr:
            m_builder.write_operator("|");
            break;
        case BinOpType::BitXor:
            m_builder.write_operator("^");
            break;
        case BinOpType::Comma:
            m_builder.write_separator(",");
            break;
        case BinOpType::AddAssign:
            m_builder.write_operator("+=");
            break;
        case BinOpType::SubtractAssign:
            m_builder.write_operator("-=");
            break;
        case BinOpType::MultiplyAssign:
            m_builder.write_operator("*=");
            break;
        case BinOpType::DivideAssign:
            m_builder.write_operator("/=");
            break;
        case BinOpType::FloorDivideAssign:
            m_builder.write_operator("div=");
            break;
        case BinOpType::ModuloAssign:
            m_builder.write_modulo_operator();
            m_builder.write_operator("=");
            break;
        case BinOpType::ExponentiationAssign:
            m_builder.write_operator("**=");
            break;
        case BinOpType::BitShiftRightAssign:
            m_builder.write_operator(">>=");
            break;
        case BinOpType::BitShiftLeftAssign:
            m_builder.write_operator("<<=");
            break;
        case BinOpType::BitAndAssign:
            m_builder.write_operator("&=");
            break;
        case BinOpType::BitOrAssign:
            m_builder.write_operator("|=");
            break;
        case BinOpType::BitXorAssign:
            m_builder.write_operator("^=");
            break;
    }
    m_builder.write_space();
    accept_node(expr.right);
}

void PseudocodeVisitor::visit(const UnaryOpExpr& expr)
{
    switch (expr.op)
    {
        case UnaryOpType::UNINITIALIZED:
            m_builder.write_operator("?");
            accept_node(expr.arg);
            break;
        case UnaryOpType::LogicalNot:
            m_builder.write_operator("!");
            accept_node(expr.arg);
            break;
        case UnaryOpType::Minus:
            m_builder.write_operator("-");
            accept_node(expr.arg);
            break;
        case UnaryOpType::Plus:
            accept_node(expr.arg);
            break;
        case UnaryOpType::Dereference:
            m_builder.write_deref_operator();
            accept_node(expr.arg);
            break;
        case UnaryOpType::AddressOf:
            m_builder.write_address_operator();
            accept_node(expr.arg);
            break;
        case UnaryOpType::PreIncrement:
            m_builder.write_operator("++");
            accept_node(expr.arg);
            break;
        case UnaryOpType::PostIncrement:
            accept_node(expr.arg);
            m_builder.write_operator("++");
            break;
        case UnaryOpType::PreDecrement:
            m_builder.write_operator("--");
            accept_node(expr.arg);
            break;
        case UnaryOpType::PostDecrement:
            accept_node(expr.arg);
            m_builder.write_operator("--");
            break;
        case UnaryOpType::BitFlip:
            m_builder.write_operator("~");
            accept_node(expr.arg);
            break;
    }
}

void PseudocodeVisitor::visit(const ParamVarRefExpr& expr)
{
    m_builder.write_param_var_name(expr.param);
}

void PseudocodeVisitor::visit(const LocalVarRefExpr& expr)
{
    m_builder.write_local_var_name(expr.var);
}

void PseudocodeVisitor::visit(const MemberVarRefExpr& expr)
{
    if (expr.owner)
    {
        accept_node(expr.owner);
        m_builder.write_separator(".");
    }
    m_builder.write_member_var_name(expr.member);
}

void PseudocodeVisitor::visit(const GlobalVarRefExpr& expr)
{
    m_builder.write_global_var_name(expr.global);
}

void PseudocodeVisitor::visit(const ClassRefExpr& expr)
{
    m_builder.write_class_name(expr.name);
}

void PseudocodeVisitor::visit(const FunctionCallExpr& expr)
{
    m_builder.write_call_word();
    m_builder.write_function_name(expr.name);
    process_params_or_args(expr.args, false);
}

void PseudocodeVisitor::visit(const MethodCallExpr& expr)
{
    if (!m_isMethodCall)
    {
        m_isMethodCall = true;
        m_builder.write_call_word();
    }
    if (expr.owner)
    {
        accept_node(expr.owner);
    }
    else
    {
        m_builder.write_this_word();
    }
    m_builder.write_separator(".");
    m_isMethodCall = false;
    m_builder.write_method_name(expr.name);
    process_params_or_args(expr.args, false);
}

void PseudocodeVisitor::visit(const LambdaCallExpr& expr)
{
    m_builder.write_call_word();
    accept_node(expr.lambda);
    process_params_or_args(expr.args, false);
}

void PseudocodeVisitor::visit(const LambdaExpr& expr)
{
    m_builder.write_lambda_word();
    process_params_or_args(expr.params, false);
    process_body(expr.body, m_config.functionBlockBracketNewLine);
}

void PseudocodeVisitor::visit(const ThisExpr& /*expr*/)
{
    m_builder.write_this_word();
}

void PseudocodeVisitor::visit(const BaseExpr& /*expr*/)
{
    m_builder.write_text("super"); // TODO: super / base?
}

void PseudocodeVisitor::visit(const ConstructorCallExpr& expr)
{
    if (!m_isConstructorCall)
    {
        m_builder.write_call_word();
        m_builder.write_constructor_word();
        m_builder.write_space();
    }
    m_isConstructorCall = true;
    accept_node(expr.type);
    m_isConstructorCall = false;
    process_params_or_args(expr.args, false);
}

void PseudocodeVisitor::visit(const NewExpr& expr)
{
    m_builder.write_new_word();
    m_isConstructorCall = true;
    accept_node(expr.init);
    m_isConstructorCall = false;
}

void PseudocodeVisitor::visit(const DeleteExpr& expr)
{
    m_builder.write_delete_word();
    accept_node(expr.arg);
}

void PseudocodeVisitor::visit(const BracketExpr& expr)
{
    m_builder.write_left_bracket("(");
    accept_node(expr.expr);
    m_builder.write_right_bracket(")");
}

void PseudocodeVisitor::visit(const UnknownExpr& /*expr*/)
{
    m_builder.write_unknown_expr();
}