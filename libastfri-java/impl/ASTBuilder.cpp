#include <libastfri-java/inc/ASTBuilder.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>

#pragma region java
namespace astfri::java {
ASTBuilder::ASTBuilder() :
    stmtTransformer(new StatementTransformer()) {
}

ASTBuilder::~ASTBuilder() {
    delete this->stmtTransformer;
}

std::string ASTBuilder::load_stream(std::istream& stream) {
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

std::string ASTBuilder::load_file(const std::string& path) {
    std::ifstream file(path);
    if (! file.is_open()) {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return "";
    }

    return load_stream(file);
}

std::string ASTBuilder::load_project(const std::string& path) {
    std::string sourceCode = "";
    const std::filesystem::path directory{path};

    if (! std::filesystem::exists(directory)) {
        std::cerr << "Directory not found!" << std::endl;
        return sourceCode;
    }

    for (const auto& dir_entry : std::filesystem::recursive_directory_iterator{directory}) {
        if (dir_entry.is_regular_file() && dir_entry.path().extension() == ".java") {
            sourceCode += this->load_file(dir_entry.path());
        }
    }
    return sourceCode;
}

TSTree* ASTBuilder::make_syntax_tree(const std::string& sourceCodeString) {
    const char* sourceCode = sourceCodeString.c_str();
    TSParser* parser       = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_java());
    TSTree* tree = ts_parser_parse_string(parser, NULL, sourceCode, strlen(sourceCode));

    ts_parser_delete(parser);
    return tree;
}

astfri::TranslationUnit* ASTBuilder::get_translation_unit(
    TSTree* tree,
    const std::string& sourceCode
) {
    astfri::TranslationUnit* translationUnit
        = stmtTransformer->fill_translation_unit(tree, sourceCode);
    return translationUnit;
}
} // namespace astfri::java

#pragma endregion

#pragma region java_in
namespace astfri {
astfri::TranslationUnit* java_in::load_stream(std::istream& stream, const astfri::java::Config& cfg) {
    (void) cfg;
    astfri::java::ASTBuilder builder;
    std::string sourceCode = builder.load_stream(stream);
    TSTree* tree = builder.make_syntax_tree(sourceCode);
    return builder.get_translation_unit(tree, sourceCode);
}

astfri::TranslationUnit* java_in::load_file(const std::string& path, const astfri::java::Config& cfg) {
    (void) cfg;
    astfri::java::ASTBuilder builder;
    std::string sourceCode = builder.load_file(path);
    TSTree* tree = builder.make_syntax_tree(sourceCode);
    return builder.get_translation_unit(tree, sourceCode);
}

astfri::TranslationUnit* java_in::load_project(const std::string& path, const astfri::java::Config& cfg) {
    (void) cfg;
    astfri::java::ASTBuilder builder;
    std::string sourceCode = builder.load_project(path);
    TSTree* tree = builder.make_syntax_tree(sourceCode);
    return builder.get_translation_unit(tree, sourceCode);
}
} // namespace astfri
#pragma endregion