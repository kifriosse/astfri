#include "output.hpp"

// ASTFRI visitor
#include <astfri-text/TextLibManager.hpp>

// ASTFRI UML
#include <astfri-uml/UMLLibWrapper.hpp>

#include <iostream>

namespace astfri::service {

void write_output(
    OutputType type,
    astfri::TranslationUnit& tu,
    const std::filesystem::path& output_config_file
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

        manager.process_and_export_ast(tu, &std::cout);
        break;
    }
    case OutputType::UML:
    {
        astfri::uml::UMLLibWrapper wrapper;
        astfri::uml::PlantUMLOutputter op;
        auto config = astfri::uml::Config::create_from_json(output_config_file);
        wrapper.init(config, op);
        wrapper.run(tu);
        break;
    }
    }
}

} // namespace astfri::service
