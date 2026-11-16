#include "input.hpp"

// ASTFRI CPP
#include <astfri-cpp/ASTBuilder.hpp>

// ASTFRI Csharp
#include <astfri-cs/ASTBuilder.hpp>

// ASTFRI Java
#include <astfri-java/ASTBuilder.hpp>

namespace astfri::service {

astfri::TranslationUnit load_input(InputType type, const std::filesystem::path& input_file)
{
    astfri::TranslationUnit tu;
    switch (type)
    {
    case InputType::Cpp:
    {
        auto astBuilder = astfri::cpp::ASTBuilder::create(
            astfri::cpp::Config::create_default());
        tu = astBuilder.load_file(input_file);
        break;
    }
    case InputType::Csharp:
    {
        auto astBuilder = astfri::csharp::ASTBuilder::create(
            astfri::csharp::Config::create_default());
        tu = astBuilder.load_file(input_file);
        break;
    }
    case InputType::Java:
    {
        auto astBuilder = astfri::java::ASTBuilder::create(
            astfri::java::Config::create_default());
        tu = astBuilder.load_file(input_file);
        break;
    }
    }
    return tu;
}

} // namespace astfri::service
