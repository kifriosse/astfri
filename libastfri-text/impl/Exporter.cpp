#include <libastfri-text/inc/Exporter.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>

Exporter::Exporter(std::shared_ptr<TextConfigurator> conf) {
    config_ = std::move(conf);
    output_ = std::make_unique<std::stringstream>();
    currentIndentation_ = 0;
    startedLine_ = false;
    row_ = 1;
}

void Exporter::check_output_file_path(std::string suffix) {
    const std::string& path = config_->get_output_file_path()->str() + suffix;
    std::ofstream file(path);
    if (file) {
        file.close();
        write_output_into_file(path);
    } else {
        file.close();
        const std::string& defaultPath = config_->get_default_output_path()->str() + std::move(suffix);
        std::cout << std::move("Zadaná cesta neexistuje! ");
        check_output_file_path(defaultPath);
    }
}

void Exporter::write_word(std::string word) {
    !startedLine_ ? write_indentation() : void();
    *output_ << std::move(word);
}

void Exporter::increase_indentation() {
    ++currentIndentation_;
}

void Exporter::decrease_indentation() {
    --currentIndentation_;
}

void Exporter::write_indentation() {
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

void Exporter::write_row_number() {
    *output_ << std::setw(4) << row_ << std::move(".");
}

void Exporter::write_new_line() {
    write_word(std::move("\n"));
    startedLine_ = false;
    ++row_;
}

void Exporter::write_space() {
    write_word(std::move(" "));
}

void Exporter::write_round_bracket(std::string br) {
    write_word(std::move(br));
}

void Exporter::write_curl_bracket(std::string br) {
    write_word(std::move(br));
}

void Exporter::write_unknown_word() {
    write_word(config_->get_unknown_word()->str());
}

void Exporter::write_invalid_word() {
    write_word(config_->get_invalid_word()->str());
}

void Exporter::write_public_word() {
    write_word(config_->get_public_word()->str());
}

void Exporter::write_private_word() {
    write_word(config_->get_private_word()->str());
}

void Exporter::write_protected_word() {
    write_word(config_->get_protected_word()->str());
}

void Exporter::write_attribs_word() {
    write_word(config_->get_acc_atrib_word()->str());
}

void Exporter::write_constrs_word() {
    write_word(config_->get_acc_constr_word()->str());
}

void Exporter::write_destrs_word() {
    write_word(config_->get_acc_destr_word()->str());
}

void Exporter::write_meths_word() {
    write_word(config_->get_acc_meth_word()->str());
}

void Exporter::write_dynamic_type() {
    write_word(config_->get_dynamic_word()->str());
}

void Exporter::write_int_type() {
    write_word(config_->get_int_word()->str());
}

void Exporter::write_float_type() {
    write_word(config_->get_float_word()->str());
}

void Exporter::write_char_type() {
    write_word(config_->get_char_word()->str());
}

void Exporter::write_bool_type() {
    write_word(config_->get_bool_word()->str());
}

void Exporter::write_void_type() {
    write_word(config_->get_void_word()->str());
}

void Exporter::write_user_type(std::string usertype) {
    write_word(std::move(usertype));
}

void Exporter::write_gen_param_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_class_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_interface_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_method_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_function_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_global_var_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_member_var_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_local_var_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_param_var_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_operator_sign(std::string sign) {
    write_word(std::move(sign));
}

void Exporter::write_assign_op_word() {
    write_word(config_->get_assign_op_word()->str());
}

void Exporter::write_separator_sign(std::string sign) {
    write_word(std::move(sign));
}

void Exporter::write_int_val(int val) {
    write_word(std::to_string(std::move(val)));
}

void Exporter::write_float_val(float val) {
    write_word(std::to_string(std::move(val)));
}

void Exporter::write_char_val(char val) {
    write_word(std::string(1, std::move(val)));
}

void Exporter::write_string_val(std::string val) {
    write_word(std::move(val));
}

void Exporter::write_bool_val(bool val) {
    val ? write_word(config_->get_true_val()->str()) : write_word(config_->get_false_val()->str());
}

void Exporter::write_null_val() {
    write_word(config_->get_null_val()->str());
}

void Exporter::write_class_word() {
    write_word(config_->get_class_word()->str());
}

void Exporter::write_interface_word() {
    write_word(config_->get_interface_word()->str());
}

void Exporter::write_implement_word() {
    write_word(config_->get_implement_word()->str());
}

void Exporter::write_extend_word() {
    write_word(config_->get_extend_word()->str());
}

void Exporter::write_this_word() {
    write_word(config_->get_this_word()->str());
}

void Exporter::write_return_word() {
    write_word(config_->get_return_word()->str());
}

void Exporter::write_continue_word() {
    write_word(config_->get_continue_word()->str());
}

void Exporter::write_break_word() {
    write_word(config_->get_break_word()->str());
}

void Exporter::write_throw_word() {
    write_word(config_->get_throw_word()->str());
}

void Exporter::write_if_word() {
    write_word(config_->get_if_word()->str());
}

void Exporter::write_else_word() {
    write_word(config_->get_else_word()->str());
}

void Exporter::write_do_word() {
    write_word(config_->get_do_word()->str());
}

void Exporter::write_while_word() {
    write_word(config_->get_while_word()->str());
}

void Exporter::write_for_word() {
    write_word(config_->get_for_word()->str());
}

void Exporter::write_repeat_word() {
    write_word(config_->get_repeat_word()->str());
}

void Exporter::write_switch_word() {
    write_word(config_->get_switch_word()->str());
}

void Exporter::write_case_word() {
    write_word(config_->get_case_word()->str());
}

void Exporter::write_default_word() {
    write_word(config_->get_default_word()->str());
}

void Exporter::write_new_word() {
    write_word(config_->get_new_word()->str());
}

void Exporter::write_delete_word() {
    write_word(config_->get_delete_word()->str());
}

void Exporter::write_pointer_word() {
    write_word(config_->get_pointer_word()->str());
}

void Exporter::write_virtual_word() {
    write_word(config_->get_virtual_word()->str());
}

void Exporter::write_abstract_word() {
    write_word(config_->get_abstract_word()->str());
}

void Exporter::write_template_word() {
    write_word(config_->get_template_word()->str());
}

void Exporter::write_constr_word() {
    write_word(config_->get_constr_word()->str());
}

void Exporter::write_destr_word() {
    write_word(config_->get_destr_word()->str());
}

void Exporter::write_method_word() {
    write_word(config_->get_method_word()->str());
}

void Exporter::write_function_word() {
    write_word(config_->get_function_word()->str());
}

void Exporter::write_lambda_word() {
    write_word(config_->get_lambda_word()->str());
}

void Exporter::write_call_word() {
    write_word(config_->get_call_word()->str());
}

void Exporter::write_define_word() {
    write_word(config_->get_define_word()->str());
}

void Exporter::write_returns_word() {
    write_word(config_->get_returns_word()->str());
}