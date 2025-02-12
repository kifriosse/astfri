#pragma once
#include <libastfri-text/inc/Exporter.hpp>

struct HtmlFileExporter : Exporter {
    HtmlFileExporter(const Configurator* conf, std::stringstream* output);
    void make_export() override;
    void write_indentation() override;
    void write_new_line() override;
    void write_class_word() override;
    void write_class_name(const std::string& name) override;
};