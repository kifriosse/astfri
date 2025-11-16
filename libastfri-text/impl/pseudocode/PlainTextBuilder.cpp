#include <libastfri-text/inc/pseudocode/PlainTextBuilder.hpp>

using namespace astfri::text;

PlainTextBuilder& PlainTextBuilder::get_instance()
{
    static PlainTextBuilder builder;
    return builder;
}

//
// GENERAL_TEXT
//

void PlainTextBuilder::append_new_line()
{
    *buildedText_ << "\n";
    isEmptyLine_ = true;
    ++rowCount_;
}

void PlainTextBuilder::append_space()
{
    *buildedText_ << " ";
}

void PlainTextBuilder::write_separator(std::string const& sep)
{
    append_text(sep);
}

void PlainTextBuilder::write_right_bracket(std::string const& br)
{
    append_text(br);
}

void PlainTextBuilder::write_left_bracket(std::string const& br)
{
    append_text(br);
}

//
// OPERATORS
//

void PlainTextBuilder::write_operator(std::string const& op)
{
    append_text(op);
}

void PlainTextBuilder::write_assign_operator()
{
    append_text(configurator_->assign_op_word()->str());
}

void PlainTextBuilder::write_modulo_operator()
{
    append_text(configurator_->modulo_op_word()->str());
}

void PlainTextBuilder::write_address_operator()
{
    append_text(configurator_->address_op_word()->str());
}

void PlainTextBuilder::write_deref_operator()
{
    append_text(configurator_->deref_op_word()->str());
}

//
// UNKNOWN_PHRASES
//

void PlainTextBuilder::write_unknown_type()
{
    append_text(configurator_->unknown_type_word()->str());
}

void PlainTextBuilder::write_unknown_expr()
{
    append_text(configurator_->unknown_expr_word()->str());
}

void PlainTextBuilder::write_unknown_stmt()
{
    append_text(configurator_->unknown_stmt_word()->str());
}

void PlainTextBuilder::write_invalid_type()
{
    append_text(configurator_->invalid_type_word()->str());
}

void PlainTextBuilder::write_invalid_expr()
{
    append_text(configurator_->invalid_expr_word()->str());
}

void PlainTextBuilder::write_invalid_stmt()
{
    append_text(configurator_->invalid_stmt_word()->str());
}

//
// ACCESS_MODIFIERS
//

void PlainTextBuilder::write_public_word()
{
    append_text(configurator_->public_word()->str());
}

void PlainTextBuilder::write_protected_word()
{
    append_text(configurator_->protected_word()->str());
}

void PlainTextBuilder::write_private_word()
{
    append_text(configurator_->private_word()->str());
}

void PlainTextBuilder::write_internal_word()
{
    append_text(configurator_->internal_word()->str());
}

void PlainTextBuilder::write_attributes_word()
{
    append_text(configurator_->attributes_word()->str());
}

void PlainTextBuilder::write_constructors_word()
{
    append_text(configurator_->constructors_word()->str());
}

void PlainTextBuilder::write_destructors_word()
{
    append_text(configurator_->destructors_word()->str());
}

void PlainTextBuilder::write_methods_word()
{
    append_text(configurator_->methods_word()->str());
}

//
// DATA_TYPES
//

void PlainTextBuilder::write_dynamic_type_word()
{
    append_text(configurator_->dynamic_type_word()->str());
}

void PlainTextBuilder::write_int_type_word()
{
    append_text(configurator_->int_type_word()->str());
}

void PlainTextBuilder::write_float_type_word()
{
    append_text(configurator_->float_type_word()->str());
}

void PlainTextBuilder::write_char_type_word()
{
    append_text(configurator_->char_type_word()->str());
}

void PlainTextBuilder::write_bool_type_word()
{
    append_text(configurator_->bool_type_word()->str());
}

void PlainTextBuilder::write_void_type_word()
{
    append_text(configurator_->void_type_word()->str());
}

void PlainTextBuilder::write_class_type(std::string const& name)
{
    append_text(name);
}

void PlainTextBuilder::write_interface_type(std::string const& name)
{
    append_text(name);
}

//
// REFERENCE_NAMES
//

void PlainTextBuilder::write_scope_name(std::string const& name)
{
    append_text(name);
}

void PlainTextBuilder::write_gen_param_name(std::string const& name)
{
    append_text(name);
}

void PlainTextBuilder::write_class_name(std::string const& name)
{
    append_text(name);
}

void PlainTextBuilder::write_interface_name(std::string const& name)
{
    append_text(name);
}

void PlainTextBuilder::write_method_name(std::string const& name)
{
    append_text(name);
}

void PlainTextBuilder::write_function_name(std::string const& name)
{
    append_text(name);
}

void PlainTextBuilder::write_global_var_name(std::string const& name)
{
    append_text(name);
}

void PlainTextBuilder::write_member_var_name(std::string const& name)
{
    append_text(name);
}

void PlainTextBuilder::write_local_var_name(std::string const& name)
{
    append_text(name);
}

void PlainTextBuilder::write_param_var_name(std::string const& name)
{
    append_text(name);
}

//
// VALUES
//

void PlainTextBuilder::write_int_val(int const& val)
{
    append_text(std::to_string(val));
}

void PlainTextBuilder::write_float_val(float const& val)
{
    append_text(std::to_string(val) + "f");
}

void PlainTextBuilder::write_char_val(char const& val)
{
    append_text("'" + std::string(1, val) + "'");
}

void PlainTextBuilder::write_string_val(std::string const& val)
{
    append_text("\"" + val + "\"");
}

void PlainTextBuilder::write_bool_val(bool const& val)
{
    if (val) {
        append_text("true");
    } else {
        append_text("false");
    }
}

void PlainTextBuilder::write_null_val()
{
    append_text(configurator_->null_val_word()->str());
}

//
// SYSTEM_EXPRESSIONS
//

void PlainTextBuilder::write_scope_word()
{
    append_text(configurator_->scope_word()->str());
}

void PlainTextBuilder::write_class_word()
{
    append_text(configurator_->class_word()->str());
}

void PlainTextBuilder::write_interface_word()
{
    append_text(configurator_->interface_word()->str());
}

void PlainTextBuilder::write_implement_word()
{
    append_text(configurator_->implement_word()->str());
}

void PlainTextBuilder::write_extend_word()
{
    append_text(configurator_->extend_word()->str());
}

void PlainTextBuilder::write_this_word()
{
    append_text(configurator_->this_word()->str());
}

void PlainTextBuilder::write_return_word()
{
    append_text(configurator_->return_word()->str());
}

void PlainTextBuilder::write_continue_word()
{
    append_text(configurator_->continue_word()->str());
}

void PlainTextBuilder::write_break_word()
{
    append_text(configurator_->break_word()->str());
}

void PlainTextBuilder::write_throw_word()
{
    append_text(configurator_->throw_word()->str());
}

void PlainTextBuilder::write_if_word()
{
    append_text(configurator_->if_word()->str());
}

void PlainTextBuilder::write_else_word()
{
    append_text(configurator_->else_word()->str());
}

void PlainTextBuilder::write_do_word()
{
    append_text(configurator_->do_word()->str());
}

void PlainTextBuilder::write_while_word()
{
    append_text(configurator_->while_word()->str());
}

void PlainTextBuilder::write_for_word()
{
    append_text(configurator_->for_word()->str());
}

void PlainTextBuilder::write_switch_word()
{
    append_text(configurator_->switch_word()->str());
}

void PlainTextBuilder::write_case_word()
{
    append_text(configurator_->case_word()->str());
}

void PlainTextBuilder::write_default_word()
{
    append_text(configurator_->default_word()->str());
}

void PlainTextBuilder::write_new_word()
{
    append_text(configurator_->new_word()->str());
}

void PlainTextBuilder::write_delete_word()
{
    append_text(configurator_->delete_word()->str());
}

void PlainTextBuilder::write_pointer_word()
{
    append_text(configurator_->pointer_word()->str());
}

void PlainTextBuilder::write_override_word()
{
    append_text(configurator_->override_word()->str());
}

void PlainTextBuilder::write_virtual_word()
{
    append_text(configurator_->virtual_word()->str());
}

void PlainTextBuilder::write_abstract_word()
{
    append_text(configurator_->abstract_word()->str());
}

void PlainTextBuilder::write_template_word()
{
    append_text(configurator_->template_word()->str());
}

//
// OTHER_EXPRESSIONS
//

void PlainTextBuilder::write_constructor_word()
{
    append_text(configurator_->constructor_word()->str());
}

void PlainTextBuilder::write_destructor_word()
{
    append_text(configurator_->destructor_word()->str());
}

void PlainTextBuilder::write_method_word()
{
    append_text(configurator_->method_word()->str());
}

void PlainTextBuilder::write_function_word()
{
    append_text(configurator_->function_word()->str());
}

void PlainTextBuilder::write_lambda_word()
{
    append_text(configurator_->lambda_word()->str());
}

void PlainTextBuilder::write_call_word()
{
    append_text(configurator_->call_word()->str());
}

void PlainTextBuilder::write_define_word()
{
    append_text(configurator_->define_word()->str());
}

void PlainTextBuilder::write_returns_word()
{
    append_text(configurator_->returns_word()->str());
}

void PlainTextBuilder::write_repeat_word()
{
    append_text(configurator_->repeat_word()->str());
}