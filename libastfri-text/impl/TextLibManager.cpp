#include <libastfri-text/inc/TextLibManager.hpp>

#include <libastfri-text/inc/HtmlFileExporter.hpp>
#include <libastfri-text/inc/TxtFileExporter.hpp>

using namespace astfri::text;

TextLibManager& TextLibManager::get_instance()
{
    static TextLibManager manager;
    return manager;
}

TextLibManager::TextLibManager()
{
    configurator_ = &TextConfigurator::get_instance();
    currentOutputFileFormat_ = "txt";
    create_new_exporter(currentOutputFileFormat_);
    visitor_ = &ASTTextVisitor::get_instance(exporter_);
}

TextLibManager::~TextLibManager()
{
    delete exporter_;
}

void TextLibManager::reload_configuration()
{
    configurator_->reload_configuration();
    check_current_file_format();
}

void TextLibManager::update_configuration()
{
    configurator_->update_configuration();
    check_current_file_format();
}

void TextLibManager::execute_export()
{
    exporter_->execute_export();
}

void TextLibManager::reset_exporter()
{
    exporter_->reset();
}

void TextLibManager::write_new_line()
{
    exporter_->write_new_line();
}

void TextLibManager::check_current_file_format()
{
    std::string format = configurator_->get_output_file_format()->str();
    if (format != currentOutputFileFormat_) {
        currentOutputFileFormat_ = format;
        delete exporter_;
        create_new_exporter(currentOutputFileFormat_);
        visitor_->set_exporter(exporter_);
    }
    reset_exporter();
}

void TextLibManager::create_new_exporter(const std::string& fileformat)
{
    if (fileformat == "html") {
        exporter_ = new HtmlFileExporter();
    } else {
        exporter_ = new TxtFileExporter();
    }
}