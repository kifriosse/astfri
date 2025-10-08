#include <libastfri-text/inc/HtmlFileExporter.hpp>

#include <fstream>
#include <cmath>

using namespace astfri::text;

HtmlFileExporter::HtmlFileExporter() : Exporter()
{
    reset();
}

void HtmlFileExporter::reset()
{
    maxBracketIndex_ = configurator_->get_bracket_colors()->size();
    currentBrIndex_ = 0;
    Exporter::reset();
}

void HtmlFileExporter::write_pseudocode_into_file(STRING fullfilepath)
{
    std::ofstream file(fullfilepath);
    file << "<!DOCTYPE html>\n";
    file << "<html lang=\"sk\">\n";
    file << "<head>\n";
    file << "<meta charset=\"UTF-8\">\n";
    file << "<title>" << configurator_->get_output_file_name()->str() << "</title>\n";
    file << "<style>\n";
    // row number
    file << ".row-num{" << configurator_->get_row_num_style()->str() << "}\n";
    // UNKNOWN_PHRASES
    //file << ".unknown-word{" << configurator_->get_unknown_word_style()->str() << "}\n";
    //file << ".invalid-word{" << configurator_->get_invalid_word_style()->str() << "}\n";
    // ACCESS_MODIFIERS
    file << ".access-mod{" << configurator_->get_access_mod_style()->str() << "}\n";
    // DATA_TYPES
    file << ".data-type{" << configurator_->get_default_type_word_style()->str() << "}\n";
    file << ".dynamic-type{" << configurator_->get_dynamic_type_word_style()->str() << "}\n";
    file << ".int-type{" << configurator_->get_int_type_word_style()->str() << "}\n";
    file << ".float-type{" << configurator_->get_float_type_word_style()->str() << "}\n";
    file << ".char-type{" << configurator_->get_char_type_word_style()->str() << "}\n";
    file << ".bool-type{" << configurator_->get_bool_type_word_style()->str() << "}\n";
    file << ".void-type{" << configurator_->get_void_type_word_style()->str() << "}\n";
    file << ".user-type{" << configurator_->get_user_type_style()->str() << "}\n";
    // REFERENCE_NAMES
    file << ".ref-name{" << configurator_->get_default_ref_name_style()->str() << "}\n";
    file << ".gen-par-name{" << configurator_->get_gen_param_name_style()->str() << "}\n";
    file << ".class-name{" << configurator_->get_class_name_style()->str() << "}\n";
    file << ".interface-name{" << configurator_->get_interface_name_style()->str() << "}\n";
    file << ".method-name{" << configurator_->get_method_name_style()->str() << "}\n";
    file << ".function-name{" << configurator_->get_function_name_style()->str() << "}\n";
    file << ".global-var-name{" << configurator_->get_global_var_name_style()->str() << "}\n";
    file << ".member-var-name{" << configurator_->get_member_var_name_style()->str() << "}\n";
    file << ".local-var-name{" << configurator_->get_local_var_name_style()->str() << "}\n";
    file << ".param-var-name{" << configurator_->get_param_var_name_style()->str() << "}\n";
    // OPERATORS & SEPARATORS
    file << ".operator{" << configurator_->get_operator_style()->str() << "}\n";
    file << ".separator{" << configurator_->get_separator_style()->str() << "}\n";
    // VALUES
    file << ".value{" << configurator_->get_default_val_style()->str() << "}\n";
    file << ".int-value{" << configurator_->get_int_val_style()->str() << "}\n";
    file << ".float-value{" << configurator_->get_float_val_style()->str() << "}\n";
    file << ".char-value{" << configurator_->get_char_val_style()->str() << "}\n";
    file << ".string-value{" << configurator_->get_string_val_style()->str() << "}\n";
    file << ".bool-value{" << configurator_->get_bool_val_style()->str() << "}\n";
    file << ".null-value{" << configurator_->get_null_val_style()->str() << "}\n";
    // SYSTEM_EXPRESSIONS
    file << ".system-expr{" << configurator_->get_system_expr_style()->str() << "}\n";
    file << ".class-word{" << configurator_->get_class_word_style()->str() << "}\n";
    file << ".interface-word{" << configurator_->get_interface_word_style()->str() << "}\n";
    file << ".implement-word{" << configurator_->get_implement_word_style()->str() << "}\n";
    file << ".extend-word{" << configurator_->get_extend_word_style()->str() << "}\n";
    file << ".this-word{" << configurator_->get_this_word_style()->str() << "}\n";
    file << ".return-word{" << configurator_->get_return_word_style()->str() << "}\n";
    file << ".continue-word{" << configurator_->get_continue_word_style()->str() << "}\n";
    file << ".break-word{" << configurator_->get_break_word_style()->str() << "}\n";
    file << ".throw-word{" << configurator_->get_throw_word_style()->str() << "}\n";
    file << ".if-word{" << configurator_->get_if_word_style()->str() << "}\n";
    file << ".else-word{" << configurator_->get_else_word_style()->str() << "}\n";
    file << ".do-word{" << configurator_->get_do_word_style()->str() << "}\n";
    file << ".while-word{" << configurator_->get_while_word_style()->str() << "}\n";
    file << ".for-word{" << configurator_->get_for_word_style()->str() << "}\n";
    file << ".repeat-word{" << configurator_->get_repeat_word_style()->str() << "}\n";
    file << ".switch-word{" << configurator_->get_switch_word_style()->str() << "}\n";
    file << ".case-word{" << configurator_->get_case_word_style()->str() << "}\n";
    file << ".default-word{" << configurator_->get_default_word_style()->str() << "}\n";
    file << ".new-word{" << configurator_->get_new_word_style()->str() << "}\n";
    file << ".delete-word{" << configurator_->get_delete_word_style()->str() << "}\n";
    file << ".pointer-word{" << configurator_->get_pointer_word_style()->str() << "}\n";
    file << ".virtual-word{" << configurator_->get_virtual_word_style()->str() << "}\n";
    file << ".abstract-word{" << configurator_->get_abstract_word_style()->str() << "}\n";
    file << ".template-word{" << configurator_->get_template_word_style()->str() << "}\n";
    // OTHER_EXPRESSIONS
    file << ".other-expr{" << configurator_->get_other_expression_style()->str() << "}\n";
    file << ".constr-word{" << configurator_->get_constructor_word_style()->str() << "}\n";
    file << ".destr-word{" << configurator_->get_destructor_word_style()->str() << "}\n";
    file << ".method-word{" << configurator_->get_method_word_style()->str() << "}\n";
    file << ".function-word{" << configurator_->get_function_word_style()->str() << "}\n";
    file << ".lambda-word{" << configurator_->get_lambda_word_style()->str() << "}\n";
    file << ".call-word{" << configurator_->get_call_word_style()->str() << "}\n";
    file << ".define-word{" << configurator_->get_define_word_style()->str() << "}\n";
    file << ".returns-word{" << configurator_->get_returns_word_style()->str() << "}\n";
    file << "</style>\n";
    file << "</head>\n";
    file << "<body style=" << configurator_->get_default_text_style()->str() << ">\n";
    if (configurator_->sh_row_num())
    {
        int row = 1;
        int delimiter = static_cast<int>(std::log10(rowCount_)) + 1;
        std::string line;
        while (std::getline(*outputPseudocode_,line))
        {
            if (line != "<br>" || (line == "<br>" && configurator_->sh_row_num_empty_row()))
            {
                for (int i = 0; i < configurator_->get_row_num_margin_left_len(); ++i)
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

void HtmlFileExporter::write_acc_mod_style(const std::string& accmod)
{
    write_text("<span class=\"access-mod\">");
    write_text(std::move(accmod));
    write_text("</span>");
}

void HtmlFileExporter::write_data_type_style(const std::string& datatype)
{
    write_text("<span class=\"data-type\">");
    write_text(std::move(datatype));
    write_text("</span>");
}

void HtmlFileExporter::write_ref_name_style(const std::string& name)
{
    write_text("<span class=\"ref-name\">");
    write_text(std::move(name));
    write_text("</span>");
}

void HtmlFileExporter::write_operator_style(const std::string& op)
{
    write_text("<span class=\"operator\">");
    write_text(std::move(op));
    write_text("</span>");
}

void HtmlFileExporter::write_separator_style(const std::string& sep)
{
    write_text("<span class=\"separator\">");
    write_text(std::move(sep));
    write_text("</span>");
}

void HtmlFileExporter::write_value_style(const std::string& val)
{
    write_text("<span class=\"value\">");
    write_text(std::move(val));
    write_text("</span>");
}

void HtmlFileExporter::write_system_expr_style(const std::string& expr)
{
    write_text("<span class=\"system-expr\">");
    write_text(std::move(expr));
    write_text("</span>");
}

void HtmlFileExporter::write_other_expr_style(const std::string& expr)
{
    write_text("<span class=\"other-expr\">");
    write_text(std::move(expr));
    write_text("</span>");
}

// BRACKETS

void HtmlFileExporter::write_left_bracket(STRING br)
{
    if (configurator_->use_bracket_colors() && maxBracketIndex_ > 0)
    {
        std::string out = "<span style=\"color:";
        out.append(configurator_->get_bracket_colors()->at(currentBrIndex_).str());
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

void HtmlFileExporter::write_right_bracket(STRING br)
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
        out.append(configurator_->get_bracket_colors()->at(currentBrIndex_).str());
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

void HtmlFileExporter::write_unknown_type_word()
{
    std::string s = "<span class=\"unknown-type-word\">" + configurator_->get_unknown_type_word_style()->str() + "</span>";
    write_text(std::move(s));
}

void HtmlFileExporter::write_unknown_expr_word()
{
    std::string s = "<span class=\"unknown-expr-word\">" + configurator_->get_unknown_expr_word_style()->str() + "</span>";
    write_text(std::move(s));
}

void HtmlFileExporter::write_unknown_stmt_word()
{
    std::string s = "<span class=\"unknown-stmt-word\">" + configurator_->get_unknown_stmt_word_style()->str() + "</span>";
    write_text(std::move(s));
}

void HtmlFileExporter::write_invalid_type_word()
{
    std::string s = "<span class=\"invalid-type-word\">" + configurator_->get_invalid_type_word_style()->str() + "</span>";
    write_text(std::move(s));
}

void HtmlFileExporter::write_invalid_expr_word()
{
    std::string s = "<span class=\"invalid-expr-word\">" + configurator_->get_invalid_expr_word_style()->str() + "</span>";
    write_text(std::move(s));
}

void HtmlFileExporter::write_invalid_stmt_word()
{
    std::string s = "<span class=\"invalid-stmt-word\">" + configurator_->get_invalid_stmt_word_style()->str() + "</span>";
    write_text(std::move(s));
}

// ACCESS_MODIFIERS

void HtmlFileExporter::write_public_word()
{
    write_acc_mod_style(configurator_->get_public_word()->str());
}

void HtmlFileExporter::write_protected_word()
{
    write_acc_mod_style(configurator_->get_protected_word()->str());
}

void HtmlFileExporter::write_private_word()
{
    write_acc_mod_style(configurator_->get_private_word()->str());
}

void HtmlFileExporter::write_internal_word()
{
    write_acc_mod_style(configurator_->get_internal_word()->str());
}

void HtmlFileExporter::write_attributes_word()
{
    write_acc_mod_style(configurator_->get_attributes_word()->str());
}

void HtmlFileExporter::write_constructors_word()
{
    write_acc_mod_style(configurator_->get_constructors_word()->str());
}

void HtmlFileExporter::write_destructors_word()
{
    write_acc_mod_style(configurator_->get_destructors_word()->str());
}

void HtmlFileExporter::write_methods_word()
{
    write_acc_mod_style(configurator_->get_methods_word()->str());
}

// DATA_TYPES

void HtmlFileExporter::write_dynamic_type_word()
{
    std::string s = "<span class=\"dynamic-word\">" + configurator_->get_dynamic_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_int_type_word()
{
    std::string s = "<span class=\"int-word\">" + configurator_->get_int_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_float_type_word()
{
    std::string s = "<span class=\"float-word\">" + configurator_->get_float_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_char_type_word()
{
    std::string s = "<span class=\"char-word\">" + configurator_->get_char_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_bool_type_word()
{
    std::string s = "<span class=\"bool-word\">" + configurator_->get_bool_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_void_type_word()
{
    std::string s = "<span class=\"void-word\">" + configurator_->get_void_type_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_user_type(std::string usertype)
{
    std::string s = "<span class=\"user-type\">" + std::move(usertype) + "</span>";
    write_data_type_style(std::move(s));
}

// REFERENCE_NAMES

void HtmlFileExporter::write_gen_param_name(std::string name)
{
    std::string s = "<span class=\"gen-par-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_class_name(std::string name)
{
    std::string s = "<span class=\"class-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_interface_name(std::string name)
{
    std::string s = "<span class=\"interface-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_method_name(std::string name)
{
    std::string s = "<span class=\"method-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_function_name(std::string name)
{
    std::string s = "<span class=\"function-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_global_var_name(std::string name)
{
    std::string s = "<span class=\"global-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_member_var_name(std::string name)
{
    std::string s = "<span class=\"member-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_local_var_name(std::string name)
{
    std::string s = "<span class=\"local-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_param_var_name(std::string name)
{
    std::string s = "<span class=\"param-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

// SIGNS

void HtmlFileExporter::write_operator_sign(STRING sign)
{
    write_operator_style(std::move(sign));
}

void HtmlFileExporter::write_assign_op_word()
{
    write_operator_style(configurator_->get_assign_op_word()->str());
}

void HtmlFileExporter::write_modulo_op_word()
{
    write_operator_style(configurator_->get_modulo_op_word()->str());
}

void HtmlFileExporter::write_address_op_word()
{
    write_operator_style(configurator_->get_address_op_word()->str());
}

void HtmlFileExporter::write_deref_op_word()
{
    write_operator_style(configurator_->get_deref_op_word()->str());
}

void HtmlFileExporter::write_separator_sign(STRING sign)
{
    write_separator_style(std::move(sign));
}

// VALUES

void HtmlFileExporter::write_int_val(int val)
{
    std::string s = "<span class=\"int-value\">" + std::to_string(std::move(val)) + "</span>";
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_float_val(float val)
{
    std::string s = "<span class=\"float-value\">" + std::to_string(std::move(val)) + "</span>";
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_char_val(char val)
{
    std::string s = "<span class=\"char-value\">" + std::string(1, std::move(val)) + "</span>";
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_string_val(std::string val)
{
    std::string s = "<span class=\"string-value\">" + std::move(val) + "</span>";
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_bool_val(bool val)
{
    std::string s = "<span class=\"bool-value\">";
    if (val)
    {
        s.append(std::move(configurator_->get_true_val_word()->str()));
    }
    else
    {
        s.append(std::move(configurator_->get_false_val_word()->str()));
    }
    s.append("</span>");
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_null_val()
{
    std::string s = "<span class=\"null-value\">" + configurator_->get_null_val_word()->str() + "</span>";
    write_value_style(std::move(s));
}

// SYSTEM_EXPRESSIONS

void HtmlFileExporter::write_class_word()
{
    std::string s = "<span class=\"class-word\">" + configurator_->get_class_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_interface_word()
{
    std::string s = "<span class=\"interface-word\">" + configurator_->get_interface_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_implement_word()
{
    std::string s = "<span class=\"implement-word\">" + configurator_->get_implement_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_extend_word()
{
    std::string s = "<span class=\"extend-word\">" + configurator_->get_extend_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_this_word()
{
    std::string s = "<span class=\"this-word\">" + configurator_->get_this_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_return_word()
{
    std::string s = "<span class=\"return-word\">" + configurator_->get_return_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_continue_word()
{
    std::string s = "<span class=\"continue-word\">" + configurator_->get_continue_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_break_word()
{
    std::string s = "<span class=\"break-word\">" + configurator_->get_break_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_throw_word()
{
    std::string s = "<span class=\"throw-word\">" + configurator_->get_throw_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_if_word()
{
    std::string s = "<span class=\"if-word\">" + configurator_->get_if_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_else_word()
{
    std::string s = "<span class=\"else-word\">" + configurator_->get_else_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_do_word()
{
    std::string s = "<span class=\"do-word\">" + configurator_->get_do_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_while_word()
{
    std::string s = "<span class=\"while-word\">" + configurator_->get_while_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_for_word()
{
    std::string s = "<span class=\"for-word\">" + configurator_->get_for_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_repeat_word()
{
    std::string s = "<span class=\"repeat-word\">" + configurator_->get_repeat_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_switch_word()
{
    std::string s = "<span class=\"switch-word\">" + configurator_->get_switch_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_case_word()
{
    std::string s = "<span class=\"case-word\">" + configurator_->get_case_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_default_word()
{
    std::string s = "<span class=\"default-word\">" + configurator_->get_default_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_new_word()
{
    std::string s = "<span class=\"new-word\">" + configurator_->get_new_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_delete_word()
{
    std::string s = "<span class=\"delete-word\">" + configurator_->get_delete_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_pointer_word()
{
    std::string s = "<span class=\"pointer-word\">" + configurator_->get_pointer_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_virtual_word()
{
    std::string s = "<span class=\"virtual-word\">" + configurator_->get_virtual_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_abstract_word()
{
    std::string s = "<span class=\"abstract-word\">" + configurator_->get_abstract_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_template_word()
{
    std::string s = "<span class=\"template-word\">" + configurator_->get_template_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

// OTHER_EXPRESSIONS

void HtmlFileExporter::write_constructor_word()
{
    std::string s = "<span class=\"constr-word\">" + configurator_->get_constructor_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_destructor_word()
{
    std::string s = "<span class=\"destr-word\">" + configurator_->get_destructor_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_method_word()
{
    std::string s = "<span class=\"method-word\">" + configurator_->get_method_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_function_word()
{
    std::string s = "<span class=\"function-word\">" + configurator_->get_function_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_lambda_word()
{
    std::string s = "<span class=\"lambda-word\">" + configurator_->get_lambda_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_call_word()
{
    std::string s = "<span class=\"call-word\">" + configurator_->get_call_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_define_word()
{
    std::string s = "<span class=\"define-word\">" + configurator_->get_define_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_returns_word()
{
    std::string s = "<span class=\"returns-word\">" + configurator_->get_returns_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}