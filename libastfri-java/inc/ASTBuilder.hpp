#ifndef JAVA_SYNTAX_TREE_BUILDER_HPP
#define JAVA_SYNTAX_TREE_BUILDER_HPP

#include <libastfri-java/impl/StatementTransformer.hpp>
#include <astfri/impl/StmtFactory.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>

#include <string>
#include "astfri/impl/Stmt.hpp"

namespace astfri::java {
class ASTBuilder {
private:
    StatementTransformer* stmtTransformer;

public:
    ASTBuilder();
    ~ASTBuilder();

    std::string load_stream(std::istream& stream);

    std::string load_file(const std::string& path);

    std::string load_project(const std::string& path);

    TSTree* make_syntax_tree(const std::string& sourceCodeString);

    astfri::TranslationUnit* get_translation_unit(TSTree* tree, const std::string& sourceCode);
};

class Config {};
} // namespace astfri::java

namespace astfri {
    struct java_in
    {
        static astfri::TranslationUnit* load_stream(std::istream& stream, const astfri::java::Config &cfg);
        static astfri::TranslationUnit* load_file(const std::string& path, const astfri::java::Config &cfg);
        static astfri::TranslationUnit* load_project(const std::string& path, const astfri::java::Config &cfg);
    };
} // namespace astfri

#endif // JAVA_SYNTAX_TREE_BUILDER_HPP