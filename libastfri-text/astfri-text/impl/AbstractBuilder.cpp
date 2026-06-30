#include <astfri-text/impl/AbstractBuilder.hpp>

using namespace astfri::text;

void AbstractBuilder::increase_indentation()
{
    ++m_indentationLevel;
}

void AbstractBuilder::decrease_indentation()
{
    --m_indentationLevel;
}

void AbstractBuilder::write_new_line_or_space(NewLineOrCurrent option)
{
    if (option == NEW_LINE)
    {
        write_new_line();
        return;
    }
    write_space();
}

void AbstractBuilder::write_opening_curl_bracket(NewLineOrCurrent option, UsingIndentation decision)
{
    write_new_line_or_space(option);
    write_left_bracket("{");
    write_new_line();
    if (decision)
    {
        ++m_indentationLevel;
    }
}

void AbstractBuilder::write_closing_curl_bracket(UsingIndentation decision)
{
    write_new_line();
    if (decision)
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