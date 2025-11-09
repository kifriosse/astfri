#ifndef LIBASTFRI_TEXT_ABSTRACT_TEXT_BUILDER
#define LIBASTFRI_TEXT_ABSTRACT_TEXT_BUILDER

#include <libastfri-text/inc/AbstractBuilder.hpp>
#include <libastfri-text/inc/pseudocode/TextConfigurator.hpp>

namespace astfri::text
{
    class AbstractTextBuilder : public AbstractBuilder
    {
    protected:
        TextConfigurator* configurator_;
        int rowCount_;
    public:
        explicit AbstractTextBuilder();
        virtual ~AbstractTextBuilder() = default;
    public:
        // SET_UP
        void reset_builder() override;
        // GENERAL_TEXT
        void append_text(std::string const& text) override;
        void append_new_line() override = 0;
        void append_space() override = 0;
        void write_opening_curl_bracket() override;
        void write_opening_else_word() override;
        // BRACKETS, SEPARATORS, OPERATORS
        virtual void write_right_bracket(std::string const& br) = 0;
        virtual void write_left_bracket(std::string const& br) = 0;
        virtual void write_separator(std::string const& sep) = 0;
        virtual void write_operator(std::string const& op) = 0;
        virtual void write_assign_operator() = 0;
        virtual void write_modulo_operator() = 0;
        virtual void write_address_operator() = 0;
        virtual void write_deref_operator() = 0;
        // UNKNOWN_PHRASES
        void write_unknown_type() override = 0;
        void write_unknown_expr() override = 0;
        void write_unknown_stmt() override = 0;
        void write_invalid_type() override = 0;
        void write_invalid_expr() override = 0;
        void write_invalid_stmt() override = 0;
        // ACCESS_MODIFIERS
        virtual void write_public_word() = 0;
        virtual void write_protected_word() = 0;
        virtual void write_private_word() = 0;
        virtual void write_internal_word() = 0;
        virtual void write_attributes_word() = 0;
        virtual void write_constructors_word() = 0;
        virtual void write_destructors_word() = 0;
        virtual void write_methods_word() = 0;
        // DATA_TYPES
        virtual void write_dynamic_type_word() = 0;
        virtual void write_int_type_word() = 0;
        virtual void write_float_type_word() = 0;
        virtual void write_char_type_word() = 0;
        virtual void write_bool_type_word() = 0;
        virtual void write_void_type_word() = 0;
        virtual void write_class_type(std::string const& name) = 0;
        virtual void write_interface_type(std::string const& name) = 0;
        // REFERENCE_NAMES
        virtual void write_scope_name(std::string const& name) = 0;
        virtual void write_gen_param_name(std::string const& name) = 0;
        virtual void write_class_name(std::string const& name) = 0;
        virtual void write_interface_name(std::string const& name) = 0;
        virtual void write_method_name(std::string const& name) = 0;
        virtual void write_function_name(std::string const& name) = 0;
        virtual void write_global_var_name(std::string const& name) = 0;
        virtual void write_member_var_name(std::string const& name) = 0;
        virtual void write_local_var_name(std::string const& name) = 0;
        virtual void write_param_var_name(std::string const& name) = 0;
        // VALUES
        void write_int_val(int const& val) override = 0;
        void write_float_val(float const& val) override = 0;
        void write_char_val(char const& val) override = 0;
        void write_string_val(std::string const& val) override = 0;
        void write_bool_val(bool const& val) override = 0;
        virtual void write_null_val() = 0;
        // SYSTEM_EXPRESSIONS
        virtual void write_scope_word() = 0;
        virtual void write_class_word() = 0;
        virtual void write_interface_word() = 0;
        virtual void write_implement_word() = 0;
        virtual void write_extend_word() = 0;
        virtual void write_this_word() = 0;
        virtual void write_return_word() = 0;
        virtual void write_continue_word() = 0;
        virtual void write_break_word() = 0;
        virtual void write_throw_word() = 0;
        virtual void write_if_word() = 0;
        virtual void write_else_word() = 0;
        virtual void write_do_word() = 0;
        virtual void write_while_word() = 0;
        virtual void write_for_word() = 0;
        virtual void write_switch_word() = 0;
        virtual void write_case_word() = 0;
        virtual void write_default_word() = 0;
        virtual void write_new_word() = 0;
        virtual void write_delete_word() = 0;
        virtual void write_pointer_word() = 0;
        virtual void write_override_word() = 0;
        virtual void write_virtual_word() = 0;
        virtual void write_abstract_word() = 0;
        virtual void write_template_word() = 0;
        // OTHER_EXPRESSIONS
        virtual void write_constructor_word() = 0;
        virtual void write_destructor_word() = 0;
        virtual void write_method_word() = 0;
        virtual void write_function_word() = 0;
        virtual void write_lambda_word() = 0;
        virtual void write_call_word() = 0;
        virtual void write_define_word() = 0;
        virtual void write_returns_word() = 0;
        virtual void write_repeat_word() = 0;
    };
}

#endif