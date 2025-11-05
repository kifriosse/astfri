#include <libastfri-text/inc/pseudocode/HtmlExporter.hpp>

#include <fstream>
#include <cmath>

using namespace astfri::text;

HtmlExporter::HtmlExporter() : Exporter()
{
    reset();
}

void HtmlExporter::reset()
{
    maxBracketIndex_ = configurator_->bracket_colors()->size();
    currentBrIndex_ = 0;
    Exporter::reset();
}

void HtmlExporter::write_pseudocode_into_file(STRING fullfilepath)
{
    std::ofstream file(fullfilepath);
    file << "<!DOCTYPE html>\n";
    file << "<html lang=\"sk\">\n";
    file << "<head>\n";
    file << "<meta charset=\"UTF-8\">\n";
    file << "<title>" << configurator_->output_file_name()->str() << "</title>\n";
    file << "<style>\n";
    // row number
    file << ".row-num{" << configurator_->row_num_style()->str() << "}\n";
    // UNKNOWN_PHRASES
    //file << ".unknown-word{" << configurator_->unknown_word_style()->str() << "}\n";
    //file << ".invalid-word{" << configurator_->invalid_word_style()->str() << "}\n";
    // ACCESS_MODIFIERS
    file << ".access-mod{" << configurator_->access_mod_style()->str() << "}\n";
    // DATA_TYPES
    file << ".data-type{" << configurator_->default_type_word_style()->str() << "}\n";
    file << ".dynamic-type{" << configurator_->dynamic_type_word_style()->str() << "}\n";
    file << ".int-type{" << configurator_->int_type_word_style()->str() << "}\n";
    file << ".float-type{" << configurator_->float_type_word_style()->str() << "}\n";
    file << ".char-type{" << configurator_->char_type_word_style()->str() << "}\n";
    file << ".bool-type{" << configurator_->bool_type_word_style()->str() << "}\n";
    file << ".void-type{" << configurator_->void_type_word_style()->str() << "}\n";
    file << ".user-type{" << configurator_->user_type_style()->str() << "}\n";
    // REFERENCE_NAMES
    file << ".ref-name{" << configurator_->default_ref_name_style()->str() << "}\n";
    file << ".gen-par-name{" << configurator_->gen_param_name_style()->str() << "}\n";
    file << ".class-name{" << configurator_->class_name_style()->str() << "}\n";
    file << ".interface-name{" << configurator_->interface_name_style()->str() << "}\n";
    file << ".method-name{" << configurator_->method_name_style()->str() << "}\n";
    file << ".function-name{" << configurator_->function_name_style()->str() << "}\n";
    file << ".global-var-name{" << configurator_->global_var_name_style()->str() << "}\n";
    file << ".member-var-name{" << configurator_->member_var_name_style()->str() << "}\n";
    file << ".local-var-name{" << configurator_->local_var_name_style()->str() << "}\n";
    file << ".param-var-name{" << configurator_->param_var_name_style()->str() << "}\n";
    // OPERATORS & SEPARATORS
    file << ".operator{" << configurator_->operator_style()->str() << "}\n";
    file << ".separator{" << configurator_->separator_style()->str() << "}\n";
    // VALUES
    file << ".value{" << configurator_->default_val_style()->str() << "}\n";
    file << ".int-value{" << configurator_->int_val_style()->str() << "}\n";
    file << ".float-value{" << configurator_->float_val_style()->str() << "}\n";
    file << ".char-value{" << configurator_->char_val_style()->str() << "}\n";
    file << ".string-value{" << configurator_->string_val_style()->str() << "}\n";
    file << ".bool-value{" << configurator_->bool_val_style()->str() << "}\n";
    file << ".null-value{" << configurator_->null_val_style()->str() << "}\n";
    // SYSTEM_EXPRESSIONS
    file << ".system-expr{" << configurator_->system_expr_style()->str() << "}\n";
    file << ".class-word{" << configurator_->class_word_style()->str() << "}\n";
    file << ".interface-word{" << configurator_->interface_word_style()->str() << "}\n";
    file << ".implement-word{" << configurator_->implement_word_style()->str() << "}\n";
    file << ".extend-word{" << configurator_->extend_word_style()->str() << "}\n";
    file << ".this-word{" << configurator_->this_word_style()->str() << "}\n";
    file << ".return-word{" << configurator_->return_word_style()->str() << "}\n";
    file << ".continue-word{" << configurator_->continue_word_style()->str() << "}\n";
    file << ".break-word{" << configurator_->break_word_style()->str() << "}\n";
    file << ".throw-word{" << configurator_->throw_word_style()->str() << "}\n";
    file << ".if-word{" << configurator_->if_word_style()->str() << "}\n";
    file << ".else-word{" << configurator_->else_word_style()->str() << "}\n";
    file << ".do-word{" << configurator_->do_word_style()->str() << "}\n";
    file << ".while-word{" << configurator_->while_word_style()->str() << "}\n";
    file << ".for-word{" << configurator_->for_word_style()->str() << "}\n";
    file << ".repeat-word{" << configurator_->repeat_word_style()->str() << "}\n";
    file << ".switch-word{" << configurator_->switch_word_style()->str() << "}\n";
    file << ".case-word{" << configurator_->case_word_style()->str() << "}\n";
    file << ".default-word{" << configurator_->default_word_style()->str() << "}\n";
    file << ".new-word{" << configurator_->new_word_style()->str() << "}\n";
    file << ".delete-word{" << configurator_->delete_word_style()->str() << "}\n";
    file << ".pointer-word{" << configurator_->pointer_word_style()->str() << "}\n";
    file << ".virtual-word{" << configurator_->virtual_word_style()->str() << "}\n";
    file << ".abstract-word{" << configurator_->abstract_word_style()->str() << "}\n";
    file << ".template-word{" << configurator_->template_word_style()->str() << "}\n";
    // OTHER_EXPRESSIONS
    file << ".other-expr{" << configurator_->other_expression_style()->str() << "}\n";
    file << ".constr-word{" << configurator_->constructor_word_style()->str() << "}\n";
    file << ".destr-word{" << configurator_->destructor_word_style()->str() << "}\n";
    file << ".method-word{" << configurator_->method_word_style()->str() << "}\n";
    file << ".function-word{" << configurator_->function_word_style()->str() << "}\n";
    file << ".lambda-word{" << configurator_->lambda_word_style()->str() << "}\n";
    file << ".call-word{" << configurator_->call_word_style()->str() << "}\n";
    file << ".define-word{" << configurator_->define_word_style()->str() << "}\n";
    file << ".returns-word{" << configurator_->returns_word_style()->str() << "}\n";
    file << "</style>\n";
    file << "</head>\n";
    file << "<body style=" << configurator_->default_text_style()->str() << ">\n";
    if (configurator_->sh_row_num())
    {
        int row = 1;
        int delimiter = static_cast<int>(std::log10(rowCount_)) + 1;
        std::string line;
        while (std::getline(*outputPseudocode_,line))
        {
            if (line != "<br>" || (line == "<br>" && configurator_->sh_row_num_empty_row()))
            {
                for (int i = 0; i < configurator_->row_num_margin_left_len(); ++i)
                {
                    file << "&nbsp;";
                }
                int curDelim = static_cast<int>(std::log10(row)) + 1;
                int spaces = delimiter - curDelim;
                for (int i = 0; i < spaces; ++i)
                {
                    file << "&nbsp;";
                }
                file << "<span class=\"row-num\">" << row;
                if (configurator_->sh_dot_after_row_num())
                {
                    file << ".";
                }
                file << "</span>";
                ++row;
            }
            else if (configurator_->reset_row_num_empty_row())
            {
                row = 1;
            }
            file << line << "\n";
        }
    }
    else
    {
        file << outputPseudocode_->str();
    }
    file << "</body>\n";
    file << "</html>";
    file.close();
    reset();
}

// WRAPPING CLASSES

void HtmlExporter::write_acc_mod_style(std::string const& accmod)
{
    write_text("<span class=\"access-mod\">");
    write_text(std::move(accmod));
    write_text("</span>");
}

void HtmlExporter::write_data_type_style(std::string const& datatype)
{
    write_text("<span class=\"data-type\">");
    write_text(std::move(datatype));
    write_text("</span>");
}

void HtmlExporter::write_ref_name_style(std::string const& name)
{
    write_text("<span class=\"ref-name\">");
    write_text(std::move(name));
    write_text("</span>");
}

void HtmlExporter::write_operator_style(std::string const& op)
{
    write_text("<span class=\"operator\">");
    write_text(std::move(op));
    write_text("</span>");
}

void HtmlExporter::write_separator_style(std::string const& sep)
{
    write_text("<span class=\"separator\">");
    write_text(std::move(sep));
    write_text("</span>");
}

void HtmlExporter::write_value_style(std::string const& val)
{
    write_text("<span class=\"value\">");
    write_text(std::move(val));
    write_text("</span>");
}

void HtmlExporter::write_system_expr_style(std::string const& expr)
{
    write_text("<span class=\"system-expr\">");
    write_text(std::move(expr));
    write_text("</span>");
}

void HtmlExporter::write_other_expr_style(std::string const& expr)
{
    write_text("<span class=\"other-expr\">");
    write_text(std::move(expr));
    write_text("</span>");
}

// BRACKETS

void HtmlExporter::write_left_bracket(STRING br)
{
    if (configurator_->use_bracket_colors() && maxBracketIndex_ > 0)
    {
        std::string out = "<span style=\"color:";
        out.append(configurator_->bracket_colors()->at(currentBrIndex_).str());
        out.append("\">");
        out.append(br);
        out.append("</span>");
        ++currentBrIndex_;
        if (currentBrIndex_ == maxBracketIndex_)
        {
            currentBrIndex_ = 0;
        }
        write_text(std::move(out));
    }
    else
    {
        write_text(std::move(br));
    }
}

void HtmlExporter::write_right_bracket(STRING br)
{
    if (configurator_->use_bracket_colors() && maxBracketIndex_ > 0)
    {
        std::string out = "<span style=\"color:";
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
        write_text(std::move(out));
    }
    else
    {
        write_text(std::move(br));
    }
}

// UNKNOWN_PHRASES

void HtmlExporter::write_unknown_type_word()
{
    std::string s = "<span class=\"unknown-type-word\">" + configurator_->unknown_type_word_style()->str() + "</span>";
    write_text(std::move(s));
}

void HtmlExporter::write_unknown_expr_word()
{
    std::string s = "<span class=\"unknown-expr-word\">" + configurator_->unknown_expr_word_style()->str() + "</span>";
    write_text(std::move(s));
}

void HtmlExporter::write_unknown_stmt_word()
{
    std::string s = "<span class=\"unknown-stmt-word\">" + configurator_->unknown_stmt_word_style()->str() + "</span>";
    write_text(std::move(s));
}

void HtmlExporter::write_invalid_type_word()
{
    std::string s = "<span class=\"invalid-type-word\">" + configurator_->invalid_type_word_style()->str() + "</span>";
    write_text(std::move(s));
}

void HtmlExporter::write_invalid_expr_word()
{
    std::string s = "<span class=\"invalid-expr-word\">" + configurator_->invalid_expr_word_style()->str() + "</span>";
    write_text(std::move(s));
}

void HtmlExporter::write_invalid_stmt_word()
{
    std::string s = "<span class=\"invalid-stmt-word\">" + configurator_->invalid_stmt_word_style()->str() + "</span>";
    write_text(std::move(s));
}

// ACCESS_MODIFIERS

void HtmlExporter::write_public_word()
{
    write_acc_mod_style(configurator_->public_word()->str());
}

void HtmlExporter::write_protected_word()
{
    write_acc_mod_style(configurator_->protected_word()->str());
}

void HtmlExporter::write_private_word()
{
    write_acc_mod_style(configurator_->private_word()->str());
}

void HtmlExporter::write_internal_word()
{
    write_acc_mod_style(configurator_->internal_word()->str());
}

void HtmlExporter::write_attributes_word()
{
    write_acc_mod_style(configurator_->attributes_word()->str());
}

void HtmlExporter::write_constructors_word()
{
    write_acc_mod_style(configurator_->constructors_word()->str());
}

void HtmlExporter::write_destructors_word()
{
    write_acc_mod_style(configurator_->destructors_word()->str());
}

void HtmlExporter::write_methods_word()
{
    write_acc_mod_style(configurator_->methods_word()->str());
}

// DATA_TYPES

void HtmlExporter::write_dynamic_type_word()
{
    std::string s = "<span class=\"dynamic-word\">" + configurator_->dynamic_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlExporter::write_int_type_word()
{
    std::string s = "<span class=\"int-word\">" + configurator_->int_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlExporter::write_float_type_word()
{
    std::string s = "<span class=\"float-word\">" + configurator_->float_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlExporter::write_char_type_word()
{
    std::string s = "<span class=\"char-word\">" + configurator_->char_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlExporter::write_bool_type_word()
{
    std::string s = "<span class=\"bool-word\">" + configurator_->bool_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlExporter::write_void_type_word()
{
    std::string s = "<span class=\"void-word\">" + configurator_->void_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlExporter::write_user_type(std::string usertype)
{
    std::string s = "<span class=\"user-type\">" + std::move(usertype) + "</span>";
    write_data_type_style(std::move(s));
}

// REFERENCE_NAMES

void HtmlExporter::write_gen_param_name(std::string name)
{
    std::string s = "<span class=\"gen-par-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlExporter::write_class_name(std::string name)
{
    std::string s = "<span class=\"class-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlExporter::write_interface_name(std::string name)
{
    std::string s = "<span class=\"interface-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlExporter::write_method_name(std::string name)
{
    std::string s = "<span class=\"method-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlExporter::write_function_name(std::string name)
{
    std::string s = "<span class=\"function-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlExporter::write_global_var_name(std::string name)
{
    std::string s = "<span class=\"global-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlExporter::write_member_var_name(std::string name)
{
    std::string s = "<span class=\"member-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlExporter::write_local_var_name(std::string name)
{
    std::string s = "<span class=\"local-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlExporter::write_param_var_name(std::string name)
{
    std::string s = "<span class=\"param-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

// SIGNS

void HtmlExporter::write_operator_sign(STRING sign)
{
    write_operator_style(std::move(sign));
}

void HtmlExporter::write_assign_op_word()
{
    write_operator_style(configurator_->assign_op_word()->str());
}

void HtmlExporter::write_modulo_op_word()
{
    write_operator_style(configurator_->modulo_op_word()->str());
}

void HtmlExporter::write_address_op_word()
{
    write_operator_style(configurator_->address_op_word()->str());
}

void HtmlExporter::write_deref_op_word()
{
    write_operator_style(configurator_->deref_op_word()->str());
}

void HtmlExporter::write_separator_sign(STRING sign)
{
    write_separator_style(std::move(sign));
}

// VALUES

void HtmlExporter::write_int_val(int val)
{
    std::string s = "<span class=\"int-value\">" + std::to_string(std::move(val)) + "</span>";
    write_value_style(std::move(s));
}

void HtmlExporter::write_float_val(float val)
{
    std::string s = "<span class=\"float-value\">" + std::to_string(std::move(val)) + "</span>";
    write_value_style(std::move(s));
}

void HtmlExporter::write_char_val(char val)
{
    std::string s = "<span class=\"char-value\">" + std::string(1, std::move(val)) + "</span>";
    write_value_style(std::move(s));
}

void HtmlExporter::write_string_val(std::string val)
{
    std::string s = "<span class=\"string-value\">" + std::move(val) + "</span>";
    write_value_style(std::move(s));
}

void HtmlExporter::write_bool_val(bool val)
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
    write_value_style(std::move(s));
}

void HtmlExporter::write_null_val()
{
    std::string s = "<span class=\"null-value\">" + configurator_->null_val_word()->str() + "</span>";
    write_value_style(std::move(s));
}

// SYSTEM_EXPRESSIONS

void HtmlExporter::write_class_word()
{
    std::string s = "<span class=\"class-word\">" + configurator_->class_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_interface_word()
{
    std::string s = "<span class=\"interface-word\">" + configurator_->interface_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_implement_word()
{
    std::string s = "<span class=\"implement-word\">" + configurator_->implement_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_extend_word()
{
    std::string s = "<span class=\"extend-word\">" + configurator_->extend_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_this_word()
{
    std::string s = "<span class=\"this-word\">" + configurator_->this_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_return_word()
{
    std::string s = "<span class=\"return-word\">" + configurator_->return_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_continue_word()
{
    std::string s = "<span class=\"continue-word\">" + configurator_->continue_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_break_word()
{
    std::string s = "<span class=\"break-word\">" + configurator_->break_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_throw_word()
{
    std::string s = "<span class=\"throw-word\">" + configurator_->throw_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_if_word()
{
    std::string s = "<span class=\"if-word\">" + configurator_->if_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_else_word()
{
    std::string s = "<span class=\"else-word\">" + configurator_->else_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_do_word()
{
    std::string s = "<span class=\"do-word\">" + configurator_->do_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_while_word()
{
    std::string s = "<span class=\"while-word\">" + configurator_->while_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_for_word()
{
    std::string s = "<span class=\"for-word\">" + configurator_->for_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_repeat_word()
{
    std::string s = "<span class=\"repeat-word\">" + configurator_->repeat_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_switch_word()
{
    std::string s = "<span class=\"switch-word\">" + configurator_->switch_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_case_word()
{
    std::string s = "<span class=\"case-word\">" + configurator_->case_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_default_word()
{
    std::string s = "<span class=\"default-word\">" + configurator_->default_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_new_word()
{
    std::string s = "<span class=\"new-word\">" + configurator_->new_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_delete_word()
{
    std::string s = "<span class=\"delete-word\">" + configurator_->delete_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_pointer_word()
{
    std::string s = "<span class=\"pointer-word\">" + configurator_->pointer_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_virtual_word()
{
    std::string s = "<span class=\"virtual-word\">" + configurator_->virtual_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_abstract_word()
{
    std::string s = "<span class=\"abstract-word\">" + configurator_->abstract_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlExporter::write_template_word()
{
    std::string s = "<span class=\"template-word\">" + configurator_->template_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

// OTHER_EXPRESSIONS

void HtmlExporter::write_constructor_word()
{
    std::string s = "<span class=\"constr-word\">" + configurator_->constructor_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlExporter::write_destructor_word()
{
    std::string s = "<span class=\"destr-word\">" + configurator_->destructor_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlExporter::write_method_word()
{
    std::string s = "<span class=\"method-word\">" + configurator_->method_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlExporter::write_function_word()
{
    std::string s = "<span class=\"function-word\">" + configurator_->function_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlExporter::write_lambda_word()
{
    std::string s = "<span class=\"lambda-word\">" + configurator_->lambda_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlExporter::write_call_word()
{
    std::string s = "<span class=\"call-word\">" + configurator_->call_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlExporter::write_define_word()
{
    std::string s = "<span class=\"define-word\">" + configurator_->define_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlExporter::write_returns_word()
{
    std::string s = "<span class=\"returns-word\">" + configurator_->returns_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}