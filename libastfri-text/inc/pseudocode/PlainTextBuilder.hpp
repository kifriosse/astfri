#ifndef LIBASTFRI_TEXT_PLAIN_TEXT_BUILDER
#define LIBASTFRI_TEXT_PLAIN_TEXT_BUILDER

#include <libastfri-text/inc/pseudocode/AbstractTextBuilder.hpp>

namespace astfri::text
{
    class PlainTextBuilder : public AbstractTextBuilder
    {
    public:
        static PlainTextBuilder& get_instance();
        PlainTextBuilder(PlainTextBuilder const&) = delete;
        PlainTextBuilder(PlainTextBuilder&&)      = delete;
        PlainTextBuilder& operator=(PlainTextBuilder const&) = delete;
        PlainTextBuilder& operator=(PlainTextBuilder&&)      = delete;
    private:
        explicit PlainTextBuilder() = default;
        ~PlainTextBuilder() = default;
    public:
        // GENERAL_TEXT
        void append_new_line() override;
        void append_space() override;
        // BRACKETS, SEPARATORS, OPERATORS
        void write_right_bracket(std::string const& br) override;
        void write_left_bracket(std::string const& br) override;
        void write_separator(std::string const& sep) override;
        void write_operator(std::string const& op) override;
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
        void write_class_type(std::string const& name) override;
        void write_interface_type(std::string const& name) override;
        // REFERENCE_NAMES
        void write_scope_name(std::string const& name) override;
        void write_gen_param_name(std::string const& name) override;
        void write_class_name(std::string const& name) override;
        void write_interface_name(std::string const& name) override;
        void write_method_name(std::string const& name) override;
        void write_function_name(std::string const& name) override;
        void write_global_var_name(std::string const& name) override;
        void write_member_var_name(std::string const& name) override;
        void write_local_var_name(std::string const& name) override;
        void write_param_var_name(std::string const& name) override;
        // VALUES
        void write_int_val(int const& val) override;
        void write_float_val(float const& val) override;
        void write_char_val(char const& val) override;
        void write_string_val(std::string const& val) override;
        void write_bool_val(bool const& val) override;
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
    };
}

#endif