#include <libastfri-text/inc/code/JavaCodeBuilder.hpp>

using namespace astfri::text;

JavaCodeBuilder& JavaCodeBuilder::get_instance()
{
    static JavaCodeBuilder builder;
    return builder;
}

//
// GENERAL_TEXT
//

void JavaCodeBuilder::write_opening_curl_bracket()
{
    append_space();
    append_text("{");
    append_new_line();
    ++indentationLevel_;
}

void JavaCodeBuilder::write_opening_else_word()
{
    append_space();
    append_text("else");
    write_opening_curl_bracket();
}