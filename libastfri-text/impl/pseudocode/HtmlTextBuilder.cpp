#include <libastfri-text/inc/pseudocode/HtmlTextBuilder.hpp>

using namespace astfri::text;

HtmlTextBuilder& HtmlTextBuilder::get_instance()
{
    static HtmlTextBuilder builder;
    return builder;
}

HtmlTextBuilder::HtmlTextBuilder() :
    maxBracketIndex_(configurator_->bracket_colors()->size()),
    currentBrIndex_(0)
{
}

//
// SET_UP
//

void HtmlTextBuilder::reset_builder()
{
    maxBracketIndex_ = configurator_->bracket_colors()->size();
    currentBrIndex_ = 0;
    AbstractTextBuilder::reset_builder();
}

//
// GENERAL_TEXT
//

void HtmlTextBuilder::append_new_line()
{
    *buildedText_ << "<br>\n";
    isEmptyLine_ = true;
    ++rowCount_;
}

void HtmlTextBuilder::append_space()
{
    *buildedText_ << "&nbsp;";
}

//
// BRACKETS, SEPARATORS, OPERATORS
//

void HtmlTextBuilder::write_right_bracket(std::string const& br)
{
    if (configurator_->use_bracket_colors() && maxBracketIndex_ > 0)
    {
        std::string out = "<span style=\"br-color:";
        if (currentBrIndex_ == 0)
        {
            currentBrIndex_ = maxBracketIndex_ - 1;
        }
        else
        {
            --currentBrIndex_;
        }
        out.append(configurator_->bracket_colors()->at(currentBrIndex_).str());
        out.append("\">");
        out.append(br);
        out.append("</span>");
        write_operator_style(out);
    }
    else
    {
        write_operator_style(br);
    }
}

void HtmlTextBuilder::write_left_bracket(std::string const& br)
{
    if (configurator_->use_bracket_colors() && maxBracketIndex_ > 0)
    {
        std::string out = "<span style=\"br-color:";
        out.append(configurator_->bracket_colors()->at(currentBrIndex_).str());
        out.append("\">");
        out.append(br);
        out.append("</span>");
        ++currentBrIndex_;
        if (currentBrIndex_ == maxBracketIndex_)
        {
            currentBrIndex_ = 0;
        }
        write_operator_style(out);
    }
    else
    {
        write_operator_style(br);
    }
}

void HtmlTextBuilder::write_separator(std::string const& sep)
{
    write_separator_style(sep);
}

void HtmlTextBuilder::write_operator(std::string const& op)
{
    write_operator_style(op);
}

void HtmlTextBuilder::write_assign_operator()
{
    write_operator_style(configurator_->assign_op_word()->str());
}

void HtmlTextBuilder::write_modulo_operator()
{
    write_operator_style(configurator_->modulo_op_word()->str());
}

void HtmlTextBuilder::write_address_operator()
{
    write_operator_style(configurator_->address_op_word()->str());
}

void HtmlTextBuilder::write_deref_operator()
{
    write_operator_style(configurator_->deref_op_word()->str());
}

//
// UNKNOWN_PHRASES
//

void HtmlTextBuilder::write_unknown_type()
{
    std::string s = "<span class=\"unknown-type\">" + configurator_->unknown_type_word()->str() + "</span>";
    write_unavailable_style(s);
}

void HtmlTextBuilder::write_unknown_expr()
{
    std::string s = "<span class=\"unknown-expr\">" + configurator_->unknown_expr_word()->str() + "</span>";
    write_unavailable_style(s);
}

void HtmlTextBuilder::write_unknown_stmt()
{
    std::string s = "<span class=\"unknown-stmt\">" + configurator_->unknown_stmt_word()->str() + "</span>";
    write_unavailable_style(s);
}

void HtmlTextBuilder::write_invalid_type()
{
    std::string s = "<span class=\"invalid-type\">" + configurator_->invalid_type_word()->str() + "</span>";
    write_unavailable_style(s);
}

void HtmlTextBuilder::write_invalid_expr()
{
    std::string s = "<span class=\"invalid-expr\">" + configurator_->invalid_expr_word()->str() + "</span>";
    write_unavailable_style(s);
}

void HtmlTextBuilder::write_invalid_stmt()
{
    std::string s = "<span class=\"invalid-stmt\">" + configurator_->invalid_stmt_word()->str() + "</span>";
    write_unavailable_style(s);
}

//
// ACCESS_MODIFIERS
//

void HtmlTextBuilder::write_public_word()
{
    write_acc_mod_style(configurator_->public_word()->str());
}

void HtmlTextBuilder::write_protected_word()
{
    write_acc_mod_style(configurator_->protected_word()->str());
}

void HtmlTextBuilder::write_private_word()
{
    write_acc_mod_style(configurator_->private_word()->str());
}

void HtmlTextBuilder::write_internal_word()
{
    write_acc_mod_style(configurator_->internal_word()->str());
}

void HtmlTextBuilder::write_attributes_word()
{
    write_acc_mod_style(configurator_->attributes_word()->str());
}

void HtmlTextBuilder::write_constructors_word()
{
    write_acc_mod_style(configurator_->constructors_word()->str());
}

void HtmlTextBuilder::write_destructors_word()
{
    write_acc_mod_style(configurator_->destructors_word()->str());
}

void HtmlTextBuilder::write_methods_word()
{
    write_acc_mod_style(configurator_->methods_word()->str());
}

//
// DATA_TYPES
//

void HtmlTextBuilder::write_dynamic_type_word()
{
    std::string s = "<span class=\"dynamic-word\">" + configurator_->dynamic_type_word()->str() + "</span>";
    write_data_type_style(s);
}

void HtmlTextBuilder::write_int_type_word()
{
    std::string s = "<span class=\"int-word\">" + configurator_->int_type_word()->str() + "</span>";
    write_data_type_style(s);
}

void HtmlTextBuilder::write_float_type_word()
{
    std::string s = "<span class=\"float-word\">" + configurator_->float_type_word()->str() + "</span>";
    write_data_type_style(s);
}

void HtmlTextBuilder::write_char_type_word()
{
    std::string s = "<span class=\"char-word\">" + configurator_->char_type_word()->str() + "</span>";
    write_data_type_style(s);
}

void HtmlTextBuilder::write_bool_type_word()
{
    std::string s = "<span class=\"bool-word\">" + configurator_->bool_type_word()->str() + "</span>";
    write_data_type_style(s);
}

void HtmlTextBuilder::write_void_type_word()
{
    std::string s = "<span class=\"void-word\">" + configurator_->void_type_word()->str() + "</span>";
    write_data_type_style(s);
}

void HtmlTextBuilder::write_class_type(std::string const& name)
{
    std::string s = "<span class=\"class-type\">" + name + "</span>";
    write_data_type_style(s);
}

void HtmlTextBuilder::write_interface_type(std::string const& name)
{
    std::string s = "<span class=\"interface-type\">" + name + "</span>";
    write_data_type_style(s);
}

//
// REFERENCE_NAMES
//

void HtmlTextBuilder::write_scope_name(std::string const& name)
{
    std::string s = "<span class=\"scope-name\">" + name + "</span>";
    write_ref_name_style(s);
}

void HtmlTextBuilder::write_gen_param_name(std::string const& name)
{
    std::string s = "<span class=\"gen-par-name\">" + name + "</span>";
    write_ref_name_style(s);
}

void HtmlTextBuilder::write_class_name(std::string const& name)
{
    std::string s = "<span class=\"class-name\">" + name + "</span>";
    write_ref_name_style(s);
}

void HtmlTextBuilder::write_interface_name(std::string const& name)
{
    std::string s = "<span class=\"interface-name\">" + name + "</span>";
    write_ref_name_style(s);
}

void HtmlTextBuilder::write_method_name(std::string const& name)
{
    std::string s = "<span class=\"method-name\">" + name + "</span>";
    write_ref_name_style(s);
}

void HtmlTextBuilder::write_function_name(std::string const& name)
{
    std::string s = "<span class=\"function-name\">" + name + "</span>";
    write_ref_name_style(s);
}

void HtmlTextBuilder::write_global_var_name(std::string const& name)
{
    std::string s = "<span class=\"global-var-name\">" + name + "</span>";
    write_ref_name_style(s);
}

void HtmlTextBuilder::write_member_var_name(std::string const& name)
{
    std::string s = "<span class=\"member-var-name\">" + name + "</span>";
    write_ref_name_style(s);
}

void HtmlTextBuilder::write_local_var_name(std::string const& name)
{
    std::string s = "<span class=\"local-var-name\">" + name + "</span>";
    write_ref_name_style(s);
}

void HtmlTextBuilder::write_param_var_name(std::string const& name)
{
    std::string s = "<span class=\"param-var-name\">" + name + "</span>";
    write_ref_name_style(s);
}

//
// VALUES
//

void HtmlTextBuilder::write_int_val(int const& val)
{
    std::string s = "<span class=\"int-value\">" + std::to_string(val) + "</span>";
    write_value_style(s);
}

void HtmlTextBuilder::write_float_val(float const& val)
{
    std::string s = "<span class=\"float-value\">" + std::to_string(val) + "</span>";
    write_value_style(s);
}

void HtmlTextBuilder::write_char_val(char const& val)
{
    std::string s = "<span class=\"char-value\">" + std::string(1, val) + "</span>";
    write_value_style(s);
}

void HtmlTextBuilder::write_string_val(std::string const& val)
{
    std::string s = "<span class=\"string-value\">" + val + "</span>";
    write_value_style(s);
}

void HtmlTextBuilder::write_bool_val(bool const& val)
{
    std::string s = "<span class=\"bool-value\">";
    if (val)
    {
        s.append(std::move(configurator_->true_val_word()->str()));
    }
    else
    {
        s.append(std::move(configurator_->false_val_word()->str()));
    }
    s.append("</span>");
    write_value_style(s);
}

void HtmlTextBuilder::write_null_val()
{
    std::string s = "<span class=\"null-value\">" + configurator_->null_val_word()->str() + "</span>";
    write_value_style(s);
}

//
// SYSTEM_EXPRESSIONS
//

void HtmlTextBuilder::write_scope_word()
{
    std::string s = "<span class=\"scope-word\">" + configurator_->scope_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_class_word()
{
    std::string s = "<span class=\"class-word\">" + configurator_->class_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_interface_word()
{
    std::string s = "<span class=\"interface-word\">" + configurator_->interface_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_implement_word()
{
    std::string s = "<span class=\"implement-word\">" + configurator_->implement_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_extend_word()
{
    std::string s = "<span class=\"extend-word\">" + configurator_->extend_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_this_word()
{
    std::string s = "<span class=\"this-word\">" + configurator_->this_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_return_word()
{
    std::string s = "<span class=\"return-word\">" + configurator_->return_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_continue_word()
{
    std::string s = "<span class=\"continue-word\">" + configurator_->continue_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_break_word()
{
    std::string s = "<span class=\"break-word\">" + configurator_->break_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_throw_word()
{
    std::string s = "<span class=\"throw-word\">" + configurator_->throw_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_if_word()
{
    std::string s = "<span class=\"if-word\">" + configurator_->if_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_else_word()
{
    std::string s = "<span class=\"else-word\">" + configurator_->else_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_do_word()
{
    std::string s = "<span class=\"do-word\">" + configurator_->do_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_while_word()
{
    std::string s = "<span class=\"while-word\">" + configurator_->while_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_for_word()
{
    std::string s = "<span class=\"for-word\">" + configurator_->for_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_switch_word()
{
    std::string s = "<span class=\"switch-word\">" + configurator_->switch_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_case_word()
{
    std::string s = "<span class=\"case-word\">" + configurator_->case_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_default_word()
{
    std::string s = "<span class=\"default-word\">" + configurator_->default_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_new_word()
{
    std::string s = "<span class=\"new-word\">" + configurator_->new_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_delete_word()
{
    std::string s = "<span class=\"delete-word\">" + configurator_->delete_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_pointer_word()
{
    std::string s = "<span class=\"pointer-word\">" + configurator_->pointer_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_override_word()
{
    std::string s = "<span class=\"override-word\">" + configurator_->override_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_virtual_word()
{
    std::string s = "<span class=\"virtual-word\">" + configurator_->virtual_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_abstract_word()
{
    std::string s = "<span class=\"abstract-word\">" + configurator_->abstract_word()->str() + "</span>";
    write_system_expr_style(s);
}

void HtmlTextBuilder::write_template_word()
{
    std::string s = "<span class=\"template-word\">" + configurator_->template_word()->str() + "</span>";
    write_system_expr_style(s);
}

//
// OTHER_EXPRESSIONS
//

void HtmlTextBuilder::write_constructor_word()
{
    std::string s = "<span class=\"constr-word\">" + configurator_->constructor_word()->str() + "</span>";
    write_other_expr_style(s);
}

void HtmlTextBuilder::write_destructor_word()
{
    std::string s = "<span class=\"destr-word\">" + configurator_->destructor_word()->str() + "</span>";
    write_other_expr_style(s);
}

void HtmlTextBuilder::write_method_word()
{
    std::string s = "<span class=\"method-word\">" + configurator_->method_word()->str() + "</span>";
    write_other_expr_style(s);
}

void HtmlTextBuilder::write_function_word()
{
    std::string s = "<span class=\"function-word\">" + configurator_->function_word()->str() + "</span>";
    write_other_expr_style(s);
}

void HtmlTextBuilder::write_lambda_word()
{
    std::string s = "<span class=\"lambda-word\">" + configurator_->lambda_word()->str() + "</span>";
    write_other_expr_style(s);
}

void HtmlTextBuilder::write_call_word()
{
    std::string s = "<span class=\"call-word\">" + configurator_->call_word()->str() + "</span>";
    write_other_expr_style(s);
}

void HtmlTextBuilder::write_define_word()
{
    std::string s = "<span class=\"define-word\">" + configurator_->define_word()->str() + "</span>";
    write_other_expr_style(s);
}

void HtmlTextBuilder::write_returns_word()
{
    std::string s = "<span class=\"returns-word\">" + configurator_->returns_word()->str() + "</span>";
    write_other_expr_style(s);
}

void HtmlTextBuilder::write_repeat_word()
{
    std::string s = "<span class=\"repeat-word\">" + configurator_->repeat_word()->str() + "</span>";
    write_system_expr_style(s);
}

//
// WRAPPING_CLASSES
//

void HtmlTextBuilder::write_unavailable_style(std::string const& phrase)
{
    append_text("<span class=\"unavailable\">");
    append_text(phrase);
    append_text("</span>");
}

void HtmlTextBuilder::write_acc_mod_style(std::string const& accmod)
{
    append_text("<span class=\"access-mod\">");
    append_text(accmod);
    append_text("</span>");
}

void HtmlTextBuilder::write_data_type_style(std::string const& datatype)
{
    append_text("<span class=\"data-type\">");
    append_text(datatype);
    append_text("</span>");
}

void HtmlTextBuilder::write_ref_name_style(std::string const& name)
{
    append_text("<span class=\"ref-name\">");
    append_text(name);
    append_text("</span>");
}

void HtmlTextBuilder::write_separator_style(std::string const& sep)
{
    append_text("<span class=\"separator\">");
    append_text(sep);
    append_text("</span>");
}

void HtmlTextBuilder::write_operator_style(std::string const& op)
{
    append_text("<span class=\"operator\">");
    append_text(op);
    append_text("</span>");
}

void HtmlTextBuilder::write_value_style(std::string const& val)
{
    append_text("<span class=\"value\">");
    append_text(val);
    append_text("</span>");
}

void HtmlTextBuilder::write_system_expr_style(std::string const& expr)
{
    append_text("<span class=\"system-expr\">");
    append_text(expr);
    append_text("</span>");
}

void HtmlTextBuilder::write_other_expr_style(std::string const& expr)
{
    append_text("<span class=\"other-expr\">");
    append_text(expr);
    append_text("</span>");
}