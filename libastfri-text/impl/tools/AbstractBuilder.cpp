#include <libastfri-text/inc/tools/AbstractBuilder.hpp>

using namespace astfri::text;

std::string& AbstractBuilder::get_builded_text()
{
    return m_buildedText;
}

void AbstractBuilder::increase_indentation()
{
    ++m_indentationLevel;
}

void AbstractBuilder::decrease_indentation()
{
    --m_indentationLevel;
}

void AbstractBuilder::write_opening_curl_bracket(bool const& onNewLine, bool incIndent)
{
    if (onNewLine)
    {
        write_new_line();
    }
    else
    {
        write_space();
    }
    write_left_bracket("{");
    write_new_line();
    if (incIndent)
    {
        ++m_indentationLevel;
    }
}

void AbstractBuilder::write_closing_curl_bracket(bool decIndent)
{
    write_new_line();
    if (decIndent)
    {
        --m_indentationLevel;
    }
    write_right_bracket("}");
}

void AbstractBuilder::write_comma_space()
{
    write_separator(",");
    write_space();
}