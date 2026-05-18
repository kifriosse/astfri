#include <libastfri-text/inc/tools/Exporter.hpp>

using namespace astfri::text;

Exporter::Exporter(TextLibConfig* config) :
    m_config(config)
{
}

void Exporter::export_pseudocode_file(std::string& /*text*/, std::ostream& /*ost*/)
{
    // TODO: create export
}

void Exporter::export_pseudocode_file(std::string& /*text*/)
{
    // TODO: create export
}

void Exporter::export_code_file(std::string& /*text*/, std::ostream& /*ost*/)
{
    // TODO: create export
}

void Exporter::export_code_file(std::string& /*text*/)
{
    // TODO: create export
}