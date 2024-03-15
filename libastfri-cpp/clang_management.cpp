#include <libastfri-cpp/clang_management.hpp>

#include <libastfri/factories/ExpressionFactory.hpp>

AstfriClangConsumer::AstfriClangConsumer(
    clang::ASTContext &context,
    libastfri::structures::TranslationUnitStatement &visitedTranslationUnit)
    : context(&context), visitedTranslationUnit(&visitedTranslationUnit) {}

void AstfriClangConsumer::HandleTranslationUnit(clang::ASTContext &p_context) {
  AstfriClangVisitor visitor;

  visitor.TraverseDecl(p_context.getTranslationUnitDecl());

  // todo - ako to spravit nejako takto 
  //   visitedTranslationUnit = &*visitor.visitedTranslationUnit;

  visitedTranslationUnit->functions =
      std::move(visitor.visitedTranslationUnit->functions);
}

AstfriClangTraverseAction::AstfriClangTraverseAction(
    libastfri::structures::TranslationUnitStatement &visitedTranslationUnit)
    : visitedTranslationUnit(&visitedTranslationUnit) {}

std::unique_ptr<clang::ASTConsumer>
AstfriClangTraverseAction::CreateASTConsumer(clang::CompilerInstance &compiler,
                                             llvm::StringRef) {
  return std::make_unique<AstfriClangConsumer>(compiler.getASTContext(),
                                               *visitedTranslationUnit);
}
