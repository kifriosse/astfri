#include <libastfri-text/inc/HtmlFileExporter.hpp>
#include <iostream>
#include <fstream>

HtmlFileExporter::HtmlFileExporter(std::shared_ptr<TextConfigurator> conf) : Exporter(std::move(conf)) {
    maxRoundBrIndex_ = config_->get_round_br_col()->size();
    maxCurlBrIndex_ = config_->get_curl_br_col()->size();
    roundBrIndex_ = 0;
    curlBrIndex_ = 0;
}

void HtmlFileExporter::make_export() { create_folder(".html"); }

void HtmlFileExporter::write_output_into_file(const std::string& filepath) {
    std::cout << "Súbor nájdeš na ceste: " << filepath << "\n";
    std::ofstream file(std::move(filepath));
    file << "<!DOCTYPE html>\n";
    file << "<html lang=\"sk\">\n";
    file << "<head>\n";
    file << "<meta charset=\"UTF-8\">\n";
    file << "<title>" << config_->get_output_file_name()->str() << "</title>\n";
    file << "<style>\n";
    file << ".unknown-word{" << config_->get_unknown_word_style()->str() << "}\n";
    file << ".invalid-word{" << config_->get_invalid_word_style()->str() << "}\n";
    file << ".access-mod{" << config_->get_acc_style()->str() << "}\n";
    file << ".data-type{" << config_->get_type_style()->str() << "}\n";
    file << ".dynamic-type{" << config_->get_dynamic_type_style()->str() << "}\n";
    file << ".int-type{" << config_->get_int_type_style()->str() << "}\n";
    file << ".float-type{" << config_->get_float_type_style()->str() << "}\n";
    file << ".char-type{" << config_->get_char_type_style()->str() << "}\n";
    file << ".bool-type{" << config_->get_bool_type_style()->str() << "}\n";
    file << ".void-type{" << config_->get_void_type_style()->str() << "}\n";
    file << ".user-type{" << config_->get_user_type_style()->str() << "}\n";
    file << ".ref-name{" << config_->get_def_ref_name_style()->str() << "}\n";
    file << ".gen-par-name{" << config_->get_gener_param_name_style()->str() << "}\n";
    file << ".class-name{" << config_->get_class_name_style()->str() << "}\n";
    file << ".interface-name{" << config_->get_interface_name_style()->str() << "}\n";
    file << ".method-name{" << config_->get_method_name_style()->str() << "}\n";
    file << ".function-name{" << config_->get_function_name_style()->str() << "}\n";
    file << ".global-var-name{" << config_->get_global_var_name_style()->str() << "}\n";
    file << ".member-var-name{" << config_->get_member_var_name_style()->str() << "}\n";
    file << ".local-var-name{" << config_->get_local_var_name_style()->str() << "}\n";
    file << ".param-var-name{" << config_->get_param_var_name_style()->str() << "}\n";
    file << ".operator{" << config_->get_operator_style()->str() << "}\n";
    file << ".separator{" << config_->get_separator_style()->str() << "}\n";
    file << ".value{" << config_->get_val_style()->str() << "}\n";
    file << ".int-value{" << config_->get_int_val_style()->str() << "}\n";
    file << ".float-value{" << config_->get_float_val_style()->str() << "}\n";
    file << ".char-value{" << config_->get_char_val_style()->str() << "}\n";
    file << ".string-value{" << config_->get_string_val_style()->str() << "}\n";
    file << ".bool-value{" << config_->get_bool_val_style()->str() << "}\n";
    file << ".null-value{" << config_->get_null_val_style()->str() << "}\n";
    file << ".row-num{" << config_->get_row_style()->str() << "}\n";
    file << ".system-expr{" << config_->get_sys_ex_style()->str() << "}\n";
    file << ".class-word{" << config_->get_class_word_style()->str() << "}\n";
    file << ".interface-word{" << config_->get_interface_word_style()->str() << "}\n";
    file << ".implement-word{" << config_->get_implement_word_style()->str() << "}\n";
    file << ".extend-word{" << config_->get_extend_word_style()->str() << "}\n";
    file << ".this-word{" << config_->get_this_word_style()->str() << "}\n";
    file << ".return-word{" << config_->get_return_word_style()->str() << "}\n";
    file << ".continue-word{" << config_->get_continue_word_style()->str() << "}\n";
    file << ".break-word{" << config_->get_break_word_style()->str() << "}\n";
    file << ".throw-word{" << config_->get_throw_word_style()->str() << "}\n";
    file << ".if-word{" << config_->get_if_word_style()->str() << "}\n";
    file << ".else-word{" << config_->get_else_word_style()->str() << "}\n";
    file << ".do-word{" << config_->get_do_word_style()->str() << "}\n";
    file << ".while-word{" << config_->get_while_word_style()->str() << "}\n";
    file << ".for-word{" << config_->get_for_word_style()->str() << "}\n";
    file << ".repeat-word{" << config_->get_repeat_word_style()->str() << "}\n";
    file << ".switch-word{" << config_->get_switch_word_style()->str() << "}\n";
    file << ".case-word{" << config_->get_case_word_style()->str() << "}\n";
    file << ".default-word{" << config_->get_default_word_style()->str() << "}\n";
    file << ".new-word{" << config_->get_new_word_style()->str() << "}\n";
    file << ".delete-word{" << config_->get_delete_word_style()->str() << "}\n";
    file << ".pointer-word{" << config_->get_pointer_word_style()->str() << "}\n";
    file << ".virtual-word{" << config_->get_virtual_word_style()->str() << "}\n";
    file << ".abstract-word{" << config_->get_abstract_word_style()->str() << "}\n";
    file << ".template-word{" << config_->get_template_word_style()->str() << "}\n";
    file << ".other-expr{" << config_->get_other_expr_style()->str() << "}\n";
    file << ".constr-word{" << config_->get_constr_style()->str() << "}\n";
    file << ".destr-word{" << config_->get_destr_style()->str() << "}\n";
    file << ".method-word{" << config_->get_method_word_style()->str() << "}\n";
    file << ".function-word{" << config_->get_function_word_style()->str() << "}\n";
    file << ".lambda-word{" << config_->get_lambda_word_style()->str() << "}\n";
    file << ".call-word{" << config_->get_call_word_style()->str() << "}\n";
    file << ".define-word{" << config_->get_define_word_style()->str() << "}\n";
    file << ".returns-word{" << config_->get_returns_word_style()->str() << "}\n";
    file << "</style>\n";
    file << "</head>\n";
    file << "<body style=" << config_->get_default_style()->str() << ">\n";
    file << output_->str();
    file << "</body>\n";
    file << "</html>";
    file.close();
    std::cout << "Zápis prebehol úspešne!\n";
}

void HtmlFileExporter::write_indentation() {
    startedLine_ = true;
    config_->sh_row() ? write_row_number() : void();
    for (int i = 0; i < config_->get_marg_len(); ++i) {
        write_space();
    }
    for (int i = 0; i < currentIndentation_; ++i) {
        for (int j = 0; j < config_->get_tab_len(); ++j) {
            write_space();
        }
    }
}

void HtmlFileExporter::write_row_number() {
    write_word("<span class=\"row-num\">");
    if (row_ > 999) {
        *output_ << row_ << ".";
    } else if (row_ > 99) {
        *output_ << "&nbsp;" << row_ << ".";
    } else if (row_ > 9) {
        *output_ << "&nbsp;&nbsp;" << row_ << ".";
    } else {
        *output_ << "&nbsp;&nbsp;&nbsp;" << row_ << ".";
    }
    write_word("</span>");
}

void HtmlFileExporter::write_new_line() {
    write_word("<br>");
    Exporter::write_new_line();
}

void HtmlFileExporter::write_space() {
    write_word("&nbsp;");
}

void HtmlFileExporter::write_round_bracket(const std::string& br) {
    if (config_->use_br_col() && maxRoundBrIndex_ > 0) {
        std::string out = "<span style=\"color:";
        if (br == "(") {
            out.append(config_->get_round_br_col()->at(roundBrIndex_).str());
            out.append("\">(</span>");
            ++roundBrIndex_;
            if (roundBrIndex_ == maxRoundBrIndex_) {
                roundBrIndex_ = 0;
            }
        } else {
            if (roundBrIndex_ == 0) {
                roundBrIndex_ = maxRoundBrIndex_ - 1;
            } else {
                --roundBrIndex_;
            }
            out.append(config_->get_round_br_col()->at(roundBrIndex_).str());
            out.append("\">)</span>");
        }
        write_word(std::move(out));
    } else {
        write_word(std::move(br));
    }
}

void HtmlFileExporter::write_curl_bracket(const std::string& br) {
    if (config_->use_br_col() && maxCurlBrIndex_ > 0) {
        std::string out = "<span style=\"color:";
        if (br == "{") {
            out.append(config_->get_curl_br_col()->at(curlBrIndex_).str());
            out.append("\">{</span>");
            ++curlBrIndex_;
            if (curlBrIndex_ == maxCurlBrIndex_) {
                curlBrIndex_ = 0;
            }
        } else {
            if (curlBrIndex_ == 0) {
                curlBrIndex_ = maxCurlBrIndex_ - 1;
            } else {
                --curlBrIndex_;
            }
            out.append(config_->get_curl_br_col()->at(curlBrIndex_).str());
            out.append("\">}</span>");
        }
        write_word(std::move(out));
    } else {
        write_word(std::move(br));
    }
}

void HtmlFileExporter::write_unknown_word() {
    std::string s = "<span class=\"unknown-word\">" + config_->get_unknown_word()->str() + "</span>";
    write_word(std::move(s));
}

void HtmlFileExporter::write_invalid_word() {
    std::string s = "<span class=\"invalid-word\">" + config_->get_invalid_word()->str() + "</span>";
    write_word(std::move(s));
}

void HtmlFileExporter::write_public_word() {
    write_acc_mod_style(config_->get_public_word()->str());
}

void HtmlFileExporter::write_private_word() {
    write_acc_mod_style(config_->get_private_word()->str());
}

void HtmlFileExporter::write_protected_word() {
    write_acc_mod_style(config_->get_protected_word()->str());
}

void HtmlFileExporter::write_internal_word() {
    write_acc_mod_style(config_->get_internal_word()->str());
}

void HtmlFileExporter::write_attribs_word() {
    write_acc_mod_style(config_->get_acc_atrib_word()->str());
}

void HtmlFileExporter::write_constrs_word() {
    write_acc_mod_style(config_->get_acc_constr_word()->str());
}

void HtmlFileExporter::write_destrs_word() {
    write_acc_mod_style(config_->get_acc_destr_word()->str());
}

void HtmlFileExporter::write_meths_word() {
    write_acc_mod_style(config_->get_acc_meth_word()->str());
}

void HtmlFileExporter::write_dynamic_type() {
    std::string s = "<span class=\"dynamic-type\">" + config_->get_dynamic_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_int_type() {
    std::string s = "<span class=\"int-type\">" + config_->get_int_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_float_type() {
    std::string s = "<span class=\"float-type\">" + config_->get_float_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_char_type() {
    std::string s = "<span class=\"char-type\">" + config_->get_char_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_bool_type() {
    std::string s = "<span class=\"bool-type\">" + config_->get_bool_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_void_type() {
    std::string s = "<span class=\"void-type\">" + config_->get_void_word()->str() + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_user_type(std::string usertype) {
    std::string s = "<span class=\"user-type\">" + std::move(usertype) + "</span>";
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_gen_param_name(std::string name) {
    std::string s = "<span class=\"gen-par-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_class_name(std::string name) {
    std::string s = "<span class=\"class-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_interface_name(std::string name) {
    std::string s = "<span class=\"interface-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_method_name(std::string name) {
    std::string s = "<span class=\"method-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_function_name(std::string name) {
    std::string s = "<span class=\"function-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_global_var_name(std::string name) {
    std::string s = "<span class=\"global-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_member_var_name(std::string name) {
    std::string s = "<span class=\"member-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_local_var_name(std::string name) {
    std::string s = "<span class=\"local-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_param_var_name(std::string name) {
    std::string s = "<span class=\"param-var-name\">" + std::move(name) + "</span>";
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_operator_sign(const std::string& sign) {
    write_operator_style(std::move(sign));
}

void HtmlFileExporter::write_assign_op_word() {
    write_operator_style(config_->get_assign_op_word()->str());
}

void HtmlFileExporter::write_modulo_op_word() {
    write_operator_style(config_->get_modulo_op_word()->str());
}

void HtmlFileExporter::write_address_op_word() {
    write_operator_style(config_->get_address_op_word()->str());
}

void HtmlFileExporter::write_deref_op_word() {
    write_operator_style(config_->get_deref_op_word()->str());
}

void HtmlFileExporter::write_separator_sign(const std::string& sign) {
    write_separator_style(std::move(sign));
}

void HtmlFileExporter::write_int_val(int val) {
    std::string s = "<span class=\"int-value\">" + std::to_string(std::move(val)) + "</span>";
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_float_val(float val) {
    std::string s = "<span class=\"float-value\">" + std::to_string(std::move(val)) + "</span>";
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_char_val(char val) {
    std::string s = "<span class=\"char-value\">" + std::string(1, std::move(val)) + "</span>";
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_string_val(std::string val) {
    std::string s = "<span class=\"string-value\">" + std::move(val) + "</span>";
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_bool_val(bool val) {
    std::string s = "<span class=\"bool-value\">";
    val ? s.append(std::move(config_->get_true_val()->str())) : s.append(std::move(config_->get_false_val()->str()));
    s.append("</span>");
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_null_val() {
    std::string s = "<span class=\"null-value\">" + config_->get_null_val()->str() + "</span>";
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_class_word() {
    std::string s = "<span class=\"class-word\">" + config_->get_class_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_interface_word() {
    std::string s = "<span class=\"interface-word\">" + config_->get_interface_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_implement_word() {
    std::string s = "<span class=\"implement-word\">" + config_->get_implement_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_extend_word() {
    std::string s = "<span class=\"extend-word\">" + config_->get_extend_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_this_word() {
    std::string s = "<span class=\"this-word\">" + config_->get_this_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_return_word() {
    std::string s = "<span class=\"return-word\">" + config_->get_return_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_continue_word() {
    std::string s = "<span class=\"continue-word\">" + config_->get_continue_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_break_word() {
    std::string s = "<span class=\"break-word\">" + config_->get_break_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_throw_word() {
    std::string s = "<span class=\"throw-word\">" + config_->get_throw_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_if_word() {
    std::string s = "<span class=\"if-word\">" + config_->get_if_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_else_word() {
    std::string s = "<span class=\"else-word\">" + config_->get_else_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_do_word() {
    std::string s = "<span class=\"do-word\">" + config_->get_do_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_while_word() {
    std::string s = "<span class=\"while-word\">" + config_->get_while_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_for_word() {
    std::string s = "<span class=\"for-word\">" + config_->get_for_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_repeat_word() {
    std::string s = "<span class=\"repeat-word\">" + config_->get_repeat_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_switch_word() {
    std::string s = "<span class=\"switch-word\">" + config_->get_switch_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_case_word() {
    std::string s = "<span class=\"case-word\">" + config_->get_case_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_default_word() {
    std::string s = "<span class=\"default-word\">" + config_->get_default_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_new_word() {
    std::string s = "<span class=\"new-word\">" + config_->get_new_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_delete_word() {
    std::string s = "<span class=\"delete-word\">" + config_->get_delete_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_pointer_word() {
    std::string s = "<span class=\"pointer-word\">" + config_->get_pointer_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_virtual_word() {
    std::string s = "<span class=\"virtual-word\">" + config_->get_virtual_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_abstract_word() {
    std::string s = "<span class=\"abstract-word\">" + config_->get_abstract_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_template_word() {
    std::string s = "<span class=\"template-word\">" + config_->get_template_word()->str() + "</span>";
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_constr_word() {
    std::string s = "<span class=\"constr-word\">" + config_->get_constr_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_destr_word() {
    std::string s = "<span class=\"destr-word\">" + config_->get_destr_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_method_word() {
    std::string s = "<span class=\"method-word\">" + config_->get_method_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_function_word() {
    std::string s = "<span class=\"function-word\">" + config_->get_function_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_lambda_word() {
    std::string s = "<span class=\"lambda-word\">" + config_->get_lambda_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_call_word() {
    std::string s = "<span class=\"call-word\">" + config_->get_call_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_define_word() {
    std::string s = "<span class=\"define-word\">" + config_->get_define_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_returns_word() {
    std::string s = "<span class=\"returns-word\">" + config_->get_returns_word()->str() + "</span>";
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_acc_mod_style(const std::string& accmod) {
    write_word("<span class=\"access-mod\">");
    write_word(std::move(accmod));
    write_word("</span>");
}

void HtmlFileExporter::write_data_type_style(const std::string& datatype) {
    write_word("<span class=\"data-type\">");
    write_word(std::move(datatype));
    write_word("</span>");
}

void HtmlFileExporter::write_ref_name_style(const std::string& name) {
    write_word("<span class=\"ref-name\">");
    write_word(std::move(name));
    write_word("</span>");
}

void HtmlFileExporter::write_operator_style(const std::string& op) {
    write_word("<span class=\"operator\">");
    write_word(std::move(op));
    write_word("</span>");
}

void HtmlFileExporter::write_separator_style(const std::string& sep) {
    write_word("<span class=\"separator\">");
    write_word(std::move(sep));
    write_word("</span>");
}

void HtmlFileExporter::write_value_style(const std::string& val) {
    write_word("<span class=\"value\">");
    write_word(std::move(val));
    write_word("</span>");
}

void HtmlFileExporter::write_system_expr_style(const std::string& expr) {
    write_word("<span class=\"system-expr\">");
    write_word(std::move(expr));
    write_word("</span>");
}

void HtmlFileExporter::write_other_expr_style(const std::string& expr) {
    write_word("<span class=\"other-expr\">");
    write_word(std::move(expr));
    write_word("</span>");
}