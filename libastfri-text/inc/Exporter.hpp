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
    //---------------GENERAL----------------------------------------------------
    std::stringstream* get_output() { return output_; };
    const Configurator* get_config() { return config_; };
    void increase_indentation() { ++currentIndentation_; };
    void decrease_indentation() { --currentIndentation_; };
    void write_word(const std::string& ss, bool newLine = false);
    void write_space();
    void write_new_line();
    //---------------SPECIFIC---------------------------------------------------
    virtual void write_curl_bracket(const std::string& s);
    virtual void write_round_bracket(const std::string& s);
    virtual void write_int_type();
    virtual void write_char_type();
    virtual void write_class_word();
    virtual void write_if_word();
    virtual void write_else_word();
    virtual void write_for_word();
    virtual void write_while_word();
private:
    void write_indentation();
};