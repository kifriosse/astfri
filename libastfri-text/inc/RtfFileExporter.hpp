#ifndef LIBASTFRI_TEXT_INC_RTF_EXPORTER
#define LIBASTFRI_TEXT_INC_RTF_EXPORTER

#include <libastfri-text/inc/Exporter.hpp>

struct RtfFileExporter : Exporter {
    RtfFileExporter(std::shared_ptr<TextConfigurator> conf);
    void make_export() override;
private:
    void write_output_into_file(std::string filepath) override;
};

#endif