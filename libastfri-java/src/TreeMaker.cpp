#include "TreeMaker.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <tree_sitter/api.h>

TreeMaker::TreeMaker(std::string const& path)
{
    sourceCode = load_file(path);
    tree       = make_syntax_tree(sourceCode.c_str());
}

std::string TreeMaker::load_file(std::string const& path)
{
    std::ifstream file(path);
    if (! file.is_open())
    {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

TSTree* TreeMaker::make_syntax_tree(char const* sourceCode)
{
    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_java());
    TSTree* tree
        = ts_parser_parse_string(parser, NULL, sourceCode, strlen(sourceCode));

    return tree;
}

std::string TreeMaker::get_source_code() 
{
    return this->sourceCode;
}

TSTree* TreeMaker::get_syntax_tree()
{
    return this->tree;
}