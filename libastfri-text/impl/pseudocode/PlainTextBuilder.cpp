#include <libastfri-text/inc/pseudocode/PlainTextBuilder.hpp>
#include <libastfri-text/inc/pseudocode/PlainTextExporter.hpp>

using namespace astfri::text;

PlainTextBuilder& PlainTextBuilder::get_instance() {
    static PlainTextBuilder builder;
    return builder;
}

//
// EXPORT
//

void PlainTextBuilder::export_text() {
    //PlainTextExporter(rowCount_, m_buildedText);
    reset_builder();
}

//
// GENERAL_TEXT
//

void PlainTextBuilder::write_new_line() {
    m_buildedText.append("\n");
    m_isEmptyLine = true;
    ++rowCount_;
}

void PlainTextBuilder::write_space() {
    m_buildedText.append(" ");
}

void PlainTextBuilder::write_separator(std::string_view sep) {
    write_text(sep);
}

void PlainTextBuilder::write_right_bracket(std::string_view br) {
    write_text(br);
}

void PlainTextBuilder::write_left_bracket(std::string_view br) {
    write_text(br);
}

//
// OPERATORS
//

void PlainTextBuilder::write_operator(std::string_view op) {
    write_text(op);
}

void PlainTextBuilder::write_assign_operator() {
    write_text(configurator_->assignOpWord);
}

void PlainTextBuilder::write_modulo_operator() {
    write_text(configurator_->moduloOpWord);
}

void PlainTextBuilder::write_address_operator() {
    write_text(configurator_->addressOpWord);
}

void PlainTextBuilder::write_deref_operator() {
    write_text(configurator_->derefOpWord);
}

//
// UNKNOWN_PHRASES
//

void PlainTextBuilder::write_unknown_type() {
    write_text(configurator_->unknownTypeWord);
}

void PlainTextBuilder::write_unknown_expr() {
    write_text(configurator_->unknownExprWord);
}

void PlainTextBuilder::write_unknown_stmt() {
    write_text(configurator_->unknownStmtWord);
}

//
// ACCESS_MODIFIERS
//

void PlainTextBuilder::write_public_word() {
    write_text(configurator_->publicWord);
}

void PlainTextBuilder::write_protected_word() {
    write_text(configurator_->protectedWord);
}

void PlainTextBuilder::write_private_word() {
    write_text(configurator_->privateWord);
}

void PlainTextBuilder::write_internal_word() {
    write_text(configurator_->internalWord);
}

void PlainTextBuilder::write_attributes_word() {
    write_text(configurator_->attributesWord);
}

void PlainTextBuilder::write_constructors_word() {
    write_text(configurator_->constructorsWord);
}

void PlainTextBuilder::write_destructors_word() {
    write_text(configurator_->destructorsWord);
}

void PlainTextBuilder::write_methods_word() {
    write_text(configurator_->methodsWord);
}

//
// DATA_TYPES
//

void PlainTextBuilder::write_dynamic_type_word() {
    write_text(configurator_->dynamicTypeWord);
}

void PlainTextBuilder::write_int_type_word() {
    write_text(configurator_->intTypeWord);
}

void PlainTextBuilder::write_float_type_word() {
    write_text(configurator_->floatTypeWord);
}

void PlainTextBuilder::write_char_type_word() {
    write_text(configurator_->charTypeWord);
}

void PlainTextBuilder::write_bool_type_word() {
    write_text(configurator_->boolTypeWord);
}

void PlainTextBuilder::write_void_type_word() {
    write_text(configurator_->voidTypeWord);
}

void PlainTextBuilder::write_class_type(std::string_view name) {
    write_text(name);
}

void PlainTextBuilder::write_interface_type(std::string_view name) {
    write_text(name);
}

//
// REFERENCE_NAMES
//

void PlainTextBuilder::write_scope_name(std::string_view name) {
    write_text(name);
}

void PlainTextBuilder::write_gen_param_name(std::string_view name) {
    write_text(name);
}

void PlainTextBuilder::write_class_name(std::string_view name) {
    write_text(name);
}

void PlainTextBuilder::write_interface_name(std::string_view name) {
    write_text(name);
}

void PlainTextBuilder::write_method_name(std::string_view name) {
    write_text(name);
}

void PlainTextBuilder::write_function_name(std::string_view name) {
    write_text(name);
}

void PlainTextBuilder::write_global_var_name(std::string_view name) {
    write_text(name);
}

void PlainTextBuilder::write_member_var_name(std::string_view name) {
    write_text(name);
}

void PlainTextBuilder::write_local_var_name(std::string_view name) {
    write_text(name);
}

void PlainTextBuilder::write_param_var_name(std::string_view name) {
    write_text(name);
}

//
// VALUES
//

void PlainTextBuilder::write_int_val(const int& val) {
    write_text(std::to_string(val));
}

void PlainTextBuilder::write_float_val(const float& val) {
    write_text(std::to_string(val) + "f");
}

void PlainTextBuilder::write_char_val(const char& val) {
    write_text("'" + std::string(1, val) + "'");
}

void PlainTextBuilder::write_string_val(std::string_view val) {
    //write_text("\"" + val + "\""); // TODO fix
    write_text(val);
}

void PlainTextBuilder::write_bool_val(const bool& val) {
    if (val) {
        write_text("true");
    }
    else {
        write_text("false");
    }
}

void PlainTextBuilder::write_null_val() {
    write_text(configurator_->nullValWord);
}

//
// SYSTEM_EXPRESSIONS
//

void PlainTextBuilder::write_scope_word() {
    write_text(configurator_->scopeWord);
}

void PlainTextBuilder::write_class_word() {
    write_text(configurator_->classWord);
}

void PlainTextBuilder::write_interface_word() {
    write_text(configurator_->interfaceWord);
}

void PlainTextBuilder::write_implement_word() {
    write_text(configurator_->implementWord);
}

void PlainTextBuilder::write_extend_word() {
    write_text(configurator_->extendWord);
}

void PlainTextBuilder::write_this_word() {
    write_text(configurator_->thisWord);
}

void PlainTextBuilder::write_return_word() {
    write_text(configurator_->returnWord);
}

void PlainTextBuilder::write_continue_word() {
    write_text(configurator_->continueWord);
}

void PlainTextBuilder::write_break_word() {
    write_text(configurator_->breakWord);
}

void PlainTextBuilder::write_throw_word() {
    write_text(configurator_->throwWord);
}

void PlainTextBuilder::write_if_word() {
    write_text(configurator_->ifWord);
}

void PlainTextBuilder::write_else_word() {
    write_text(configurator_->elseWord);
}

void PlainTextBuilder::write_do_word() {
    write_text(configurator_->doWord);
}

void PlainTextBuilder::write_while_word() {
    write_text(configurator_->whileWord);
}

void PlainTextBuilder::write_for_word() {
    write_text(configurator_->forWord);
}

void PlainTextBuilder::write_switch_word() {
    write_text(configurator_->switchWord);
}

void PlainTextBuilder::write_case_word() {
    write_text(configurator_->caseWord);
}

void PlainTextBuilder::write_default_word() {
    write_text(configurator_->defaultWord);
}

void PlainTextBuilder::write_new_word() {
    write_text(configurator_->newWord);
}

void PlainTextBuilder::write_delete_word() {
    write_text(configurator_->deleteWord);
}

void PlainTextBuilder::write_pointer_word() {
    write_text(configurator_->pointerWord);
}

void PlainTextBuilder::write_override_word() {
    write_text(configurator_->overrideWord);
}

void PlainTextBuilder::write_virtual_word() {
    write_text(configurator_->virtualWord);
}

void PlainTextBuilder::write_abstract_word() {
    write_text(configurator_->abstractWord);
}

void PlainTextBuilder::write_template_word() {
    write_text(configurator_->templateWord);
}

//
// OTHER_EXPRESSIONS
//

void PlainTextBuilder::write_constructor_word() {
    write_text(configurator_->constructorWord);
}

void PlainTextBuilder::write_destructor_word() {
    write_text(configurator_->destructorWord);
}

void PlainTextBuilder::write_method_word() {
    write_text(configurator_->methodWord);
}

void PlainTextBuilder::write_function_word() {
    write_text(configurator_->functionWord);
}

void PlainTextBuilder::write_lambda_word() {
    write_text(configurator_->lambdaWord);
}

void PlainTextBuilder::write_call_word() {
    write_text(configurator_->callWord);
}

void PlainTextBuilder::write_define_word() {
    write_text(configurator_->defineWord);
}

void PlainTextBuilder::write_returns_word() {
    write_text(configurator_->returnsWord);
}

void PlainTextBuilder::write_repeat_word() {
    write_text(configurator_->repeatWord);
}