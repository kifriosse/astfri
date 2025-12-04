#ifndef LIBASTFRI_TEXT_PLAIN_TEXT_EXPORTER
#define LIBASTFRI_TEXT_PLAIN_TEXT_EXPORTER

#include <sstream>

namespace astfri::text
{
    class PlainTextExporter
    {
    public:
        PlainTextExporter(int rowCount, std::stringstream* const& text);
    private:
        void execute(int rowCount, std::stringstream* const& text);
        void write_pseudocode_into_file(int r, std::stringstream* const& text, std::string const& fullfilepath);
    };
}

#endif