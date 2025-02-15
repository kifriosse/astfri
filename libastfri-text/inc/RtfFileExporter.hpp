#pragma once
#include <libastfri-text/inc/Exporter.hpp>

struct RtfFileExporter : Exporter {
    RtfFileExporter(Configurator& conf);
    void make_export() override;
};