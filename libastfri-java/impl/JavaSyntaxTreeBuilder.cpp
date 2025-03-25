#include <libastfri-java/inc/JavaSyntaxTreeBuilder.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

JavaSyntaxTreeBuilder::JavaSyntaxTreeBuilder() : stmtTransformer(new StatementTransformer())
{    
}

std::string JavaSyntaxTreeBuilder::load_file(std::string const& path)
{
    std::ifstream file(path);
    if (! file.is_open())
    {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

std::string JavaSyntaxTreeBuilder::load_directory(std::string const& path)
{
    std::string sourceCode = "";
    const std::filesystem::path directory{path};
    
    if (!std::filesystem::exists(directory))
    {
        std::cerr << "Directory not found!" << std::endl;
        return sourceCode;
    }

    for (auto const& dir_entry : std::filesystem::directory_iterator{directory})
    {
        if (dir_entry.path().extension() == ".java")
        {
            sourceCode += this->load_file(dir_entry.path());
        }
    }
    return sourceCode;
}

TSTree* JavaSyntaxTreeBuilder::make_syntax_tree(std::string const& sourceCodeString)
{
    char const* sourceCode = sourceCodeString.c_str();
    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_java());
    TSTree* tree
        = ts_parser_parse_string(parser, NULL, sourceCode, strlen(sourceCode));

    return tree;
}

astfri::TranslationUnit* JavaSyntaxTreeBuilder::get_translation_unit(TSTree* tree, std::string const& sourceCode)
{
    astfri::TranslationUnit* translationUnit = stmtTransformer->fill_translation_unit(tree, sourceCode);
    return translationUnit;
}