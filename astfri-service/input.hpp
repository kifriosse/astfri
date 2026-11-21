#ifndef ASTFRI_SERVICE_INPUT_HPP
#define ASTFRI_SERVICE_INPUT_HPP

#include <astfri/Astfri.hpp>

#include <filesystem>
#include <iosfwd>

namespace astfri::service {

enum class InputType
{
    Cpp,
    Csharp,
    Java
};

astfri::TranslationUnit load_cli_input(InputType type, const std::filesystem::path& input_file);

astfri::TranslationUnit load_input(InputType type, std::istream& input);

} // namespace astfri::service

#endif
