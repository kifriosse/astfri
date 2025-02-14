#include <libastfri-cpp/inc/ClangVisitor.hpp>
#include "libastfri/inc/Stmt.hpp"
#include <iostream>
#include <vector>

namespace astfri::cpp
{
ClangVisitor::ClangVisitor(TranslationUnit& visitedTranslationUnit) :
    tu_(&visitedTranslationUnit)
{
    this->stmt_factory_ = &StmtFactory::get_instance();
    this->expr_factory_ = &ExprFactory::get_instance();
    this->type_factory_ = &TypeFactory::get_instance();
}

// TODO: prejst cez vsetky deklaracie a statementy pomocou vracania true, budem si pamatat kontext
// a podla toho budem vytvarat AST, teraz to robim rucne ale to asi nieje búhví co
// zatial su vsetky false, tak sa to asi nedostane nikde, tak treba zacat vracat false len po triede asi

// visit deklaracie
bool ClangVisitor::VisitFunctionDecl(clang::FunctionDecl *FD) {
    llvm::outs() << "Function: " << FD->getNameAsString() << "\n";
    if(this->class_index_) {
        this->method_index_ = new int(this->tu_->classes_[*(this->class_index_)]->methods_.size());
        for(auto param : FD->parameters()) {
            TraverseParmVarDecl(param);
        }

        std::vector<ParamVarDefStmt*> params;
        this->tu_->classes_[*(this->class_index_)]->methods_.push_back(
            this->stmt_factory_->mk_method_def(
                this->tu_->classes_[*(this->class_index_)],
                this->stmt_factory_->mk_function_def(
                    FD->getNameAsString(),
                    params,
                    this->type_factory_->mk_int(),
                    this->stmt_factory_->mk_compound(std::vector<Stmt *> {})),
                astfri::AccessModifier::Public // TODO: access modifier
            )
        );
        for(auto stmt : FD->getBody()->children()) {
            TraverseStmt(stmt);
        }
        delete this->method_index_;
        this->method_index_ = nullptr;
    }


    return false;
}
bool ClangVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl *RD) {
    llvm::outs() << "Class: " << RD->getNameAsString() << "\n";
    this->class_index_ = new int(this->tu_->classes_.size());
    
    // vytvorenie triedy
    this->tu_->classes_.push_back(this->stmt_factory_->mk_class_def(
        RD->getNameAsString(),
        std::vector<MemberVarDefStmt*> {},
        std::vector<MethodDefStmt*> {},
        std::vector<GenericParam*> {}
    ));

    // prejdenie vsetkych metod
    for(auto func : RD->methods()) {
        TraverseFunctionDecl(func);
    }

    // prejdenie vsetkych atributov, mozno cez traverse metody by nebolo odveci
    for(auto field : RD->fields()) {
        this->tu_->classes_.back()->vars_.push_back(this->stmt_factory_->mk_member_var_def(
            field->getNameAsString(),
            //TODO: fixnut typy
            this->type_factory_->mk_user(field->getType().getAsString()),
            this->expr_factory_->mk_int_literal(100),
            astfri::AccessModifier::Public)
        );
    }

    delete this->class_index_;
    this->class_index_ = nullptr;
    return false;
}
bool ClangVisitor::VisitVarDecl(clang::VarDecl *VD) {
    llvm::outs() << "Variable:" << VD->getNameAsString() << "\n";
    return false;
}
bool ClangVisitor::VisitParmVarDecl(clang::ParmVarDecl *PVD) {
    llvm::outs() << "Parameter:" << PVD->getNameAsString() << "\n";
    // TODO: typy a hodnoty fixnut
    if (this->class_index_) {
        this->tu_->classes_[*(this->class_index_)]->methods_[*(this->method_index_)]->func_->params_.push_back(
            this->stmt_factory_->mk_param_var_def(
                PVD->getNameAsString(),
                this->type_factory_->mk_int(),
                this->expr_factory_->mk_int_literal(0)
            )
        );
    }
    return false;
}
bool ClangVisitor::VisitFieldDecl(clang::FieldDecl *FD) {
    llvm::outs() << "Field:" << FD->getNameAsString() << "\n";
    return false;
}
bool ClangVisitor::VisitNamespaceDecl(clang::NamespaceDecl *ND) {
    llvm::outs() << "Namespace:" << ND->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::VisitTypedefDecl(clang::TypedefDecl *TD) {
    llvm::outs() << "Typedef:" << TD->getNameAsString() << "\n";
    return false;
}
bool ClangVisitor::VisitEnumDecl(clang::EnumDecl *ED) {
    llvm::outs() << "Enum:" << ED->getNameAsString() << "\n";
    return false;
}
// visit statementy
bool ClangVisitor::VisitStmt(clang::Stmt *S) {
    llvm::outs() << "Statement:" << S->getStmtClassName() << "\n";

    if (this->class_index_ && this->method_index_) {
        //TODO: budovanie tela funkcie
        this->tu_->classes_[*(this->class_index_)]->methods_[*(this->method_index_)]->func_->body_->stmts_.push_back(
            this->stmt_factory_->mk_uknown()
        );
    
    }

    return false;
}
bool ClangVisitor::VisitReturnStmt(clang::ReturnStmt *RS) {
    llvm::outs() << "Return Statement (value): " << RS->getRetValue() << "\n";
    return false;
}
bool ClangVisitor::VisitIfStmt(clang::IfStmt *IS) {
    llvm::outs() << "If statement: " << IS->getCond() << "\n";
    return false;
}
bool ClangVisitor::VisitForStmt(clang::ForStmt *FS) {
    llvm::outs() << "For statement: " << FS->getConditionVariableDeclStmt() << "\n";
    //FS->getBody()->dump();
    return false;
}
bool ClangVisitor::VisitWhileStmt(clang::WhileStmt *WS) {
    llvm::outs() << "While statement: " << WS->getCond() << "\n";
    return false;
}
// visit expression
bool ClangVisitor::VisitExpr(clang::Expr *E) {
    llvm::outs() << "Expression:" << E->getStmtClassName() << "\n";
    return false;
}
bool ClangVisitor::VisitCallExpr(clang::CallExpr *CE) {
    llvm::outs() << "Call expression: " << CE->getCallee() <<"\n";
    return false;
}
bool ClangVisitor::VisitBinaryOperator(clang::BinaryOperator *BO) {
    llvm::outs() << "Binary operator\n";
    return false;
}
bool ClangVisitor::VisitUnaryOperator(clang::UnaryOperator *UO) {
    llvm::outs() << "Unary operator\n";
    return false;
}
} // namespace libastfri::cpp