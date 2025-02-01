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

void Exporter::write_word(const std::string& ss, bool newLine) {
    !startedLine_ ? write_indentation() : void();
    *output_ << ss;
    newLine ? startedLine_ = false : true;
}

void Exporter::write_indentation() {
    for (int i = 0; i < currentIndentation_; ++i) {
        *output_ << config_->get_tab_word()->str();
        startedLine_ = true;
    }
}

void Exporter::write_int_type() {
    write_word(config_->get_int_word()->str());
}