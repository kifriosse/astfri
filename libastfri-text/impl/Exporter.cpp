#include <libastfri-text/inc/Exporter.hpp>

Exporter::Exporter(const Configurator* conf, std::stringstream* output) {
    config_ = conf;
    output_ = output;
    currentIndentation_ = 0;
    startedLine_ = false;
}

Exporter::~Exporter() {
    config_ = nullptr;
    output_ = nullptr;
}

//---------------GENERAL----------------------------------------------------

void Exporter::write_word(const std::string& ss, bool newLine) {
    !startedLine_ ? write_indentation() : void();
    *output_ << ss;
    newLine ? startedLine_ = false : true;
}

void Exporter::write_space() {
    write_word(" ");
}

//---------------SPECIFIC---------------------------------------------------

void Exporter::write_indentation() {
    for (int i = 0; i < currentIndentation_; ++i) {
        *output_ << config_->get_tab_word()->str();
        startedLine_ = true;
    }
}

void Exporter::write_new_line() {
    write_word("\n", true);
}

void Exporter::write_curl_bracket(const std::string& s) {
    write_word(s);
}

void Exporter::write_round_bracket(const std::string& s) {
    write_word(s);
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