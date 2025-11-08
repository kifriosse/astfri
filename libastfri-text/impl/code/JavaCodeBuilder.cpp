#include <libastfri-text/inc/code/JavaCodeBuilder.hpp>

using namespace astfri::text;

JavaCodeBuilder& JavaCodeBuilder::get_instance()
{
    static JavaCodeBuilder builder;
    return builder;
}

//
// BRACKETS, SEPARATORS, OPERATORS
//

void JavaCodeBuilder::write_opening_curl_bracket()
{
    append_space();
    append_text("{");
    append_new_line();
    ++indentationLevel_;
}