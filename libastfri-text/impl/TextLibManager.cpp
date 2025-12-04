#include <libastfri-text/inc/TextLibManager.hpp>

#include <libastfri-text/inc/pseudocode/PseudocodeVisitor.hpp>
#include <libastfri-text/inc/code/JavaCodeVisitor.hpp>
#include <libastfri-text/inc/code/CxxCodeVisitor.hpp>

using namespace astfri::text;

TextLibManager& TextLibManager::get_instance()
{
    static TextLibManager manager;
    return manager;
}

TextLibManager::TextLibManager() :
    visitor_(&PseudocodeVisitor::get_instance())
{
}

//
// -----
//

void TextLibManager::change_output(OutputFormat const& format)
{
    switch (format)
    {
        case OutputFormat::CxxCode:
            // TODO: implement cxx visitor
            //visitor_ = &CxxCodeVisitor::get_instance();
            break;
        case OutputFormat::JavaCode:
            visitor_ = &JavaCodeVisitor::get_instance();
            break;
        case OutputFormat::Pseudocode:
            visitor_ = &PseudocodeVisitor::get_instance();
            break;
    }
    clear_builder();
}

void TextLibManager::execute_export()
{
    if (PseudocodeVisitor* visitor = dynamic_cast<PseudocodeVisitor*>(visitor_))
    {
        visitor->export_pseudocode();
    }
}

void TextLibManager::clear_builder()
{
    if (PseudocodeVisitor* visitor = dynamic_cast<PseudocodeVisitor*>(visitor_))
    {
        visitor->clear_builder();
    }
}

void TextLibManager::append_text(std::string const& text)
{
    if (PseudocodeVisitor* visitor = dynamic_cast<PseudocodeVisitor*>(visitor_))
    {
        visitor->append_text(text);
    }
}

void TextLibManager::append_new_line()
{
    if (PseudocodeVisitor* visitor = dynamic_cast<PseudocodeVisitor*>(visitor_))
    {
        visitor->append_new_line();
    }
}

void TextLibManager::append_space()
{
    if (PseudocodeVisitor* visitor = dynamic_cast<PseudocodeVisitor*>(visitor_))
    {
        visitor->append_space();
    }
}

void TextLibManager::update_configuration()
{
    if (PseudocodeVisitor* visitor = dynamic_cast<PseudocodeVisitor*>(visitor_))
    {
        visitor->update_configuration();
    }
}

void TextLibManager::reload_configuration()
{
    if (PseudocodeVisitor* visitor = dynamic_cast<PseudocodeVisitor*>(visitor_))
    {
        visitor->reload_configuration();
    }
}