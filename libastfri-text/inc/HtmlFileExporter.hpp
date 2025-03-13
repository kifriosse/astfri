#ifndef LIBASTFRI_TEXT_INC_HTML_EXPORTER
#define LIBASTFRI_TEXT_INC_HTML_EXPORTER

#include <libastfri-text/inc/Exporter.hpp>

class HtmlFileExporter : public Exporter {
    int maxRoundBrIndex_;
    int maxCurlBrIndex_;
    int roundBrIndex_;
    int curlBrIndex_;
public:
    HtmlFileExporter(std::shared_ptr<TextConfigurator> conf);
    void make_export() override;
private:
    void write_output_into_file(std::string filepath) override;
    void write_indentation() override;
    void write_row_number() override;
public:
    void write_new_line() override;
    void write_space() override;
    void write_round_bracket(std::string br) override;
    void write_curl_bracket(std::string br) override;
    void write_unknown_word() override;
    void write_invalid_word() override;
    void write_public_word() override;
    void write_private_word() override;
    void write_protected_word() override;
    void write_attribs_word() override;
    void write_constrs_word() override;
    void write_destrs_word() override;
    void write_meths_word() override;
    void write_dynamic_type() override;
    void write_int_type() override;
    void write_float_type() override;
    void write_char_type() override;
    void write_bool_type() override;
    void write_void_type() override;
    void write_user_type(std::string usertype) override;
    void write_gen_param_name(std::string name) override;
    void write_class_name(std::string name) override;
    void write_interface_name(std::string name) override;
    void write_method_name(std::string name) override;
    void write_function_name(std::string name) override;
    void write_global_var_name(std::string name) override;
    void write_member_var_name(std::string name) override;
    void write_local_var_name(std::string name) override;
    void write_param_var_name(std::string name) override;
    void write_operator_sign(std::string sign) override;
    void write_assign_op_word() override;
    void write_separator_sign(std::string sign) override;
    void write_int_val(int val) override;
    void write_float_val(float val) override;
    void write_char_val(char val) override;
    void write_string_val(std::string val) override;
    void write_bool_val(bool val) override;
    void write_null_val() override;
    void write_class_word() override;
    void write_interface_word() override;
    void write_implement_word() override;
    void write_extend_word() override;
    void write_this_word() override;
    void write_return_word() override;
    void write_continue_word() override;
    void write_break_word() override;
    void write_throw_word() override;
    void write_if_word() override;
    void write_else_word() override;
    void write_do_word() override;
    void write_while_word() override;
    void write_for_word() override;
    void write_repeat_word() override;
    void write_switch_word() override;
    void write_case_word() override;
    void write_default_word() override;
    void write_new_word() override;
    void write_delete_word() override;
    void write_pointer_word() override;
    void write_virtual_word() override;
    void write_constr_word() override;
    void write_destr_word() override;
    void write_method_word() override;
    void write_function_word() override;
    void write_lambda_word() override;
    void write_call_word() override;
    void write_define_word() override;
    void write_returns_word() override;
private:
    void write_acc_mod_style(std::string accmod);
    void write_data_type_style(std::string datatype);
    void write_ref_name_style(std::string name);
    void write_operator_style(std::string op);
    void write_separator_style(std::string sep);
    void write_value_style(std::string val);
    void write_system_expr_style(std::string expr);
    void write_other_expr_style(std::string expr);
};

#endif