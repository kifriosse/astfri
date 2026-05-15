#include <libastfri-text/inc/tools/TextBuilder.hpp>

using namespace astfri::text;

std::string& TextBuilder::get_builded_text()
{
    return m_buildedText;
}

void TextBuilder::reset_text_builder()
{
    m_buildedText.clear();
    m_indentationLevel = 0;
    m_isEmptyLine      = true;
}

void TextBuilder::increase_indentation()
{
    ++m_indentationLevel;
}

void TextBuilder::decrease_indentation()
{
    --m_indentationLevel;
}

void TextBuilder::write_opening_curl_bracket(bool const& onNewLine, bool incIndent)
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

void TextBuilder::write_closing_curl_bracket(bool decIndent)
{
    write_new_line();
    if (decIndent)
    {
        --m_indentationLevel;
    }
    write_right_bracket("}");
}

void TextBuilder::write_comma_space()
{
    write_separator(",");
    write_space();
}