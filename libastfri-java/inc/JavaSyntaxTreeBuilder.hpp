#ifndef JAVA_SYNTAX_TREE_BUILDER_HPP
#define JAVA_SYNTAX_TREE_BUILDER_HPP

#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri-java/impl/StatementTransformer.hpp>

#include <cstring>
#include <string>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>

class JavaSyntaxTreeBuilder
{
private:
    StatementTransformer* stmtTransformer;
    
    public:
    JavaSyntaxTreeBuilder();
    std::string load_file (std::string const& path);
    std::string load_directory(std::string const& path);
    TSTree* make_syntax_tree (std::string const& sourceCodeString);
    astfri::TranslationUnit* get_translation_unit(TSTree* tree, std::string const& sourceCode);
};

#endif // JAVA_SYNTAX_TREE_BUILDER_HPP