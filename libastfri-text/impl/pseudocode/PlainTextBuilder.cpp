#include <libastfri-text/inc/pseudocode/PlainTextBuilder.hpp>

using namespace astfri::text;

void PlainTextBuilder::write_space()
{
    m_buildedText.append(" ");
}

void PlainTextBuilder::write_new_line()
{
    m_buildedText.append("\n");
    m_isEmptyLine = true;
    ++m_rowCount;
}

void PlainTextBuilder::write_unknown_type()
{
    write_text(m_config.unknownTypeWord);
}

void PlainTextBuilder::write_unknown_expr()
{
    write_text(m_config.unknownExprWord);
}

void PlainTextBuilder::write_unknown_stmt()
{
    write_text(m_config.unknownStmtWord);
}

void PlainTextBuilder::write_left_bracket(std::string_view br)
{
    write_text(br);
}

void PlainTextBuilder::write_right_bracket(std::string_view br)
{
    write_text(br);
}

void PlainTextBuilder::write_operator(std::string_view op)
{
    write_text(op);
}

void PlainTextBuilder::write_separator(std::string_view sep)
{
    write_text(sep);
}

void PlainTextBuilder::write_assign_operator()
{
    write_text(m_config.assignWord);
}

void PlainTextBuilder::write_modulo_operator()
{
    write_text(m_config.moduloWord);
}

void PlainTextBuilder::write_address_operator()
{
    write_text(m_config.addressWord);
}

void PlainTextBuilder::write_deref_operator()
{
    write_text(m_config.derefWord);
}

void PlainTextBuilder::write_public_word()
{
    write_text(m_config.publicWord);
}

void PlainTextBuilder::write_protected_word()
{
    write_text(m_config.protectedWord);
}

void PlainTextBuilder::write_private_word()
{
    write_text(m_config.privateWord);
}

void PlainTextBuilder::write_internal_word()
{
    write_text(m_config.internalWord);
}

void PlainTextBuilder::write_attributes_word()
{
    write_text(m_config.attributesWord);
}

void PlainTextBuilder::write_constructors_word()
{
    write_text(m_config.constructorsWord);
}

void PlainTextBuilder::write_destructors_word()
{
    write_text(m_config.destructorsWord);
}

void PlainTextBuilder::write_methods_word()
{
    write_text(m_config.methodsWord);
}

void PlainTextBuilder::write_dynamic_type_word()
{
    write_text(m_config.dynamicTypeWord);
}

void PlainTextBuilder::write_int_type_word()
{
    write_text(m_config.intTypeWord);
}

void PlainTextBuilder::write_float_type_word()
{
    write_text(m_config.floatTypeWord);
}

void PlainTextBuilder::write_char_type_word()
{
    write_text(m_config.charTypeWord);
}

void PlainTextBuilder::write_bool_type_word()
{
    write_text(m_config.boolTypeWord);
}

void PlainTextBuilder::write_void_type_word()
{
    write_text(m_config.voidTypeWord);
}

void PlainTextBuilder::write_system_type(std::string_view name)
{
    write_text(name);
}

void PlainTextBuilder::write_scope_name(std::string_view name)
{
    write_text(name);
}

void PlainTextBuilder::write_template_name(std::string_view name)
{
    write_text(name);
}

void PlainTextBuilder::write_class_name(std::string_view name)
{
    write_text(name);
}

void PlainTextBuilder::write_interface_name(std::string_view name)
{
    write_text(name);
}

void PlainTextBuilder::write_method_name(std::string_view name)
{
    write_text(name);
}

void PlainTextBuilder::write_function_name(std::string_view name)
{
    write_text(name);
}

void PlainTextBuilder::write_global_var_name(std::string_view name)
{
    write_text(name);
}

void PlainTextBuilder::write_member_var_name(std::string_view name)
{
    write_text(name);
}

void PlainTextBuilder::write_local_var_name(std::string_view name)
{
    write_text(name);
}

void PlainTextBuilder::write_param_var_name(std::string_view name)
{
    write_text(name);
}

void PlainTextBuilder::write_null_val()
{
    write_text(m_config.nullWord);
}

void PlainTextBuilder::write_int_val(int const& val)
{
    write_text(std::to_string(val));
}

void PlainTextBuilder::write_float_val(float const& val)
{
    write_text(std::to_string(val));
}

void PlainTextBuilder::write_char_val(char const& val)
{
    write_text("'" + std::string(1, val) + "'");
}

void PlainTextBuilder::write_string_val(std::string_view val)
{
    //write_text("\"" + val + "\""); // TODO fix
    write_text(val);
}

void PlainTextBuilder::write_bool_val(bool const& val)
{
    if (val)
    {
        write_text(m_config.trueWord);
    }
    else
    {
        write_text(m_config.falseWord);
    }
}

void PlainTextBuilder::write_scope_word()
{
    write_text(m_config.scopeWord);
}

void PlainTextBuilder::write_template_word()
{
    write_text(m_config.templateWord);
}

void PlainTextBuilder::write_class_word()
{
    write_text(m_config.classWord);
}

void PlainTextBuilder::write_interface_word()
{
    write_text(m_config.interfaceWord);
}

void PlainTextBuilder::write_implement_word()
{
    write_text(m_config.implementWord);
}

void PlainTextBuilder::write_extend_word()
{
    write_text(m_config.extendWord);
}

void PlainTextBuilder::write_virtual_word()
{
    write_text(m_config.virtualWord);
}

void PlainTextBuilder::write_abstract_word()
{
    write_text(m_config.abstractWord);
}

void PlainTextBuilder::write_static_word()
{
    write_text(m_config.staticWord);
}

void PlainTextBuilder::write_override_word()
{
    write_text(m_config.overrideWord);
}

void PlainTextBuilder::write_this_word()
{
    write_text(m_config.thisWord);
}

void PlainTextBuilder::write_new_word()
{
    write_text(m_config.newWord);
}

void PlainTextBuilder::write_delete_word()
{
    write_text(m_config.deleteWord);
}

void PlainTextBuilder::write_return_word()
{
    write_text(m_config.returnWord);
}

void PlainTextBuilder::write_continue_word()
{
    write_text(m_config.continueWord);
}

void PlainTextBuilder::write_break_word()
{
    write_text(m_config.breakWord);
}

void PlainTextBuilder::write_if_word()
{
    write_text(m_config.ifWord);
}

void PlainTextBuilder::write_elseif_word()
{
    write_text(m_config.elseifWord);
}

void PlainTextBuilder::write_else_word()
{
    write_text(m_config.elseWord);
}

void PlainTextBuilder::write_switch_word()
{
    write_text(m_config.switchWord);
}

void PlainTextBuilder::write_case_word()
{
    write_text(m_config.caseWord);
}

void PlainTextBuilder::write_default_word()
{
    write_text(m_config.defaultWord);
}

void PlainTextBuilder::write_do_word()
{
    write_text(m_config.doWord);
}

void PlainTextBuilder::write_while_word()
{
    write_text(m_config.whileWord);
}

void PlainTextBuilder::write_for_word()
{
    write_text(m_config.forWord);
}

void PlainTextBuilder::write_foreach_word()
{
    write_text(m_config.foreachWord);
}

void PlainTextBuilder::write_try_word()
{
    write_text(m_config.tryWord);
}

void PlainTextBuilder::write_catch_word()
{
    write_text(m_config.catchWord);
}

void PlainTextBuilder::write_throw_word()
{
    write_text(m_config.throwWord);
}

void PlainTextBuilder::write_pointer_word()
{
    write_text(m_config.pointerWord);
}

void PlainTextBuilder::write_constructor_word()
{
    write_text(m_config.constructorWord);
}

void PlainTextBuilder::write_destructor_word()
{
    write_text(m_config.destructorWord);
}

void PlainTextBuilder::write_method_word()
{
    write_text(m_config.methodWord);
}

void PlainTextBuilder::write_function_word()
{
    write_text(m_config.functionWord);
}

void PlainTextBuilder::write_lambda_word()
{
    write_text(m_config.lambdaWord);
}

void PlainTextBuilder::write_call_word()
{
    write_text(m_config.callWord);
}

void PlainTextBuilder::write_define_word()
{
    write_text(m_config.defineWord);
}

void PlainTextBuilder::write_returns_word()
{
    write_text(m_config.returnsWord);
}

void PlainTextBuilder::write_repeat_word()
{
    write_text(m_config.repeatWord);
}