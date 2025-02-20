#pragma once

#include <libastfri/impl/ExprFwd.hpp>
#include <libastfri/impl/StmtFwd.hpp>
#include <libastfri/impl/TypeFwd.hpp>

#include "libastfri/inc/Expr.hpp"
#include "libastfri/inc/ExprFactory.hpp"
#include "libastfri/inc/Stmt.hpp"
#include "libastfri/inc/StmtFactory.hpp"
#include "libastfri/inc/Type.hpp"
#include "libastfri/inc/TypeFactory.hpp"

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>
#include <vector>

namespace astfri::cpp
{
    struct ClangASTLocation {
        const clang::Decl* decl_ = nullptr;
        const clang::Stmt* stmt_ = nullptr;
        const clang::Expr* expr_ = nullptr;
        const clang::BinaryOperator* bin_op_ = nullptr;
        const clang::IntegerLiteral* int_lit_ = nullptr;
        const clang::FloatingLiteral *float_lit_ = nullptr;
        const clang::CXXBoolLiteralExpr *bool_lit_ = nullptr;
        const clang::StringLiteral *string_lit_ = nullptr;
        const ClangASTLocation* parent_ = nullptr;
    };

    struct AstfriASTLocation {
        Stmt* stmt_ = nullptr;
        Expr* expr_ = nullptr;
        Type* type_ = nullptr;
        AstfriASTLocation* parent_ = nullptr;
    };
        
class ClangVisitor : public clang::RecursiveASTVisitor<ClangVisitor>
{
    using Base = RecursiveASTVisitor<ClangVisitor>; // Definujeme alias na base class
public:
    ClangVisitor(TranslationUnit& visitedTranslationUnit);
    TranslationUnit* visitedTranslationUnit;
    // traverse deklaracie
    // bool TraverseDecl(clang::Decl *D);
    bool TraverseCXXConstructorDecl(clang::CXXConstructorDecl *Ctor);
    bool TraverseFunctionDecl(clang::FunctionDecl *FD);
    bool TraverseCXXMethodDecl(clang::CXXMethodDecl *Decl);
    bool TraverseCXXRecordDecl(clang::CXXRecordDecl *RD);
    bool TraverseVarDecl(clang::VarDecl *VD);
    bool TraverseParmVarDecl(clang::ParmVarDecl *PVD);
    bool TraverseFieldDecl(clang::FieldDecl *FD);
    // bool TraverseNamespaceDecl(clang::NamespaceDecl *ND);
    // bool TraverseTypedefDecl(clang::TypedefDecl *TD);
    // bool TraverseEnumDecl(clang::EnumDecl *ED);
    // // Traverse statementy
    // bool TraverseStmt(clang::Stmt *S);
    bool TraverseCompoundStmt(clang::CompoundStmt *CS);
    bool TraverseReturnStmt(clang::ReturnStmt *RS);
    // bool TraverseIfStmt(clang::IfStmt *IS);
    // bool TraverseForStmt(clang::ForStmt *FS);
    // bool TraverseWhileStmt(clang::WhileStmt *WS);
    // // Traverse expression
    bool TraverseCXXConstructExpr(clang::CXXConstructExpr *Ctor);
    bool TraverseDeclRefExpr(clang::DeclRefExpr *DRE);
    // bool TraverseExpr(clang::Expr *E);
    // bool TraverseCallExpr(clang::CallExpr *CE);
    // bool TraverseBinaryOperator(clang::BinaryOperator *BO);
    // bool TraverseUnaryOperator(clang::UnaryOperator *UO);
    // // Traverse literals
    bool TraverseIntegerLiteral(clang::IntegerLiteral *IL);
    bool TraverseFloatingLiteral(clang::FloatingLiteral *FL);
    bool TraverseStringLiteral(clang::StringLiteral *SL);
    bool TraverseCXXBoolLiteralExpr(clang::CXXBoolLiteralExpr *BL);
    // Traverse operators
    bool TraverseBinaryOperator(clang::BinaryOperator *BO);

private:
    TranslationUnit* tu_;

    StmtFactory* stmt_factory_;
    ExprFactory* expr_factory_;
    TypeFactory* type_factory_;

    // context variables
    ClassDefStmt* class_ = nullptr;
    MethodDefStmt* method_ = nullptr;
    ParamVarDefStmt* param_ = nullptr;
    MemberVarDefStmt* field_ = nullptr;
    VarDefStmt* var_ = nullptr;
    FunctionDefStmt* function_ = nullptr;
    CompoundStmt* compound_ = nullptr;

    std::vector<ClangASTLocation*> clang_locationStack;
    std::vector<AstfriASTLocation*> astfri_locationStack;

};
} // namespace libastfri::cpp
