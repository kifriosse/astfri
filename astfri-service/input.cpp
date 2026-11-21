#include "input.hpp"

// ASTFRI CPP
#include <astfri-cpp/ASTBuilder.hpp>

// ASTFRI Csharp
#include <astfri-cs/ASTBuilder.hpp>

// ASTFRI Java
#include <astfri-java/ASTBuilder.hpp>

#include <format>
#include <fstream>
#include <stdexcept>

namespace astfri::service {

astfri::TranslationUnit load_cli_input(InputType type, const std::filesystem::path& input_file)
{
    std::ifstream stream(input_file);
    if (! stream.is_open())
    {
        throw std::runtime_error(std::format("Failed to open `{}`.", input_file.string()));
    }
    return load_input(type, stream);
}

astfri::TranslationUnit load_input(InputType type, std::istream& input)
{
    astfri::TranslationUnit tu;
    switch (type)
    {
    case InputType::Cpp:
    {
        auto astBuilder = astfri::cpp::ASTBuilder::create(
            astfri::cpp::Config::create_default());
        tu = astBuilder.load_file(input);
        break;
    }
    case InputType::Csharp:
    {
        auto astBuilder = astfri::csharp::ASTBuilder::create(
            astfri::csharp::Config::create_default());
        tu = astBuilder.load_file(input);
        break;
    }
    case InputType::Java:
    {
        auto astBuilder = astfri::java::ASTBuilder::create(
            astfri::java::Config::create_default());
        tu = astBuilder.load_file(input);
        break;
    }
    }
    return tu;
}

} // namespace astfri::service
