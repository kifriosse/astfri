#ifndef LIBASTFRI_TEXT_EXPORTER
#define LIBASTFRI_TEXT_EXPORTER

#include <libastfri-text/inc/TextConfigurator.hpp>

namespace astfri::text
{
    class Exporter
    {
    protected:
        int rowCount_;
        TextConfigurator* configurator_;
        uptr<ss> outputPseudocode_;
    private:
        int currentIndentationLevel_;
        bool isEmptyLine_;
    public:
        Exporter();
        virtual ~Exporter() = default;
    public:
        virtual void reset();
        void execute_export();
    private:
        virtual void write_pseudocode_into_file(STRING fullfilepath);
    public:
        // GENERAL
        void increase_indentation();
        void decrease_indentation();
        void write_text(std::string const& text);
        void write_new_line();
        void write_space();
        // BRACKETS
        virtual void write_left_bracket(STRING br);
        virtual void write_right_bracket(STRING br);
        // UNKNOWN_PHRASES
        virtual void write_unknown_type_word();
        virtual void write_unknown_expr_word();
        virtual void write_unknown_stmt_word();
        virtual void write_invalid_type_word();
        virtual void write_invalid_expr_word();
        virtual void write_invalid_stmt_word();
        // ACCESS_MODIFIERS
        virtual void write_public_word();
        virtual void write_protected_word();
        virtual void write_private_word();
        virtual void write_internal_word();
        virtual void write_attributes_word();
        virtual void write_constructors_word();
        virtual void write_destructors_word();
        virtual void write_methods_word();
        // DATA_TYPES
        virtual void write_dynamic_type_word();
        virtual void write_int_type_word();
        virtual void write_float_type_word();
        virtual void write_char_type_word();
        virtual void write_bool_type_word();
        virtual void write_void_type_word();
        virtual void write_user_type(std::string usertype);
        // REFERENCE_NAMES
        virtual void write_gen_param_name(std::string name);
        virtual void write_class_name(std::string name);
        virtual void write_interface_name(std::string name);
        virtual void write_method_name(std::string name);
        virtual void write_function_name(std::string name);
        virtual void write_global_var_name(std::string name);
        virtual void write_member_var_name(std::string name);
        virtual void write_local_var_name(std::string name);
        virtual void write_param_var_name(std::string name);
        // OPERATORS
        virtual void write_operator_sign(STRING sign);
        virtual void write_assign_op_word();
        virtual void write_modulo_op_word();
        virtual void write_address_op_word();
        virtual void write_deref_op_word();
        // SEPARATORS
        virtual void write_separator_sign(STRING sign);
        // VALUES
        virtual void write_int_val(int val);
        virtual void write_float_val(float val);
        virtual void write_char_val(char val);
        virtual void write_string_val(std::string val);
        virtual void write_bool_val(bool val);
        virtual void write_null_val();
        // SYSTEM_EXPRESSIONS
        virtual void write_class_word();
        virtual void write_interface_word();
        virtual void write_implement_word();
        virtual void write_extend_word();
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
        virtual void write_abstract_word();
        virtual void write_template_word();
        // OTHER_EXPRESSIONS
        virtual void write_constructor_word();
        virtual void write_destructor_word();
        virtual void write_method_word();
        virtual void write_function_word();
        virtual void write_lambda_word();
        virtual void write_call_word();
        virtual void write_define_word();
        virtual void write_returns_word();
    };
}

#endif