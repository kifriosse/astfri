#ifndef LIBASTFRI_TEXT_ABSTRACT_TEXT_BUILDER
#define LIBASTFRI_TEXT_ABSTRACT_TEXT_BUILDER

#include <sstream>
#include <memory>

namespace astfri::text
{
    class AbstractTextBuilder
    {
    protected:
        short indentationLevel_;
        bool isEmptyLine_;
        std::unique_ptr<std::stringstream> buildedText_;
    public:
        AbstractTextBuilder();
        virtual ~AbstractTextBuilder() = 0;
    public:
        // SET_UP
        void reset_builder();
        void increase_indentation();
        void decrease_indentation();
        // GENERAL_TEXT
        virtual void append_text(std::string const& text);
        virtual void append_new_line();
        virtual void append_space();
        // BRACKETS, SEPARATORS, OPERATORS
        virtual void write_opening_curl_bracket() {}
        virtual void write_right_bracket(std::string const& br);
        virtual void write_left_bracket(std::string const& br);
        virtual void write_separator(std::string const& sep);
        virtual void write_operator(std::string const& op);
        virtual void write_assign_operator();
        virtual void write_modulo_operator();
        virtual void write_address_operator() {}
        virtual void write_deref_operator() {}
        // UNKNOWN_PHRASES
        virtual void write_unknown_type();
        virtual void write_unknown_expr();
        virtual void write_unknown_stmt();
        virtual void write_invalid_type();
        virtual void write_invalid_expr();
        virtual void write_invalid_stmt();
        // ACCESS_MODIFIERS
        virtual void write_public_word();
        virtual void write_protected_word();
        virtual void write_private_word();
        virtual void write_internal_word() {}
        virtual void write_attributes_word() {}
        virtual void write_constructors_word() {}
        virtual void write_destructors_word() {}
        virtual void write_methods_word() {}
        // DATA_TYPES
        virtual void write_dynamic_type_word() {}
        virtual void write_int_type_word();
        virtual void write_float_type_word() {}
        virtual void write_char_type_word();
        virtual void write_bool_type_word() {}
        virtual void write_void_type_word();
        virtual void write_class_type(std::string const& name);
        virtual void write_interface_type(std::string const& name);
        // REFERENCE_NAMES
        virtual void write_scope_name(std::string const& name);
        virtual void write_gen_param_name(std::string const& name);
        virtual void write_class_name(std::string const& name);
        virtual void write_interface_name(std::string const& name);
        virtual void write_method_name(std::string const& name);
        virtual void write_function_name(std::string const& name);
        virtual void write_global_var_name(std::string const& name);
        virtual void write_member_var_name(std::string const& name);
        virtual void write_local_var_name(std::string const& name);
        virtual void write_param_var_name(std::string const& name);
        // VALUES
        virtual void write_int_val(int const& val);
        virtual void write_float_val(float const& val);
        virtual void write_char_val(char const& val);
        virtual void write_string_val(std::string const& val);
        virtual void write_bool_val(bool const& val);
        virtual void write_null_val() {}
        // SYSTEM_EXPRESSIONS
        virtual void write_scope_word() {}
        virtual void write_class_word();
        virtual void write_interface_word() {}
        virtual void write_implement_word() {}
        virtual void write_extend_word() {}
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
        virtual void write_switch_word();
        virtual void write_case_word();
        virtual void write_default_word();
        virtual void write_new_word();
        virtual void write_delete_word() {}
        virtual void write_pointer_word() {}
        virtual void write_override_word();
        virtual void write_virtual_word() {}
        virtual void write_abstract_word() {}
        virtual void write_template_word() {}
        // OTHER_EXPRESSIONS
        virtual void write_constructor_word() {}
        virtual void write_destructor_word() {}
        virtual void write_method_word() {}
        virtual void write_function_word() {}
        virtual void write_lambda_word() {}
        virtual void write_call_word() {}
        virtual void write_define_word() {}
        virtual void write_returns_word() {}
        virtual void write_repeat_word() {}
    };
}

#endif