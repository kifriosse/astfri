// astfri headers
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri/inc/Expr.hpp>
#include <libastfri/inc/Stmt.hpp>
#include <libastfri-cpp/inc/ClangVisitor.hpp>
// clang headers
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/raw_ostream.h>
#include <vector>
#include <iostream>

#include <libastfri-text/inc/ASTVisitor.hpp>


// AST Consumer
class CppASTConsumer : public clang::ASTConsumer {
public:
    CppASTConsumer(astfri::TranslationUnit& _tu) : Visitor(_tu) {}
    void HandleTranslationUnit(clang::ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    astfri::cpp::ClangVisitor Visitor;
};


// Frontend Action
class CppFrontendAction : public clang::ASTFrontendAction {
    public:
    CppFrontendAction(astfri::TranslationUnit& _tu) : tu(_tu) {}
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override {
        return std::make_unique<CppASTConsumer>(tu);
    }
    private:
    astfri::TranslationUnit& tu;
};

// Custom Frontend Action Factory
class CppFrontendActionFactory : public clang::tooling::FrontendActionFactory {
    public:
    CppFrontendActionFactory(astfri::TranslationUnit& _tu) : tu(_tu) {}

    std::unique_ptr<clang::FrontendAction> create() override {
        return std::make_unique<CppFrontendAction>(tu);
    }

    private:
    astfri::TranslationUnit& tu;
};

// Main function
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

int main(int argc, const char **argv) {

    
    
    astfri::TranslationUnit tu(
        std::vector<astfri::ClassDefStmt*> {},
        std::vector<astfri::FunctionDefStmt*> {},
        std::vector<astfri::GlobalVarDefStmt*> {}
    );
    auto OptionsParser = clang::tooling::CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!OptionsParser) {
        llvm::errs() << OptionsParser.takeError();
        return 1;
    }
    clang::tooling::ClangTool Tool(OptionsParser->getCompilations(), OptionsParser->getSourcePathList());
    Tool.run(std::make_unique<CppFrontendActionFactory>(tu).get());
    
    // AST Visitor - nice
    ASTVisitor* visitor = new ASTVisitor();
    visitor->visit(tu);
    visitor->write_file();

    return 0;
}