#include <libastfri-text/inc/pseudocode/AbstractTextBuilder.hpp>

using namespace astfri::text;

//
// SET_UP
//

void AbstractTextBuilder::reset_builder()
{
    rowCount_ = 1;
    AbstractBuilder::reset_builder();
}

//
// GENERAL_TEXT
//

void AbstractTextBuilder::write_text(std::string_view text)
{
    if (m_isEmptyLine)
    {
        for (int i = 0; i < configurator_->textMarginLeftLen; ++i)
        {
            write_space();
        }
        for (int i = 0; i < m_indentationLevel * configurator_->tabulatorLength; ++i)
        {
            write_space();
        }
        m_isEmptyLine = false;
    }
    m_buildedText.append(text);
}

void AbstractTextBuilder::write_opening_curl_bracket()
{
    if (true/*configurator_->new_line_curl_bracket()*/) // TODO repair
    {
        write_new_line();
    }
    else {
        write_space();
    }
    write_left_bracket("{");
    write_new_line();
    ++m_indentationLevel;
}

void AbstractTextBuilder::write_opening_else_word()
{
    if (configurator_->elseConditionNewLine) // TODO repair
    {
        write_new_line();
    }
    else {
        write_space();
    }
    write_else_word();
    write_opening_curl_bracket();
}