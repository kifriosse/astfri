#ifndef LIBASTFRI_TEXT_TXT_FILE_EXPORTER
#define LIBASTFRI_TEXT_TXT_FILE_EXPORTER

#include <libastfri-text/inc/Exporter.hpp>

namespace astfri::text
{
    class TxtFileExporter : public Exporter
    {
    public:
        inline TxtFileExporter() : Exporter() {}
    };
}

#endif