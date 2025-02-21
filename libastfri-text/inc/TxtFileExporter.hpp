#ifndef LIBASTFRI_TEXT_INC_TXT_EXPORTER
#define LIBASTFRI_TEXT_INC_TXT_EXPORTER

#include <libastfri-text/inc/Exporter.hpp>

struct TxtFileExporter : Exporter {
    TxtFileExporter(std::shared_ptr<TextConfigurator> conf);
    void make_export() override;
private:
    void write_file(std::string path) override;
};

#endif