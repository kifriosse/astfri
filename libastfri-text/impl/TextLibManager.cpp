#include <libastfri-text/inc/TextLibManager.hpp>

using namespace astfri::text;

TextLibManager& TextLibManager::get_instance()
{
    static TextLibManager manager;
    return manager;
}

TextLibManager::TextLibManager()
{
    /*configurator_ = &TextConfigurator::get_instance();
    currentOutputFileFormat_ = configurator_->output_file_format()->str();
    create_new_exporter();
    visitor_ = &PseudocodeVisitor::get_instance(exporter_);*/
}

TextLibManager::~TextLibManager()
{
    delete builder_;
}

/*void TextLibManager::update_configuration()
{
    configurator_->update_configuration();
    check_current_file_format();
}

void TextLibManager::reload_configuration()
{
    configurator_->reload_configuration();
    check_current_file_format();
}*/

void TextLibManager::export_pseudocode()
{
    //builder_->execute_export();
}

void TextLibManager::reset_builder()
{
    builder_->reset_builder();
}

/*void TextLibManager::write_new_line()
{
    exporter_->write_new_line();
}*/

/*void TextLibManager::check_current_file_format()
{
    std::string format = configurator_->output_file_format()->str();
    if (format != currentOutputFileFormat_)
    {
        currentOutputFileFormat_ = format;
        delete exporter_;
        create_new_exporter();
        visitor_->set_exporter(exporter_);
    }
}*/

void TextLibManager::create_new_builder()
{
    /*if (currentOutputFileFormat_ == "html")
    {
        exporter_ = new HtmlExporter();
    }
    else
    {
        exporter_ = new Exporter();
    }*/
}