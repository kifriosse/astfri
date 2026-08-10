#ifndef ASTFRI_CPP_IMPL_CLANG_VISITOR_HPP
#define ASTFRI_CPP_IMPL_CLANG_VISITOR_HPP

#include <astfri/Astfri.hpp>

#include <clang/AST/APValue.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>
#include <clang/AST/OperationKinds.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Type.h>
#include <clang/AST/TypeLoc.h>
#include <clang/Basic/Specifiers.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/raw_ostream.h>

#include <cstdio>


namespace astfri::cpp {


struct ClangASTLocation {
    clang::Decl *decl                         = nullptr;
    clang::Stmt *stmt                         = nullptr;
    clang::Expr *expr                         = nullptr;
    clang::BinaryOperator *bin_op             = nullptr;
    clang::UnaryOperator *un_op               = nullptr;
    clang::IntegerLiteral *int_lit            = nullptr;
    clang::FloatingLiteral *float_lit         = nullptr;
    clang::CXXBoolLiteralExpr *bool_lit       = nullptr;
    clang::StringLiteral *string_lit          = nullptr;
    clang::CharacterLiteral *char_lit         = nullptr;
    clang::CXXNullPtrLiteralExpr *nullptr_lit = nullptr;
};


struct AstfriASTLocation {
    Stmt *stmt = nullptr;
    Expr *expr = nullptr;
    Type *type = nullptr;
};


class ClangVisitor : public clang::RecursiveASTVisitor<ClangVisitor> {
public:
    ClangVisitor(TranslationUnit &visitedTranslationUnit, clang::SourceManager *pSM);

    /**
     * @brief Filtering includes.
     */
    bool isInMainFile(clang::SourceLocation Loc) const;

    /**
     * @brief Returning desired namespace (filtering includes).
     */
    clang::NamespaceDecl* get_desired_namespace(clang::TranslationUnitDecl* TU);

    void setSM(clang::SourceManager* pSM);

    clang::SourceManager* getSM();

    void setMainFileID(clang::FileID MFID);

    clang::FileID getMFID();

    // // Declarations:
    bool VisitNamespaceDecl(clang::NamespaceDecl* ND);
    bool TraverseCXXConstructorDecl(clang::CXXConstructorDecl* Ctor);
    bool TraverseCXXDestructorDecl(clang::CXXDestructorDecl* Dtor);
    bool TraverseFunctionDecl(clang::FunctionDecl* FD);
    bool TraverseCXXMethodDecl(clang::CXXMethodDecl* Decl);
    bool TraverseCXXRecordDecl(clang::CXXRecordDecl* RD);
    bool TraverseVarDecl(clang::VarDecl* VD);
    bool TraverseParmVarDecl(clang::ParmVarDecl* PVD);
    bool TraverseFieldDecl(clang::FieldDecl* FD);

    // // Statements:
    bool TraverseDeclStmt(clang::DeclStmt* DS);
    bool TraverseCompoundStmt(clang::CompoundStmt* CS);
    bool TraverseReturnStmt(clang::ReturnStmt* RS);
    bool TraverseIfStmt(clang::IfStmt* IS);
    bool TraverseForStmt(clang::ForStmt* FS);
    bool TraverseWhileStmt(clang::WhileStmt* WS);
    bool TraverseDoStmt(clang::DoStmt* DS);
    bool TraverseSwitchStmt(clang::SwitchStmt* SS);
    bool TraverseContinueStmt(clang::ContinueStmt* CS);
    bool TraverseBreakStmt(clang::BreakStmt* BS);

    // // Expressions:
    bool TraverseCXXConstructExpr(clang::CXXConstructExpr* Ctor);
    bool TraverseDeclRefExpr(clang::DeclRefExpr* DRE);
    bool TraverseLambdaExpr(clang::LambdaExpr* LBD);
    bool TraverseMemberExpr(clang::MemberExpr* ME);
    bool TraverseCallExpr(clang::CallExpr* CE);
    bool TraverseCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr* DSME);
    bool TraverseCXXThisExpr(clang::CXXThisExpr* TE);
    bool TraverseCXXMemberCallExpr(clang::CXXMemberCallExpr* MCE);
    bool TraverseCXXNewExpr(clang::CXXNewExpr* NE);
    bool TraverseCXXDeleteExpr(clang::CXXDeleteExpr* DE);
    bool TraverseCXXThrowExpr(clang::CXXThrowExpr* TE);
    bool TraverseCXXOperatorCallExpr(clang::CXXOperatorCallExpr *COCE);
    bool TraverseParenExpr(clang::ParenExpr *PE);

    // // Literals:
    bool TraverseIntegerLiteral(clang::IntegerLiteral* IL);
    bool TraverseFloatingLiteral(clang::FloatingLiteral* FL);
    bool TraverseStringLiteral(clang::StringLiteral* SL);
    bool TraverseCXXBoolLiteralExpr(clang::CXXBoolLiteralExpr* BL);
    bool TraverseCharacterLiteral(clang::CharacterLiteral* CL);
    bool TraverseCXXNullPtrLiteralExpr(clang::CXXNullPtrLiteralExpr* NPLE);

    // // Operators:
    bool TraverseCompoundAssignOperator(clang::CompoundAssignOperator* CAO);
    bool TraverseBinaryOperator(clang::BinaryOperator* BO);
    bool TraverseUnaryOperator(clang::UnaryOperator* UO);

private:
    clang::SourceManager* m_SM; //!< For filtering includes.
    clang::FileID m_MainFileID; //!< For filtering includes.

    astfri::AccessModifier getAccessModifier(clang::Decl* decl);
    astfri::ClassDefStmt* get_existing_class(std::string name);

    astfri::BinOpType get_astfri_bin_op_type(clang::BinaryOperatorKind clang_type);
    astfri::UnaryOpType get_astfri_un_op_type(clang::UnaryOperatorKind clang_type);
    astfri::Type* get_astfri_type(clang::QualType QT); // only this is used in visitor
    astfri::Type* get_astfri_type_from_clang_builtintype(const clang::BuiltinType* builtin);
    astfri::Type* get_astfri_pointee(const clang::PointerType* pointer);

    TranslationUnit* m_tu;

    StmtFactory* m_stmt_factory;
    ExprFactory* m_expr_factory;
    TypeFactory* m_type_factory;

    ClangASTLocation m_clang_location; //!< For context, clang location.
    AstfriASTLocation m_astfri_location; //!< For context, astfri location.
    bool m_expr_as_stmt;
};


} // namespace astfri::cpp

#endif // ASTFRI_CPP_IMPL_CLANG_VISITOR_HPP
