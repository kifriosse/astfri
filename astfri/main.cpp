#include <iostream>

// astfri headers
#include <libastfri/inc/Astfri.hpp>

// ASTFRI visitor
#include <libastfri-text/inc/TextLibManager.hpp>

// ASTFRI UML
#include <libastfri-uml/inc/UMLLibWrapper.hpp>

// to co je treba na moje
#include <libastfri-cpp/inc/ClangManagement.hpp>

#include "cxxopts.hpp"

enum class InputType
{
    Cpp,
    Csharp,
    Java,
    Unknown // default
};
const std::map<std::string, InputType> input_map = {
    {"cpp",    InputType::Cpp   },
    {"csharp", InputType::Csharp},
    {"java",   InputType::Java  }
};

enum class OutputType
{
    Text,
    UML,
    Unknown // default
};
const std::map<std::string, OutputType> output_map = {
    {"text", OutputType::Text},
    {"uml",  OutputType::UML }
};

// map keys to string (seperated by ,)
template<typename T>
std::string map_keys_to_string(const std::map<std::string, T>& map)
{
    std::string result;
    for (const auto& pair : map)
    {
        if (! result.empty())
        {
            result += ", ";
        }
        result += pair.first;
    }
    return result;
}

int main(int argc, const char** argv)
{
    cxxopts::Options options(
        "astfri",
        "ASTFRI - CLI interface for library astfri"
    );

    options.add_options()
        ("h,help", "Print help")
        ("i,input", "Type of input library. Possible values: " + map_keys_to_string(input_map), cxxopts::value<std::string>()->default_value("cpp"))
        ("o,output", "Type of output library. Possible values: " + map_keys_to_string(output_map), cxxopts::value<std::string>()->default_value("text"))
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    // parse intput lib
    auto input_lib_str = result["input"].as<std::string>();
    auto input_lib     = InputType::Unknown;
    if (input_map.find(input_lib_str) != input_map.end())
    {
        input_lib = input_map.at(input_lib_str);
    }
    else
    {
        std::cerr << "Unknown input library type: " << input_lib_str
                  << std::endl;
        exit(1);
    }

    auto output_lib_str = result["output"].as<std::string>();
    auto output_lib     = OutputType::Unknown;
    if (output_map.find(output_lib_str) != output_map.end())
    {
        output_lib = output_map.at(output_lib_str);
    }
    else
    {
        std::cerr << "Unknown output library type: " << output_lib_str
                  << std::endl;
        exit(1);
    }

    // TODO - input libs

    (void)argc;
    astfri::TranslationUnit tu;

    if (astfri::astfri_cpp::fill_translation_unit(tu, argv[1]) != 0)
    {
        std::cout << "chyba pri fill_translation_unit\n";
        return 1;
    }
    // koniec mojho
    // std::cout << "Ill be back!" << std::endl;
    // AST Visitor - nice

    // TODO - output libs

    // ako toto rozbehať?
    astfri::text::TextLibManager& man
        = astfri::text::TextLibManager::get_instance();
    man.visit_and_export(tu);

    // UML
    astfri::uml::UMLLibWrapper wrapper;
    astfri::uml::Config config;
    astfri::uml::PlantUMLOutputter op;
    config.separator_ = ':';
    wrapper.init(config, op);
    wrapper.run(tu);

    return 0;
}
