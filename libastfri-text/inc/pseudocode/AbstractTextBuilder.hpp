#ifndef LIBASTFRI_TEXT_ABSTRACT_TEXT_BUILDER
#define LIBASTFRI_TEXT_ABSTRACT_TEXT_BUILDER

#include <libastfri-text/inc/general/AbstractBuilder.hpp>
#include <libastfri-text/inc/settings/TextConfig.hpp>

namespace astfri::text
{
    class AbstractTextBuilder : public AbstractBuilder
    {
    protected:
        TextConfig m_config;
        int m_rowCount = 1;
    public:
        virtual ~AbstractTextBuilder() = default;
        //
        void reset_builder();
        // GENERAL_TEXT
        void write_text(std::string_view text) override;
        void write_opening_else_word();
        void write_opening_catch_word();
        // OPERATORS
        virtual void write_operator(std::string_view op) = 0;
        virtual void write_assign_operator()  = 0;
        virtual void write_modulo_operator()  = 0;
        virtual void write_address_operator() = 0;
        virtual void write_deref_operator()   = 0;
        // ACCESS_MODIFIERS
        virtual void write_public_word()       = 0;
        virtual void write_protected_word()    = 0;
        virtual void write_private_word()      = 0;
        virtual void write_internal_word()     = 0;
        virtual void write_attributes_word()   = 0;
        virtual void write_constructors_word() = 0;
        virtual void write_destructors_word()  = 0;
        virtual void write_methods_word()      = 0;
        // DATA_TYPES
        virtual void write_dynamic_type_word() = 0;
        virtual void write_int_type_word()     = 0;
        virtual void write_float_type_word()   = 0;
        virtual void write_char_type_word()    = 0;
        virtual void write_bool_type_word()    = 0;
        virtual void write_void_type_word()    = 0;
        virtual void write_system_type(std::string_view name) = 0;
        // REFERENCE_NAMES
        virtual void write_scope_name(std::string_view name)      = 0;
        virtual void write_template_name(std::string_view name)   = 0;
        virtual void write_class_name(std::string_view name)      = 0;
        virtual void write_interface_name(std::string_view name)  = 0;
        virtual void write_method_name(std::string_view name)     = 0;
        virtual void write_function_name(std::string_view name)   = 0;
        virtual void write_global_var_name(std::string_view name) = 0;
        virtual void write_member_var_name(std::string_view name) = 0;
        virtual void write_local_var_name(std::string_view name)  = 0;
        virtual void write_param_var_name(std::string_view name)  = 0;
        // VALUES
        virtual void write_null_val()                       = 0;
        virtual void write_int_val(int const& val)          = 0;
        virtual void write_float_val(float const& val)      = 0;
        virtual void write_char_val(char const& val)        = 0;
        virtual void write_string_val(std::string_view val) = 0;
        virtual void write_bool_val(bool const& val)        = 0;
        // SYSTEM_EXPRESSIONS
        virtual void write_scope_word()     = 0;
        virtual void write_template_word()  = 0;
        virtual void write_class_word()     = 0;
        virtual void write_interface_word() = 0;
        virtual void write_implement_word() = 0;
        virtual void write_extend_word()    = 0;
        virtual void write_virtual_word()   = 0;
        virtual void write_abstract_word()  = 0;
        virtual void write_static_word()    = 0;
        virtual void write_override_word()  = 0;
        virtual void write_this_word()      = 0;
        virtual void write_new_word()       = 0;
        virtual void write_delete_word()    = 0;
        virtual void write_return_word()    = 0;
        virtual void write_continue_word()  = 0;
        virtual void write_break_word()     = 0;
        virtual void write_if_word()        = 0;
        virtual void write_elseif_word()    = 0;
        virtual void write_else_word()      = 0;
        virtual void write_switch_word()    = 0;
        virtual void write_case_word()      = 0;
        virtual void write_default_word()   = 0;
        virtual void write_do_word()        = 0;
        virtual void write_while_word()     = 0;
        virtual void write_for_word()       = 0;
        virtual void write_foreach_word()   = 0;
        virtual void write_try_word()       = 0;
        virtual void write_catch_word()     = 0;
        virtual void write_throw_word()     = 0;
        virtual void write_pointer_word()   = 0;
        // OTHER_EXPRESSIONS
        virtual void write_constructor_word() = 0;
        virtual void write_destructor_word()  = 0;
        virtual void write_method_word()      = 0;
        virtual void write_function_word()    = 0;
        virtual void write_lambda_word()      = 0;
        virtual void write_call_word()        = 0;
        virtual void write_define_word()      = 0;
        virtual void write_returns_word()     = 0;
        virtual void write_repeat_word()      = 0;
    };
}

#endif