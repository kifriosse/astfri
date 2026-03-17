#include <libastfri-text/inc/pseudocode/PseudocodeVisitor.hpp>

#include <libastfri-text/inc/pseudocode/PlainTextBuilder.hpp>
#include <libastfri-text/inc/pseudocode/HtmlTextBuilder.hpp>

using namespace astfri::text;

constexpr int LOCAL_VAR = 0;
constexpr int PARAM_VAR = 1;
constexpr int MEMBER_VAR = 2;
constexpr int GLOBAL_VAR = 3;

PseudocodeVisitor& PseudocodeVisitor::get_instance()
{
    AbstractTextBuilder* builder;
    TextConfigurator& tc = TextConfigurator::get_instance();
    if (tc.output_file_format()->view() == "html")
    {
        builder = &HtmlTextBuilder::get_instance();
    }
    else
    {
        builder = &PlainTextBuilder::get_instance();
    }
    builder->reset_builder();
    static PseudocodeVisitor visitor(*builder);
    return visitor;
}

PseudocodeVisitor::PseudocodeVisitor(AbstractTextBuilder& builder) :
    AbstractVisitor(builder),
    builder_(&builder),
    configurator_(&TextConfigurator::get_instance()),
    isMethodCall_(false),
    isConstructorCall_(false)
{
}

//
// -----
//

void PseudocodeVisitor::export_pseudocode()
{
    static_cast<AbstractTextBuilder*>(builder_)->export_text();
}

void PseudocodeVisitor::clear_builder()
{
    builder_->reset_builder();
}

void PseudocodeVisitor::append_text(std::string const& text)
{
    builder_->append_text(text);
}

void PseudocodeVisitor::append_new_line()
{
    builder_->append_new_line();
}

void PseudocodeVisitor::append_space()
{
    builder_->append_space();
}

void PseudocodeVisitor::update_configuration()
{
    configurator_->update_configuration();
    if (configurator_->output_file_format()->view() == "html")
    {
        builder_ = &HtmlTextBuilder::get_instance();
    }
    else
    {
        builder_ = &PlainTextBuilder::get_instance();
    }
    builder_->reset_builder();
}

void PseudocodeVisitor::reload_configuration()
{
    configurator_->reload_configuration();
    if (configurator_->output_file_format()->view() == "html")
    {
        builder_ = &HtmlTextBuilder::get_instance();
    }
    else
    {
        builder_ = &PlainTextBuilder::get_instance();
    }
    builder_->reset_builder();
}

// -----

void PseudocodeVisitor::visit(DynamicType const& /*type*/)
{
    builder_->write_dynamic_type_word();
}

void PseudocodeVisitor::visit(IntType const& /*type*/)
{
    builder_->write_int_type_word();
}

void PseudocodeVisitor::visit(FloatType const& /*type*/)
{
    builder_->write_float_type_word();
}

void PseudocodeVisitor::visit(CharType const& /*type*/)
{
    builder_->write_char_type_word();
}

void PseudocodeVisitor::visit(BoolType const& /*type*/)
{
    builder_->write_bool_type_word();
}

void PseudocodeVisitor::visit(VoidType const& /*type*/)
{
    builder_->write_void_type_word();
}

void PseudocodeVisitor::visit(IndirectionType const& type)
{
    if (!try_accept_node(type.indirect))
    {
        builder_->write_invalid_type();
    }
    builder_->write_pointer_word();
}

void PseudocodeVisitor::visit(ClassType const& type)
{
    builder_->write_class_type(type.name);
}

void PseudocodeVisitor::visit(InterfaceType const& type)
{
    builder_->write_interface_type(type.name);
}

void PseudocodeVisitor::visit(LambdaType const& type)
{
    builder_->write_function_name(type.name);
}

void PseudocodeVisitor::visit(IncompleteType const& type)
{
    builder_->write_class_type(type.name);
}

void PseudocodeVisitor::visit(UnknownType const& /*type*/)
{
    builder_->write_unknown_type();
}

// -----

void PseudocodeVisitor::visit(IntLiteralExpr const& expr)
{
    builder_->write_int_val(expr.val);
}

void PseudocodeVisitor::visit(FloatLiteralExpr const& expr)
{
    builder_->write_float_val(expr.val);
}

void PseudocodeVisitor::visit(CharLiteralExpr const& expr)
{
    builder_->write_char_val(expr.val);
}

void PseudocodeVisitor::visit(StringLiteralExpr const& expr)
{
    builder_->write_string_val(expr.val);
}

void PseudocodeVisitor::visit(BoolLiteralExpr const& expr)
{
    builder_->write_bool_val(expr.val);
}

void PseudocodeVisitor::visit(NullLiteralExpr const& /*expr*/)
{
    builder_->write_null_val();
}

void PseudocodeVisitor::visit(IfExpr const& expr)
{
    builder_->write_if_word();
    process_condition(expr.cond);
    builder_->append_new_line();
    builder_->increase_indentation();
    builder_->write_do_word();
    builder_->append_space();
    if (!try_accept_node(expr.iftrue))
    {
        builder_->write_invalid_expr();
    }
    builder_->append_new_line();
    builder_->write_else_word();
    builder_->append_space();
    if (!try_accept_node(expr.iffalse))
    {
        builder_->write_invalid_expr();
    }
    builder_->decrease_indentation();
}

void PseudocodeVisitor::visit(BinOpExpr const& expr)
{
    if (!try_accept_node(expr.left))
    {
        builder_->write_invalid_expr();
    }
    builder_->append_space();
    switch (expr.op)
    {
        case BinOpType::Assign:
            builder_->write_assign_operator();
            break;
        case BinOpType::Add:
            builder_->write_operator("+");
            break;
        case BinOpType::Subtract:
            builder_->write_operator("-");
            break;
        case BinOpType::Multiply:
            builder_->write_operator("*");
            break;
        case BinOpType::Divide:
            builder_->write_operator("/");
            break;
        case BinOpType::FloorDivide:
            builder_->write_operator("div");
            break;
        case BinOpType::Modulo:
            builder_->write_modulo_operator();
            break;
        case BinOpType::Exponentiation:
            builder_->write_operator("**");
            break;
        case BinOpType::Equal:
            builder_->write_operator("==");
            break;
        case BinOpType::NotEqual:
            builder_->write_operator("!=");
            break;
        case BinOpType::Less:
            builder_->write_operator("<");
            break;
        case BinOpType::LessEqual:
            builder_->write_operator("<=");
            break;
        case BinOpType::Greater:
            builder_->write_operator(">");
            break;
        case BinOpType::GreaterEqual:
            builder_->write_operator(">=");
            break;
        case BinOpType::LogicalAnd:
            builder_->write_operator("&&");
            break;
        case BinOpType::LogicalOr:
            builder_->write_operator("||");
            break;
        case BinOpType::BitShiftRight:
            builder_->write_operator(">>");
            break;
        case BinOpType::BitShiftLeft:
            builder_->write_operator("<<");
            break;
        case BinOpType::BitAnd:
            builder_->write_operator("&");
            break;
        case BinOpType::BitOr:
            builder_->write_operator("|");
            break;
        case BinOpType::BitXor:
            builder_->write_operator("^");
            break;
        case BinOpType::Comma:
            builder_->write_separator(",");
            break;
        case BinOpType::AddAssign:
            builder_->write_operator("+=");
            break;
        case BinOpType::SubtractAssign:
            builder_->write_operator("-=");
            break;
        case BinOpType::MultiplyAssign:
            builder_->write_operator("*=");
            break;
        case BinOpType::DivideAssign:
            builder_->write_operator("/=");
            break;
        case BinOpType::FloorDivideAssign:
            builder_->write_operator("div=");
            break;
        case BinOpType::ModuloAssign:
            builder_->write_operator("%=");
            break;
        case BinOpType::ExponentiationAssign:
            builder_->write_operator("**=");
            break;
        case BinOpType::BitShiftRightAssign:
            builder_->write_operator(">>=");
            break;
        case BinOpType::BitShiftLeftAssign:
            builder_->write_operator("<<=");
            break;
        case BinOpType::BitAndAssign:
            builder_->write_operator("&=");
            break;
        case BinOpType::BitOrAssign:
            builder_->write_operator("|=");
            break;
        case BinOpType::BitXorAssign:
            builder_->write_operator("^=");
            break;
    }
    builder_->append_space();
    if (!try_accept_node(expr.right))
    {
        builder_->write_invalid_expr();
    }
}

void PseudocodeVisitor::visit(UnaryOpExpr const& expr)
{
    switch (expr.op)
    {
        case UnaryOpType::LogicalNot:
            builder_->write_operator("!");
            if (!try_accept_node(expr.arg))
            {
                builder_->write_invalid_expr();
            }
            break;
        case UnaryOpType::Minus:
            builder_->write_operator("-");
            if (!try_accept_node(expr.arg))
            {
                builder_->write_invalid_expr();
            }
            break;
        case UnaryOpType::Plus:
            if (!try_accept_node(expr.arg))
            {
                builder_->write_invalid_expr();
            }
            break;
        case UnaryOpType::Dereference:
            builder_->write_deref_operator();
            if (!try_accept_node(expr.arg))
            {
                builder_->write_invalid_expr();
            }
            break;
        case UnaryOpType::AddressOf:
            builder_->write_address_operator();
            if (!try_accept_node(expr.arg))
            {
                builder_->write_invalid_expr();
            }
            break;
        case UnaryOpType::PreIncrement:
            builder_->write_operator("++");
            if (!try_accept_node(expr.arg))
            {
                builder_->write_invalid_expr();
            }
            break;
        case UnaryOpType::PostIncrement:
            if (!try_accept_node(expr.arg))
            {
                builder_->write_invalid_expr();
            }
            builder_->write_operator("++");
            break;
        case UnaryOpType::PreDecrement:
            builder_->write_operator("--");
            if (!try_accept_node(expr.arg))
            {
                builder_->write_invalid_expr();
            }
            break;
        case UnaryOpType::PostDecrement:
            if (!try_accept_node(expr.arg))
            {
                builder_->write_invalid_expr();
            }
            builder_->write_operator("--");
            break;
        case UnaryOpType::BitFlip:
            builder_->write_operator("~");
            if (!try_accept_node(expr.arg))
            {
                builder_->write_invalid_expr();
            }
            break;
    }
}

void PseudocodeVisitor::visit(ParamVarRefExpr const& expr)
{
    builder_->write_param_var_name(expr.param);
}

void PseudocodeVisitor::visit(LocalVarRefExpr const& expr)
{
    builder_->write_local_var_name(expr.var);
}

void PseudocodeVisitor::visit(MemberVarRefExpr const& expr)
{
    if (try_accept_node(expr.owner))
    {
        builder_->write_separator(".");
    }
    builder_->write_member_var_name(expr.member);
}

void PseudocodeVisitor::visit(GlobalVarRefExpr const& expr)
{
    builder_->write_global_var_name(expr.global);
}

void PseudocodeVisitor::visit(ClassRefExpr const& expr)
{
    builder_->write_class_name(expr.name);
}

void PseudocodeVisitor::visit(FunctionCallExpr const& expr)
{
    if (configurator_->sh_other_expr())
    {
        builder_->write_call_word();
        builder_->append_space();
    }
    builder_->write_function_name(expr.name);
    process_pargs(expr.args, false);
}

void PseudocodeVisitor::visit(MethodCallExpr const& expr)
{
    if (configurator_->sh_other_expr() && !isMethodCall_)
    {
        builder_->write_call_word();
        builder_->append_space();
        isMethodCall_ = true;
    }
    if (!try_accept_node(expr.owner))
    {
        builder_->write_this_word();
    }
    builder_->write_separator(".");
    isMethodCall_ = false;
    builder_->write_method_name(expr.name);
    process_pargs(expr.args, false);
}

void PseudocodeVisitor::visit(LambdaCallExpr const& expr)
{
    try_accept_node(expr.lambda);
    process_pargs(expr.args, false);
}

void PseudocodeVisitor::visit(LambdaExpr const& expr)
{
    builder_->write_lambda_word();
    process_pargs(expr.params, false);
    process_body(expr.body);
}

void PseudocodeVisitor::visit(ThisExpr const& /*expr*/)
{
    builder_->write_this_word();
}

void PseudocodeVisitor::visit(ConstructorCallExpr const& expr)
{
    if (configurator_->sh_other_expr() && !isConstructorCall_)
    {
        builder_->write_call_word();
        builder_->append_space();
        builder_->write_constructor_word();
        builder_->append_space();
    }
    isConstructorCall_ = true;
    if (!try_accept_node(expr.type))
    {
        builder_->write_invalid_type();
    }
    isConstructorCall_ = false;
    process_pargs(expr.args, false);
}

void PseudocodeVisitor::visit(NewExpr const& expr)
{
    builder_->write_new_word();
    builder_->append_space();
    isConstructorCall_ = true;
    if (!try_accept_node(expr.init))
    {
        builder_->write_invalid_expr();
    }
    isConstructorCall_ = false;
}

void PseudocodeVisitor::visit(DeleteExpr const& expr)
{
    builder_->write_delete_word();
    builder_->append_space();
    if (!try_accept_node(expr.arg))
    {
        builder_->write_invalid_expr();
    }
}

void PseudocodeVisitor::visit(BracketExpr const& expr)
{
    builder_->write_left_bracket("(");
    if (!try_accept_node(expr.expr))
    {
        builder_->write_invalid_expr();
    }
    builder_->write_right_bracket(")");
}

void PseudocodeVisitor::visit(UnknownExpr const& /*expr*/)
{
    builder_->write_unknown_expr();
}

// -----

void PseudocodeVisitor::visit(TranslationUnit const& stmt)
{
    bool previous = false;
    if (configurator_->sh_global_vars())
    {
        for (GlobalVarDefStmt* a : stmt.globals)
        {
            if (a)
            {
                a->accept(*this);
                builder_->append_new_line();
                previous = true;
            }
        }
    }
    if (configurator_->sh_func_declar())
    {
        for (FunctionDefStmt* a : stmt.functions)
        {
            if (a)
            {
                if (previous)
                {
                    builder_->append_new_line();
                }
                a->accept(*this);
                builder_->append_new_line();
                previous = true;
            }
        }
    }
    if (configurator_->sh_interf_declar())
    {
        for (InterfaceDefStmt* a : stmt.interfaces)
        {
            if (a)
            {
                if (previous)
                {
                    builder_->append_new_line();
                }
                a->accept(*this);
                builder_->append_new_line();
                previous = true;
            }
        }
    }
    for (ClassDefStmt* a : stmt.classes)
    {
        if (a)
        {
            if (previous)
            {
                builder_->append_new_line();
            }
            a->accept(*this);
            builder_->append_new_line();
            previous = true;
        }
    }
}

void PseudocodeVisitor::visit(CompoundStmt const& stmt)
{
    for (size_t i = 0; i < stmt.stmts.size(); ++i)
    {
        if (!try_accept_node(stmt.stmts.at(i)))
        {
            builder_->write_invalid_stmt();
        }
        if (i < stmt.stmts.size() - 1)
        {
            builder_->append_new_line();
        }
    }
}

void PseudocodeVisitor::visit(ReturnStmt const& stmt)
{
    builder_->write_return_word();
    if (stmt.val)
    {
        builder_->append_space();
        try_accept_node(stmt.val);
    }
}

void PseudocodeVisitor::visit(ExprStmt const& stmt)
{
    if (!try_accept_node(stmt.expr))
    {
        builder_->write_invalid_stmt();
    }
}

void PseudocodeVisitor::visit(IfStmt const& stmt)
{
    builder_->write_if_word();
    process_condition(stmt.cond);
    builder_->append_space();
    builder_->write_do_word();
    process_body(stmt.iftrue);
    if (stmt.iffalse)
    {
        builder_->write_opening_else_word();
        process_body(stmt.iffalse);
    }
}

void PseudocodeVisitor::visit(CaseStmt const& stmt)
{
    builder_->write_case_word();
    builder_->append_space();
    if (stmt.exprs.empty())
    {
        builder_->write_invalid_stmt();
    }
    else
    {
        for (size_t i = 0; i < stmt.exprs.size(); ++i)
        {
            if (!try_accept_node(stmt.exprs.at(i)))
            {
                builder_->write_invalid_stmt();
            }
            if (i < stmt.exprs.size() - 1)
            {
                builder_->write_separator(",");
                builder_->append_space();
            }
        }
    }
    builder_->append_space();
    builder_->write_do_word();
    process_body(stmt.body);
}

void PseudocodeVisitor::visit(DefaultCaseStmt const& stmt)
{
    builder_->write_default_word();
    builder_->append_space();
    builder_->write_do_word();
    process_body(stmt.body);
}

void PseudocodeVisitor::visit(SwitchStmt const& stmt)
{
    builder_->write_switch_word();
    process_condition(stmt.expr_);
    builder_->write_opening_curl_bracket();
    builder_->increase_indentation();
    for (size_t i = 0; i < stmt.cases.size(); ++i)
    {
        if (!try_accept_node(stmt.cases.at(i)))
        {
            builder_->write_invalid_stmt();
        }
        if (i < stmt.cases.size() - 1)
        {
            builder_->append_new_line();
        }
    }
    builder_->decrease_indentation();
    builder_->append_new_line();
    builder_->write_right_bracket("}");
}

void PseudocodeVisitor::visit(WhileStmt const& stmt)
{
    builder_->write_while_word();
    process_condition(stmt.cond);
    builder_->append_space();
    builder_->write_repeat_word();
    process_body(stmt.body);
}

void PseudocodeVisitor::visit(DoWhileStmt const& stmt)
{
    builder_->write_repeat_word();
    process_body(stmt.body);
    builder_->append_space();
    builder_->write_while_word();
    process_condition(stmt.cond);
}

void PseudocodeVisitor::visit(ForStmt const& stmt)
{
    builder_->write_for_word();
    builder_->append_space();
    builder_->write_left_bracket("(");
    if (!try_accept_node(stmt.init))
    {
        builder_->write_invalid_stmt();
    }
    builder_->write_separator(";");
    builder_->append_space();
    if (!try_accept_node(stmt.cond))
    {
        builder_->write_invalid_expr();
    }
    builder_->write_separator(";");
    builder_->append_space();
    if (!try_accept_node(stmt.step))
    {
        builder_->write_invalid_stmt();
    }
    builder_->write_right_bracket(")");
    builder_->append_space();
    builder_->write_repeat_word();
    process_body(stmt.body);
}

void PseudocodeVisitor::visit(ThrowStmt const& stmt)
{
    builder_->write_throw_word();
    builder_->append_space();
    if (!try_accept_node(stmt.val))
    {
        builder_->write_invalid_expr();
    }
}

void PseudocodeVisitor::visit(UnknownStmt const& /*stmt*/)
{
    builder_->write_unknown_stmt();
}

void PseudocodeVisitor::visit(LocalVarDefStmt const& stmt)
{
    process_var_def(stmt, LOCAL_VAR);
}

void PseudocodeVisitor::visit(ParamVarDefStmt const& stmt)
{
    process_var_def(stmt, PARAM_VAR);
}

void PseudocodeVisitor::visit(MemberVarDefStmt const& stmt)
{
    process_var_def(stmt, MEMBER_VAR);
}

void PseudocodeVisitor::visit(GlobalVarDefStmt const& stmt)
{
    process_var_def(stmt, GLOBAL_VAR);
}

void PseudocodeVisitor::visit(FunctionDefStmt const& stmt)
{
    if (!configurator_->sh_func_declar())
    {
        return;
    }
    if (configurator_->sh_other_expr())
    {
        builder_->write_function_word();
        builder_->append_space();
    }
    builder_->write_function_name(stmt.name);
    process_pargs(stmt.params, false);
    process_return_type(stmt.retType);
    if (configurator_->sh_func_defin())
    {
        process_body(stmt.body);
        return;
    }
    builder_->append_space();
    builder_->write_left_bracket("{");
    builder_->write_right_bracket("}");
}

void PseudocodeVisitor::visit(DefStmt const& stmt)
{
    if (stmt.defs.empty())
    {
        builder_->write_invalid_stmt();
        return;
    }
    if (configurator_->sh_other_expr())
    {
        builder_->write_define_word();
        builder_->append_space();
    }
    if (!try_accept_node(stmt.defs.at(0)->type))
    {
        builder_->write_invalid_type();
    }
    for (size_t i = 0; i < stmt.defs.size(); ++i)
    {
        builder_->append_space();
        builder_->write_local_var_name(stmt.defs.at(i)->name);
        if (stmt.defs.at(i)->initializer)
        {
            builder_->append_space();
            builder_->write_assign_operator();
            builder_->append_space();
            if (!try_accept_node(stmt.defs.at(i)->initializer))
            {
                builder_->write_invalid_expr();
            }
        }
        if (i < stmt.defs.size() - 1)
        {
            builder_->write_separator(",");
            builder_->append_space();
        }
    }
}

void PseudocodeVisitor::visit(MethodDefStmt const& stmt)
{
    if (!configurator_->sh_con_des_meth_defin() ||
        (stmt.access != AccessModifier::Public && !configurator_->use_inner_view()) ||
        !stmt.func
    )
    {
        return;
    }
    ClassDefStmt* ownerClass = static_cast<ClassDefStmt*>(stmt.owner);
    if (configurator_->sh_con_des_meth_owner() && configurator_->sh_con_des_meth_template() && stmt.owner)
    {
        process_generic_params_decl(ownerClass->tparams);
    }
    if (configurator_->sh_other_expr())
    {
        builder_->write_method_word();
        builder_->append_space();
    }
    if (configurator_->sh_con_des_meth_owner() && stmt.owner)
    {
        //builder_->write_class_name(stmt.owner_->name_); // TODO what's next
        if (!ownerClass->tparams.empty() && configurator_->sh_con_des_meth_template())
        {
            if (configurator_->sh_generic_param())
            {
                process_pargs(ownerClass->tparams, true);
            }
        }
        builder_->write_separator("::");
    }
    builder_->write_method_name(stmt.func->name);
    process_pargs(stmt.func->params, false);
    process_return_type(stmt.func->retType);
    process_body(stmt.func->body);
}

void PseudocodeVisitor::visit(BaseInitializerStmt const& stmt)
{
    if (configurator_->sh_other_expr())
    {
        builder_->write_call_word();
        builder_->append_space();
        builder_->write_constructor_word();
        builder_->append_space();
    }
    //builder_->write_class_name(stmt.base_);
    builder_->write_class_name(stmt.type->name); // TODO check
    process_pargs(stmt.args, false);
}

void PseudocodeVisitor::visit(ConstructorDefStmt const& stmt)
{
    if (!configurator_->sh_con_des_meth_defin() || (stmt.access != AccessModifier::Public && !configurator_->use_inner_view()))
    {
        return;
    }
    if (configurator_->sh_con_des_meth_owner() && configurator_->sh_con_des_meth_template() && stmt.owner)
    {
        if (configurator_->sh_generic_param() && !stmt.owner->tparams.empty())
        {
            builder_->write_template_word();
            process_pargs(stmt.owner->tparams, true);
            builder_->append_new_line();
        }
    }
    if (configurator_->sh_other_expr())
    {
        builder_->write_constructor_word();
        builder_->append_space();
    }
    if (configurator_->sh_con_des_meth_owner() && stmt.owner)
    {
        //builder_->write_class_name(stmt.owner_->name_);
        builder_->write_class_name(stmt.owner->type->name); // TODO check
        if (!stmt.owner->tparams.empty() && configurator_->sh_con_des_meth_template())
        {
            process_pargs(stmt.owner->tparams, true);
        }
        builder_->write_separator("::");
    }
    if (stmt.owner)
    {
        //builder_->write_class_name(stmt.owner_->name_);
        builder_->write_class_name(stmt.owner->type->name); // TODO check
    }
    else
    {
        builder_->write_invalid_expr();
    }
    process_pargs(stmt.params, false);
    if (!stmt.baseInit.empty())
    {
        for (size_t i = 0; i < stmt.baseInit.size(); ++i)
        {
            builder_->append_new_line();
            builder_->append_space();
            builder_->write_separator("->");
            builder_->append_space();
            if (!try_accept_node(stmt.baseInit.at(i)))
            {
                builder_->write_invalid_stmt();
            }
        }
    }
    process_body(stmt.body);
}

void PseudocodeVisitor::visit(DestructorDefStmt const& stmt)
{
    if (!configurator_->sh_con_des_meth_defin() && !stmt.body)
    {
        return;
    }
    if (configurator_->sh_con_des_meth_owner() && configurator_->sh_con_des_meth_template() && stmt.owner)
    {
        if (configurator_->sh_generic_param() && !stmt.owner->tparams.empty())
        {
            builder_->write_template_word();
            process_pargs(stmt.owner->tparams, true);
            builder_->append_new_line();
        }
    }
    if (configurator_->sh_other_expr())
    {
        builder_->write_destructor_word();
        builder_->append_space();
    }
    if (configurator_->sh_con_des_meth_owner() && stmt.owner)
    {
        //builder_->write_class_name(stmt.owner_->name_);
        builder_->write_class_name(stmt.owner->type->name); // TODO check
        if (!stmt.owner->tparams.empty() && configurator_->sh_con_des_meth_template())
        {
            process_pargs(stmt.owner->tparams, true);
        }
        builder_->write_separator("::");
    }
    if (stmt.owner)
    {
        //builder_->write_class_name("~" + stmt.owner_->name_);
        builder_->write_class_name("~" + stmt.owner->type->name); // TODO check
    }
    else
    {
        builder_->write_invalid_expr();
    }
    builder_->write_left_bracket("(");
    builder_->write_right_bracket(")");
    process_body(stmt.body);
}

void PseudocodeVisitor::visit(GenericParam const& stmt)
{
    builder_->write_gen_param_name(stmt.name);
}

void PseudocodeVisitor::visit(InterfaceDefStmt const& stmt)
{
    if (!configurator_->sh_interf_declar())
    {
        return;
    }
    if (configurator_->sh_generic_param() && !stmt.tparams.empty())
    {
        builder_->write_template_word();
        process_pargs(stmt.tparams, true);
        builder_->append_new_line();
    }
    builder_->write_interface_word();
    builder_->append_space();
    //builder_->write_interface_name(stmt.name_);
    builder_->write_interface_name(stmt.type->name); // TODO check
    if (!stmt.bases.empty())
    {
        process_relations(stmt.bases, true);
    }
    builder_->write_opening_curl_bracket();
    if (configurator_->sh_interf_defin() && configurator_->sh_con_des_meth_declar() && !stmt.methods.empty())
    {
        process_method_decl(stmt.methods);
    }
    builder_->write_right_bracket("}");
    builder_->append_new_line();
}

void PseudocodeVisitor::visit(ClassDefStmt const& stmt)
{
    if (configurator_->sh_class_declar())
    {
        if (configurator_->sh_generic_param() && !stmt.tparams.empty())
        {
            builder_->write_template_word();
            process_pargs(stmt.tparams, true);
            builder_->append_new_line();
        }
        builder_->write_class_word();
        builder_->append_space();
        //builder_->write_class_name(stmt.name_);
        builder_->write_class_name(stmt.type->name); // TODO check
        if (!stmt.interfaces.empty())
        {
            process_relations(stmt.interfaces, true);
        }
        if (!stmt.bases.empty())
        {
            process_relations(stmt.bases, false);
        }
        builder_->write_opening_curl_bracket();
        if (configurator_->sh_class_defin())
        {
            if (configurator_->sh_member_vars() && !stmt.vars.empty())
            {
                process_member_var_decl(stmt.vars);
            }
            if (configurator_->sh_con_des_meth_declar())
            {
                if (!stmt.constructors.empty())
                {
                    process_constructor_decl(stmt.constructors);
                }
                if (!stmt.destructors.empty())
                {
                    process_destructor_decl(stmt.destructors);
                }
                if (!stmt.methods.empty())
                {
                    process_method_decl(stmt.methods);
                }
            }
        }
        builder_->write_right_bracket("}");
        builder_->append_new_line();
    }
    if (configurator_->sh_con_des_meth_defin())
    {
        for (size_t i = 0; i < stmt.constructors.size(); ++i)
        {
            if (stmt.constructors.at(i))
            {
                builder_->append_new_line();
                stmt.constructors.at(i)->accept(*this);
                builder_->append_new_line();
            }
        }
        for (size_t i = 0; i < stmt.destructors.size(); ++i)
        {
            if (stmt.destructors.at(i) && stmt.destructors.at(i)->body)
            {
                builder_->append_new_line();
                stmt.destructors.at(i)->accept(*this);
                builder_->append_new_line();
            }
        }
        for (size_t i = 0; i < stmt.methods.size(); ++i)
        {
            if (stmt.methods.at(i) && stmt.methods.at(i)->func && stmt.methods.at(i)->func->body)
            {
                builder_->append_new_line();
                stmt.methods.at(i)->accept(*this);
                builder_->append_new_line();
            }
        }
    }
}

void PseudocodeVisitor::visit(ContinueStmt const& /*stmt*/)
{
    builder_->write_continue_word();
}

void PseudocodeVisitor::visit(BreakStmt const& /*stmt*/)
{
    builder_->write_break_word();
}

//
// -----
//

void PseudocodeVisitor::process_var_def(VarDefStmt const& var, int vartype)
{
    if (configurator_->sh_other_expr() && vartype != PARAM_VAR)
    {
        builder_->write_define_word();
        builder_->append_space();
    }
    if (!try_accept_node(var.type))
    {
        builder_->write_invalid_type();
    }
    builder_->append_space();
    if (vartype == LOCAL_VAR)
    {
        builder_->write_local_var_name(var.name);
    }
    else if (vartype == PARAM_VAR)
    {
        builder_->write_param_var_name(var.name);
    }
    else if (vartype == MEMBER_VAR)
    {
        builder_->write_member_var_name(var.name);
    }
    else if (vartype == GLOBAL_VAR)
    {
        builder_->write_global_var_name(var.name);
    }
    if (var.initializer)
    {
        builder_->append_space();
        builder_->write_assign_operator();
        builder_->append_space();
        if (!try_accept_node(var.initializer))
        {
            builder_->write_invalid_expr();
        }
    }
}

void PseudocodeVisitor::process_return_type(Type const* const& type)
{
    builder_->append_space();
    builder_->write_separator("->");
    builder_->append_space();
    if (configurator_->sh_other_expr())
    {
        builder_->write_returns_word();
        builder_->append_space();
    }
    if (!try_accept_node(type))
    {
        builder_->write_invalid_type();
    }
}

void PseudocodeVisitor::process_generic_params_decl(std::vector<GenericParam*> const& vgeneric)
{
    if (configurator_->sh_generic_param() && !vgeneric.empty())
    {
        builder_->write_template_word();
        process_pargs(vgeneric, true);
        builder_->write_new_word();
    }
}

void PseudocodeVisitor::process_member_var_decl(std::vector<MemberVarDefStmt*> const& vmembervars)
{
    std::vector<MemberVarDefStmt*> vfound {};
    if (try_find_access_mod(vmembervars, vfound, AccessModifier::Public))
    {
        builder_->write_public_word();
        process_member_var(vfound);
    }
    if (configurator_->use_inner_view())
    {
        if (try_find_access_mod(vmembervars, vfound, AccessModifier::Private))
        {
            builder_->write_private_word();
            process_member_var(vfound);
        }
        if (try_find_access_mod(vmembervars, vfound, AccessModifier::Protected))
        {
            builder_->write_protected_word();
            process_member_var(vfound);
        }
        if (try_find_access_mod(vmembervars, vfound, AccessModifier::Internal))
        {
            builder_->write_internal_word();
            process_member_var(vfound);
        }
    }
}

void PseudocodeVisitor::process_member_var(std::vector<MemberVarDefStmt*>& vmembervars)
{
    builder_->append_space();
    builder_->write_attributes_word();
    builder_->append_new_line();
    builder_->increase_indentation();
    for (size_t i = 0; i < vmembervars.size(); ++i)
    {
        if (vmembervars.at(i))
        {
            vmembervars.at(i)->accept(*this);
            builder_->append_new_line();
        }
    }
    builder_->decrease_indentation();
    vmembervars.clear();
}

void PseudocodeVisitor::process_constructor_decl(std::vector<ConstructorDefStmt*> const& vconstructors)
{
    std::vector<ConstructorDefStmt*> vfound {};
    if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Public)))
    {
        builder_->write_public_word();
        process_constructor(vfound);
    }
    if (configurator_->use_inner_view())
    {
        if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Private)))
        {
            builder_->write_private_word();
            process_constructor(vfound);
        }
        if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Protected)))
        {
            builder_->write_protected_word();
            process_constructor(vfound);
        }
        if (try_find_access_mod(vconstructors, vfound, std::move(AccessModifier::Internal)))
        {
            builder_->write_internal_word();
            process_constructor(vfound);
        }
    }
}

void PseudocodeVisitor::process_constructor(std::vector<ConstructorDefStmt*>& constr)
{
    builder_->append_space();
    builder_->write_constructors_word();
    builder_->append_new_line();
    builder_->increase_indentation();
    for (size_t i = 0; i < constr.size(); ++i)
    {
        if (constr.at(i) && constr.at(i)->owner)
        {
            //builder_->write_class_name(constr.at(i)->owner_->name_);
            builder_->write_class_name(constr.at(i)->owner->type->name); // TODO check
            process_pargs(constr.at(i)->params, false);
        }
        else
        {
            builder_->write_invalid_stmt();
        }
        builder_->append_new_line();
    }
    builder_->decrease_indentation();
    constr.clear();
}

void PseudocodeVisitor::process_destructor_decl(std::vector<DestructorDefStmt*> const& vdestructors)
{
    builder_->write_destructors_word();
    builder_->append_new_line();
    builder_->increase_indentation();
    for (size_t i = 0; i < vdestructors.size(); ++i)
    {
        if (vdestructors.at(i) && vdestructors.at(i)->owner)
        {
            //builder_->write_class_name("~" + vdestructors.at(i)->owner_->name_);
            builder_->write_class_name("~" + vdestructors.at(i)->owner->type->name); // TODO check
            builder_->write_left_bracket("(");
            builder_->write_right_bracket(")");
        }
        else
        {
            builder_->write_invalid_stmt();
        }
        builder_->append_new_line();
    }
    builder_->decrease_indentation();
}

void PseudocodeVisitor::process_method_decl(std::vector<MethodDefStmt*> const& vmethods)
{
    std::vector<MethodDefStmt*> vfound {};
    if (try_find_access_mod(vmethods, vfound, AccessModifier::Public))
    {
        builder_->write_public_word();
        process_method(vfound);
    }
    if (configurator_->use_inner_view())
    {
        if (try_find_access_mod(vmethods, vfound, AccessModifier::Private))
        {
            builder_->write_private_word();
            process_method(vfound);
        }
        if (try_find_access_mod(vmethods, vfound, AccessModifier::Protected))
        {
            builder_->write_protected_word();
            process_method(vfound);
        }
        if (try_find_access_mod(vmethods, vfound, AccessModifier::Internal))
        {
            builder_->write_internal_word();
            process_method(vfound);
        }
    }
}

void PseudocodeVisitor::process_method(std::vector<MethodDefStmt*>& meth)
{
    builder_->append_space();
    builder_->write_methods_word();
    builder_->append_new_line();
    builder_->increase_indentation();
    for (size_t i = 0; i < meth.size(); ++i)
    {
        if (meth.at(i) && meth.at(i)->func)
        {
            if (meth.at(i)->func->body)
            {
                if (meth.at(i)->virtuality == Virtuality::Virtual)
                {
                    builder_->write_virtual_word();
                    builder_->write_separator(" -> ");
                }
            }
            else
            {
                builder_->write_abstract_word();
                builder_->write_separator(" -> ");
            }
            builder_->write_method_name(meth.at(i)->func->name);
            process_pargs(meth.at(i)->func->params, false);
            process_return_type(meth.at(i)->func->retType);
        }
        else
        {
            builder_->write_invalid_stmt();
        }
        builder_->append_new_line();
    }
    builder_->decrease_indentation();
    meth.clear();
}