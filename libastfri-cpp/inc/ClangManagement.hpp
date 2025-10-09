#ifndef CLANGMANAGEMENT_HPP
#define CLANGMANAGEMENT_HPP

// astfri
#include <libastfri-cpp/inc/ClangVisitor.hpp>
#include <libastfri/inc/Stmt.hpp>
// clang
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>
#include <memory>

namespace astfri::astfri_cpp
{
class CppASTConsumer : public clang::ASTConsumer
{
public:
    CppASTConsumer(astfri::TranslationUnit& _tu);
    void HandleTranslationUnit(clang::ASTContext& Context) override;

private:
    astfri::astfri_cpp::ClangVisitor Visitor;
};

// Frontend Action
class CppFrontendAction : public clang::ASTFrontendAction
{
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
class CppFrontendActionFactory : public clang::tooling::FrontendActionFactory
{
public:
    CppFrontendActionFactory(astfri::TranslationUnit& _tu);

    std::unique_ptr<clang::FrontendAction> create() override;

private:
    astfri::TranslationUnit& tu;
};

int fill_translation_unit(astfri::TranslationUnit& tu, std::string const& file_path);

} // namespace astfri::astfri_cpp
#endif // CLANGMANAGEMENT_HPP