#ifndef CLANGMANAGEMENT_HPP
#define CLANGMANAGEMENT_HPP

// std
#include <filesystem>
// astfri
#include <astfri-cpp/impl/ClangVisitor.hpp>
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

// do buducna sa moze nastavit, co je treba z ast prejst, zatial prazdne
class Config {
};

} // namespace astfri::cpp

namespace astfri {
// musi byt v namespace astfri, preto je von z astfri::cpp
struct cpp_in
{
    static astfri::TranslationUnit load_file(const std::filesystem::path& file_path, const astfri::cpp::Config &cfg);
    static astfri::TranslationUnit load_file(std::istream& is, const astfri::cpp::Config &cfg);
    static astfri::TranslationUnit load_project(std::filesystem::path& path, const astfri::cpp::Config &cfg);
};
// static_assert(astfri::IsInputLibInterface<cpp_in, cpp::Config>, "");
} // namespace astfri

#endif // CLANGMANAGEMENT_HPP