#include <libastfri-text/inc/Exporter.hpp>
#include <iomanip>

Exporter::Exporter(Configurator& conf) {
    config_ = &conf;
    output_ = new std::stringstream();
    currentIndentation_ = 0;
    startedLine_ = false;
    row_ = 1;
}

Exporter::~Exporter() {
    delete output_;
    config_ = nullptr;
}

//---------------GENERAL----------------------------------------------------

void Exporter::increase_indentation() {
    ++currentIndentation_;
}

void Exporter::decrease_indentation() {
    --currentIndentation_;
}

void Exporter::write_word(const std::string& ss) {
    !startedLine_ ? write_indentation() : void();
    *output_ << ss;
}

void Exporter::write_space() {
    write_word(" ");
}

//---------------SPECIFIC---------------------------------------------------

void Exporter::write_indentation() {
    config_->show_row_number() ? write_row_number() : void();
    *output_ << "  ";
    for (int i = 0; i < currentIndentation_; ++i) {
        *output_ << config_->get_tab_word()->str();
    }
    startedLine_ = true;
}

void Exporter::write_new_line() {
    write_word("\n");
    startedLine_ = false;
    ++row_;
}

void Exporter::write_curl_bracket(const std::string& s) {
    write_word(s);
}

void Exporter::write_round_bracket(const std::string& s) {
    write_word(s);
}

void Exporter::write_dynamic_type() {
    write_word("dynamic");
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

void Exporter::write_user_type(const std::string& usertype) {
    write_word(usertype);
}

void Exporter::write_int_val(const int val) {
    write_word(std::to_string(val));
}

void Exporter::write_float_val(const float val) {
    write_word(std::to_string(val));
}

void Exporter::write_char_val(const char val) {
    write_word(std::to_string(val));
}

void Exporter::write_string_val(const std::string& val) {
    write_word(val);
}

void Exporter::write_bool_val(const bool val) {
    val ? write_word("true") : write_word("false");
}

void Exporter::write_null_val() {
    write_word("NULL");
}

void Exporter::write_param_var_name(const std::string& name) {
    write_word(name);
}

void Exporter::write_local_var_name(const std::string& name) {
    write_word(name);
}

void Exporter::write_member_var_name(const std::string& name) {
    write_word(name);
}

void Exporter::write_global_var_name(const std::string& name) {
    write_word(name);
}

void Exporter::write_function_name(const std::string& name) {
    write_word(name);
}

void Exporter::write_method_name(const std::string& name) {
    write_word(name);
}

void Exporter::write_assign_word() {
    write_word(config_->get_assign_word()->str());
}

void Exporter::write_private_word() {
    write_word(config_->get_private_word()->str());
}

void Exporter::write_public_word() {
    write_word(config_->get_public_word()->str());
}

void Exporter::write_class_word() {
    write_word(config_->get_class_word()->str());
}

void Exporter::write_class_name(const std::string& name) {
    write_word(name);
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

void Exporter::write_return_word() {
    write_word(config_->get_return_word()->str());
}

void Exporter::write_throw_word() {
    write_word(config_->get_throw_word()->str());
}

void Exporter::write_switch_word() {
    write_word(config_->get_switch_word()->str());
}

void Exporter::write_case_word() {
    write_word(config_->get_case_word()->str());
}

void Exporter::write_this_word() {
    write_word(config_->get_this_word()->str());
}

void Exporter::write_unknown_type() {
    write_word("unk-type");
}

void Exporter::write_unknown_expr() {
    write_word("unk-expr");
}

void Exporter::write_unknown_stat() {
    write_word("unk-stat");
}

void Exporter::write_row_number() {
    *output_ << std::setw(3) << row_ << ".";
}