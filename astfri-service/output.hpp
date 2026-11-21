#ifndef ASTFRI_SERVICE_OUTPUT_HPP
#define ASTFRI_SERVICE_OUTPUT_HPP

#include <astfri/Astfri.hpp>

#include <filesystem>
#include <iosfwd>
#include <string_view>

namespace astfri::service {

enum class OutputType
{
    Text_Pseudocode,
    Text_JavaCode,
    UML
};

void write_cli_output(
    OutputType type,
    astfri::TranslationUnit& tu,
    const std::filesystem::path& output_config_file
);

void write_output(
    OutputType type,
    astfri::TranslationUnit& tu,
    std::string_view config_json,
    std::ostream& out
);

} // namespace astfri::service

#endif
