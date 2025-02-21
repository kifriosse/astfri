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

void Exporter::check_file(std::string suffix) {
    const std::string& path = config_->get_output_file_path()->str() + suffix;
    std::ofstream file(path);
    if (file) {
        file.close();
        write_file(path);
    } else {
        file.close();
        const std::string& defaultPath = config_->get_default_output_path()->str() + std::move(suffix);
        std::cout << std::move("Zadaná cesta neexistuje! ");
        write_file(defaultPath);
    }
}

void Exporter::increase_indentation() {
    ++currentIndentation_;
}

void Exporter::decrease_indentation() {
    --currentIndentation_;
}

void Exporter::write_word(std::string word) {
    !startedLine_ ? write_indentation() : void();
    *output_ << std::move(word);
}

void Exporter::write_indentation() {
    config_->sh_row_number() ? write_row_number() : void();
    for (int i = 0; i < config_->get_len_left_margin(); ++i) {
        *output_ << std::move(" ");
    }
    for (int i = 0; i < currentIndentation_; ++i) {
        for (int j = 0; j < config_->get_len_tab_word(); ++j) {
            *output_ << std::move(" ");
        }
    }
    startedLine_ = true;
}

void Exporter::write_row_number() {
    *output_ << std::setw(3) << row_ << std::move(".");
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

void Exporter::write_square_bracket(std::string br) {
    write_word(std::move(br));
}

void Exporter::write_curl_bracket(std::string br) {
    write_word(std::move(br));
}

void Exporter::write_operator_sign(std::string sign) {
    write_word(std::move(sign));
}

void Exporter::write_separator_sign(std::string sign) {
    write_word(std::move(sign));
}

void Exporter::write_dynamic_type() {
    write_word(std::move("dynamic"));
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

void Exporter::write_unknown_type() {
    write_word(std::move("unk-type"));
}

void Exporter::write_int_val(int val) {
    write_word(std::to_string(std::move(val)));
}

void Exporter::write_float_val(float val) {
    write_word(std::to_string(std::move(val)));
}

void Exporter::write_char_val(char val) {
    write_word(std::to_string(std::move(val)));
}

void Exporter::write_string_val(std::string val) {
    write_word(std::move(val));
}

void Exporter::write_bool_val(bool val) {
    val ? write_word(std::move("true")) : write_word(std::move("false"));
}

void Exporter::write_null_val() {
    write_word(std::move("NULL"));
}

void Exporter::write_unknown_expr() {
    write_word(std::move("unk-expr"));
}

void Exporter::write_param_var_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_local_var_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_member_var_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_global_var_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_function_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_method_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_class_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_gen_param_name(std::string name) {
    write_word(std::move(name));
}

void Exporter::write_gen_param_constr(std::string constraint) {
    write_word(std::move(constraint));
}

void Exporter::write_unknown_stat() {
    write_word(std::move("unk-stat"));
}

void Exporter::write_private_word() {
    write_word(config_->get_private_word()->str());
}

void Exporter::write_protected_word() {
    write_word(config_->get_protected_word()->str());
}

void Exporter::write_public_word() {
    write_word(config_->get_public_word()->str());
}

void Exporter::write_class_word() {
    write_word(config_->get_class_word()->str());
}

void Exporter::write_this_word() {
    write_word(config_->get_this_word()->str());
}

void Exporter::write_return_word() {
    write_word(config_->get_return_word()->str());
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

void Exporter::write_for_word() {
    write_word(config_->get_for_word()->str());
}

void Exporter::write_while_word() {
    write_word(config_->get_while_word()->str());
}

void Exporter::write_do_word() {
    write_word(config_->get_do_word()->str());
}

void Exporter::write_switch_word() {
    write_word(config_->get_switch_word()->str());
}

void Exporter::write_case_word() {
    write_word(config_->get_case_word()->str());
}

void Exporter::write_assign_word() {
    write_word(config_->get_assign_word()->str());
}