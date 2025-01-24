#pragma once
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Expr.h>

namespace astfri::cpp
{
class CppASTVisitor : public clang::RecursiveASTVisitor<CppASTVisitor> {
public:
    // visit deklaracie
    bool VisitFunctionDecl(clang::FunctionDecl *FD);
    bool VisitCXXRecordDecl(clang::CXXRecordDecl *RD);
    bool VisitVarDecl(clang::VarDecl *VD);
    bool VisitFieldDecl(clang::FieldDecl *FD);
    bool VisitNamespaceDecl(clang::NamespaceDecl *ND);
    bool VisitTypedefDecl(clang::TypedefDecl *TD);
    bool VisitEnumDecl(clang::EnumDecl *ED);
    // visit statementy
    bool VisitStmt(clang::Stmt *S);
    bool VisitReturnStmt(clang::ReturnStmt *RS);
    bool VisitIfStmt(clang::IfStmt *IS);
    bool VisitForStmt(clang::ForStmt *FS);
    bool VisitWhileStmt(clang::WhileStmt *WS);
    // visit expression
    bool VisitExpr(clang::Expr *E);
    bool VisitCallExpr(clang::CallExpr *CE);
    bool VisitBinaryOperator(clang::BinaryOperator *BO);
    bool VisitUnaryOperator(clang::UnaryOperator *UO);
};
} // namespace libastfri::cpp