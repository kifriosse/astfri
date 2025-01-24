// astfri headers
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri/inc/Expr.hpp>
#include <libastfri/inc/Stmt.hpp>
// clang headers
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>

#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/FrontendActions.h>

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/Support/raw_ostream.h>

#include <libastfri-cpp/inc/CppASTVisitor.hpp>


// AST Consumer
class CppASTConsumer : public clang::ASTConsumer {
public:
    void HandleTranslationUnit(clang::ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    astfri::cpp::CppASTVisitor Visitor;
};

// Frontend Action
class CppFrontendAction : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override {
        return std::make_unique<CppASTConsumer>();
    }
};

// Main function
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

int main(int argc, const char **argv) {
    auto OptionsParser = clang::tooling::CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!OptionsParser) {
        llvm::errs() << OptionsParser.takeError();
        return 1;
    }
    clang::tooling::ClangTool Tool(OptionsParser->getCompilations(), OptionsParser->getSourcePathList());
    return Tool.run(clang::tooling::newFrontendActionFactory<CppFrontendAction>().get());
}