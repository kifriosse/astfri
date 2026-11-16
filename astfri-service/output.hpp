#ifndef ASTFRI_SERVICE_OUTPUT_HPP
#define ASTFRI_SERVICE_OUTPUT_HPP

#include <astfri/Astfri.hpp>

#include <filesystem>

namespace astfri::service {

enum class OutputType
{
    Text_Pseudocode,
    Text_JavaCode,
    UML
};

void write_output(
    OutputType type,
    astfri::TranslationUnit& tu,
    const std::filesystem::path& output_config_file
);

} // namespace astfri::service

#endif
