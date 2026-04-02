#ifndef JAVA_SYNTAX_TREE_BUILDER_HPP
#define JAVA_SYNTAX_TREE_BUILDER_HPP

#include <libastfri-java/impl/StatementTransformer.hpp>
#include <astfri/impl/StmtFactory.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>

#include <cstring>
#include <string>

namespace astfri::java {
class ASTBuilder {
private:
    StatementTransformer* stmtTransformer;

public:
    ASTBuilder();
    ~ASTBuilder();

    std::string load_stream(std::istream& stream);

    std::string load_file(const std::string& path);

    std::string load_directory(const std::string& path);

    TSTree* make_syntax_tree(const std::string& sourceCodeString);

    astfri::TranslationUnit* get_translation_unit(TSTree* tree, const std::string& sourceCode);
};
} // namespace astfri::java
#endif // JAVA_SYNTAX_TREE_BUILDER_HPP