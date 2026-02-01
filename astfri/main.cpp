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
    Text,
    UML,
    Unknown // default
};
const std::map<std::string, OutputType> output_map = {
    {"text", OutputType::Text},
    {"uml",  OutputType::UML }
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
        ("i,input", "Type of input library. Possible values: " + map_keys_to_string(input_map), cxxopts::value<std::string>()->default_value("cpp"))
        ("if,input-file", "Input source file", cxxopts::value<std::string>())
        ("o,output", "Type of output library. Possible values: " + map_keys_to_string(output_map), cxxopts::value<std::string>()->default_value("text"))
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

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
        exit(1);
    }
    // input file check
    if (result.count("input-file") == 0)
    {
        std::cerr << "Input file not specified!" << std::endl;
        exit(1);
    }
    auto input_file = result["input-file"].as<std::string>();
    // verify that input file exists
    {
        std::ifstream infile(input_file);
        if (! infile.good())
        {
            std::cerr << "Input file does not exist: " << input_file
                      << std::endl;
            exit(1);
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
        exit(1);
    }
    #pragma endregion ARGS

    #pragma region INPUT
    
    (void)argc;
    astfri::TranslationUnit tu;
    switch (input_lib)
    {
    case InputType::Cpp:
    {
        if (astfri::astfri_cpp::fill_translation_unit(tu, input_file) != 0)
        {
            std::cout << "chyba pri fill_translation_unit\n";
            return 1;
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

        std::string sourceCode = tb->load_directory(input_file);
        TSTree* tree           = tb->make_syntax_tree(sourceCode);
        astfri::TranslationUnit* tu_ptr = tb->get_translation_unit(tree, sourceCode);
        tu = *tu_ptr;

        // freeing memory
        ts_tree_delete(tree);
        delete (tb);
        break;
    }
    case InputType::Unknown:
        std::cerr << "Input library not implemented yet!" << std::endl;
        return 1;
    }
    #pragma endregion INPUT

    #pragma region OUTPUT
    // TODO

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

    #pragma endregion OUTPUT

    return 0;
}
