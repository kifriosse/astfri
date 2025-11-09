#include <libastfri-text/inc/code/CxxCodeBuilder.hpp>

using namespace astfri::text;

CxxCodeBuilder& CxxCodeBuilder::get_instatnce()
{
    static CxxCodeBuilder builder;
    return builder;
}

//
// GENERAL_TEXT
//

void CxxCodeBuilder::write_opening_curl_bracket()
{
    append_new_line();
    append_text("{");
    append_new_line();
    ++indentationLevel_;
}

void CxxCodeBuilder::write_opening_else_word()
{
    append_new_line();
    append_text("else");
    append_new_line();
    write_opening_curl_bracket();
}