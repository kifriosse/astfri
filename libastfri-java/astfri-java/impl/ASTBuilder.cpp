#include <astfri-java/ASTBuilder.hpp>

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <iterator>
#include <stdexcept>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-java.h>


namespace astfri::java {


ASTBuilder ASTBuilder::create(astfri::java::Config config) {
    return ASTBuilder(std::move(config));
}

ASTBuilder::ASTBuilder(astfri::java::Config config) :
    m_config(std::move(config)),
    m_exprFactory(&astfri::ExprFactory::get_instance()),
    m_stmtFactory(&astfri::StmtFactory::get_instance()),
    m_typeFactory(&astfri::TypeFactory::get_instance()),
    m_nodeMapper(m_typeFactory),
    m_exprTransformer(m_exprFactory, m_typeFactory, &m_stmtTransformer, &m_nodeMapper),
    m_stmtTransformer(m_exprFactory, m_stmtFactory, m_typeFactory, &m_exprTransformer, &m_nodeMapper)
{
}

astfri::TranslationUnit ASTBuilder::load_file(std::istream &ist) {
    std::string str(
        std::istreambuf_iterator<std::istream::char_type>(ist),
        std::istreambuf_iterator<std::istream::char_type>{});
    const char* sourceCode = str.c_str();
    TSParser* parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_java());
    TSTree* tree = ts_parser_parse_string(parser, NULL, sourceCode, strlen(sourceCode));
    ts_parser_delete(parser);
    return m_stmtTransformer.fill_translation_unit(tree, sourceCode);
}

astfri::TranslationUnit ASTBuilder::load_file(const std::filesystem::path &path) {
    std::ifstream file(path);
    if (! file.is_open()) {
        throw std::runtime_error(std::format("Failed to open `{}`.", path.string()));
    }
    return this->load_file(file);
}

std::vector<astfri::TranslationUnit> ASTBuilder::load_project(const std::filesystem::path &path) {
    std::vector<std::filesystem::path> paths;
    std::vector<std::ifstream> ifsts;
    for (const std::filesystem::directory_entry& dirEntry :
            std::filesystem::recursive_directory_iterator(path)) {
        if (dirEntry.is_regular_file() && dirEntry.path().extension() == ".java") {
            ifsts.emplace_back(dirEntry.path());
        }
    }

    for (const std::filesystem::path &path : paths) {
        ifsts.emplace_back(path);
        if (! ifsts.back().is_open()) {
            throw std::runtime_error(std::format("Failed to open `{}`.", path.string()));
        }
    }

    std::vector<astfri::TranslationUnit> tus;
    tus.reserve(ifsts.size());
    for (std::ifstream &ifst : ifsts) {
        this->load_file(ifst);
    }

    return tus;
}


} // namespace astfri::java
