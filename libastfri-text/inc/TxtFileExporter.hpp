#ifndef LIBASTFRI_TEXT_INC_TXT_EXPORTER
#define LIBASTFRI_TEXT_INC_TXT_EXPORTER

#include <libastfri-text/inc/Exporter.hpp>

class TxtFileExporter : public Exporter {
public:
    TxtFileExporter(std::shared_ptr<TextConfigurator> conf);
    void make_export() override;
private:
    void write_output_into_file(std::string filepath) override;
};

#endif