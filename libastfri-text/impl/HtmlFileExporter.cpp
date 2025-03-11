#include <libastfri-text/inc/HtmlFileExporter.hpp>
#include <iostream>
#include <fstream>

HtmlFileExporter::HtmlFileExporter(std::shared_ptr<TextConfigurator> conf) : Exporter(std::move(conf)) {
    maxRoundBrIndex_ = config_->get_round_br_col()->size();
    maxCurlBrIndex_ = config_->get_curl_br_col()->size();
    roundBrIndex_ = 0;
    curlBrIndex_ = 0;
}

void HtmlFileExporter::make_export() { check_output_file_path(std::move(".html")); }

void HtmlFileExporter::write_output_into_file(std::string filepath) {
    std::cout << std::move("Súbor nájdeš na ceste: ") << filepath << std::move("\n");
    size_t pos = std::move(filepath.find_last_of(std::move("/")));
    std::string title = filepath.substr(std::move(pos + 1));
    std::ofstream file(std::move(filepath));
    file << std::move("<!DOCTYPE html>\n");
    file << std::move("<html lang=\"sk\">\n");
    file << std::move("<head>\n");
    file << std::move("<meta charset=\"UTF-8\">\n");
    file << std::move("<title>") << std::move(title) << std::move("</title>\n");
    file << std::move("<style>\n");
    file << std::move(".unknown-word{") << config_->get_unknown_word_style()->str() << std::move("}\n");
    file << std::move(".invalid-word{") << config_->get_invalid_word_style()->str() << std::move("}\n");
    file << std::move(".access-mod{") << config_->get_acc_style()->str() << std::move("}\n");
    file << std::move(".data-type{") << config_->get_type_style()->str() << std::move("}\n");
    file << std::move(".dynamic-type{") << config_->get_dynamic_type_style()->str() << std::move("}\n");
    file << std::move(".int-type{") << config_->get_int_type_style()->str() << std::move("}\n");
    file << std::move(".float-type{") << config_->get_float_type_style()->str() << std::move("}\n");
    file << std::move(".char-type{") << config_->get_char_type_style()->str() << std::move("}\n");
    file << std::move(".bool-type{") << config_->get_bool_type_style()->str() << std::move("}\n");
    file << std::move(".void-type{") << config_->get_void_type_style()->str() << std::move("}\n");
    file << std::move(".user-type{") << config_->get_user_type_style()->str() << std::move("}\n");
    file << std::move(".ref-name{") << config_->get_def_ref_name_style()->str() << std::move("}\n");
    file << std::move(".gen-par-name{") << config_->get_gener_param_name_style()->str() << std::move("}\n");
    file << std::move(".class-name{") << config_->get_class_name_style()->str() << std::move("}\n");
    file << std::move(".interface-name{") << config_->get_interface_name_style()->str() << std::move("}\n");
    file << std::move(".method-name{") << config_->get_method_name_style()->str() << std::move("}\n");
    file << std::move(".function-name{") << config_->get_function_name_style()->str() << std::move("}\n");
    file << std::move(".global-var-name{") << config_->get_global_var_name_style()->str() << std::move("}\n");
    file << std::move(".member-var-name{") << config_->get_member_var_name_style()->str() << std::move("}\n");
    file << std::move(".local-var-name{") << config_->get_local_var_name_style()->str() << std::move("}\n");
    file << std::move(".param-var-name{") << config_->get_param_var_name_style()->str() << std::move("}\n");
    file << std::move(".operator{") << config_->get_operator_style()->str() << std::move("}\n");
    file << std::move(".separator{") << config_->get_separator_style()->str() << std::move("}\n");
    file << std::move(".value{") << config_->get_val_style()->str() << std::move("}\n");
    file << std::move(".int-value{") << config_->get_int_val_style()->str() << std::move("}\n");
    file << std::move(".float-value{") << config_->get_float_val_style()->str() << std::move("}\n");
    file << std::move(".char-value{") << config_->get_char_val_style()->str() << std::move("}\n");
    file << std::move(".string-value{") << config_->get_string_val_style()->str() << std::move("}\n");
    file << std::move(".bool-value{") << config_->get_bool_val_style()->str() << std::move("}\n");
    file << std::move(".null-value{") << config_->get_null_val_style()->str() << std::move("}\n");
    file << std::move(".row-num{") << config_->get_row_style()->str() << std::move("}\n");
    file << std::move(".system-expr{") << config_->get_sys_ex_style()->str() << std::move("}\n");
    file << std::move(".class-word{") << config_->get_class_word_style()->str() << std::move("}\n");
    file << std::move(".interface-word{") << config_->get_interface_word_style()->str() << std::move("}\n");
    file << std::move(".implement-word{") << config_->get_implement_word_style()->str() << std::move("}\n");
    file << std::move(".extend-word{") << config_->get_extend_word_style()->str() << std::move("}\n");
    file << std::move(".this-word{") << config_->get_this_word_style()->str() << std::move("}\n");
    file << std::move(".return-word{") << config_->get_return_word_style()->str() << std::move("}\n");
    file << std::move(".continue-word{") << config_->get_continue_word_style()->str() << std::move("}\n");
    file << std::move(".break-word{") << config_->get_break_word_style()->str() << std::move("}\n");
    file << std::move(".throw-word{") << config_->get_throw_word_style()->str() << std::move("}\n");
    file << std::move(".if-word{") << config_->get_if_word_style()->str() << std::move("}\n");
    file << std::move(".else-word{") << config_->get_else_word_style()->str() << std::move("}\n");
    file << std::move(".do-word{") << config_->get_do_word_style()->str() << std::move("}\n");
    file << std::move(".while-word{") << config_->get_while_word_style()->str() << std::move("}\n");
    file << std::move(".for-word{") << config_->get_for_word_style()->str() << std::move("}\n");
    file << std::move(".repeat-word{") << config_->get_repeat_word_style()->str() << std::move("}\n");
    file << std::move(".switch-word{") << config_->get_switch_word_style()->str() << std::move("}\n");
    file << std::move(".case-word{") << config_->get_case_word_style()->str() << std::move("}\n");
    file << std::move(".default-word{") << config_->get_default_word_style()->str() << std::move("}\n");
    file << std::move(".new-word{") << config_->get_new_word_style()->str() << std::move("}\n");
    file << std::move(".delete-word{") << config_->get_delete_word_style()->str() << std::move("}\n");
    file << std::move(".pointer-word{") << config_->get_pointer_word_style()->str() << std::move("}\n");
    file << std::move(".virtual-word{") << config_->get_virtual_word_style()->str() << std::move("}\n");
    file << std::move(".other-expr{") << config_->get_other_expr_style()->str() << std::move("}\n");
    file << std::move(".constr-word{") << config_->get_constr_style()->str() << std::move("}\n");
    file << std::move(".destr-word{") << config_->get_destr_style()->str() << std::move("}\n");
    file << std::move(".method-word{") << config_->get_method_word_style()->str() << std::move("}\n");
    file << std::move(".function-word{") << config_->get_function_word_style()->str() << std::move("}\n");
    file << std::move(".lambda-word{") << config_->get_lambda_word_style()->str() << std::move("}\n");
    file << std::move(".call-word{") << config_->get_call_word_style()->str() << std::move("}\n");
    file << std::move(".define-word{") << config_->get_define_word_style()->str() << std::move("}\n");
    file << std::move(".returns-word{") << config_->get_returns_word_style()->str() << std::move("}\n");
    file << std::move("</style>\n");
    file << std::move("</head>\n");
    file << std::move("<body style=") << config_->get_default_style()->str() << std::move(">\n");
    file << std::move(output_->str());
    file << std::move("</body>\n");
    file << std::move("</html>");
    file.close();
    std::cout << std::move("Zápis prebehol úspešne!\n");
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
    write_word(std::move("<span class=\"row-num\">"));
    if (row_ > 999) {
        *output_ << row_ << std::move(".");
    } else if (row_ > 99) {
        *output_ << std::move("&nbsp;") << row_ << std::move(".");
    } else if (row_ > 9) {
        *output_ << std::move("&nbsp;&nbsp;") << row_ << std::move(".");
    } else {
        *output_ << std::move("&nbsp;&nbsp;&nbsp;") << row_ << std::move(".");
    }
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_new_line() {
    write_word(std::move("<br>"));
    Exporter::write_new_line();
}

void HtmlFileExporter::write_space() {
    write_word(std::move("&nbsp;"));
}

void HtmlFileExporter::write_round_bracket(std::string br) {
    if (config_->use_br_col() && maxRoundBrIndex_ > 0) {
        std::string out = std::move("<span style=\"color:");
        if (std::move(br) == std::move("(")) {
            out.append(std::move(config_->get_round_br_col()->at(roundBrIndex_).str()));
            out.append(std::move("\">(</span>"));
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
            out.append(std::move(config_->get_round_br_col()->at(roundBrIndex_).str()));
            out.append(std::move("\">)</span>"));
        }
        write_word(std::move(out));
    } else {
        write_word(std::move(br));
    }
}

void HtmlFileExporter::write_curl_bracket(std::string br) {
    if (config_->use_br_col() && maxCurlBrIndex_ > 0) {
        std::string out = std::move("<span style=\"color:");
        if (std::move(br) == std::move("{")) {
            out.append(std::move(config_->get_curl_br_col()->at(curlBrIndex_).str()));
            out.append(std::move("\">{</span>"));
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
            out.append(std::move(config_->get_curl_br_col()->at(curlBrIndex_).str()));
            out.append(std::move("\">}</span>"));
        }
        write_word(std::move(out));
    } else {
        write_word(std::move(br));
    }
}

void HtmlFileExporter::write_unknown_word() {
    std::string s = std::move("<span class=\"unknown-word\">") + config_->get_unknown_word()->str() + std::move("</span>");
    write_word(std::move(s));
}

void HtmlFileExporter::write_invalid_word() {
    std::string s = std::move("<span class=\"invalid-word\">") + config_->get_invalid_word()->str() + std::move("</span>");
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
    std::string s = std::move("<span class=\"dynamic-type\">") + config_->get_dynamic_word()->str() + std::move("</span>");
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_int_type() {
    std::string s = std::move("<span class=\"int-type\">") + config_->get_int_word()->str() + std::move("</span>");
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_float_type() {
    std::string s = std::move("<span class=\"float-type\">") + config_->get_float_word()->str() + std::move("</span>");
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_char_type() {
    std::string s = std::move("<span class=\"char-type\">") + config_->get_char_word()->str() + std::move("</span>");
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_bool_type() {
    std::string s = std::move("<span class=\"bool-type\">") + config_->get_bool_word()->str() + std::move("</span>");
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_void_type() {
    std::string s = std::move("<span class=\"void-type\">") + config_->get_void_word()->str() + std::move("</span>");
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_user_type(std::string usertype) {
    std::string s = std::move("<span class=\"user-type\">") + std::move(usertype) + std::move("</span>");
    write_data_type_style(std::move(s));
}

void HtmlFileExporter::write_gen_param_name(std::string name) {
    std::string s = std::move("<span class=\"gen-par-name\">") + std::move(name) + std::move("</span>");
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_class_name(std::string name) {
    std::string s = std::move("<span class=\"class-name\">") + std::move(name) + std::move("</span>");
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_interface_name(std::string name) {
    std::string s = std::move("<span class=\"interface-name\">") + std::move(name) + std::move("</span>");
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_method_name(std::string name) {
    std::string s = std::move("<span class=\"method-name\">") + std::move(name) + std::move("</span>");
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_function_name(std::string name) {
    std::string s = std::move("<span class=\"function-name\">") + std::move(name) + std::move("</span>");
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_global_var_name(std::string name) {
    std::string s = std::move("<span class=\"global-var-name\">") + std::move(name) + std::move("</span>");
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_member_var_name(std::string name) {
    std::string s = std::move("<span class=\"member-var-name\">") + std::move(name) + std::move("</span>");
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_local_var_name(std::string name) {
    std::string s = std::move("<span class=\"local-var-name\">") + std::move(name) + std::move("</span>");
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_param_var_name(std::string name) {
    std::string s = std::move("<span class=\"param-var-name\">") + std::move(name) + std::move("</span>");
    write_ref_name_style(std::move(s));
}

void HtmlFileExporter::write_operator_sign(std::string sign) {
    write_operator_style(std::move(sign));
}

void HtmlFileExporter::write_assign_op_word() {
    write_operator_style(config_->get_assign_op_word()->str());
}

void HtmlFileExporter::write_separator_sign(std::string sign) {
    write_separator_style(std::move(sign));
}

void HtmlFileExporter::write_int_val(int val) {
    std::string s = std::move("<span class=\"int-value\">") + std::to_string(std::move(val)) + std::move("</span>");
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_float_val(float val) {
    std::string s = std::move("<span class=\"float-value\">") + std::to_string(std::move(val)) + std::move("</span>");
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_char_val(char val) {
    std::string s = std::move("<span class=\"char-value\">") + std::to_string(std::move(val)) + std::move("</span>");
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_string_val(std::string val) {
    std::string s = std::move("<span class=\"string-value\">") + std::move(val) + std::move("</span>");
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_bool_val(bool val) {
    std::string s = std::move("<span class=\"bool-value\">");
    val ? s.append(std::move(config_->get_true_val()->str())) : s.append(std::move(config_->get_false_val()->str()));
    s.append(std::move("</span>"));
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_null_val() {
    std::string s = std::move("<span class=\"null-value\">") + config_->get_null_val()->str() + std::move("</span>");
    write_value_style(std::move(s));
}

void HtmlFileExporter::write_class_word() {
    std::string s = std::move("<span class=\"class-word\">") + config_->get_class_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_interface_word() {
    std::string s = std::move("<span class=\"interface-word\">") + config_->get_interface_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_implement_word() {
    std::string s = std::move("<span class=\"implement-word\">") + config_->get_implement_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_extend_word() {
    std::string s = std::move("<span class=\"extend-word\">") + config_->get_extend_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_this_word() {
    std::string s = std::move("<span class=\"this-word\">") + config_->get_this_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_return_word() {
    std::string s = std::move("<span class=\"return-word\">") + config_->get_return_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_continue_word() {
    std::string s = std::move("<span class=\"continue-word\">") + config_->get_continue_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_break_word() {
    std::string s = std::move("<span class=\"break-word\">") + config_->get_break_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_throw_word() {
    std::string s = std::move("<span class=\"throw-word\">") + config_->get_throw_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_if_word() {
    std::string s = std::move("<span class=\"if-word\">") + config_->get_if_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_else_word() {
    std::string s = std::move("<span class=\"else-word\">") + config_->get_else_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_do_word() {
    std::string s = std::move("<span class=\"do-word\">") + config_->get_do_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_while_word() {
    std::string s = std::move("<span class=\"while-word\">") + config_->get_while_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_for_word() {
    std::string s = std::move("<span class=\"for-word\">") + config_->get_for_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_repeat_word() {
    std::string s = std::move("<span class=\"repeat-word\">") + config_->get_repeat_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_switch_word() {
    std::string s = std::move("<span class=\"switch-word\">") + config_->get_switch_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_case_word() {
    std::string s = std::move("<span class=\"case-word\">") + config_->get_case_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_default_word() {
    std::string s = std::move("<span class=\"default-word\">") + config_->get_default_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_new_word() {
    std::string s = std::move("<span class=\"new-word\">") + config_->get_new_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_delete_word() {
    std::string s = std::move("<span class=\"delete-word\">") + config_->get_delete_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_pointer_word() {
    std::string s = std::move("<span class=\"pointer-word\">") + config_->get_pointer_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_virtual_word() {
    std::string s = std::move("<span class=\"virtual-word\">") + config_->get_virtual_word()->str() + std::move("</span>");
    write_system_expr_style(std::move(s));
}

void HtmlFileExporter::write_constr_word() {
    std::string s = std::move("<span class=\"constr-word\">") + config_->get_constr_word()->str() + std::move("</span>");
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_destr_word() {
    std::string s = std::move("<span class=\"destr-word\">") + config_->get_destr_word()->str() + std::move("</span>");
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_method_word() {
    std::string s = std::move("<span class=\"method-word\">") + config_->get_method_word()->str() + std::move("</span>");
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_function_word() {
    std::string s = std::move("<span class=\"function-word\">") + config_->get_function_word()->str() + std::move("</span>");
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_lambda_word() {
    std::string s = std::move("<span class=\"lambda-word\">") + config_->get_lambda_word()->str() + std::move("</span>");
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_call_word() {
    std::string s = std::move("<span class=\"call-word\">") + config_->get_call_word()->str() + std::move("</span>");
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_define_word() {
    std::string s = std::move("<span class=\"define-word\">") + config_->get_define_word()->str() + std::move("</span>");
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_returns_word() {
    std::string s = std::move("<span class=\"returns-word\">") + config_->get_returns_word()->str() + std::move("</span>");
    write_other_expr_style(std::move(s));
}

void HtmlFileExporter::write_acc_mod_style(std::string accmod) {
    write_word(std::move("<span class=\"access-mod\">"));
    write_word(std::move(accmod));
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_data_type_style(std::string datatype) {
    write_word(std::move("<span class=\"data-type\">"));
    write_word(std::move(datatype));
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_ref_name_style(std::string name) {
    write_word(std::move("<span class=\"ref-name\">"));
    write_word(std::move(name));
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_operator_style(std::string op) {
    write_word(std::move("<span class=\"operator\">"));
    write_word(std::move(op));
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_separator_style(std::string sep) {
    write_word(std::move("<span class=\"separator\">"));
    write_word(std::move(sep));
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_value_style(std::string val) {
    write_word(std::move("<span class=\"value\">"));
    write_word(std::move(val));
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_system_expr_style(std::string expr) {
    write_word(std::move("<span class=\"system-expr\">"));
    write_word(std::move(expr));
    write_word(std::move("</span>"));
}

void HtmlFileExporter::write_other_expr_style(std::string expr) {
    write_word(std::move("<span class=\"other-expr\">"));
    write_word(std::move(expr));
    write_word(std::move("</span>"));
}