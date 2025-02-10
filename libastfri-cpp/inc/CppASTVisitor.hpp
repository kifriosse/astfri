#pragma once
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Expr.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Stmt.h>
#include "libastfri/inc/ExprFactory.hpp"
#include "libastfri/inc/StmtFactory.hpp"
#include "libastfri/inc/TypeFactory.hpp"

//#include <libastfri/inc/Stmt.hpp>

namespace astfri::cpp
{
class CppASTVisitor : public clang::RecursiveASTVisitor<CppASTVisitor> {
public:
    CppASTVisitor();
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
    ~CppASTVisitor();
private:
    astfri::TranslationUnit* tu;
    astfri::StmtFactory& stmt_factory = astfri::StmtFactory::get_instance();
    astfri::ExprFactory& expr_factory = astfri::ExprFactory::get_instance();
    astfri::TypeFactory& type_factory = astfri::TypeFactory::get_instance();

    // zatial len pre jednu triedu
    std::string name;
    std::vector<MemberVarDefStmt*> vars;
    std::vector<MethodDefStmt*> methods;
    std::vector<GenericParam*> tparams;
};

} // namespace libastfri::cpp