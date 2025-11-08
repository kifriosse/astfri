#include <libastfri-text/inc/code/CxxCodeBuilder.hpp>

using namespace astfri::text;

//
// BRACKETS, SEPARATORS, OPERATORS
//

void CxxCodeBuilder::write_opening_curl_bracket()
{
    append_new_line();
    write_left_bracket("{");
    append_new_line();
    ++indentationLevel_;
}

void CxxCodeBuilder::write_address_operator()
{
    write_operator("&");
}

void CxxCodeBuilder::write_deref_operator()
{
    write_operator("*");
}

//
// DATA_TYPES
//

void CxxCodeBuilder::write_dynamic_type_word()
{
    append_text("auto");
}

void CxxCodeBuilder::write_float_type_word()
{
    append_text("float");
}

void CxxCodeBuilder::write_bool_type_word()
{
    append_text("bool");
}

//
// VALUES
//

void CxxCodeBuilder::write_null_val()
{
    append_text("nullptr");
}

//
// SYSTEM_EXPRESSIONS
//

void CxxCodeBuilder::write_scope_word()
{
    append_text("namespace");
}

void CxxCodeBuilder::write_delete_word()
{
    append_text("delete");
}

void CxxCodeBuilder::write_pointer_word()
{
    append_text("*");
}

void CxxCodeBuilder::write_virtual_word()
{
    append_text("virtual");
}

void CxxCodeBuilder::write_template_word()
{
    append_text("template");
}