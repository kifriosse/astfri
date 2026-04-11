#include <libastfri-text/inc/general/AbstractBuilder.hpp>

using namespace astfri::text;

void AbstractBuilder::reset_builder()
{
    m_buildedText.clear();
    m_indentationLevel = 0;
    m_isEmptyLine = true;
}

void AbstractBuilder::increase_indentation()
{
    ++m_indentationLevel;
}

void AbstractBuilder::decrease_indentation()
{
    --m_indentationLevel;
}