#pragma once

#include <libastfri/impl/ExpressionFwd.hpp>
#include <libastfri/impl/StatementFwd.hpp>
#include <libastfri/impl/TypeFwd.hpp>

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>

namespace astfri::cpp
{
class ClangVisitor : public clang::RecursiveASTVisitor<ClangVisitor>
{
public:
    ClangVisitor(TranslationUnit& visitedTranslationUnit);
    TranslationUnit* visitedTranslationUnit;

private:
    TranslationUnit* tu_;
};
} // namespace libastfri::cpp
