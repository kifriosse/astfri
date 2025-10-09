#include <libastfri-text/inc/ASTTextVisitor.hpp>

#include <libastfri/inc/Expr.hpp>
#include <libastfri/inc/Type.hpp>

using namespace astfri::text;

constexpr int LOCAL_VAR = 0;
constexpr int PARAM_VAR = 1;
constexpr int MEMBER_VAR = 2;
constexpr int GLOBAL_VAR = 3;

ASTTextVisitor& ASTTextVisitor::get_instance(Exporter*& exp)
{
    static ASTTextVisitor visitor(exp);
    return visitor;
}

ASTTextVisitor::ASTTextVisitor(Exporter*& exp) :
    GeneralASTVisitor(exp),
    configurator_(&TextConfigurator::get_instance()),
    isMethodCall_(false),
    isConstructorCall_(false)
{
    currClassName_ = std::make_unique<std::stringstream>();
    currInterfName_ = std::make_unique<std::stringstream>();
    currGenParams_ = std::make_unique<std::vector<std::stringstream>>();
}

void ASTTextVisitor::visit(DynamicType const& /*type*/)
{
    exporter_->write_dynamic_type_word();
}

void ASTTextVisitor::visit(IntType const& /*type*/)
{
    exporter_->write_int_type_word();
}

void ASTTextVisitor::visit(FloatType const& /*type*/)
{
    exporter_->write_float_type_word();
}

void ASTTextVisitor::visit(CharType const& /*type*/)
{
    exporter_->write_char_type_word();
}

void ASTTextVisitor::visit(BoolType const& /*type*/)
{
    exporter_->write_bool_type_word();
}

void ASTTextVisitor::visit(VoidType const& /*type*/)
{
    exporter_->write_void_type_word();
}

void ASTTextVisitor::visit(UserType const& type)
{
    //type.ref_->accept(*this);
    write_identifier_from_string(type.name_);
}

void ASTTextVisitor::visit(IndirectionType const& type)
{
    check_and_accept_pointer_w_error(type.indirect_);
    exporter_->write_pointer_word();
}

/*void ASTTextVisitor::visit(ObjectType const& type)
{
    exporter_->write_object_type(type.name_);
    if (!type.targs_.empty())
    {
        write_params_or_args(type.targs_, true);
    }
}*/

/*void ASTTextVisitor::visit(GenericType const& type)
{
    exporter_->write_gen_arg_name(type.name_);
}*/

void ASTTextVisitor::visit(UnknownType const& /*type*/)
{
    exporter_->write_unknown_type_word();
}

void ASTTextVisitor::visit(IntLiteralExpr const& expr)
{
    exporter_->write_int_val(expr.val_);
}

void ASTTextVisitor::visit(FloatLiteralExpr const& expr)
{
    exporter_->write_float_val(expr.val_);
}

void ASTTextVisitor::visit(CharLiteralExpr const& expr)
{
    exporter_->write_char_val(expr.val_);
}

void ASTTextVisitor::visit(StringLiteralExpr const& expr)
{
    exporter_->write_string_val(expr.val_);
}

void ASTTextVisitor::visit(BoolLiteralExpr const& expr)
{
    exporter_->write_bool_val(expr.val_);
}

void ASTTextVisitor::visit(NullLiteralExpr const& /*expr*/)
{
    exporter_->write_null_val();
}

void ASTTextVisitor::visit(IfExpr const& expr)
{
    exporter_->write_if_word();
    write_cond(expr.cond_);
    exporter_->write_new_line();
    exporter_->increase_indentation();
    exporter_->write_do_word();
    exporter_->write_space();
    check_and_accept_pointer_w_error(expr.iftrue_);
    exporter_->write_new_line();
    exporter_->write_else_word();
    exporter_->write_space();
    check_and_accept_pointer_w_error(expr.iffalse_);
    exporter_->decrease_indentation();
}

void ASTTextVisitor::visit(BinOpExpr const& expr)
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
        exporter_->write_left_bracket("(");
    }
    check_and_accept_pointer_w_error(expr.left_);
    exporter_->write_space();
    switch (expr.op_)
    {
        case BinOpType::Assign:
            exporter_->write_assign_op_word();
            break;
        case BinOpType::Add:
            exporter_->write_operator_sign("+");
            break;
        case BinOpType::Subtract:
            exporter_->write_operator_sign("-");
            break;
        case BinOpType::Multiply:
            exporter_->write_operator_sign("*");
            break;
        case BinOpType::Divide:
            exporter_->write_operator_sign("/");
            break;
        case BinOpType::FloorDivide:
            exporter_->write_operator_sign("div");
            break;
        case BinOpType::Modulo:
            exporter_->write_modulo_op_word();
            break;
        case BinOpType::Exponentiation:
            exporter_->write_operator_sign("**");
            break;
        case BinOpType::Equal:
            exporter_->write_operator_sign("==");
            break;
        case BinOpType::NotEqual:
            exporter_->write_operator_sign("!=");
            break;
        case BinOpType::Less:
            exporter_->write_operator_sign("<");
            break;
        case BinOpType::LessEqual:
            exporter_->write_operator_sign("<=");
            break;
        case BinOpType::Greater:
            exporter_->write_operator_sign(">");
            break;
        case BinOpType::GreaterEqual:
            exporter_->write_operator_sign(">=");
            break;
        case BinOpType::LogicalAnd:
            exporter_->write_operator_sign("&&");
            break;
        case BinOpType::LogicalOr:
            exporter_->write_operator_sign("||");
            break;
        case BinOpType::BitShiftRight:
            exporter_->write_operator_sign(">>");
            break;
        case BinOpType::BitShiftLeft:
            exporter_->write_operator_sign("<<");
            break;
        case BinOpType::BitAnd:
            exporter_->write_operator_sign("&");
            break;
        case BinOpType::BitOr:
            exporter_->write_operator_sign("|");
            break;
        case BinOpType::BitXor:
            exporter_->write_operator_sign("^");
            break;
        case BinOpType::Comma:
            exporter_->write_separator_sign(",");
            break;
        case BinOpType::AddAssign:
            exporter_->write_operator_sign("+=");
            break;
        case BinOpType::SubtractAssign:
            exporter_->write_operator_sign("-=");
            break;
        case BinOpType::MultiplyAssign:
            exporter_->write_operator_sign("*=");
            break;
        case BinOpType::DivideAssign:
            exporter_->write_operator_sign("/=");
            break;
        case BinOpType::FloorDivideAssign:
            exporter_->write_operator_sign("div=");
            break;
        case BinOpType::ModuloAssign:
            exporter_->write_operator_sign("%=");
            break;
        case BinOpType::ExponentiationAssign:
            exporter_->write_operator_sign("**=");
            break;
        case BinOpType::BitShiftRightAssign:
            exporter_->write_operator_sign(">>=");
            break;
        case BinOpType::BitShiftLeftAssign:
            exporter_->write_operator_sign("<<=");
            break;
        case BinOpType::BitAndAssign:
            exporter_->write_operator_sign("&=");
            break;
        case BinOpType::BitOrAssign:
            exporter_->write_operator_sign("|=");
            break;
        case BinOpType::BitXorAssign:
            exporter_->write_operator_sign("^=");
            break;
    }
    exporter_->write_space();
    check_and_accept_pointer_w_error(expr.right_);
    if (usebr)
    {
        exporter_->write_right_bracket(")");
    }
}

void ASTTextVisitor::visit(UnaryOpExpr const& expr)
{
    switch (expr.op_)
    {
        case UnaryOpType::LogicalNot:
            exporter_->write_operator_sign("!");
            check_and_accept_pointer_w_error(expr.arg_);
            break;
        case UnaryOpType::Minus:
            exporter_->write_left_bracket("(");
            exporter_->write_operator_sign("-");
            check_and_accept_pointer_w_error(expr.arg_);
            exporter_->write_right_bracket(")");
            break;
        case UnaryOpType::Plus:
            check_and_accept_pointer_w_error(expr.arg_);
            break;
        case UnaryOpType::Dereference:
            exporter_->write_deref_op_word();
            exporter_->write_left_bracket("(");
            check_and_accept_pointer_w_error(expr.arg_);
            exporter_->write_right_bracket(")");
            break;
        case UnaryOpType::AddressOf:
            exporter_->write_address_op_word();
            exporter_->write_left_bracket("(");
            check_and_accept_pointer_w_error(expr.arg_);
            exporter_->write_right_bracket(")");
            break;
        case UnaryOpType::PreIncrement:
            exporter_->write_operator_sign("++");
            check_and_accept_pointer_w_error(expr.arg_);
            break;
        case UnaryOpType::PostIncrement:
            check_and_accept_pointer_w_error(expr.arg_);
            exporter_->write_operator_sign("++");
            break;
        case UnaryOpType::PreDecrement:
            exporter_->write_operator_sign("--");
            check_and_accept_pointer_w_error(expr.arg_);
            break;
        case UnaryOpType::PostDecrement:
            check_and_accept_pointer_w_error(expr.arg_);
            exporter_->write_operator_sign("--");
            break;
        case UnaryOpType::BitFlip:
            exporter_->write_operator_sign("~");
            check_and_accept_pointer_w_error(expr.arg_);
            break;
    }
}

void ASTTextVisitor::visit(ParamVarRefExpr const& expr)
{
    exporter_->write_param_var_name(expr.param_);
}

void ASTTextVisitor::visit(LocalVarRefExpr const& expr)
{
    exporter_->write_local_var_name(expr.var_);
}

void ASTTextVisitor::visit(MemberVarRefExpr const& expr)
{
    if (expr.owner_)
    {
        expr.owner_->accept(*this);
        exporter_->write_separator_sign(".");
    }
    exporter_->write_member_var_name(expr.member_);
}

void ASTTextVisitor::visit(GlobalVarRefExpr const& expr)
{
    exporter_->write_global_var_name(expr.global_);
}

/*void ASTTextVisitor::visit(TypeExpr const& expr)
{
    check_and_accept_pointer_w_error(expr.type_);
}*/

/*void ASTTextVisitor::visit(GenericArg const& expr)
{
    check_and_accept_pointer_w_error(expr.arg_);
}*/

void ASTTextVisitor::visit(ClassRefExpr const& expr)
{
    /*exporter_->write_class_name(expr.def_->name_);
    if (!expr.targs_.empty() && !isConstructorCall_)
    {
        write_params_or_args(expr.targs_, true);
    }*/
    write_identifier_from_string(expr.name_);
}

/*void ASTTextVisitor::visit(InterfaceRefExpr const& expr)
{
    exporter_->write_interface_name(expr.def_->name_);
    if (!expr.targs_.empty() && !isConstructorCall_)
    {
        write_params_or_args(expr.targs_, true);
    }
}*/

void ASTTextVisitor::visit(FunctionCallExpr const& expr)
{
    if (configurator_->sh_other_expr())
    {
        exporter_->write_call_word();
        exporter_->write_space();
    }
    exporter_->write_function_name(expr.name_);
    /*if (!expr.targs_.empty())
    {
        write_params_or_args(expr.targs_, true);
    }*/
    write_params_or_args(expr.args_);
}

void ASTTextVisitor::visit(MethodCallExpr const& expr)
{
    if (configurator_->sh_other_expr() && !isMethodCall_)
    {
        exporter_->write_call_word();
        exporter_->write_space();
        isMethodCall_ = true;
    }
    if (expr.owner_)
    {
        expr.owner_->accept(*this);
        exporter_->write_separator_sign(".");
    }
    isMethodCall_ = false;
    exporter_->write_method_name(expr.name_);
    /*if (!expr.targs_.empty())
    {
        write_params_or_args(expr.targs_, true);
    }*/
    write_params_or_args(expr.args_);
}

void ASTTextVisitor::visit(LambdaCallExpr const& expr)
{
    check_and_accept_pointer(expr.lambda_);
    write_params_or_args(expr.args_);
}

void ASTTextVisitor::visit(LambdaExpr const& expr)
{
    exporter_->write_lambda_word();
    write_params_or_args(expr.params_);
    write_body(expr.body_);
}

void ASTTextVisitor::visit(ThisExpr const& /*expr*/)
{
    exporter_->write_this_word();
}

void ASTTextVisitor::visit(ConstructorCallExpr const& expr)
{
    if (configurator_->sh_other_expr() && !isConstructorCall_)
    {
        exporter_->write_call_word();
        exporter_->write_space();
        exporter_->write_constructor_word();
        exporter_->write_space();
    }
    isConstructorCall_ = true;
    check_and_accept_pointer_w_error(expr.type_);
    isConstructorCall_ = false;
    /*if (!expr.targs_.empty())
    {
        write_params_or_args(expr.targs_, true);
    }*/
    write_params_or_args(expr.args_);
}

/*void ASTTextVisitor::visit(DestructorCallExpr const& expr)
{
    if (configurator_->sh_other_expr())
    {
        exporter_->write_call_word();
        exporter_->write_space();
        exporter_->write_destr_word();
        exporter_->write_space();
    }
    if (expr.owner_)
    {
        expr.owner_->accept(*this);
        exporter_->write_separator_sign(".");
    }
    if (expr.type_)
    {
        exporter_->write_class_name("~");
        isConstructorCall_ = true;
        expr.type_->accept(*this);
        isConstructorCall_ = false;
        exporter_->write_round_bracket("(");
        exporter_->write_round_bracket(")");
    }
        else
        {
        exporter_->write_invalid_word();
    }
}*/

void ASTTextVisitor::visit(NewExpr const& expr)
{
    exporter_->write_new_word();
    exporter_->write_space();
    isConstructorCall_ = true;
    check_and_accept_pointer_w_error(expr.init_);
    isConstructorCall_ = false;
}

void ASTTextVisitor::visit(DeleteExpr const& expr)
{
    exporter_->write_delete_word();
    exporter_->write_space();
    check_and_accept_pointer_w_error(expr.arg_);
}

void ASTTextVisitor::visit(UnknownExpr const& /*expr*/)
{
    exporter_->write_unknown_expr_word();
}

void ASTTextVisitor::visit(TranslationUnit const& stmt)
{
    bool predch = false;
    if (configurator_->sh_global_vars())
    {
        for (GlobalVarDefStmt* a : stmt.globals_)
        {
            if (a)
            {
                a->accept(*this);
                exporter_->write_new_line();
                predch = true;
            }
        }
    }
    /*for (Concept* a : stmt.concepts_)
    {
        if (a)
        {
            a->accept(*this);
            exporter_->write_new_line();
            predch = true;
        }
    }*/
    if (configurator_->sh_func_declar())
    {
        for (FunctionDefStmt* a : stmt.functions_)
        {
            if (a)
            {
                if (predch)
                {
                    exporter_->write_new_line();
                }
                a->accept(*this);
                exporter_->write_new_line();
                predch = true;
            }
        }
    }
    if (configurator_->sh_interf_declar())
    {
        for (InterfaceDefStmt* a : stmt.interfaces_)
        {
            if (a)
            {
                if (predch)
                {
                    exporter_->write_new_line();
                }
                a->accept(*this);
                exporter_->write_new_line();
                predch = true;
            }
        }
    }
    for (ClassDefStmt* a : stmt.classes_)
    {
        if (a)
        {
            if (predch)
            {
                exporter_->write_new_line();
            }
            a->accept(*this);
            exporter_->write_new_line();
            predch = true;
        }
    }
}

void ASTTextVisitor::visit(CompoundStmt const& stmt)
{
    for (size_t i = 0; i < stmt.stmts_.size(); ++i)
    {
        check_and_accept_pointer_w_error(stmt.stmts_.at(i));
        if (i < stmt.stmts_.size() - 1)
        {
            exporter_->write_new_line();
        }
    }
}

void ASTTextVisitor::visit(ReturnStmt const& stmt)
{
    exporter_->write_return_word();
    if (stmt.val_)
    {
        exporter_->write_space();
        stmt.val_->accept(*this);
    }
}

void ASTTextVisitor::visit(ExprStmt const& stmt)
{
    check_and_accept_pointer_w_error(stmt.expr_);
}

void ASTTextVisitor::visit(IfStmt const& stmt)
{
    exporter_->write_if_word();
    write_cond(stmt.cond_);
    exporter_->write_space();
    exporter_->write_do_word();
    write_body(stmt.iftrue_);
    if (stmt.iffalse_)
    {
        exporter_->write_space();
        exporter_->write_else_word();
        write_body(stmt.iffalse_);
    }
}

void ASTTextVisitor::visit(CaseStmt const& stmt)
{
    exporter_->write_case_word();
    exporter_->write_space();
    if (stmt.exprs_.empty())
    {
        exporter_->write_invalid_stmt_word();
    }
    else
    {
        for (size_t i = 0; i < stmt.exprs_.size(); ++i)
        {
            check_and_accept_pointer_w_error(stmt.exprs_.at(i));
            write_comma_space(i < stmt.exprs_.size() - 1);
        }
    }
    exporter_->write_space();
    exporter_->write_do_word();
    write_body(stmt.body_);
}

void ASTTextVisitor::visit(DefaultCaseStmt const& stmt)
{
    exporter_->write_default_word();
    exporter_->write_space();
    exporter_->write_do_word();
    write_body(stmt.body_);
}

void ASTTextVisitor::visit(SwitchStmt const& stmt)
{
    exporter_->write_switch_word();
    write_cond(stmt.expr_);
    write_open_curl_bracket();
    exporter_->increase_indentation();
    for (size_t i = 0; i < stmt.cases_.size(); ++i)
    {
        check_and_accept_pointer_w_error(stmt.cases_.at(i));
        if (i < stmt.cases_.size() - 1)
        {
            exporter_->write_new_line();
        }
    }
    exporter_->decrease_indentation();
    exporter_->write_new_line();
    exporter_->write_right_bracket("}");
}

void ASTTextVisitor::visit(WhileStmt const& stmt)
{
    exporter_->write_while_word();
    write_cond(stmt.cond_);
    exporter_->write_space();
    exporter_->write_repeat_word();
    write_body(stmt.body_);
}

void ASTTextVisitor::visit(DoWhileStmt const& stmt)
{
    exporter_->write_repeat_word();
    write_body(stmt.body_);
    exporter_->write_space();
    exporter_->write_while_word();
    write_cond(stmt.cond_);
}

void ASTTextVisitor::visit(ForStmt const& stmt)
{
    exporter_->write_for_word();
    exporter_->write_space();
    exporter_->write_left_bracket("(");
    check_and_accept_pointer(stmt.init_);
    exporter_->write_separator_sign(";");
    exporter_->write_space();
    check_and_accept_pointer(stmt.cond_);
    exporter_->write_separator_sign(";");
    exporter_->write_space();
    check_and_accept_pointer(stmt.step_);
    exporter_->write_right_bracket(")");
    exporter_->write_space();
    exporter_->write_repeat_word();
    write_body(stmt.body_);
}

/*void ASTTextVisitor::visit(ForEachStmt const& stmt)
{
    exporter_->write_foreach_word();
    exporter_->write_space();
    exporter_->write_round_bracket("(");
    check_and_accept_pointer_w_error(stmt.item_);
    exporter_->write_space();
    exporter_->write_from_word();
    exporter_->write_space();
    check_and_accept_pointer_w_error(stmt.container_);
    exporter_->write_round_bracket(")");
    exporter_->write_space();
    exporter_->write_repeat_word();
    write_body(stmt.body_);
}*/

void ASTTextVisitor::visit(ThrowStmt const& stmt)
{
    exporter_->write_throw_word();
    exporter_->write_space();
    check_and_accept_pointer_w_error(stmt.val_);
}

void ASTTextVisitor::visit(UnknownStmt const& /*stmt*/)
{
    exporter_->write_unknown_stmt_word();
}

void ASTTextVisitor::visit(LocalVarDefStmt const& stmt)
{
    write_var_def(stmt, LOCAL_VAR);
}

void ASTTextVisitor::visit(ParamVarDefStmt const& stmt)
{
    write_var_def(stmt, PARAM_VAR);
}

void ASTTextVisitor::visit(MemberVarDefStmt const& stmt)
{
    write_var_def(stmt, MEMBER_VAR);
}

void ASTTextVisitor::visit(GlobalVarDefStmt const& stmt)
{
    write_var_def(stmt, GLOBAL_VAR);
}

void ASTTextVisitor::visit(FunctionDefStmt const& stmt)
{
    if (!configurator_->sh_func_declar())
    {
        return;
    }
    //write_template_head(stmt.tparams_);
    if (configurator_->sh_other_expr())
    {
        exporter_->write_function_word();
        exporter_->write_space();
    }
    exporter_->write_function_name(stmt.name_);
    write_params_or_args(stmt.params_);
    write_return_type(stmt.retType_);
    if (configurator_->sh_func_defin())
    {
        write_body(stmt.body_);
        return;
    }
    exporter_->write_space();
    exporter_->write_left_bracket("{");
    exporter_->write_right_bracket("}");
}

void ASTTextVisitor::visit(DefStmt const& stmt)
{
    if (stmt.defs_.empty())
    {
        exporter_->write_invalid_stmt_word();
        return;
    }
    if (configurator_->sh_other_expr())
    {
        exporter_->write_define_word();
        exporter_->write_space();
    }
    check_and_accept_pointer_w_error(stmt.defs_.at(0)->type_);
    for (size_t i = 0; i < stmt.defs_.size(); ++i)
    {
        exporter_->write_space();
        exporter_->write_local_var_name(stmt.defs_.at(i)->name_);
        if (stmt.defs_.at(i)->initializer_)
        {
            write_initialization(stmt.defs_.at(i));
        }
        write_comma_space(i < stmt.defs_.size() - 1);
    }
}

void ASTTextVisitor::visit(MethodDefStmt const& stmt)
{
    if (!configurator_->sh_con_des_meth_defin() ||
        (stmt.access_ != AccessModifier::Public && !configurator_->use_inner_view()) ||
        !stmt.func_
    )
    {
        return;
    }
    ClassDefStmt* ownerClass = static_cast<ClassDefStmt*>(stmt.owner_);//
    if (configurator_->sh_con_des_meth_owner() && configurator_->sh_con_des_meth_template() && stmt.owner_)
    {
        //write_template_head(stmt.owner_->tparams_);
        write_generic_params_decl(ownerClass->tparams_);//
    }
    //write_template_head(stmt.func_->tparams_);
    if (configurator_->sh_other_expr())
    {
        exporter_->write_method_word();
        exporter_->write_space();
    }
    if (configurator_->sh_con_des_meth_owner() && stmt.owner_)
    {
        exporter_->write_class_name(stmt.owner_->name_);
        /*if (!stmt.owner_->tparams_.empty() && configurator_->sh_meth_templ())
        {
            write_params_or_args(stmt.owner_->tparams_, true);
        }*/
        if (!ownerClass->tparams_.empty() && configurator_->sh_con_des_meth_template())
        {
            write_generic_params(ownerClass->tparams_);
        }
        exporter_->write_separator_sign("::");
    }
    exporter_->write_method_name(stmt.func_->name_);
    write_params_or_args(stmt.func_->params_);
    write_return_type(stmt.func_->retType_);
    write_body(stmt.func_->body_);
}

/*void ASTTextVisitor::visit(BaseInitializerStmt const& stmt)
{
    check_and_accept_pointer_w_error(stmt.base_);
}*/

void ASTTextVisitor::visit(BaseInitializerStmt const& stmt)
{
    if (configurator_->sh_other_expr())
    {
        exporter_->write_call_word();
        exporter_->write_space();
        exporter_->write_constructor_word();
        exporter_->write_space();
    }
    write_identifier_from_string(stmt.base_);
    write_params_or_args(stmt.args_);
}

void ASTTextVisitor::visit(ConstructorDefStmt const& stmt)
{
    if (!configurator_->sh_con_des_meth_defin() || (stmt.access_ != AccessModifier::Public && !configurator_->use_inner_view()))
    {
        return;
    }
    if (configurator_->sh_con_des_meth_owner() && configurator_->sh_con_des_meth_template() && stmt.owner_)
    {
        write_template_head(stmt.owner_->tparams_);
    }
    //write_template_head(stmt.tparams_);
    if (configurator_->sh_other_expr())
    {
        exporter_->write_constructor_word();
        exporter_->write_space();
    }
    if (configurator_->sh_con_des_meth_owner() && stmt.owner_)
    {
        exporter_->write_class_name(stmt.owner_->name_);
        if (!stmt.owner_->tparams_.empty() && configurator_->sh_con_des_meth_template())
        {
            write_params_or_args(stmt.owner_->tparams_, true);
        }
        exporter_->write_separator_sign("::");
    }
    if (stmt.owner_)
    {
        exporter_->write_class_name(stmt.owner_->name_);
    }
    else
    {
        exporter_->write_invalid_expr_word();
    }
    write_params_or_args(stmt.params_);
    if (!stmt.baseInit_.empty())
    {
        for (size_t i = 0; i < stmt.baseInit_.size(); ++i)
        {
            exporter_->write_new_line();
            write_arrow();
            check_and_accept_pointer_w_error(stmt.baseInit_.at(i));
        }
    }
    write_body(stmt.body_);
}

void ASTTextVisitor::visit(DestructorDefStmt const& stmt)
{
    if (!configurator_->sh_con_des_meth_defin() && !stmt.body_)
    {
        return;
    }
    if (configurator_->sh_con_des_meth_owner() && configurator_->sh_con_des_meth_template() && stmt.owner_)
    {
        write_template_head(stmt.owner_->tparams_);
    }
    if (configurator_->sh_other_expr())
    {
        exporter_->write_destructor_word();
        exporter_->write_space();
    }
    if (configurator_->sh_con_des_meth_owner() && stmt.owner_)
    {
        exporter_->write_class_name(stmt.owner_->name_);
        if (!stmt.owner_->tparams_.empty() && configurator_->sh_con_des_meth_template())
        {
            write_params_or_args(stmt.owner_->tparams_, true);
        }
        exporter_->write_separator_sign("::");
    }
    if (stmt.owner_)
    {
        exporter_->write_class_name("~" + stmt.owner_->name_);
    }
    else
    {
        exporter_->write_invalid_expr_word();
    }
    exporter_->write_left_bracket("(");
    exporter_->write_right_bracket(")");
    write_body(stmt.body_);
}

/*
void ASTTextVisitor::visit(Concept const& stmt)
{
}*/

/*void ASTTextVisitor::visit(GenericParam const& stmt)
{
    if (stmt.type_)
    {
        exporter_->write_gen_param_name(stmt.type_->name_);
    }
        else
        {
        exporter_->write_invalid_word();
    }
}*/

void ASTTextVisitor::visit(GenericParam const& stmt)
{
    exporter_->write_gen_param_name(stmt.name_);
    currGenParams_->push_back(std::stringstream(stmt.name_));
}

void ASTTextVisitor::visit(InterfaceDefStmt const& stmt)
{
    if (!configurator_->sh_interf_declar())
    {
        return;
    }
    write_template_head(stmt.tparams_);
    exporter_->write_interface_word();
    exporter_->write_space();
    exporter_->write_interface_name(stmt.name_);
    if (!stmt.bases_.empty())
    {
        write_relations(stmt.bases_, true);
    }
    write_open_curl_bracket();
    if (configurator_->sh_interf_defin() && configurator_->sh_con_des_meth_declar() && !stmt.methods_.empty())
    {
        write_method_decl(stmt.methods_);
    }
    exporter_->write_right_bracket("}");
    exporter_->write_new_line();
}

void ASTTextVisitor::visit(ClassDefStmt const& stmt)
{
    if (configurator_->sh_class_declar())
    {
        write_template_head(stmt.tparams_);
        exporter_->write_class_word();
        exporter_->write_space();
        exporter_->write_class_name(stmt.name_);
        if (!stmt.interfaces_.empty())
        {
            write_relations(stmt.interfaces_, true);
        }
        if (!stmt.bases_.empty())
        {
            write_relations(stmt.bases_, false);
        }
        write_open_curl_bracket();
        if (configurator_->sh_class_defin())
        {
            if (configurator_->sh_member_vars() && !stmt.vars_.empty())
            {
                write_member_var_decl(stmt.vars_);
            }
            if (configurator_->sh_con_des_meth_declar())
            {
                if (!stmt.constructors_.empty())
                {
                    write_constructor_decl(stmt.constructors_);
                }
                if (!stmt.destructors_.empty())
                {
                    write_destructor_decl(stmt.destructors_);
                }
                if (!stmt.methods_.empty())
                {
                    write_method_decl(stmt.methods_);
                }
            }
        }
        exporter_->write_right_bracket("}");
        exporter_->write_new_line();
    }
    if (configurator_->sh_con_des_meth_defin())
    {
        for (size_t i = 0; i < stmt.constructors_.size(); ++i)
        {
            if (stmt.constructors_.at(i))
            {
                exporter_->write_new_line();
                stmt.constructors_.at(i)->accept(*this);
                exporter_->write_new_line();
            }
        }
        for (size_t i = 0; i < stmt.destructors_.size(); ++i)
        {
            if (stmt.destructors_.at(i) && stmt.destructors_.at(i)->body_)
            {
                exporter_->write_new_line();
                stmt.destructors_.at(i)->accept(*this);
                exporter_->write_new_line();
            }
        }
        for (size_t i = 0; i < stmt.methods_.size(); ++i)
        {
            if (stmt.methods_.at(i) && stmt.methods_.at(i)->func_ && stmt.methods_.at(i)->func_->body_)
            {
                exporter_->write_new_line();
                stmt.methods_.at(i)->accept(*this);
                exporter_->write_new_line();
            }
        }
    }
}

void ASTTextVisitor::visit(ContinueStmt const& /*stmt*/)
{
    exporter_->write_continue_word();
}

void ASTTextVisitor::visit(BreakStmt const& /*stmt*/)
{
    exporter_->write_break_word();
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------

void ASTTextVisitor::write_open_curl_bracket()
{
    if (configurator_->new_line_curl_bracket())
    {
        exporter_->write_new_line();
    }
    else
    {
        exporter_->write_space();
    }
    exporter_->write_left_bracket("{");
    exporter_->write_new_line();
}

void ASTTextVisitor::write_arrow()
{
    exporter_->write_space();
    exporter_->write_separator_sign("->");
    exporter_->write_space();
}

void ASTTextVisitor::write_body(Stmt* body)
{
    write_open_curl_bracket();
    exporter_->increase_indentation();
    if (body)
    {
        body->accept(*this);
        exporter_->write_new_line();
    }
    exporter_->decrease_indentation();
    exporter_->write_right_bracket("}");
}

void ASTTextVisitor::write_cond(Expr* cond)
{
    exporter_->write_space();
    exporter_->write_left_bracket("(");
    check_and_accept_pointer_w_error(cond);
    exporter_->write_right_bracket(")");
}

void ASTTextVisitor::write_return_type(Type* type)
{
    write_arrow();
    if (configurator_->sh_other_expr())
    {
        exporter_->write_returns_word();
        exporter_->write_space();
    }
    check_and_accept_pointer_w_error(type);
}

void ASTTextVisitor::write_identifier_from_string(std::string classname)
{
    size_t st = std::move(classname.find('<'));
    size_t en = std::move(classname.find('>'));
    if (st != std::move(std::string::npos) && en != std::move(std::string::npos) && st < en)
    {
        exporter_->write_class_name(classname.substr(0, st));
        if (configurator_->sh_generic_param())
        {
            exporter_->write_separator_sign("<");
            exporter_->write_gen_param_name(classname.substr(st + 1, en - st - 1));
            exporter_->write_separator_sign(">");
        }
    }
    else
    {
        if (!currGenParams_->empty())
        {
            for (auto& a : *currGenParams_)
            {
                if (std::move(a.str()) == classname)
                {
                    exporter_->write_gen_param_name(std::move(classname));
                    return;
                }
            }
        }
        if (classname == std::move(currClassName_->str()))
        {
            exporter_->write_class_name(std::move(classname));
        }
        else if (classname == std::move(currInterfName_->str()))
        {
            exporter_->write_interface_name(std::move(classname));
        }
        else if (classname == "this")
        {
            exporter_->write_this_word();
        }
        else if (classname[0] >= 'A' && classname[0] <= 'Z')
        {
            exporter_->write_class_name(std::move(classname));
        }
        else
        {
            exporter_->write_user_type(std::move(classname));
        }
    }
}

void ASTTextVisitor::write_initialization(VarDefStmt const* var)
{
    exporter_->write_space();
    exporter_->write_assign_op_word();
    exporter_->write_space();
    var->initializer_->accept(*this);
}

void ASTTextVisitor::write_generic_params_decl(std::vector<GenericParam*> const& vgeneric)
{
    if (configurator_->sh_generic_param() && !vgeneric.empty())
    {
        exporter_->write_template_word();
        write_generic_params(vgeneric);
        exporter_->write_new_line();
    }
}

void ASTTextVisitor::write_generic_params(std::vector<GenericParam*> const& vgeneric)
{
    if (!configurator_->sh_generic_param())
    {
        return;
    }
    write_params_or_args(vgeneric, true);
    /*exporter_->write_separator_sign("<");
    for (size_t i = 0; i < vgeneric.size(); ++i)
    {
        vgeneric.at(i) ? vgeneric.at(i)->accept(*this) : exporter_->write_invalid_word();
        (i < vgeneric.size() - 1) ? (exporter_->write_separator_sign(","), exporter_->write_space()) : void();
    }
    exporter_->write_separator_sign(">");*/
}

void ASTTextVisitor::write_var_def(VarDefStmt const& var, int vartype)
{
    if (configurator_->sh_other_expr() && vartype != PARAM_VAR)
    {
        exporter_->write_define_word();
        exporter_->write_space();
    }
    check_and_accept_pointer_w_error(var.type_);
    exporter_->write_space();
    if (vartype == LOCAL_VAR)
    {
        exporter_->write_local_var_name(var.name_);
    }
    else if (vartype == PARAM_VAR)
    {
        exporter_->write_param_var_name(var.name_);
    }
    else if (vartype == MEMBER_VAR)
    {
        exporter_->write_member_var_name(var.name_);
    }
    else if (vartype == GLOBAL_VAR)
    {
        exporter_->write_global_var_name(var.name_);
    }
    if (var.initializer_)
    {
        write_initialization(&var);
    }
}

void ASTTextVisitor::write_template_head(std::vector<GenericParam*> const& vgen)
{
    if (configurator_->sh_generic_param() && !vgen.empty())
    {
        exporter_->write_template_word();
        write_params_or_args(vgen, true);
        exporter_->write_new_line();
    }
}

void ASTTextVisitor::write_member_var_decl(std::vector<MemberVarDefStmt*> const& vmembervars)
{
    std::vector<MemberVarDefStmt*> vfound {};
    if (has_acc_mod(vmembervars, vfound, AccessModifier::Public))
    {
        exporter_->write_public_word();
        write_member_var(vfound);
    }
    if (configurator_->use_inner_view())
    {
        if (has_acc_mod(vmembervars, vfound, AccessModifier::Private))
        {
            exporter_->write_private_word();
            write_member_var(vfound);
        }
        if (has_acc_mod(vmembervars, vfound, AccessModifier::Protected))
        {
            exporter_->write_protected_word();
            write_member_var(vfound);
        }
        if (has_acc_mod(vmembervars, vfound, AccessModifier::Internal))
        {
            exporter_->write_internal_word();
            write_member_var(vfound);
        }
    }
}

void ASTTextVisitor::write_member_var(std::vector<MemberVarDefStmt*>& vmembervars)
{
    exporter_->write_space();
    exporter_->write_attributes_word();
    exporter_->write_new_line();
    exporter_->increase_indentation();
    for (size_t i = 0; i < vmembervars.size(); ++i)
    {
        if (vmembervars.at(i))
        {
            vmembervars.at(i)->accept(*this);
            exporter_->write_new_line();
        }
    }
    exporter_->decrease_indentation();
    vmembervars.clear();
}

void ASTTextVisitor::write_constructor_decl(std::vector<ConstructorDefStmt*> const& vconstructors)
{
    std::vector<ConstructorDefStmt*> vfound {};
    if (has_acc_mod(vconstructors, vfound, std::move(AccessModifier::Public)))
    {
        exporter_->write_public_word();
        write_constructor(vfound);
    }
    if (configurator_->use_inner_view())
    {
        if (has_acc_mod(vconstructors, vfound, std::move(AccessModifier::Private)))
        {
            exporter_->write_private_word();
            write_constructor(vfound);
        }
        if (has_acc_mod(vconstructors, vfound, std::move(AccessModifier::Protected)))
        {
            exporter_->write_protected_word();
            write_constructor(vfound);
        }
        if (has_acc_mod(vconstructors, vfound, std::move(AccessModifier::Internal)))
        {
            exporter_->write_internal_word();
            write_constructor(vfound);
        }
    }
}

void ASTTextVisitor::write_constructor(std::vector<ConstructorDefStmt*>& constr)
{
    exporter_->write_space();
    exporter_->write_constructors_word();
    exporter_->write_new_line();
    exporter_->increase_indentation();
    for (size_t i = 0; i < constr.size(); ++i)
    {
        if (constr.at(i) && constr.at(i)->owner_)
        {
            exporter_->write_class_name(constr.at(i)->owner_->name_);
            /*if (!constr.at(i)->tparams_.empty())
            {
                write_params_or_args(constr.at(i)->tparams_, true);
            }*/
            write_params_or_args(constr.at(i)->params_);
        }
        else
        {
            exporter_->write_invalid_stmt_word();
        }
        exporter_->write_new_line();
    }
    exporter_->decrease_indentation();
    constr.clear();
}

void ASTTextVisitor::write_destructor_decl(std::vector<DestructorDefStmt*> const& vdestructors)
{
    exporter_->write_destructors_word();
    exporter_->write_new_line();
    exporter_->increase_indentation();
    for (size_t i = 0; i < vdestructors.size(); ++i)
    {
        if (vdestructors.at(i) && vdestructors.at(i)->owner_)
        {
            exporter_->write_class_name("~" + vdestructors.at(i)->owner_->name_);
            exporter_->write_left_bracket("(");
            exporter_->write_right_bracket(")");
        }
        else
        {
            exporter_->write_invalid_stmt_word();
        }
        exporter_->write_new_line();
    }
    exporter_->decrease_indentation();
}

void ASTTextVisitor::write_method_decl(std::vector<MethodDefStmt*> const& vmethods)
{
    std::vector<MethodDefStmt*> vfound {};
    if (has_acc_mod(vmethods, vfound, AccessModifier::Public))
    {
        exporter_->write_public_word();
        write_method(vfound);
    }
    if (configurator_->use_inner_view())
    {
        if (has_acc_mod(vmethods, vfound, AccessModifier::Private))
        {
            exporter_->write_private_word();
            write_method(vfound);
        }
        if (has_acc_mod(vmethods, vfound, AccessModifier::Protected))
        {
            exporter_->write_protected_word();
            write_method(vfound);
        }
        if (has_acc_mod(vmethods, vfound, AccessModifier::Internal))
        {
            exporter_->write_internal_word();
            write_method(vfound);
        }
    }
}

void ASTTextVisitor::write_method(std::vector<MethodDefStmt*>& meth)
{
    exporter_->write_space();
    exporter_->write_methods_word();
    exporter_->write_new_line();
    exporter_->increase_indentation();
    for (size_t i = 0; i < meth.size(); ++i)
    {
        if (meth.at(i) && meth.at(i)->func_)
        {
            if (meth.at(i)->func_->body_)
            {
                if (meth.at(i)->virtuality_ == Virtuality::Virtual)
                {
                    exporter_->write_virtual_word();
                    write_arrow();
                }
            }
            else
            {
                exporter_->write_abstract_word();
                write_arrow();
            }
            exporter_->write_method_name(meth.at(i)->func_->name_);
            /*if (!meth.at(i)->func_->tparams_.empty())
            {
                write_params_or_args(meth.at(i)->func_->tparams_, true);
            }*/
            write_params_or_args(meth.at(i)->func_->params_);
            write_return_type(meth.at(i)->func_->retType_);
        }
        else
        {
            exporter_->write_invalid_stmt_word();
        }
        exporter_->write_new_line();
    }
    exporter_->decrease_indentation();
    meth.clear();
}