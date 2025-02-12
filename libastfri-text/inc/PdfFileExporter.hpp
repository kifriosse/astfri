#pragma once
#include <libastfri-text/inc/Exporter.hpp>

struct PdfFileExporter : Exporter {
    PdfFileExporter(const Configurator* conf, std::stringstream* output);
    void make_export() override;
};