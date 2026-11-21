#include <fstream>
#include <iostream>

// astfri headers
#include <astfri/Astfri.hpp>

#include <cxxopts.hpp>

#include "input.hpp"
#include "output.hpp"
#include "shm_server.hpp"

#define ASTFRI_CLI_VERSION "0.0.1"

// exit code meanings:
//  - 0 - OK
//  - 1X - problem with astfri wrapper
//  - 2X - problem with input library
//  - 3X - problem with output library
#define EXIT_OK 0
#define EXIT_ASTFRI_WRAPPER_ERROR 10
#define EXIT_INPUT_LIB_ERROR 20
#define EXIT_OUTPUT_LIB_ERROR 30

using astfri::service::InputType;
const std::map<std::string, InputType> input_map = {
    {"cpp",    InputType::Cpp   },
    {"csharp", InputType::Csharp},
    {"java",   InputType::Java  }
};

using astfri::service::OutputType;
const std::map<std::string, OutputType> output_map = {
    {"text-pseudocode", OutputType::Text_Pseudocode},
    {"text-javacode",   OutputType::Text_JavaCode  },
    {"uml",             OutputType::UML            }
};

int main(int argc, const char** argv)
{
#pragma region ARGS

    cxxopts::Options options("astfri");

    options.add_options()
        ("version", "Print version of ASTFRI and input/output libraries. Then exit.")
        ("input", "Type of input library", cxxopts::value<std::string>())
        ("input-file", "Input source file", cxxopts::value<std::string>())
        ("output", "Type of output library", cxxopts::value<std::string>())
        ("output-config-file", "Output config file", cxxopts::value<std::string>()->default_value(""))
        ("shm-serve", "Run as shared-memory server on the given block file", cxxopts::value<std::string>())
    ;

    auto result = options.parse(argc, argv);

    auto mode_version = result.count("version");
    if (mode_version)
    {
        std::cout << "ASTFRI-CLI-WRAPPER " << ASTFRI_CLI_VERSION << "\n";
        std::cout << "Input libraries:\n";
        for (const auto& pair : input_map)
        {
            std::cout << "  " << pair.first << "\n";
        }
        std::cout << "Output libraries:\n";
        for (const auto& pair : output_map)
        {
            std::cout << "  " << pair.first << "\n";
        }
        exit(EXIT_OK);
    }

    if (result.count("shm-serve"))
    {
        return astfri::service::run_shm_server(result["shm-serve"].as<std::string>())
            ? EXIT_OK
            : EXIT_ASTFRI_WRAPPER_ERROR;
    }

    // parse input lib
    if (result.count("input") == 0)
    {
        std::cerr << "Input library not specified!" << std::endl;
        exit(EXIT_ASTFRI_WRAPPER_ERROR);
    }
    auto input_lib_str = result["input"].as<std::string>();
    if (input_map.find(input_lib_str) == input_map.end())
    {
        std::cerr << "Unknown input library type: " << input_lib_str
                  << std::endl;
        exit(EXIT_ASTFRI_WRAPPER_ERROR);
    }
    auto input_lib = input_map.at(input_lib_str);
    // input file check
    if (result.count("input-file") == 0)
    {
        std::cerr << "Input file not specified!" << std::endl;
        exit(EXIT_ASTFRI_WRAPPER_ERROR);
    }
    auto input_file = result["input-file"].as<std::string>();
    // verify that input file exists
    {
        std::ifstream infile(input_file);
        if (! infile.good())
        {
            std::cerr << "Input file does not exist: " << input_file
                      << std::endl;
            exit(EXIT_ASTFRI_WRAPPER_ERROR);
        }
    }

    // parse output lib
    if (result.count("output") == 0)
    {
        std::cerr << "Output library not specified!" << std::endl;
        exit(EXIT_ASTFRI_WRAPPER_ERROR);
    }
    auto output_lib_str = result["output"].as<std::string>();
    if (output_map.find(output_lib_str) == output_map.end())
    {
        std::cerr << "Unknown output library type: " << output_lib_str
                  << std::endl;
        exit(EXIT_ASTFRI_WRAPPER_ERROR);
    }
    auto output_lib = output_map.at(output_lib_str);
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
            exit(EXIT_ASTFRI_WRAPPER_ERROR);
        }
    }
#pragma endregion ARGS

    astfri::TranslationUnit tu = astfri::service::load_cli_input(input_lib, input_file);
    astfri::service::write_cli_output(output_lib, tu, output_config_file);

    return EXIT_OK;
}
