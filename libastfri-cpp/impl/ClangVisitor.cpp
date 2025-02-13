#include <libastfri-cpp/inc/ClangVisitor.hpp>
#include "libastfri/inc/Stmt.hpp"

namespace astfri::cpp
{
ClangVisitor::ClangVisitor(TranslationUnit& visitedTranslationUnit) :
    tu_(&visitedTranslationUnit)
{
}
// visit deklaracie
bool ClangVisitor::VisitFunctionDecl(clang::FunctionDecl *FD) {
    llvm::outs() << "Function: " << FD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl *RD) {
    llvm::outs() << "Class: " << RD->getNameAsString() << "\n";
    this->tu_->classes_.push_back(this->stmt_factory.mk_class_def(
        RD->getNameAsString(),
        std::vector<MemberVarDefStmt*> {},
        std::vector<MethodDefStmt*> {},
        std::vector<GenericParam*> {}
    ));

    for(auto field : RD->fields()) {
        // visit member variables
        this->tu_->classes_.back()->vars_.push_back(this->stmt_factory.mk_member_var_def(
            field->getNameAsString(),
            this->type_factory.mk_user(field->getType().getAsString()),
            this->expr_factory.mk_int_literal(100))
        );
    }

    return true;
}
bool ClangVisitor::VisitVarDecl(clang::VarDecl *VD) {
    llvm::outs() << "Variable:" << VD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::VisitFieldDecl(clang::FieldDecl *FD) {
    llvm::outs() << "Field:" << FD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::VisitNamespaceDecl(clang::NamespaceDecl *ND) {
    llvm::outs() << "Namespace:" << ND->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::VisitTypedefDecl(clang::TypedefDecl *TD) {
    llvm::outs() << "Typedef:" << TD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::VisitEnumDecl(clang::EnumDecl *ED) {
    llvm::outs() << "Enum:" << ED->getNameAsString() << "\n";
    return true;
}
// visit statementy
bool ClangVisitor::VisitStmt(clang::Stmt *S) {
    llvm::outs() << "Statement:" << S->getStmtClassName() << "\n";
    return true;
}
bool ClangVisitor::VisitReturnStmt(clang::ReturnStmt *RS) {
    llvm::outs() << "Return Statement (value): " << RS->getRetValue() << "\n";
    return true;
}
bool ClangVisitor::VisitIfStmt(clang::IfStmt *IS) {
    llvm::outs() << "If statement: " << IS->getCond() << "\n";
    return true;
}
bool ClangVisitor::VisitForStmt(clang::ForStmt *FS) {
    llvm::outs() << "For statement: " << FS->getConditionVariableDeclStmt() << "\n";
    //FS->getBody()->dump();
    return true;
}
bool ClangVisitor::VisitWhileStmt(clang::WhileStmt *WS) {
    llvm::outs() << "While statement: " << WS->getCond() << "\n";
    return true;
}
// visit expression
bool ClangVisitor::VisitExpr(clang::Expr *E) {
    llvm::outs() << "Expression:" << E->getStmtClassName() << "\n";
    return true;
}
bool ClangVisitor::VisitCallExpr(clang::CallExpr *CE) {
    llvm::outs() << "Call expression: " << CE->getCallee() <<"\n";
    return true;
}
bool ClangVisitor::VisitBinaryOperator(clang::BinaryOperator *BO) {
    llvm::outs() << "Binary operator\n";
    return true;
}
bool ClangVisitor::VisitUnaryOperator(clang::UnaryOperator *UO) {
    llvm::outs() << "Unary operator\n";
    return true;
}
} // namespace libastfri::cpp