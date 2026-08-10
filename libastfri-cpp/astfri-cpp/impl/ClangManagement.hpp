#ifndef ASTFRI_CPP_IMPL_CLANGMANAGEMENT_HPP
#define ASTFRI_CPP_IMPL_CLANGMANAGEMENT_HPP

#include <astfri-cpp/impl/visitor-methods/ClangVisitor.hpp>

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>

#include <memory>


namespace astfri::cpp {


/**
 * @brief AST Consumer.
 */
class CppASTConsumer : public clang::ASTConsumer {
public:
    CppASTConsumer(astfri::TranslationUnit& _tu);
    void HandleTranslationUnit(clang::ASTContext& Context) override;

private:
    astfri::cpp::ClangVisitor m_visitor;
};


/**
 * @brief Frontend Action.
 */
class CppFrontendAction : public clang::ASTFrontendAction {
public:
    CppFrontendAction(astfri::TranslationUnit &_tu);

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance &CI,
        clang::StringRef file) override;

private:
    astfri::TranslationUnit *m_tu;
};


/**
 * @brief Custom Frontend Action Factory.
 */
class CppFrontendActionFactory : public clang::tooling::FrontendActionFactory {
public:
    CppFrontendActionFactory(astfri::TranslationUnit &tu);

    std::unique_ptr<clang::FrontendAction> create() override;

private:
    astfri::TranslationUnit *m_tu;
};


} // namespace astfri::cpp

#endif // ASTFRI_CPP_IMPL_CLANGMANAGEMENT_HPP
