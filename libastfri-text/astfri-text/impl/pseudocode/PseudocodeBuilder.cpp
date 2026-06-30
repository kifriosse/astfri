#include <astfri-text/impl/pseudocode/PseudocodeBuilder.hpp>

using namespace astfri::text;

PseudocodeBuilder::PseudocodeBuilder(Config const& config) :
    m_config(config)
{
}

void PseudocodeBuilder::write_text(std::string_view text)
{
    if (m_isEmptyLine)
    {
        for (int i = 0; i < m_config.textMarginLeft; ++i)
        {
            write_space();
        }
        for (int i = 0; i < m_indentationLevel * m_config.tabulatorLength; ++i)
        {
            write_space();
        }
        m_isEmptyLine = false;
    }
    m_buildedText.append(text);
}