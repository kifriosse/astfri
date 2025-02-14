#pragma once
#include <libastfri-text/inc/Exporter.hpp>

struct TxtFileExporter : Exporter {
    TxtFileExporter(Configurator& conf);
    void make_export() override;
};