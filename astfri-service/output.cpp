#include "output.hpp"

// ASTFRI visitor
#include <astfri-text/TextLibManager.hpp>

// ASTFRI UML
#include <astfri-uml/UMLLibWrapper.hpp>

#include <rapidjson/document.h>

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace astfri::service {

namespace {

std::string read_file(const std::filesystem::path& path)
{
    std::ifstream stream(path);
    if (! stream.is_open())
    {
        throw std::invalid_argument(std::format("Failed to open `{}`.", path.string()));
    }
    std::ostringstream content;
    content << stream.rdbuf();
    return content.str();
}

} // namespace

void write_cli_output(
    OutputType type,
    astfri::TranslationUnit& tu,
    const std::filesystem::path& output_config_file
)
{
    std::string config;
    if (! output_config_file.empty())
    {
        config = read_file(output_config_file);
    }
    write_output(type, tu, config, std::cout);
}

void write_output(
    OutputType type,
    astfri::TranslationUnit& tu,
    std::string_view config_json,
    std::ostream& out
)
{
    switch (type)
    {
    case OutputType::Text_Pseudocode:
    case OutputType::Text_JavaCode:
    {
        astfri::text::TextLibManager manager;

        switch (type)
        {
        case OutputType::Text_Pseudocode:
            manager.change_output_format(astfri::text::TextOutputFormat::TxtPseudocode);
            break;
        case OutputType::Text_JavaCode:
            manager.change_output_format(astfri::text::TextOutputFormat::JavaCode);
            break;
        default:
            break;
        }

        manager.process_and_export_ast(tu, &out);
        break;
    }
    case OutputType::UML:
    {
        rapidjson::Document doc;
        doc.Parse(config_json.data(), config_json.size());
        if (doc.HasParseError())
        {
            throw std::runtime_error(std::format(
                "Failed to parse output config. Error code {}.",
                static_cast<int>(doc.GetParseError())));
        }

        astfri::uml::UMLLibWrapper wrapper;
        astfri::uml::PlantUMLOutputter op;
        auto config = astfri::uml::Config::create_from_json(doc);
        config.writeToFile = false;
        wrapper.init(config, op);

        std::streambuf* original = std::cout.rdbuf(out.rdbuf());
        try
        {
            wrapper.run(tu);
        }
        catch (...)
        {
            std::cout.rdbuf(original);
            throw;
        }
        std::cout.rdbuf(original);
        break;
    }
    }
}

} // namespace astfri::service
