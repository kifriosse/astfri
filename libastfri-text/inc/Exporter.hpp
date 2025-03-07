#ifndef LIBASTFRI_TEXT_INC_EXPORTER
#define LIBASTFRI_TEXT_INC_EXPORTER

#include <libastfri-text/inc/TextConfigurator.hpp>

class Exporter {
protected:
    std::shared_ptr<TextConfigurator> config_;
    std::unique_ptr<std::stringstream> output_;
    int currentIndentation_;
    int row_;
    bool startedLine_;
protected:
    Exporter(std::shared_ptr<TextConfigurator> conf);
    virtual void write_output_into_file(std::string filepath) = 0;
    void check_output_file_path(std::string suffix);
    void write_word(std::string word);
    virtual void write_indentation();
    virtual void write_row_number();
public:
    virtual void make_export() = 0;
    void increase_indentation();
    void decrease_indentation();
    virtual void write_new_line();
    virtual void write_space();
    virtual void write_round_bracket(std::string br);
    virtual void write_curl_bracket(std::string br);
    virtual void write_unknown_word();
    virtual void write_invalid_word();
    virtual void write_public_word();
    virtual void write_private_word();
    virtual void write_protected_word();
    virtual void write_attribs_word();
    virtual void write_constrs_word();
    virtual void write_destrs_word();
    virtual void write_meths_word();
    virtual void write_dynamic_type();
    virtual void write_int_type();
    virtual void write_float_type();
    virtual void write_char_type();
    virtual void write_bool_type();
    virtual void write_void_type();
    virtual void write_user_type(std::string usertype);
    virtual void write_gen_param_name(std::string name);
    virtual void write_class_name(std::string name);
    virtual void write_interface_name(std::string name);
    virtual void write_method_name(std::string name);
    virtual void write_function_name(std::string name);
    virtual void write_global_var_name(std::string name);
    virtual void write_member_var_name(std::string name);
    virtual void write_local_var_name(std::string name);
    virtual void write_param_var_name(std::string name);
    virtual void write_operator_sign(std::string sign);
    virtual void write_assign_op_word();
    virtual void write_separator_sign(std::string sign);
    virtual void write_int_val(int val);
    virtual void write_float_val(float val);
    virtual void write_char_val(char val);
    virtual void write_string_val(std::string val);
    virtual void write_bool_val(bool val);
    virtual void write_null_val();
    virtual void write_class_word();
    virtual void write_interface_word();
    virtual void write_implement_word();
    virtual void write_this_word();
    virtual void write_return_word();
    virtual void write_continue_word();
    virtual void write_break_word();
    virtual void write_throw_word();
    virtual void write_if_word();
    virtual void write_else_word();
    virtual void write_do_word();
    virtual void write_while_word();
    virtual void write_for_word();
    virtual void write_repeat_word();
    virtual void write_switch_word();
    virtual void write_case_word();
    virtual void write_default_word();
    virtual void write_new_word();
    virtual void write_delete_word();
    virtual void write_pointer_word();
    virtual void write_virtual_word();
    virtual void write_constr_word();
    virtual void write_destr_word();
    virtual void write_method_word();
    virtual void write_function_word();
    virtual void write_lambda_word();
    virtual void write_call_word();
    virtual void write_define_word();
    virtual void write_returns_word();
};

#endif