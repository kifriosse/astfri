#include <libastfri-text/inc/code/CxxCodeBuilder.hpp>

using namespace astfri::text;

CxxCodeBuilder& CxxCodeBuilder::get_instatnce()
{
    static CxxCodeBuilder builder;
    return builder;
}

//
// BRACKETS, SEPARATORS, OPERATORS
//

void CxxCodeBuilder::write_opening_curl_bracket()
{
    append_new_line();
    append_text("{");
    append_new_line();
    ++indentationLevel_;
}