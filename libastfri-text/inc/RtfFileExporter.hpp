#pragma once
#include <libastfri-text/inc/Exporter.hpp>

struct RtfFileExporter : Exporter {
    RtfFileExporter(const Configurator* conf, std::stringstream* output);
    void make_export() override;
};