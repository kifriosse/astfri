#include <libastfri-text/inc/pseudocode/PseudocodeBuilder.hpp>

using namespace astfri::text;

void PseudocodeBuilder::reset_pseudocode_builder()
{
    reset_text_builder();
    m_rowCount = 1;
}

void PseudocodeBuilder::write_opening_else_word()
{
    if (m_config->elseConditionNewLine)
    {
        write_new_line();
    }
    else
    {
        write_space();
    }
    write_else_word();
    write_opening_curl_bracket(m_config->conditionBlockBracketNewLine);
}

void PseudocodeBuilder::write_text(std::string_view text)
{
    if (m_isEmptyLine)
    {
        for (int i = 0; i < m_config->textMarginLeft; ++i)
        {
            write_space();
        }
        for (int i = 0; i < m_indentationLevel * m_config->tabulatorLength; ++i)
        {
            write_space();
        }
        m_isEmptyLine = false;
    }
    m_buildedText.append(text);
}