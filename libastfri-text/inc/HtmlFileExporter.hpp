#ifndef LIBASTFRI_TEXT_INC_HTML_EXPORTER
#define LIBASTFRI_TEXT_INC_HTML_EXPORTER

#include <libastfri-text/inc/Exporter.hpp>

struct HtmlFileExporter : Exporter {
    HtmlFileExporter(std::shared_ptr<TextConfigurator> conf);
    void make_export() override;
    void write_indentation() override;
    void write_new_line() override;
    void write_class_word() override;
    void write_class_name(std::string name) override;
private:
    void write_row_number() override;
};

#endif