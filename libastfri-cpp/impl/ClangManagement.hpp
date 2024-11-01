#pragma once

#include <libastfri/inc/Stmt.hpp>

#include <libastfri-cpp/inc/ClangVisitor.hpp>

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>
#include <memory>

namespace astfri::cpp
{
class ClangConsumer : public clang::ASTConsumer
{
public:
    clang::ASTContext* context;
    astfri::TranslationUnit* visitedTranslationUnit;

    ClangConsumer(
        clang::ASTContext& context,
        astfri::TranslationUnit& visitedTranslationUnit
    );
    void HandleTranslationUnit (clang::ASTContext& p_context);
};

class ClangTraverseAction : public clang::ASTFrontendAction
{
    using TUnit = astfri::TranslationUnit;
    TUnit* visitedTranslationUnit;

public:
    ClangTraverseAction(TUnit& visitedTranslationUnit);
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer (
        clang::CompilerInstance& compiler,
        llvm::StringRef
    );
};
} // namespace libastfri::cpp