#ifndef TREE_MAKER_CLASS_HPP
#define TREE_MAKER_CLASS_HPP

#include <cstring>
#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>

class TreeMaker
{
private:
    std::string sourceCode;
    TSTree* tree;
    std::string load_file (std::string const& path);
    TSTree* make_syntax_tree (char const* sourceCode);

public:
    TreeMaker(std::string const& path);
    std::string get_source_code();
    TSTree* get_syntax_tree();
};

#endif // TREE_MAKER_CLASS_HPP