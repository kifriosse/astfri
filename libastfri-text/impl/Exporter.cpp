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

void Exporter::write_new_line() {
    write_word("\n", true);
}

void Exporter::write_indentation() {
    for (int i = 0; i < currentIndentation_; ++i) {
        *output_ << config_->get_tab_word()->str();
        startedLine_ = true;
    }
}

//---------------SPECIFIC---------------------------------------------------

void Exporter::write_curl_bracket(const std::string& s) {
    write_word(s);
}

void Exporter::write_round_bracket(const std::string& s) {
    write_word(s);
}

void Exporter::write_int_type() {
    write_word(config_->get_int_word()->str());
}

void Exporter::write_char_type() {
    write_word(config_->get_char_word()->str());
}

void Exporter::write_class_word() {
    write_word(config_->get_class_word()->str());
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