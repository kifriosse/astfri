#ifndef CLANGMANAGEMENT_HPP
#define CLANGMANAGEMENT_HPP

// std
#include <filesystem>
// astfri
#include <libastfri-cpp/inc/ClangVisitor.hpp>
#include <astfri/Astfri.hpp>
// clang
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>
#include <memory>

namespace astfri::cpp {
class CppASTConsumer : public clang::ASTConsumer {
public:
    CppASTConsumer(astfri::TranslationUnit& _tu);
    void HandleTranslationUnit(clang::ASTContext& Context) override;

private:
    astfri::cpp::ClangVisitor Visitor;
};

// Frontend Action
class CppFrontendAction : public clang::ASTFrontendAction {
public:
    CppFrontendAction(astfri::TranslationUnit& _tu);
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& CI,
        clang::StringRef file
    ) override;

private:
    astfri::TranslationUnit& tu;
};

// Custom Frontend Action Factory
class CppFrontendActionFactory : public clang::tooling::FrontendActionFactory {
public:
    CppFrontendActionFactory(astfri::TranslationUnit& _tu);

    std::unique_ptr<clang::FrontendAction> create() override;

private:
    astfri::TranslationUnit& tu;
};


struct cpp_in
{
    astfri::TranslationUnit load_file(const std::filesystem::path& file_path); //const cpp::Config &cfg
    astfri::TranslationUnit load_file(std::istream& is); //const cpp::Config &cfg
    astfri::TranslationUnit load_project(std::filesystem::path& path); //const cpp::Config &cfg
};

// static_assert(astfri::IsInputLibInterface<cpp_in, cpp::Config>, "");

} // namespace astfri::cpp
#endif // CLANGMANAGEMENT_HPP