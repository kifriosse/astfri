#ifndef LIBASTFRI_TEXT_EXPORTER
#define LIBASTFRI_TEXT_EXPORTER

#include <libastfri-text/inc/TextLibConfig.hpp>

namespace astfri::text
{
    class Exporter
    {
        TextLibConfig* m_config;
    public:
        explicit Exporter(TextLibConfig* config);
        //
        void export_pseudocode_file(std::string& text, std::ostream& ost);
        void export_pseudocode_file(std::string& text);
        void export_code_file(std::string& text, std::ostream& ost);
        void export_code_file(std::string& text);
    };
}

#endif