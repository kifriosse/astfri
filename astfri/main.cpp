#include <fstream>
#include <iostream>

// astfri headers
#include <libastfri/inc/Astfri.hpp>

// ASTFRI visitor
#include <libastfri-text/inc/TextLibManager.hpp>

// ASTFRI UML
#include <libastfri-uml/inc/UMLLibWrapper.hpp>

// ASTFRI CPP
#include <libastfri-cpp/inc/ClangManagement.hpp>

// ASTFRI Csharp
#include <libastfri-cs/inc/CSharpASTBuilder.hpp>

// ASTFRI Java
#include <libastfri-java/inc/ASTBuilder.hpp>

#include "cxxopts.hpp"

#pragma region DEFINITIONS

#define ASTFRI_VERSION "0.0.1"

// exit code meanings:
//  - 0 - OK
//  - 1X - problem with astfri wrapper
//  - 2X - problem with input library
//  - 3X - problem with output library
#define EXIT_OK 0
#define EXIT_ASTFRI_WRAPPER_ERROR 10
#define EXIT_INPUT_LIB_ERROR 20
#define EXIT_OUTPUT_LIB_ERROR 30


#pragma region INPUT ENUM
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
#pragma endregion INPUT ENUM

#pragma region OUTPUT ENUM
enum class OutputType
{
    Text_Pseudocode,
    Text_JavaCode,
    UML,
    Unknown // default
};
const std::map<std::string, OutputType> output_map = {
    {"text-pseudocode", OutputType::Text_Pseudocode},
    {"text-javacode",   OutputType::Text_JavaCode  },
    {"uml",             OutputType::UML            }
};
#pragma endregion OUTPUT ENUM

#pragma region HELPERS

// map keys to string (separated by ,)
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

#pragma endregion HELPERS
#pragma endregion DEFINITIONS

int main(int argc, const char** argv)
{
#pragma region ARGS

    cxxopts::Options options(
        "astfri",
        "ASTFRI - CLI interface for library astfri"
    );

    options.add_options()
        ("h,help", "Print help")
        ("version", "Print version of ASTFRI and input/output libraries. Then exit.")
        ("v,verbose", "Verbose mode - advanced logging. Not suitable for API use")
        ("i,input", "Type of input library. Possible values: " + map_keys_to_string(input_map), cxxopts::value<std::string>()->default_value("cpp"))
        ("if,input-file", "Input source file", cxxopts::value<std::string>())
        ("o,output", "Type of output library. Possible values: " + map_keys_to_string(output_map), cxxopts::value<std::string>()->default_value("text"))
        ("ocf,output-config-file", "Output config file", cxxopts::value<std::string>()->default_value(""))
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(EXIT_OK);
    }

    auto mode_version = result.count("version");
#pragma region VERSION
    if (mode_version)
    {
        std::cout << "ASTFRI version: " << ASTFRI_VERSION << "\n";
        std::cout << "Input libraries:\n";
        for (const auto& pair : input_map)
        {
            std::cout << "  " << pair.first << " vTODO" << "\n";
        }
        std::cout << "Output libraries:\n";
        for (const auto& pair : output_map)
        {
            std::cout << "  " << pair.first << " vTODO" << "\n";
        }
        exit(EXIT_OK);
    }
#pragma endregion VERSION
    auto mode_verbose = result.count("verbose");

    // parse input lib
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
        exit(EXIT_INPUT_LIB_ERROR);
    }
    // input file check
    if (result.count("input-file") == 0)
    {
        std::cerr << "Input file not specified!" << std::endl;
        exit(EXIT_INPUT_LIB_ERROR);
    }
    auto input_file = result["input-file"].as<std::string>();
    // verify that input file exists
    {
        std::ifstream infile(input_file);
        if (! infile.good())
        {
            std::cerr << "Input file does not exist: " << input_file
                      << std::endl;
            exit(EXIT_INPUT_LIB_ERROR);
        }
    }

    // parse output lib
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
        exit(EXIT_OUTPUT_LIB_ERROR);
    }
    // output config file
    auto output_config_file = result["output-config-file"].as<std::string>();
    // if output config file is specified, verify that it exists
    if (! output_config_file.empty())
    {
        std::ifstream outfile(output_config_file);
        if (! outfile.good())
        {
            std::cerr << "Output config file does not exist: "
                      << output_config_file << std::endl;
            exit(EXIT_OUTPUT_LIB_ERROR);
        }
    }
#pragma endregion ARGS

#pragma region INPUT

    if (mode_verbose)
    {
        std::cout << "Input type: " << input_lib_str << std::endl;
        std::cout << "Input file: " << input_file << std::endl;
    }

    astfri::TranslationUnit tu;
    switch (input_lib)
    {
    case InputType::Cpp:
    {
        if (astfri::astfri_cpp::fill_translation_unit(tu, input_file) != 0)
        {
            if (mode_verbose)
            {
                std::cerr
                    << "Error filling translation unit from C++ source file: "
                    << input_file << std::endl;
            }
            return EXIT_INPUT_LIB_ERROR;
        }
        break;
    }
    case InputType::Csharp:
    {
        const astfri::csharp::CSharpASTBuilder ast_builder;
        astfri::text::TextLibManager& manager
            = astfri::text::TextLibManager::get_instance();

        tu = *(ast_builder.make_ast(input_file));
        manager.visit_and_export(tu);
        break;
    }
    case InputType::Java:
    {
        astfri::java::ASTBuilder* tb = new astfri::java::ASTBuilder();

        std::string sourceCode       = tb->load_directory(input_file);
        TSTree* tree                 = tb->make_syntax_tree(sourceCode);
        astfri::TranslationUnit* tu_ptr
            = tb->get_translation_unit(tree, sourceCode);
        tu = *tu_ptr;

        // freeing memory
        ts_tree_delete(tree);
        delete (tb);
        break;
    }
    case InputType::Unknown:
        std::cerr << "Input library not implemented yet!" << std::endl;
        return EXIT_INPUT_LIB_ERROR;
    }

    if (mode_verbose)
    {
        std::cout << "Translation unit loaded." << std::endl;
    }
#pragma endregion INPUT

#pragma region OUTPUT
    if (mode_verbose)
    {
        std::cout << "Output type: " << output_lib_str << std::endl;
        std::cout << "Output config file: "
                  << (output_config_file.empty() ? "none" : output_config_file)
                  << std::endl;
    }

    switch (output_lib)
    {
    case OutputType::Text_Pseudocode:
    case OutputType::Text_JavaCode:
    {
        astfri::text::TextLibManager& manager
            = astfri::text::TextLibManager::get_instance();

        switch (output_lib)
        {
        case OutputType::Text_Pseudocode:
            manager.change_output(astfri::text::OutputFormat::Pseudocode);
            break;
        case OutputType::Text_JavaCode:
            manager.change_output(astfri::text::OutputFormat::JavaCode);
            break;
        default:
            break;
        }

        manager.visit_and_export(tu);
        break;
    }
    case OutputType::UML:
    {
        astfri::uml::UMLLibWrapper wrapper;
        astfri::uml::PlantUMLOutputter op;

        astfri::uml::Config config;
        if (! output_config_file.empty())
        {
            if (! config.parse_json(output_config_file.c_str()))
            {
                config.use_default_values();
                if (mode_verbose)
                {
                    std::cout << "Unable to parse JSON config. Using default "
                                 "values.\n";
                }
            }
        }

        wrapper.init(config, op);
        wrapper.run(tu);
        break;
    }
    case OutputType::Unknown:
    {
        std::cerr << "Output library not implemented yet!" << std::endl;
        return EXIT_OUTPUT_LIB_ERROR;
    }
    }

    if (mode_verbose)
    {
        std::cout << "Output generated." << std::endl;
    }
#pragma endregion OUTPUT

    return EXIT_OK;
}
