#pragma once

// astfri
#include <libastfri/impl/ExprFwd.hpp>
#include <libastfri/impl/StmtFwd.hpp>
#include <libastfri/impl/TypeFwd.hpp>
#include <libastfri/inc/Expr.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/Stmt.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/Type.hpp>
#include <libastfri/inc/TypeFactory.hpp>

// clang a llvm
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>
#include <clang/AST/OperationKinds.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/APValue.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Type.h>
#include <clang/AST/TypeLoc.h>
#include <clang/Basic/Specifiers.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Casting.h>
#include <llvm-18/llvm/Support/Casting.h> // je to priamo pre 18, moze byt problem asi

// potrebne
#include <vector>
#include <iostream>

namespace astfri::astfri_cpp
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
        const clang::CXXNullPtrLiteralExpr *nullptr_lit_ = nullptr;
    };

    struct AstfriASTLocation {
        Stmt* stmt_ = nullptr;
        Expr* expr_ = nullptr;
        Type* type_ = nullptr;
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
    // // Traverse statementy
    bool TraverseDeclStmt(clang::DeclStmt *DS);
    bool TraverseCompoundStmt(clang::CompoundStmt *CS);
    bool TraverseReturnStmt(clang::ReturnStmt *RS);
    bool TraverseIfStmt(clang::IfStmt *IS);
    bool TraverseForStmt(clang::ForStmt *FS);
    bool TraverseWhileStmt(clang::WhileStmt *WS);
    bool TraverseDoStmt(clang::DoStmt *DS);
    bool TraverseSwitchStmt(clang::SwitchStmt *SS);
    bool TraverseContinueStmt(clang::ContinueStmt *CS);
    bool TraverseBreakStmt(clang::BreakStmt *BS);
    // // Traverse expression
    bool TraverseCXXConstructExpr(clang::CXXConstructExpr *Ctor);
    bool TraverseDeclRefExpr(clang::DeclRefExpr *DRE);
    bool TraverseMemberExpr(clang::MemberExpr *ME);
    bool TraverseCallExpr(clang::CallExpr *CE);
    bool TraverseCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr *DSME);
    bool TraverseCXXThisExpr(clang::CXXThisExpr *TE);
    bool TraverseCXXMemberCallExpr(clang::CXXMemberCallExpr* MCE);
    bool TraverseCXXNewExpr(clang::CXXNewExpr *NE);
    bool TraverseCXXDeleteExpr(clang::CXXDeleteExpr *DE);
    bool TraverseCXXThrowExpr(clang::CXXThrowExpr *TE);
    // // Traverse literals
    bool TraverseIntegerLiteral(clang::IntegerLiteral *IL);
    bool TraverseFloatingLiteral(clang::FloatingLiteral *FL);
    bool TraverseStringLiteral(clang::StringLiteral *SL);
    bool TraverseCXXBoolLiteralExpr(clang::CXXBoolLiteralExpr *BL);
    bool TraverseCharacterLiteral(clang::CharacterLiteral *CL);
    bool TraverseCXXNullPtrLiteralExpr(clang::CXXNullPtrLiteralExpr *NPLE);
    // Traverse operators
    bool TraverseCompoundAssignOperator(clang::CompoundAssignOperator *CAO);
    bool TraverseBinaryOperator(clang::BinaryOperator *BO);
    bool TraverseUnaryOperator(clang::UnaryOperator *UO);

private:
    astfri::AccessModifier getAccessModifier(clang::Decl* decl);
    astfri::ClassDefStmt* get_existing_class(std::string name);
    // types
    astfri::BinOpType get_astfri_bin_op_type(clang::BinaryOperatorKind clang_type);
    astfri::UnaryOpType get_astfri_un_op_type(clang::UnaryOperatorKind clang_type);
    // types:types :D
    astfri::Type* get_astfri_type(clang::QualType QT); // only this is used in visitor
    astfri::Type* get_astfri_type_from_clang_builtintype(const clang::BuiltinType* builtin);
    astfri::Type* get_astfri_pointee(const clang::PointerType* pointer);
    
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
