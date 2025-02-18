#ifndef LIBASTFRI_TEXT_INC_EXPORTER
#define LIBASTFRI_TEXT_INC_EXPORTER

#include <libastfri-text/inc/TextConfigurator.hpp>

struct Exporter {
protected:
    std::shared_ptr<TextConfigurator> config_;
    std::unique_ptr<std::stringstream> output_;
    int currentIndentation_;
    int row_;
    bool startedLine_;
public:
    Exporter(std::shared_ptr<TextConfigurator> conf);
    virtual void make_export() = 0;
    void increase_indentation();
    void decrease_indentation();
    virtual void write_space();
    virtual void write_new_line();
    virtual void write_curl_bracket(std::string s);
    virtual void write_round_bracket(std::string s);
    virtual void write_operator_sign(std::string s);
    virtual void write_separator_sign(std::string s);
    virtual void write_dynamic_type();
    virtual void write_int_type();
    virtual void write_float_type();
    virtual void write_char_type();
    virtual void write_bool_type();
    virtual void write_void_type();
    virtual void write_user_type(std::string usertype);
    virtual void write_int_val(int val);
    virtual void write_float_val(float val);
    virtual void write_char_val(char val);
    virtual void write_string_val(std::string val);
    virtual void write_bool_val(bool val);
    virtual void write_null_val();
    virtual void write_param_var_name(std::string name);
    virtual void write_local_var_name(std::string name);
    virtual void write_member_var_name(std::string name);
    virtual void write_global_var_name(std::string name);
    virtual void write_gen_param_name(std::string name);
    virtual void write_gen_param_constr(std::string constr);
    virtual void write_function_name(std::string name);
    virtual void write_method_name(std::string name);
    virtual void write_assign_word();
    virtual void write_private_word();
    virtual void write_public_word();
    virtual void write_class_word();
    virtual void write_class_name(std::string name);
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
    void write_word(std::string word);
    virtual void write_indentation();
    virtual void write_row_number();
};

#endif