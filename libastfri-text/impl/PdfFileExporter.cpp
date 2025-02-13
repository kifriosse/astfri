#include <libastfri-text/inc/PdfFileExporter.hpp>

PdfFileExporter::PdfFileExporter(const Configurator* conf, std::stringstream* output) : Exporter(conf, output) {}

void PdfFileExporter::make_export() {}