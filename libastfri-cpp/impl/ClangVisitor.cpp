#include <libastfri-cpp/inc/ClangVisitor.hpp>
#include "libastfri/inc/Expr.hpp"
#include "libastfri/inc/Stmt.hpp"
#include "libastfri/inc/Type.hpp"
#include <clang/AST/APValue.h>
#include <clang/AST/Decl.h>
#include <clang/Basic/Specifiers.h>
#include <cstddef>
#include <iostream>
#include <llvm-18/llvm/Support/raw_ostream.h>
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

// traverse deklaracie
// bool ClangVisitor::TraverseDecl(clang::Decl *D) {
//     llvm::outs() << "Declaration:" << D->getDeclKindName() << "\n";

//     bool result = Base::TraverseDecl(D);

//     return true;
// }
bool ClangVisitor::TraverseCXXConstructorDecl(clang::CXXConstructorDecl *Ctor) {
    llvm::outs() << "Konstruktor zaciatok: " << Ctor->getNameAsString() << "\n";

    //TODO: napraviť aj pre inline a neskoršiu defuníciu
    // ak ma konstruktor telo tak sa vytvori
    MethodDefStmt* new_ctor = nullptr;
    if (Ctor->hasBody()) {
        // vytvorenie konstruktora
        auto new_ctor = this->stmt_factory_->mk_method_def(
            ((ClassDefStmt*)this->astfri_locationStack.back()->stmt_),
            this->stmt_factory_->mk_function_def(
                Ctor->getNameAsString(),
                std::vector<ParamVarDefStmt*> {},
                this->type_factory_->mk_user(Ctor->getNameAsString()),
                this->stmt_factory_->mk_compound(std::vector<Stmt *> {})),
            astfri::AccessModifier::Public //TODO: vies co XD
        );
        ((ClassDefStmt*)this->astfri_locationStack.back()->stmt_)->methods_.push_back(new_ctor);
    }

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->stmt_ = new_ctor;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->decl_ = Ctor;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseCXXConstructorDecl(Ctor);

    // vymazanie AST location
    delete this->astfri_locationStack.back(); 
    this->astfri_locationStack.pop_back();
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();

    llvm::outs() << "Konstruktor koniec: " << Ctor->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::TraverseFunctionDecl(clang::FunctionDecl *FD) {
    llvm::outs() << "Function zaciatok: " << FD->getNameAsString() << "\n";

    // akcia na tomto vrchole
    auto new_function = this->stmt_factory_->mk_function_def(
        FD->getNameAsString(),
        std::vector<ParamVarDefStmt*> {},
        this->type_factory_->mk_user(FD->getReturnType().getAsString()), //TODO: fixnut typ
        this->stmt_factory_->mk_compound(std::vector<Stmt *> {})
    );
    this->tu_->functions_.push_back(new_function);

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->stmt_ = new_function;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->decl_ = FD;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseFunctionDecl(FD);

    // vymazanie AST location
    delete this->astfri_locationStack.back(); 
    this->astfri_locationStack.pop_back();
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();
    llvm::outs() << "Function koniec: " << FD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::TraverseCXXMethodDecl(clang::CXXMethodDecl *MD) {
    llvm::outs() << "Metoda zaciatok: " << MD->getNameAsString() << "\n";

    // akcia na tomto vrchole
    auto new_method = this->stmt_factory_->mk_method_def(
        (ClassDefStmt*)this->astfri_locationStack.back()->stmt_,
        this->stmt_factory_->mk_function_def(
            MD->getNameAsString(),
            std::vector<ParamVarDefStmt*> {},
            this->type_factory_->mk_user(MD->getReturnType().getAsString()), //TODO: fixnut typ
            this->stmt_factory_->mk_compound(std::vector<Stmt *> {})),
        astfri::AccessModifier::Public
    );
    ((ClassDefStmt*)this->astfri_locationStack.back()->stmt_)->methods_.push_back(new_method);

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->stmt_ = new_method;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->decl_ = MD;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseCXXMethodDecl(MD);

    // vymazanie AST location
    delete this->astfri_locationStack.back(); 
    this->astfri_locationStack.pop_back();
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();
    llvm::outs() << "Metoda koniec: " << MD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::TraverseCXXRecordDecl(clang::CXXRecordDecl *RD) {
    llvm::outs() << "Class zaciatok: " << RD->getNameAsString() << "\n";

    // akcia na vrchole
    auto new_class = this->stmt_factory_->mk_class_def(
        RD->getNameAsString(),
        std::vector<MemberVarDefStmt*> {},
        std::vector<MethodDefStmt*> {},
        std::vector<GenericParam*> {}
    );
    this->tu_->classes_.push_back(new_class);
    this->class_ = new_class;

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->stmt_ = new_class;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->decl_ = RD;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseCXXRecordDecl(RD);

    // vymazanie AST location
    delete this->astfri_locationStack.back(); 
    this->astfri_locationStack.pop_back();
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();
    
    llvm::outs() << "Class koniec: " << RD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::TraverseVarDecl(clang::VarDecl *VD) {
    llvm::outs() << "Variabilna zaciatok: " << VD->getNameAsString() << "\n";

    // akcia na tomto vrchole
    VarDefStmt* new_var = nullptr;
    if (this->astfri_locationStack.empty()) {
        // globalna premenna
        new_var = this->stmt_factory_->mk_global_var_def(
            VD->getNameAsString(),
            this->type_factory_->mk_user(VD->getType().getAsString()), //TODO: fixnut
            nullptr
        );
        this->tu_->globals_.push_back((GlobalVarDefStmt*)new_var);
    } else {
        // premenna v compounde
        new_var = this->stmt_factory_->mk_local_var_def(
            VD->getNameAsString(),
            this->type_factory_->mk_user(VD->getType().getAsString()), //TODO: fixnut
            nullptr
        );
        ((CompoundStmt*)this->astfri_locationStack.back()->stmt_)->stmts_.push_back(new_var);
    }

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->stmt_ = new_var;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->decl_ = VD;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseVarDecl(VD);

    if (VD->hasInit()) {
        new_var->initializer_ = this->astfri_locationStack.back()->expr_;
        // vymazanie AST location inicializatora
        delete this->astfri_locationStack.back(); 
        this->astfri_locationStack.pop_back();
        delete this->clang_locationStack.back();
        this->clang_locationStack.pop_back();
    }

    // vymazanie AST location
    delete this->astfri_locationStack.back(); 
    this->astfri_locationStack.pop_back();
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();

    llvm::outs() << "Variabilna koniec: " << VD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::TraverseParmVarDecl(clang::ParmVarDecl *PVD) {
    llvm::outs() << "Parameter zaciatok: " << PVD->getNameAsString() << "\n";

    // akcia na tomto vrchole
    ParamVarDefStmt* new_par = nullptr;
    new_par = this->stmt_factory_->mk_param_var_def(
        PVD->getNameAsString(),
        this->type_factory_->mk_int(), //TODO: fixnut typ
        nullptr
    );
    if(this->astfri_locationStack.size() == 1) {
        // toto ma iny kod na vkladanie oproti ostatnym traverse metodam
        this->tu_->functions_.back()->params_.push_back(new_par);
    } else {
        // parameter funkcie (nie metody)
        this->tu_->classes_.back()->methods_.back()->func_->params_.push_back(new_par);
    }

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->stmt_ = new_par;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->decl_ = PVD;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseVarDecl(PVD);

    // vymazanie AST location
    delete this->astfri_locationStack.back(); 
    this->astfri_locationStack.pop_back();
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();
    llvm::outs() << "Parameter koniec: " << PVD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::TraverseFieldDecl(clang::FieldDecl *FD) {
    // akcia na tomto node
    llvm::outs() << "Field zaciatok: " << FD->getNameAsString() << "\n";

    // vytvorenie premennej triedy
    astfri::AccessModifier access;
    switch (FD->getAccess()) {
    case clang::AccessSpecifier::AS_public:
        access = astfri::AccessModifier::Public;
        break;
    case clang::AccessSpecifier::AS_protected:
        access = astfri::AccessModifier::Protected;
        break;
    case clang::AccessSpecifier::AS_private:
        access = astfri::AccessModifier::Private;
        break;
    default:
        access = astfri::AccessModifier::Public;
        break;
    }
    auto new_member = this->stmt_factory_->mk_member_var_def(
        FD->getNameAsString(),
        this->type_factory_->mk_int(), //TODO: fixnut typ a inicializator
        nullptr,
        access
    );
    ((ClassDefStmt*)this->astfri_locationStack.back()->stmt_)->vars_.push_back(new_member);

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->stmt_ = new_member;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->decl_ = FD;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseFieldDecl(FD);

    // ak ma inicializator
    if (FD->hasInClassInitializer()) {
        new_member->initializer_ = this->astfri_locationStack.back()->expr_;
        // vymazanie AST location inicializatora
        delete this->astfri_locationStack.back(); 
        this->astfri_locationStack.pop_back();
        delete this->clang_locationStack.back();
        this->clang_locationStack.pop_back();
    }

    // vymazanie AST location
    delete this->astfri_locationStack.back(); 
    this->astfri_locationStack.pop_back();
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();

    llvm::outs() << "Field koniec: " << FD->getNameAsString() << "\n";
    
    return true;
}
// bool ClangVisitor::TraverseNamespaceDecl(clang::NamespaceDecl *ND) {
//     llvm::outs() << "Namespace:" << ND->getNameAsString() << "\n";
//     return true;
// }
// bool ClangVisitor::TraverseTypedefDecl(clang::TypedefDecl *TD) {
//     llvm::outs() << "Typedef:" << TD->getNameAsString() << "\n";
//     return true;
// }
// bool ClangVisitor::TraverseEnumDecl(clang::EnumDecl *ED) {
//     llvm::outs() << "Enum:" << ED->getNameAsString() << "\n";
//     return true;
// }
// // traverse statementy
// bool ClangVisitor::TraverseStmt(clang::Stmt *S) {
//     llvm::outs() << "Statement:" << S->getStmtClassName() << "\n";
//     return true;
// }
bool ClangVisitor::TraverseCompoundStmt(clang::CompoundStmt *CS) {
    llvm::outs() << "Compound Statement zaciatok:" << "\n";

    // akcia na tomto vrchole
    CompoundStmt* new_compound;
    if (this->astfri_locationStack.size() == 1) {
        new_compound = ((FunctionDefStmt*)this->astfri_locationStack.back()->stmt_)->body_;
    } else {
        new_compound = ((MethodDefStmt*)this->astfri_locationStack.back()->stmt_)->func_->body_;
    }

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->stmt_ = new_compound;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->stmt_ = CS;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseCompoundStmt(CS);

    // vymazanie AST location
    delete this->astfri_locationStack.back(); 
    this->astfri_locationStack.pop_back();
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();
    llvm::outs() << "Compound Statement koniec:" << "\n";
    return true;
}
bool ClangVisitor::TraverseReturnStmt(clang::ReturnStmt *RS) {
    llvm::outs() << "Return Statement (value): " << RS->getRetValue() << "\n";

    // akcia na tomto vrchole
    ReturnStmt* new_return = this->stmt_factory_->mk_return(
        nullptr //TODO: fixnut
    );
    std::cout << "Sem som sa dostal" << std::endl;
    ((CompoundStmt*)this->astfri_locationStack.back()->stmt_)->stmts_.push_back(new_return);
    // if(this->astfri_locationStack.size() == 1) {
    //     // globalna funkcia
    // } else {
    //     // metoda triedy
    // }

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->stmt_ = new_return;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->stmt_ = RS;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseReturnStmt(RS);

    // naplnenie return stmt
    new_return->val_ = this->astfri_locationStack.back()->expr_;
    //vymazanie AST location navratovej hodnoty
    delete this->astfri_locationStack.back();
    this->astfri_locationStack.pop_back();
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();


    // vymazanie AST location
    delete this->astfri_locationStack.back(); 
    this->astfri_locationStack.pop_back();
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();
    return true;
}
// bool ClangVisitor::TraverseIfStmt(clang::IfStmt *IS) {
//     llvm::outs() << "If statement: ";
//     return true;
// }
// bool ClangVisitor::TraverseForStmt(clang::ForStmt *FS) {
//     llvm::outs() << "For statement: " << FS->getConditionVariableDeclStmt() << "\n";
//     return true;
// }
// bool ClangVisitor::TraverseWhileStmt(clang::WhileStmt *WS) {
//     llvm::outs() << "While statement: " << WS->getCond() << "\n";
//     return true;
// }
// // visit expression
bool ClangVisitor::TraverseCXXConstructExpr(clang::CXXConstructExpr *Ctor) {
    llvm::outs() << "CXXConstructExpr: " << Ctor->getConstructor()->getNameAsString() << "\n";

    // akcia na tomto vrchole
    auto new_ctor = this->expr_factory_->mk_method_call(
        //TODO: zatial dam len ako string, neviem ci netreba nejaky pointer alebo nieco
        this->expr_factory_->mk_string_literal(Ctor->getConstructor()->getParent()->getNameAsString()),
        Ctor->getConstructor()->getNameAsString(),
        std::vector<Expr *> {}
    );

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->expr_ = new_ctor;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->expr_ = Ctor;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseCXXConstructExpr(Ctor);

    // vymazanie AST location netreba
    return true;
}
bool ClangVisitor::TraverseDeclRefExpr(clang::DeclRefExpr *DRE) {
    llvm::outs() << "DeclRefExpr: " << DRE->getDecl()->getNameAsString() << "\n";

    // akcia na tomto vrchole
    Expr* new_decl_ref_expr = nullptr;
    new_decl_ref_expr = this->expr_factory_->mk_param_var_ref(
        llvm::dyn_cast<clang::ParmVarDecl>(DRE->getDecl())->getNameAsString()
    );
    
    // ak je parameter
    if (auto PVD = llvm::dyn_cast<clang::ParmVarDecl>(DRE->getDecl())) {
    }

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->expr_ = new_decl_ref_expr;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->expr_= DRE;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseDeclRefExpr(DRE);

    // vymazanie AST location, zatial vymazem aby som to mohol zobrat v bin op
    // delete this->astfri_locationStack.back(); 
    // this->astfri_locationStack.pop_back();
    // delete this->clang_locationStack.back();
    // this->clang_locationStack.pop_back();
    
    return true;
}
// bool ClangVisitor::TraverseExpr(clang::Expr *E) {
//     llvm::outs() << "Expression:" << E->getStmtClassName() << "\n";
//     return true;
// }
// bool ClangVisitor::TraverseCallExpr(clang::CallExpr *CE) {
//     llvm::outs() << "Call expression: " << CE->getCallee() <<"\n";
//     return true;
// }
// bool ClangVisitor::TraverseUnaryOperator(clang::UnaryOperator *UO) {
//     llvm::outs() << "Unary operator\n";
//     return true;
// }

// literaly
 bool ClangVisitor::TraverseIntegerLiteral(clang::IntegerLiteral *IL) {
    llvm::outs() << "Integer literal: " << IL->getValue().getSExtValue() << "\n";

    // akcia na tomto vrchole
    auto int_literal = this->expr_factory_->mk_int_literal(IL->getValue().getSExtValue());

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->expr_ = int_literal;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->int_lit_ = IL;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseIntegerLiteral(IL); // ja viem ze to je asi zbytocne ale pre istotu XD

    // vymazanie AST location netreba 
    return true;
 }
 bool ClangVisitor::TraverseFloatingLiteral(clang::FloatingLiteral *FL) {
    llvm::outs() << "Floating literal: " << FL->getValue().convertToFloat() << "\n";
    
    auto float_literal = this->expr_factory_->mk_float_literal(FL->getValue().convertToFloat());

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->expr_ = float_literal;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->float_lit_ = FL;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseFloatingLiteral(FL);
    // vymazanie AST location netreba
    return true;
 }
 bool ClangVisitor::TraverseStringLiteral(clang::StringLiteral *SL) {
    llvm::outs() << "String literal: " << SL->getString() << "\n";

    auto string_literal = this->expr_factory_->mk_string_literal(SL->getString().str());

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->expr_ = string_literal;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->string_lit_ = SL;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseStringLiteral(SL);
    // vymazanie AST location netreba
    return true;
 }
 bool ClangVisitor::TraverseCXXBoolLiteralExpr(clang::CXXBoolLiteralExpr *BL) {
    llvm::outs() << "Bool literal: " << BL->getValue() << "\n";

    auto bool_literal = this->expr_factory_->mk_bool_literal(BL->getValue());

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->expr_ = bool_literal;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->bool_lit_ = BL;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseCXXBoolLiteralExpr(BL);
    // vymazanie AST location netreba
    return true;
 }
 // operatory
 bool ClangVisitor::TraverseBinaryOperator(clang::BinaryOperator *BO) {
    BinOpExpr* bin_op = nullptr;
    llvm::outs() << "Binary operator: " << BO->getOpcodeStr() << "\n";
    // =
    //  if (BO->getOpcodeStr().equals("=")) { //TODO
    //      llvm::outs() << "Priradenie: " << BO->getLHS()->getStmtClassName()
    //                   << " = " << BO->getRHS()->getStmtClassName() << "\n";
    //      auto assign_op = this->expr_factory_->mk_assign(
    //                 nullptr,
    //         nullptr
    //     );
    // } else 
    if (BO->getOpcodeStr().equals("+")) {
        // operator +
        bin_op = this->expr_factory_->mk_bin_on(
            nullptr,
            BinOpType::Add,
            nullptr
        );
    } else if (BO->getOpcodeStr().equals("-")) {
        // operator -
        bin_op = this->expr_factory_->mk_bin_on(
            nullptr,
            BinOpType::Subtract,
            nullptr
        );
    }
    // ak je nad return statement
    if (this->clang_locationStack.back()->stmt_ && llvm::dyn_cast<clang::ReturnStmt>(this->clang_locationStack.back()->stmt_)) {
        // vytvorenie return statementu
        ((ReturnStmt*)this->astfri_locationStack.back()->stmt_)->val_ = bin_op;    
    }
    // ak je nad vardecl
    if (this->clang_locationStack.back()->decl_ && llvm::dyn_cast<clang::VarDecl>(this->clang_locationStack.back()->decl_)) {
        // vytvorenie inicializacie
        ((VarDefStmt*)this->astfri_locationStack.back()->stmt_)->initializer_ = bin_op;
    }

    // vytvorenie AST location
    AstfriASTLocation* astfri_location = new AstfriASTLocation();
    astfri_location->expr_ = bin_op;
    this->astfri_locationStack.push_back(astfri_location);
    ClangASTLocation* clang_location = new ClangASTLocation();
    clang_location->bin_op_ = BO;
    this->clang_locationStack.push_back(clang_location);

    Base::TraverseBinaryOperator(BO);

    // akcia na zobratie referencii v zasobniku
    bin_op->right_ = this->astfri_locationStack.back()->expr_;
    // vymazanie predoslich dvoch AST location
    delete this->astfri_locationStack.back();
    this->astfri_locationStack.pop_back();
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();
    
    bin_op->left_ = this->astfri_locationStack.back()->expr_;
    // vymazanie predoslich dvoch AST location
    delete this->clang_locationStack.back();
    this->clang_locationStack.pop_back();
    delete this->astfri_locationStack.back();
    this->astfri_locationStack.pop_back();

    // vymazanie AST location
    // delete this->astfri_locationStack.back(); 
    // this->astfri_locationStack.pop_back();
    // delete this->clang_locationStack.back();
    // this->clang_locationStack.pop_back();
    return true;
}
} // namespace libastfri::cpp