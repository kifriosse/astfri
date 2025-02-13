#pragma once
#include <libastfri-text/inc/Exporter.hpp>

struct TxtFileExporter : Exporter {
    TxtFileExporter(const Configurator* conf, std::stringstream* output);
    void make_export() override;
};