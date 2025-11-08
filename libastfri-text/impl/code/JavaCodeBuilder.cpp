#include <libastfri-text/inc/code/JavaCodeBuilder.hpp>

using namespace astfri::text;

//
// BRACKETS, SEPARATORS, OPERATORS
//

void JavaCodeBuilder::write_opening_curl_bracket()
{
    append_space();
    write_left_bracket("{");
    append_new_line();
    ++indentationLevel_;
}

//
// DATA_TYPES
//

void JavaCodeBuilder::write_dynamic_type_word()
{
    append_text("var");
}

void JavaCodeBuilder::write_float_type_word()
{
    append_text("double");
}

void JavaCodeBuilder::write_bool_type_word()
{
    append_text("boolean");
}

//
// VALUES
//

void JavaCodeBuilder::write_null_val()
{
    append_text("null");
}

//
// SYSTEM_EXPRESSIONS
//

void JavaCodeBuilder::write_scope_word()
{
    append_text("package");
}

void JavaCodeBuilder::write_interface_word()
{
    append_text("interface");
}

void JavaCodeBuilder::write_implement_word()
{
    append_text("implements");
}

void JavaCodeBuilder::write_extend_word()
{
    append_text("extends");
}

void JavaCodeBuilder::write_abstract_word()
{
    append_text("abstract");
}