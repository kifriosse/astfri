#pragma once
#include <libastfri-text/inc/Configurator.hpp>

struct Exporter {
private:
    const Configurator* config_;
    std::stringstream* output_;
    int currentIndentation_;
    bool startedLine_;
public:
    Exporter(const Configurator* conf, std::stringstream* output);
    virtual ~Exporter();
    virtual void make_export() = 0;
    std::stringstream* get_output() { return output_; };
    void increase_indentation() { ++currentIndentation_; };
    void decrease_indentation() { --currentIndentation_; };
    void write_word(const std::string& ss, bool newLine = false);
    void write_indentation();
    virtual void write_int_type();
};