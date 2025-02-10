#include <libastfri-cpp/inc/CppASTVisitor.hpp>
#include "libastfri/inc/Stmt.hpp"
namespace astfri::cpp
{
CppASTVisitor::CppASTVisitor() {
    // konstruktor
}
// visit deklaracie
bool CppASTVisitor::VisitFunctionDecl(clang::FunctionDecl *FD) {
    llvm::outs() << "Function: " << FD->getNameAsString() << "\n";
    // this->methods.push_back(this->stmt_factory.mk_function_def(
    //     FD->getNameAsString(),
    //     std::vector<ParamVarDefStmt*>(),
    //     this->type_factory.mk_user(FD->getReturnType().getAsString()),
    //     nullptr
    // ));
    return true;
}
bool CppASTVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl *RD) {
    llvm::outs() << "Class: " << RD->getNameAsString() << "\n";
    this->name = RD->getNameAsString();
    return true;
}
bool CppASTVisitor::VisitVarDecl(clang::VarDecl *VD) {
    llvm::outs() << "Variable:" << VD->getNameAsString() << "\n";
    return true;
}
bool CppASTVisitor::VisitFieldDecl(clang::FieldDecl *FD) {
    llvm::outs() << "Field:" << FD->getNameAsString() << "\n";
    this->vars.push_back(this->stmt_factory.mk_member_var_def(
        FD->getNameAsString(),
        this->type_factory.mk_user(FD->getType().getAsString()),
        nullptr
    ));
    return true;
}
bool CppASTVisitor::VisitNamespaceDecl(clang::NamespaceDecl *ND) {
    llvm::outs() << "Namespace:" << ND->getNameAsString() << "\n";
    return true;
}
bool CppASTVisitor::VisitTypedefDecl(clang::TypedefDecl *TD) {
    llvm::outs() << "Typedef:" << TD->getNameAsString() << "\n";
    return true;
}
bool CppASTVisitor::VisitEnumDecl(clang::EnumDecl *ED) {
    llvm::outs() << "Enum:" << ED->getNameAsString() << "\n";
    return true;
}
// visit statementy
bool CppASTVisitor::VisitStmt(clang::Stmt *S) {
    llvm::outs() << "Statement:" << S->getStmtClassName() << "\n";
    return true;
}
bool CppASTVisitor::VisitReturnStmt(clang::ReturnStmt *RS) {
    llvm::outs() << "Return Statement (value): " << RS->getRetValue() << "\n";
    return true;
}
bool CppASTVisitor::VisitIfStmt(clang::IfStmt *IS) {
    llvm::outs() << "If statement: " << IS->getCond() << "\n";
    return true;
}
bool CppASTVisitor::VisitForStmt(clang::ForStmt *FS) {
    llvm::outs() << "For statement: " << FS->getConditionVariableDeclStmt() << "\n";
    return true;
}
bool CppASTVisitor::VisitWhileStmt(clang::WhileStmt *WS) {
    llvm::outs() << "While statement: " << WS->getCond() << "\n";
    return true;
}
// visit expression
bool CppASTVisitor::VisitExpr(clang::Expr *E) {
    llvm::outs() << "Expression:" << E->getStmtClassName() << "\n";
    return true;
}
bool CppASTVisitor::VisitCallExpr(clang::CallExpr *CE) {
    llvm::outs() << "Call expression: " << CE->getCallee() <<"\n";
    return true;
}
bool CppASTVisitor::VisitBinaryOperator(clang::BinaryOperator *BO) {
    llvm::outs() << "Binary operator\n";
    return true;
}
bool CppASTVisitor::VisitUnaryOperator(clang::UnaryOperator *UO) {
    llvm::outs() << "Unary operator\n";
    return true;
}
CppASTVisitor::~CppASTVisitor() {
    // awd
}
} // namespace libastfri::cpp