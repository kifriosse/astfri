#ifndef LIBASTFRI_TEXT_EXPORTER
#define LIBASTFRI_TEXT_EXPORTER

#include <astfri-text/TextLibConfig.hpp>

namespace astfri::text
{
    class Exporter
    {
        Config* const m_config;
        std::filesystem::path m_defPath;
    public:
        explicit Exporter(Config* config);
        //
        void export_file(std::string_view text, std::ostream* ost) const;
    private:
        std::string create_file_name() const;
    };
}

#endif