// astfri headers
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri/inc/Expr.hpp>
#include <libastfri/inc/Stmt.hpp>
// clang headers
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::tooling;

// Custom RecursiveASTVisitor
class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
    // visit deklaracie
    bool VisitFunctionDecl(FunctionDecl *FD) {
        llvm::outs() << "Function: " << FD->getNameAsString() << "\n";
        return true;
    }
    bool VisitCXXRecordDecl(CXXRecordDecl *RD) {
        llvm::outs() << "Class: " << RD->getNameAsString() << "\n";
        return true;
    }
    bool VisitVarDecl(VarDecl *VD) {
        llvm::outs() << "Variable:" << VD->getNameAsString() << "\n";
        return true;
    }
    bool VisitFieldDecl(FieldDecl *FD) {
        llvm::outs() << "Field:" << FD->getNameAsString() << "\n";
        return true;
    }
    bool VisitNamespaceDecl(NamespaceDecl *ND) {
        llvm::outs() << "Namespace:" << ND->getNameAsString() << "\n";
        return true;
    }
    bool VisitTypedefDecl(TypedefDecl *TD) {
        llvm::outs() << "Typedef:" << TD->getNameAsString() << "\n";
        return true;
    }
    bool VisitEnumDecl(EnumDecl *ED) {
        llvm::outs() << "Enum:" << ED->getNameAsString() << "\n";
        return true;
    }

    // visit statementy
    // bool VisitStmt(Stmt *S) {
    //     llvm::outs() << "Statement:" << S->getStmtClassName() << "\n";
    //     return true;
    // }
    bool VisitReturnStmt(ReturnStmt *RS) {
        llvm::outs() << "Return statement\n";
        return true;
    }
    bool VisitIfStmt(IfStmt *IS) {
        llvm::outs() << "If statement\n";
        return true;
    }
    bool VisitForStmt(ForStmt *FS) {
        llvm::outs() << "For statement\n";
        return true;
    }
    bool VisitWhileStmt(WhileStmt *WS) {
        llvm::outs() << "While statement\n";
        return true;
    }

    // visit expression
    // bool VisitExpr(Expr *E) {
    //     llvm::outs() << "Expression:" << E->getStmtClassName() << "\n";
    //     return true;
    // }
    bool VisitCallExpr(CallExpr *CE) {
        llvm::outs() << "Call expression\n";
        return true;
    }
    bool VisitBinaryOperator(BinaryOperator *BO) {
        llvm::outs() << "Binary operator\n";
        return true;
    }
    bool VisitUnaryOperator(UnaryOperator *UO) {
        llvm::outs() << "Unary operator\n";
        return true;
    }
};

// AST Consumer
class MyASTConsumer : public ASTConsumer {
public:
    void HandleTranslationUnit(ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    MyASTVisitor Visitor;
};

// Frontend Action
class MyFrontendAction : public ASTFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) override {
        return std::make_unique<MyASTConsumer>();
    }
};

// Main function
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

int main(int argc, const char **argv) {
    auto OptionsParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!OptionsParser) {
        llvm::errs() << OptionsParser.takeError();
        return 1;
    }
    ClangTool Tool(OptionsParser->getCompilations(), OptionsParser->getSourcePathList());
    return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}