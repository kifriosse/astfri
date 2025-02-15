#pragma once

#include <libastfri/impl/ExprFwd.hpp>
#include <libastfri/impl/StmtFwd.hpp>
#include <libastfri/impl/TypeFwd.hpp>

#include "libastfri/inc/ExprFactory.hpp"
#include "libastfri/inc/StmtFactory.hpp"
#include "libastfri/inc/TypeFactory.hpp"

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
    // visit deklaracie
    bool VisitFunctionDecl(clang::FunctionDecl *FD);
    bool VisitCXXRecordDecl(clang::CXXRecordDecl *RD);
    bool VisitVarDecl(clang::VarDecl *VD);
    bool VisitParmVarDecl(clang::ParmVarDecl *PVD);
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

private:
    TranslationUnit* tu_;

    StmtFactory* stmt_factory_;
    ExprFactory* expr_factory_;
    TypeFactory* type_factory_;

    // context variables
    int* class_index_ = nullptr;
    int* method_index_ = nullptr;
};
} // namespace libastfri::cpp
