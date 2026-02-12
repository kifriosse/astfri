#include <libastfri-java/inc/ASTBuilder.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace astfri::java
{
ASTBuilder::ASTBuilder() :
    stmtTransformer(new StatementTransformer())
{
}

ASTBuilder::~ASTBuilder()
{
    delete this->stmtTransformer;
}

std::string ASTBuilder::load_stream(std::istream& stream)
{
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

std::string ASTBuilder::load_file(std::string const& path)
{
    std::ifstream file(path);
    if (! file.is_open())
    {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return "";
    }

    return load_stream(file);
}

std::string ASTBuilder::load_directory(std::string const& path)
{
    std::string sourceCode = "";
    std::filesystem::path const directory{path};

    if (! std::filesystem::exists(directory))
    {
        std::cerr << "Directory not found!" << std::endl;
        return sourceCode;
    }

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{directory})
    {
        if (dir_entry.is_regular_file() && dir_entry.path().extension() == ".java")
        {
            sourceCode += this->load_file(dir_entry.path());
        }
    }
    return sourceCode;
}

TSTree* ASTBuilder::make_syntax_tree(std::string const& sourceCodeString)
{
    char const* sourceCode = sourceCodeString.c_str();
    TSParser* parser       = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_java());
    TSTree* tree = ts_parser_parse_string(parser, NULL, sourceCode, strlen(sourceCode));

    ts_parser_delete(parser);
    return tree;
}

astfri::TranslationUnit* ASTBuilder::get_translation_unit(
    TSTree* tree,
    std::string const& sourceCode
)
{
    astfri::TranslationUnit* translationUnit
        = stmtTransformer->fill_translation_unit(tree, sourceCode);
    return translationUnit;
}
} // namespace astfri::java