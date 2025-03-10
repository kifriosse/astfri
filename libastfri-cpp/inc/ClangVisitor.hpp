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
        const clang::UnaryOperator* un_op_ = nullptr;
        const clang::IntegerLiteral* int_lit_ = nullptr;
        const clang::FloatingLiteral *float_lit_ = nullptr;
        const clang::CXXBoolLiteralExpr *bool_lit_ = nullptr;
        const clang::StringLiteral *string_lit_ = nullptr;
        const clang::CharacterLiteral *char_lit_ = nullptr;
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
public:
    ClangVisitor(TranslationUnit& visitedTranslationUnit);
    TranslationUnit* visitedTranslationUnit;
    // traverse deklaracie
    bool TraverseCXXConstructorDecl(clang::CXXConstructorDecl *Ctor);
    bool TraverseCXXDestructorDecl(clang::CXXDestructorDecl *Dtor);
    bool TraverseFunctionDecl(clang::FunctionDecl *FD);
    bool TraverseCXXMethodDecl(clang::CXXMethodDecl *Decl);
    bool TraverseCXXRecordDecl(clang::CXXRecordDecl *RD);
    bool TraverseVarDecl(clang::VarDecl *VD);
    bool TraverseParmVarDecl(clang::ParmVarDecl *PVD);
    bool TraverseFieldDecl(clang::FieldDecl *FD);
    // bool TraverseClassTemplateDecl(clang::ClassTemplateDecl *CTD);
    // bool TraverseTypedefDecl(clang::TypedefDecl *TD);
    // bool TraverseEnumDecl(clang::EnumDecl *ED);
    // // Traverse statementy
    bool TraverseDeclStmt(clang::DeclStmt *DS);
    bool TraverseCompoundStmt(clang::CompoundStmt *CS);
    bool TraverseReturnStmt(clang::ReturnStmt *RS);
    bool TraverseIfStmt(clang::IfStmt *IS);
    bool TraverseForStmt(clang::ForStmt *FS);
    bool TraverseWhileStmt(clang::WhileStmt *WS);
    bool TraverseDoStmt(clang::DoStmt *DS);
    bool TraverseSwitchStmt(clang::SwitchStmt *SS);
    // // Traverse expression
    bool TraverseCXXConstructExpr(clang::CXXConstructExpr *Ctor);
    bool TraverseDeclRefExpr(clang::DeclRefExpr *DRE);
    bool TraverseMemberExpr(clang::MemberExpr *ME);
    bool TraverseCXXThisExpr(clang::CXXThisExpr *TE);
    bool TraverseCXXMemberCallExpr(clang::CXXMemberCallExpr* MCE);
    // bool TraverseCallExpr(clang::CallExpr *CE);
    // // Traverse literals
    bool TraverseIntegerLiteral(clang::IntegerLiteral *IL);
    bool TraverseFloatingLiteral(clang::FloatingLiteral *FL);
    bool TraverseStringLiteral(clang::StringLiteral *SL);
    bool TraverseCXXBoolLiteralExpr(clang::CXXBoolLiteralExpr *BL);
    bool TraverseCharacterLiteralExpr(clang::CharacterLiteral *CL);
    // Traverse operators
    bool TraverseBinaryOperator(clang::BinaryOperator *BO);
    bool TraverseUnaryOperator(clang::UnaryOperator *UO);

private:
    astfri::AccessModifier getAccessModifier(clang::Decl* decl);
    astfri::ClassDefStmt* get_existing_class(std::string name);
    TranslationUnit* tu_;

    StmtFactory* stmt_factory_;
    ExprFactory* expr_factory_;
    TypeFactory* type_factory_;

    // context variables
    ClangASTLocation clang_location;
    AstfriASTLocation astfri_location;
    bool expr_as_stmt;
};
} // namespace libastfri::cpp
