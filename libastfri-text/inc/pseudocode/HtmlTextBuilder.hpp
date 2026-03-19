#ifndef LIBASTFRI_TEXT_HTML_TEXT_BUILDER
#define LIBASTFRI_TEXT_HTML_TEXT_BUILDER

#include <libastfri-text/inc/pseudocode/AbstractTextBuilder.hpp>

namespace astfri::text {
class HtmlTextBuilder : public AbstractTextBuilder {
private:
    int maxBracketIndex_;
    int currentBrIndex_;

public:
    static HtmlTextBuilder& get_instance();
    HtmlTextBuilder(const HtmlTextBuilder&)            = delete;
    HtmlTextBuilder(HtmlTextBuilder&&)                 = delete;
    HtmlTextBuilder& operator=(const HtmlTextBuilder&) = delete;
    HtmlTextBuilder& operator=(HtmlTextBuilder&&)      = delete;

private:
    explicit HtmlTextBuilder();
    ~HtmlTextBuilder() = default;

public:
    // SET_UP
    void reset_builder() override;
    // EXPORT
    void export_text() override;
    // GENERAL_TEXT
    void append_new_line() override;
    void append_space() override;
    void write_separator(const std::string& sep) override;
    void write_right_bracket(const std::string& br) override;
    void write_left_bracket(const std::string& br) override;
    // OPERATORS
    void write_operator(const std::string& op) override;
    void write_assign_operator() override;
    void write_modulo_operator() override;
    void write_address_operator() override;
    void write_deref_operator() override;
    // UNKNOWN_PHRASES
    void write_unknown_type() override;
    void write_unknown_expr() override;
    void write_unknown_stmt() override;
    void write_invalid_type() override;
    void write_invalid_expr() override;
    void write_invalid_stmt() override;
    // ACCESS_MODIFIERS
    void write_public_word() override;
    void write_protected_word() override;
    void write_private_word() override;
    void write_internal_word() override;
    void write_attributes_word() override;
    void write_constructors_word() override;
    void write_destructors_word() override;
    void write_methods_word() override;
    // DATA_TYPES
    void write_dynamic_type_word() override;
    void write_int_type_word() override;
    void write_float_type_word() override;
    void write_char_type_word() override;
    void write_bool_type_word() override;
    void write_void_type_word() override;
    void write_class_type(const std::string& name) override;
    void write_interface_type(const std::string& name) override;
    // REFERENCE_NAMES
    void write_scope_name(const std::string& name) override;
    void write_gen_param_name(const std::string& name) override;
    void write_class_name(const std::string& name) override;
    void write_interface_name(const std::string& name) override;
    void write_method_name(const std::string& name) override;
    void write_function_name(const std::string& name) override;
    void write_global_var_name(const std::string& name) override;
    void write_member_var_name(const std::string& name) override;
    void write_local_var_name(const std::string& name) override;
    void write_param_var_name(const std::string& name) override;
    // VALUES
    void write_int_val(const int& val) override;
    void write_float_val(const float& val) override;
    void write_char_val(const char& val) override;
    void write_string_val(const std::string& val) override;
    void write_bool_val(const bool& val) override;
    void write_null_val() override;
    // SYSTEM_EXPRESSIONS
    void write_scope_word() override;
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
    void write_switch_word() override;
    void write_case_word() override;
    void write_default_word() override;
    void write_new_word() override;
    void write_delete_word() override;
    void write_pointer_word() override;
    void write_override_word() override;
    void write_virtual_word() override;
    void write_abstract_word() override;
    void write_template_word() override;
    // OTHER_EXPRESSIONS
    void write_constructor_word() override;
    void write_destructor_word() override;
    void write_method_word() override;
    void write_function_word() override;
    void write_lambda_word() override;
    void write_call_word() override;
    void write_define_word() override;
    void write_returns_word() override;
    void write_repeat_word() override;

private:
    // WRAPPING_CLASSES
    void write_unavailable_style(const std::string& phrase);
    void write_acc_mod_style(const std::string& accmod);
    void write_data_type_style(const std::string& datatype);
    void write_ref_name_style(const std::string& name);
    void write_operator_style(const std::string& op);
    void write_separator_style(const std::string& sep);
    void write_value_style(const std::string& val);
    void write_system_expr_style(const std::string& expr);
    void write_other_expr_style(const std::string& expr);
};
} // namespace astfri::text

#endif