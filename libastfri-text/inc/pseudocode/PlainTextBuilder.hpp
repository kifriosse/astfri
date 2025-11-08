#ifndef LIBASTFRI_TEXT_PLAIN_TEXT_BUILDER
#define LIBASTFRI_TEXT_PLAIN_TEXT_BUILDER

#include <libastfri-text/inc/AbstractTextBuilder.hpp>
#include <libastfri-text/inc/pseudocode/TextConfigurator.hpp>

namespace astfri::text
{
    class PlainTextBuilder : public AbstractTextBuilder
    {
    private:
        TextConfigurator* configurator_;
        int rowCount_;
    public:
        explicit PlainTextBuilder();
        ~PlainTextBuilder() = default;
    public:
        // SET_UP
        void reset_builder();
        // GENERAL_TEXT
        void append_text(std::string const& text) override;
        void append_new_line() override;
        // BRACKETS, SEPARATORS, OPERATORS
        void write_opening_curl_bracket() override;
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
        // VALUES
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