#include <astfri-cpp/ASTBuilder.hpp>

#include <astfri/impl/ExprFactory.hpp>
#include <astfri/impl/TypeFactory.hpp>

#include <astfri-cpp/impl/ClangManagement.hpp>

#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

#include <format>
#include <fstream>
#include <stdexcept>


namespace astfri::cpp {


ASTBuilder ASTBuilder::create(astfri::cpp::Config config) {
    return ASTBuilder(std::move(config));
}

std::string_view ASTBuilder::version() {
    return ASTFRI_CPP_VERSION;
}

ASTBuilder::ASTBuilder(astfri::cpp::Config config) :
    m_config(std::move(config)),
    m_exprFactory(&astfri::ExprFactory::get_instance()),
    m_stmtFactory(&astfri::StmtFactory::get_instance()),
    m_typeFactory(&astfri::TypeFactory::get_instance())
{
}

astfri::TranslationUnit ASTBuilder::load_file(std::istream &ist) {
    std::string content(
      std::istreambuf_iterator<std::istream::char_type>(ist),
      std::istreambuf_iterator<std::istream::char_type>{});
    std::string virtual_file_name = "input_from_stream.cpp";

    // Just a fixed empty compilation database for now.
    std::vector<std::string> compilations = {};
    clang::tooling::FixedCompilationDatabase Compilations(".", compilations);

    clang::tooling::ClangTool Tool(Compilations, {virtual_file_name});

    // Makes sure that the file is not seeked on the disc.
    Tool.mapVirtualFile(virtual_file_name, content);

    astfri::TranslationUnit tu;
    auto action = std::make_unique<cpp::CppFrontendActionFactory>(tu);
    Tool.run(action.get());
    return tu;
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
        if (dirEntry.is_regular_file() && dirEntry.path().extension() == ".cpp") {
            paths.emplace_back(dirEntry.path());
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
        tus.push_back(this->load_file(ifst));
    }

    return tus;
}


} // namespace astfri::cpp
