#pragma once
#include <libastfri-text/inc/Configurator.hpp>

struct Exporter {
protected:
    const Configurator* config_;
    std::stringstream* output_;
    int currentIndentation_;
    int row_;
    bool startedLine_;
public:
    Exporter(const Configurator* conf, std::stringstream* output);
    virtual ~Exporter();
    virtual void make_export() = 0;
    //---------------GENERAL----------------------------------------------------
    void increase_indentation() { ++currentIndentation_; };
    void decrease_indentation() { --currentIndentation_; };
    void write_word(const std::string& ss);
    void write_space();
    //---------------SPECIFIC---------------------------------------------------
    virtual void write_new_line();
    virtual void write_curl_bracket(const std::string& s);
    virtual void write_round_bracket(const std::string& s);
    virtual void write_dynamic_type();
    virtual void write_int_type();
    virtual void write_float_type();
    virtual void write_char_type();
    virtual void write_bool_type();
    virtual void write_void_type();
    virtual void write_user_type(const std::string& usertype);
    virtual void write_int_val(const int val);
    virtual void write_float_val(const float val);
    virtual void write_char_val(const char val);
    virtual void write_string_val(const std::string& val);
    virtual void write_bool_val(const bool val);
    virtual void write_null_val();
    virtual void write_param_var_name(const std::string& name);
    virtual void write_local_var_name(const std::string& name);
    virtual void write_member_var_name(const std::string& name);
    virtual void write_global_var_name(const std::string& name);
    virtual void write_function_name(const std::string& name);
    virtual void write_method_name(const std::string& name);
    virtual void write_assign_word();
    virtual void write_private_word();
    virtual void write_public_word();
    virtual void write_class_word();
    virtual void write_class_name(const std::string& name);
    virtual void write_if_word();
    virtual void write_else_word();
    virtual void write_for_word();
    virtual void write_while_word();
    virtual void write_do_word();
    virtual void write_return_word();
    virtual void write_throw_word();
    virtual void write_switch_word();
    virtual void write_case_word();
    virtual void write_this_word();
    virtual void write_unknown_type();
    virtual void write_unknown_expr();
    virtual void write_unknown_stat();
protected:
    virtual void write_indentation();
    virtual void write_row_number();
};