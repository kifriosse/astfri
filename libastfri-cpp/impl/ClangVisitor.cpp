#include <libastfri-cpp/inc/ClangVisitor.hpp>
#include "libastfri/inc/Expr.hpp"
#include "libastfri/inc/Stmt.hpp"
#include "libastfri/inc/Type.hpp"
#include <clang/AST/APValue.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/OperationKinds.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/Specifiers.h>
#include <cstddef>
#include <exception>
#include <iostream>
#include <llvm/Support/Casting.h>
#include <llvm/Support/raw_ostream.h>
#include <vector>

namespace astfri::cpp
{
ClangVisitor::ClangVisitor(TranslationUnit& visitedTranslationUnit) :
    tu_(&visitedTranslationUnit)
{
    this->stmt_factory_ = &StmtFactory::get_instance();
    this->expr_factory_ = &ExprFactory::get_instance();
    this->type_factory_ = &TypeFactory::get_instance();
    this->expr_as_stmt = false;
}

astfri::AccessModifier ClangVisitor::getAccessModifier(clang::Decl* decl) {
    switch (decl->getAccess()) {
        case clang::AccessSpecifier::AS_public:
            return astfri::AccessModifier::Public;
            break;
        case clang::AccessSpecifier::AS_protected:
            return astfri::AccessModifier::Protected;
            break;
        case clang::AccessSpecifier::AS_private:
            return astfri::AccessModifier::Private;
            break;
        default:
            return astfri::AccessModifier::Public;
            break;
    }
}

// traverse deklaracie
bool ClangVisitor::TraverseCXXConstructorDecl(clang::CXXConstructorDecl *Ctor) {
    llvm::outs() << "Konstruktor zaciatok: " << Ctor->getNameAsString() << "\n";

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // akcia na tomto vrchole
    //MethodDefStmt* new_ctor = nullptr;
    if (Ctor->hasBody()) {
        // vytvorenie konstruktora
        auto new_ctor = this->stmt_factory_->mk_constructor_def(
            (ClassDefStmt*)this->astfri_location.stmt_,
            std::vector<ParamVarDefStmt*>{},
            std::vector<BaseInitializerStmt*>{},
            nullptr,
            this->getAccessModifier(Ctor)
        );
        // TODO: ked pride podpora treba odkomentovat
        // ((ClassDefStmt*)this->astfri_location.stmt_)->methods_.push_back(new_ctor);
        
        TraverseStmt(Ctor->getBody());
        new_ctor->body_ = (CompoundStmt*)this->astfri_location.stmt_;
        for (auto parm : Ctor->parameters()) {
            TraverseDecl(parm);
            new_ctor->params_.push_back((ParamVarDefStmt*)this->astfri_location.stmt_);
        }
    }

    // vratenie naspat AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    llvm::outs() << "Konstruktor koniec: " << Ctor->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::TraverseCXXDestructorDecl(clang::CXXDestructorDecl *Dtor) {
    llvm::outs() << "Destruktor\n";

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // akcia na tomto vrchole
    auto new_dtor = this->stmt_factory_->mk_destructor_def(
        (ClassDefStmt*)this->astfri_location.stmt_,
        nullptr
    );
    // TODO: ked bude podpora pre destruktory,
    // ((ClassDefStmt*)this->astfri_location.stmt_)->methods_.push_back(new_dtor);
    TraverseStmt(Dtor->getBody());
    new_dtor->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    llvm::outs() << "Destruktor koniec\n";
    return true;
}
bool ClangVisitor::TraverseFunctionDecl(clang::FunctionDecl *FD) {
    llvm::outs() << "Function zaciatok: " << FD->getNameAsString() << "\n";

    // akcia na tomto vrchole
    auto new_function = this->stmt_factory_->mk_function_def(
        FD->getNameAsString(),
        std::vector<ParamVarDefStmt*> {},
        this->type_factory_->mk_user(FD->getReturnType().getAsString()), //TODO: fixnut typ
        nullptr
    );
    this->tu_->functions_.push_back(new_function);

    // zapamatanie si AST Location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_function;
    this->clang_location.decl_ = FD;

    // parametre
    for (auto parm : FD->parameters()) {
        TraverseDecl(parm);
        new_function->params_.push_back((ParamVarDefStmt*)this->astfri_location.stmt_);
    }

    // prejdenie tela funkcie
    auto body = FD->getBody();
    TraverseStmt(body);

    //priradenie comopund statementu funkcii
    new_function->body_ = (CompoundStmt*)this->astfri_location.stmt_;
    
    // vratenie naspat location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    llvm::outs() << "Function koniec: " << FD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::TraverseCXXMethodDecl(clang::CXXMethodDecl *MD) {
    llvm::outs() << "Metoda zaciatok: " << MD->getNameAsString() << "\n";

    // akcia na tomto vrchole
    auto new_method = this->stmt_factory_->mk_method_def(
        (ClassDefStmt*)this->astfri_location.stmt_,
        this->stmt_factory_->mk_function_def(
            MD->getNameAsString(),
            std::vector<ParamVarDefStmt*> {},
            this->type_factory_->mk_user(MD->getReturnType().getAsString()), //TODO: fixnut typ
            nullptr),
            this->getAccessModifier(MD)
    );
    ((ClassDefStmt*)this->astfri_location.stmt_)->methods_.push_back(new_method);

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_method;
    this->clang_location.decl_ = MD;

    // parametre
    for (auto parm : MD->parameters()) {
        TraverseDecl(parm);
        new_method->func_->params_.push_back((ParamVarDefStmt*)this->astfri_location.stmt_);
    }

    auto body = MD->getBody();
    TraverseStmt(body);

    new_method->func_->body_ = (CompoundStmt*)this->astfri_location.stmt_;
    
    // vratenie naspat AST Location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

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

    // zapamatanie si predoslich location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_class;
    this->clang_location.decl_ = RD;

    for(auto field : RD->fields()) {
        TraverseDecl(field);
    }

    for(auto method : RD->methods()) {
        TraverseDecl(method);
    }

    // vratenie naspat na predosly location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;
    
    llvm::outs() << "Class koniec: " << RD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::TraverseVarDecl(clang::VarDecl *VD) {
    llvm::outs() << "Variabilna zaciatok: " << VD->getNameAsString() << "\n";

    // akcia na tomto vrchole
    VarDefStmt* new_var = nullptr;
    if (this->astfri_location.stmt_) {
        // premenna v compounde premenna
        new_var = this->stmt_factory_->mk_local_var_def(
            VD->getNameAsString(),
            this->type_factory_->mk_user(VD->getType().getAsString()), //TODO: fixnut
            nullptr
        );
        ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_var);
    } else {
        // globalna premenna
        new_var = this->stmt_factory_->mk_global_var_def(
            VD->getNameAsString(),
            this->type_factory_->mk_user(VD->getType().getAsString()), //TODO: fixnut
            nullptr
        );
        this->tu_->globals_.push_back((GlobalVarDefStmt*)new_var);
    }

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_var;
    this->clang_location.decl_ = VD;

    if (auto init = VD->getInit()) {
        TraverseStmt(init);
        new_var->initializer_ = this->astfri_location.expr_;
    }

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    llvm::outs() << "Variabilna koniec: " << VD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::TraverseParmVarDecl(clang::ParmVarDecl *PVD) {
    llvm::outs() << "Parameter zaciatok: " << PVD->getNameAsString() << "\n";

    // akcia na tomto vrchole
    ParamVarDefStmt* new_par = this->stmt_factory_->mk_param_var_def(
        PVD->getNameAsString(),
        this->type_factory_->mk_user(PVD->getType().getAsString()), //TODO: fixnut typ
        nullptr
    );

    // ak ma inicializator
    if(auto init = PVD->getInit()) {
        TraverseStmt(init);
        new_par->initializer_ = this->astfri_location.expr_;
    }

    this->astfri_location.stmt_ = new_par;
    this->clang_location.decl_ = PVD;

    llvm::outs() << "Parameter koniec: " << PVD->getNameAsString() << "\n";
    return true;
}
bool ClangVisitor::TraverseFieldDecl(clang::FieldDecl *FD) {
    // akcia na tomto node
    llvm::outs() << "Field zaciatok: " << FD->getNameAsString() << "\n";

    // vytvorenie premennej triedy
    astfri::AccessModifier access = this->getAccessModifier(FD);
    auto new_member = this->stmt_factory_->mk_member_var_def(
        FD->getNameAsString(),
        this->type_factory_->mk_user(FD->getType().getAsString()), //TODO: fixnut typ
        nullptr,
        access
    );
    ((ClassDefStmt*)this->astfri_location.stmt_)->vars_.push_back(new_member);

    // zapamatanie si predoslich location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_member;
    this->clang_location.decl_ = FD;

    // ak ma inicializator
    if (auto init = FD->getInClassInitializer()) {
        TraverseStmt(init);
        new_member->initializer_ = this->astfri_location.expr_;
    }

    // vratenie naspat na predosly location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    llvm::outs() << "Field koniec: " << FD->getNameAsString() << "\n";
    
    return true;
}
// bool ClangVisitor::TraverseTypedefDecl(clang::TypedefDecl *TD) {
//     llvm::outs() << "Typedef:" << TD->getNameAsString() << "\n";
//     return true;
// }
// bool ClangVisitor::TraverseEnumDecl(clang::EnumDecl *ED) {
//     llvm::outs() << "Enum:" << ED->getNameAsString() << "\n";
//     return true;
// }
// // traverse statementy

bool ClangVisitor::TraverseCompoundStmt(clang::CompoundStmt *CS) {
    llvm::outs() << "Compound Statement zaciatok:" << "\n";

    // akcia na tomto vrchole
    CompoundStmt* new_compound = this->stmt_factory_->mk_compound(std::vector<Stmt *>{});

    // prepisanie AST location
    this->astfri_location.stmt_ = new_compound;
    this->clang_location.stmt_ = CS;

    for (auto stmt : CS->body()) {
        // ak je binary operator, tak sa zoberie ako stmt
        if (llvm::dyn_cast<clang::BinaryOperator>(stmt) ||
        llvm::dyn_cast<clang::UnaryOperator>(stmt) ) {
            this->expr_as_stmt = true;
            TraverseStmt(stmt);
            this->expr_as_stmt = false;
        } else {
            TraverseStmt(stmt);
        }
    }

    llvm::outs() << "Compound Statement koniec:" << "\n";
    return true;
}
bool ClangVisitor::TraverseReturnStmt(clang::ReturnStmt *RS) {
    llvm::outs() << "Return Statement (value): " << RS->getRetValue() << "\n";

    // akcia na tomto vrchole
    ReturnStmt* new_return = this->stmt_factory_->mk_return(
        nullptr //TODO: fixnut
    );
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_return);

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie location
    this->astfri_location.stmt_ = new_return;
    this->clang_location.stmt_ = RS;

    TraverseStmt(RS->getRetValue());
    // naplnenie return stmt
    new_return->val_ = this->astfri_location.expr_;

    // vratenie ast location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    llvm::outs() << "Return statement koniec: " << "\n";
    return true;
}
bool ClangVisitor::TraverseIfStmt(clang::IfStmt *IS) {
    std::cout << "If statement zaciatok: " << "\n";

    // akcia na tomto vrchole
    auto new_if = this->stmt_factory_->mk_if(
        nullptr,
        nullptr,
        nullptr
    );
    // priradenie stmt tam kde patri
    // if (this->clang_locationStack.back()->stmt_ && 
    // llvm::dyn_cast<clang::CompoundStmt>(this->clang_locationStack.back()->stmt_)) {
    //     // ak je v compounde
    //     ((CompoundStmt*)this->astfri_locationStack.back()->stmt_)->stmts_.push_back(new_if);
    // } else if (this->clang_locationStack[this->clang_locationStack.size() - 3]->stmt_ && 
    // llvm::dyn_cast<clang::IfStmt>(this->clang_locationStack[this->clang_locationStack.size() - 3]->stmt_)) {
    //     // ak je v dalsom if-e
    //     ((IfStmt*)this->astfri_locationStack[this->astfri_locationStack.size() - 3]->stmt_)->iffalse_ = new_if;
    // }

    // TODO: prepoklada sa ze je v compounde a nie v else vetve
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_if);

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST Location
    this->astfri_location.stmt_ = new_if;
    this->clang_location.stmt_ = IS;
    
    // priradenie podmienky
    auto cond = IS->getCond();
    TraverseStmt(cond);
    new_if->cond_ = this->astfri_location.expr_;
    
    // priradenie true vetvy
    auto true_stmt = IS->getThen();
    TraverseStmt(true_stmt);
    new_if->iftrue_ = this->astfri_location.stmt_;

    // else vetva
    if (auto else_stmt = IS->getElse()) {
        // TODO: ak ma compound ako else dorobit, zatial sa prepoklada ze je to compound vzdy
        TraverseStmt(else_stmt);
        new_if->iffalse_ = this->astfri_location.stmt_;
    }

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    std::cout << "If statement koniec" << "\n";
    return true;
}
bool ClangVisitor::TraverseForStmt(clang::ForStmt *FS) {
    llvm::outs() << "For statement zaciatok: " << FS->getConditionVariableDeclStmt() << "\n";

    // akcia na tomto vrchole
    auto new_for = this->stmt_factory_->mk_for(
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_for);

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_for;
    this->clang_location.stmt_ = FS;

    //naplnenie new_for
    auto cond = FS->getCond();
    TraverseStmt(cond);
    new_for->cond_ = this->astfri_location.expr_;

    auto body = FS->getBody();
    TraverseStmt(body);
    new_for->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vytvorim si compound stmt a do toho pojdu init statementy, potom ich hodim do vardef
    auto init_compound = this->stmt_factory_->mk_compound(
        std::vector<Stmt *> {}
    );
    this->astfri_location.stmt_ = init_compound;
    TraverseStmt(FS->getInit());
    new_for->init_ = ((CompoundStmt*)this->astfri_location.stmt_)->stmts_[0];

    // auto step = FS->getInc();
    // TraverseStmt(step);
    // new_for->step_ = this->stmt_factory_->mk_expr(
    //     this->astfri_location.expr_
    // );
    // TODO: dorobit aj exoticke srandy na forku
    new_for->step_ = this->stmt_factory_->mk_expr(
        this->expr_factory_->mk_unary_op(
            UnaryOpType::PostIncrement,
            this->expr_factory_->mk_local_var_ref("i")
        )
    );

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    llvm::outs() << "For statement koniec: " << "\n";
    return true;
}
bool ClangVisitor::TraverseWhileStmt(clang::WhileStmt *WS) {
    llvm::outs() << "While statement: " << WS->getCond() << "\n";

    // akcia na tomto vrchole
    auto new_while = this->stmt_factory_->mk_while(
        nullptr,
        nullptr
    );
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_while);

    // zapamatanie si AST Location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_while;
    this->clang_location.stmt_ = WS;

    // naplnenie new_while
    TraverseStmt(WS->getCond());
    new_while->cond_ = this->astfri_location.expr_;
    TraverseStmt(WS->getBody());
    new_while->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vratenie AST Location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    llvm::outs() << "While statement koniec" << WS->getCond() << "\n";
    return true;
}
bool ClangVisitor::TraverseDoStmt(clang::DoStmt *DS) {
    llvm::outs() << "Do While statement zaciatok" << DS->getCond() << "\n";

    // akcia na tomto vrchole
    auto new_do_while = this->stmt_factory_->mk_do_while(
        nullptr,
        nullptr
    );
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_do_while);

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_do_while;
    this->clang_location.stmt_ = DS;

    // naplnenie new_do_while
    TraverseStmt(DS->getCond());
    new_do_while->cond_ = this->astfri_location.expr_;
    TraverseStmt(DS->getBody());
    new_do_while->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    llvm::outs() << "Do While statement koniec" << DS->getCond() << "\n";
    return true;
}
bool ClangVisitor::TraverseSwitchStmt(clang::SwitchStmt *SS) {
    llvm::outs() << "Switch stmt zaciatok\n";

    // akcia na tomto vrchole
    auto new_switch = this->stmt_factory_->mk_switch(
        nullptr,
        std::vector<CaseStmt*> {}
    );
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_switch);

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_switch;
    this->clang_location.stmt_ = SS;

    // naplnenie new_switch
    // akcia na kazdom case
    for (auto sw_case = SS->getSwitchCaseList(); sw_case; sw_case = sw_case->getNextSwitchCase()) {
        llvm::outs() << "Sw case: " << sw_case->getStmtClassName() << "\n";
        // ak je to case stmt a nie default
        if (auto case_stmt = llvm::dyn_cast<clang::CaseStmt>(sw_case)) {
            auto new_case = this->stmt_factory_->mk_case(
                nullptr,
                nullptr
            );
            // naplnenie case-u
            auto case_cond = case_stmt->getLHS();
            TraverseStmt(case_cond);
            new_case->expr_ = this->astfri_location.expr_;

            auto case_body = case_stmt->getSubStmt();
            // ak je v compounde
            if (auto CS_body = llvm::dyn_cast<clang::CompoundStmt>(case_body)) {
                TraverseStmt(CS_body);
                new_case->body_ = this->astfri_location.stmt_;
            } else {
                // nieje compound, iba jeden prikaz (dam do compoundu a potom vyberiem)
                auto temp_compund = this->stmt_factory_->mk_compound(std::vector<Stmt *> {});
                this->astfri_location.stmt_ = temp_compund;
                TraverseStmt(case_body);
                if (!((CompoundStmt*)this->astfri_location.stmt_)->stmts_.empty())
                    new_case->body_ = temp_compund->stmts_[0];
            }
            new_switch->cases_.push_back(new_case);
        } else if (auto default_stmt = llvm::dyn_cast<clang::DefaultStmt>(sw_case)){
            // ak je to default
            auto new_default = this->stmt_factory_->mk_case(
                nullptr,
                nullptr
            );
            // naplnenie default-u
            auto default_body = default_stmt->getSubStmt();
            // ak je v compounde
            if (auto CS_body = llvm::dyn_cast<clang::CompoundStmt>(default_body)) {
                TraverseStmt(CS_body);
                new_default->body_ = this->astfri_location.stmt_;
            } else {
                // TODO: potom dorobit ked bude default case
                auto len_zatial = this->expr_factory_->mk_string_literal("Default");
                new_default->expr_ = len_zatial;
                // nieje compound, iba jeden prikaz (dam do compoundu a potom vyberiem)
                auto temp_compund = this->stmt_factory_->mk_compound(std::vector<Stmt *> {});
                this->astfri_location.stmt_ = temp_compund;
                TraverseStmt(default_body);
                if (!((CompoundStmt*)this->astfri_location.stmt_)->stmts_.empty())
                    new_default->body_ = temp_compund->stmts_[0];
            }
            new_switch->cases_.push_back(new_default);
        }
    }

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    llvm::outs() << "Switch stmt koniec\n";
    return true;
}
// visit expression
bool ClangVisitor::TraverseCXXConstructExpr(clang::CXXConstructExpr *Ctor) {
    llvm::outs() << "CXXConstructExpr: " << Ctor->getConstructor()->getNameAsString() << "\n";

    // akcia na tomto vrchole
    auto new_ctor_expr = this->expr_factory_->mk_constructor_call(
        nullptr,
        std::vector<Expr *> {}
    );

    auto type = this->type_factory_->mk_user(
        Ctor->getType().getAsString().c_str()
    );
    new_ctor_expr->type_ = type;

    for (auto arg : Ctor->arguments()) {
        TraverseStmt(arg);
        new_ctor_expr->args_.push_back(this->astfri_location.expr_);
    }

    this->astfri_location.expr_ = new_ctor_expr;
    this->clang_location.expr_ = Ctor;

    llvm::outs() << "CXXConstructExpr koniec\n";
    return true;
}
bool ClangVisitor::TraverseDeclRefExpr(clang::DeclRefExpr *DRE) {
    llvm::outs() << "DeclRefExpr: " << DRE->getDecl()->getNameAsString() << "\n";

    // akcia na tomto vrchole
    Expr* new_decl_ref_expr = nullptr;
    if (auto PVD = llvm::dyn_cast<clang::ParmVarDecl>(DRE->getDecl())) {
        // pre parameter
        new_decl_ref_expr = this->expr_factory_->mk_param_var_ref(
            PVD->getNameAsString()
        );
    } else if (auto VD = llvm::dyn_cast<clang::VarDecl>(DRE->getDecl())) {
        // pre lokalnu premennu
        new_decl_ref_expr = this->expr_factory_->mk_local_var_ref(
            VD->getNameAsString()
        );
    }

    this->astfri_location.expr_ = new_decl_ref_expr;
    this->clang_location.expr_ = DRE;
    
    return true;
}
bool ClangVisitor::TraverseMemberExpr(clang::MemberExpr* ME) {
    llvm::outs() << "MemberExpr zaciatok\n";
    
    // akcia na tomto vrchole
    auto new_mem_expr = this->expr_factory_->mk_member_var_ref(
        ME->getMemberNameInfo().getAsString().c_str()
    );
    this->astfri_location.expr_ = new_mem_expr;
    this->clang_location.expr_ = ME;

    llvm::outs() << "MemberExpr koniec\n";
    return true;
}
bool ClangVisitor::TraverseCXXThisExpr(clang::CXXThisExpr *TE) {
    llvm::outs() << "This expr zaciatok\n";

    this->astfri_location.expr_ = this->expr_factory_->mk_this();
    this->clang_location.expr_ = TE;
    
    llvm::outs() << "This expr koniec\n";
    return true;
}
bool ClangVisitor::TraverseCXXMemberCallExpr(clang::CXXMemberCallExpr* MCE) {
    llvm::outs() << "CXXMemberCallExpr zaciatok\n";

    // akcia na tomto vrchole
    std::vector<Expr*> args;
    for (auto arg : MCE->arguments()) {
        TraverseStmt(arg);
        args.push_back(this->astfri_location.expr_);
    }
    auto owner = this->expr_factory_->mk_class_ref(MCE->getRecordDecl()->getNameAsString().c_str());
    auto new_mem_call = this->expr_factory_->mk_method_call(
        owner,
        MCE->getMethodDecl()->getNameAsString().c_str(),
        args
    );

    this->astfri_location.expr_ = new_mem_call;
    this->clang_location.expr_ = MCE;

    llvm::outs() << "CXXMemberCallExpr koniec\n";
    return true;
}
// bool ClangVisitor::TraverseCallExpr(clang::CallExpr *CE) {
//     llvm::outs() << "Call expression: " << CE->getCallee() <<"\n";
//     return true;
// }

// literaly
 bool ClangVisitor::TraverseIntegerLiteral(clang::IntegerLiteral *IL) {
    llvm::outs() << "Integer literal: " << IL->getValue().getSExtValue() << "\n";

    // akcia na tomto vrchole
    auto int_literal = this->expr_factory_->mk_int_literal(IL->getValue().getSExtValue());

    // vytvorenie AST location
    this->astfri_location.expr_ = int_literal;
    this->clang_location.int_lit_ = IL;
    
    return true;
 }
 bool ClangVisitor::TraverseFloatingLiteral(clang::FloatingLiteral *FL) {
    llvm::outs() << "Floating literal: " << FL->getValue().convertToFloat() << "\n";
    
    // akcia na tomto vrchole
    auto float_literal = this->expr_factory_->mk_float_literal(FL->getValue().convertToFloat());

    // vytvorenie AST location
    this->astfri_location.expr_ = float_literal;
    this->clang_location.float_lit_ = FL;

    return true;
 }
 bool ClangVisitor::TraverseStringLiteral(clang::StringLiteral *SL) {
    llvm::outs() << "String literal: " << SL->getString() << "\n";
    
    // akcia na tomto vrchole
    auto string_literal = this->expr_factory_->mk_string_literal(SL->getString().str());

    // vytvorenie AST location
    this->astfri_location.expr_ = string_literal;
    this->clang_location.string_lit_ = SL;
    
    return true;
 }
 bool ClangVisitor::TraverseCXXBoolLiteralExpr(clang::CXXBoolLiteralExpr *BL) {
    llvm::outs() << "Bool literal: " << BL->getValue() << "\n";

    // akcia na tomto vrchole
    auto bool_literal = this->expr_factory_->mk_bool_literal(BL->getValue());

    // vytvorenie AST location
    this->astfri_location.expr_ = bool_literal;
    this->clang_location.bool_lit_ = BL;

    return true;
 }
 bool ClangVisitor::TraverseCharacterLiteralExpr(clang::CharacterLiteral *CL) {
    llvm::outs() << "Character Literal: " << (char)CL->getValue() << "\n";

    auto new_char = this->expr_factory_->mk_char_literal(CL->getValue());
    this->astfri_location.expr_ = new_char;
    this->clang_location.char_lit_ = CL;

    return true;
 }
 // operatory
 bool ClangVisitor::TraverseBinaryOperator(clang::BinaryOperator *BO) {
    llvm::outs() << "Binary operator: " << BO->getOpcodeStr() << "\n";
    
    BinOpExpr* bin_op = nullptr;

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
        
    } else if (BO->getOpcodeStr().equals("*")) {
        // operator *
         bin_op = this->expr_factory_->mk_bin_on(
            nullptr,
            BinOpType::Multiply,
            nullptr
        );
    } else if (BO->getOpcodeStr().equals("/")) {
        // operator /
         bin_op = this->expr_factory_->mk_bin_on(
            nullptr,
            BinOpType::Divide,
            nullptr
        );
    } else if (BO->getOpcodeStr().equals(">")) {
        // operator >
         bin_op = this->expr_factory_->mk_bin_on(
            nullptr,
            BinOpType::Greater,
            nullptr
        );
    } else if (BO->getOpcodeStr().equals("<")) {
        // operator <
         bin_op = this->expr_factory_->mk_bin_on(
            nullptr,
            BinOpType::Less,
            nullptr
        );
    } else if (BO->getOpcodeStr().equals("=")) {
        // operator <
         bin_op = this->expr_factory_->mk_bin_on(
            nullptr,
            BinOpType::Assign,
            nullptr
        );
    } else if (BO->getOpcodeStr().equals("==")) {
        // operator <
         bin_op = this->expr_factory_->mk_bin_on(
            nullptr,
            BinOpType::Equal,
            nullptr
        );
    }
    // pridanie na svoje miesto
    // ak je nad return statement
    // if (this->clang_locationStack.back()->stmt_ && llvm::dyn_cast<clang::ReturnStmt>(this->clang_locationStack.back()->stmt_)) {
    //     ((ReturnStmt*)this->astfri_locationStack.back()->stmt_)->val_ = bin_op;    
    // }
    // // ak je nad vardecl
    // if (this->clang_locationStack.back()->decl_ && llvm::dyn_cast<clang::VarDecl>(this->clang_locationStack.back()->decl_)) {
    //     ((VarDefStmt*)this->astfri_locationStack.back()->stmt_)->initializer_ = bin_op;
    // }
    // // ak je nad ifstmt
    // if (this->clang_locationStack.back()->stmt_ && llvm::dyn_cast<clang::IfStmt>(this->clang_locationStack.back()->stmt_)) {
    //     ((IfStmt*)this->astfri_locationStack.back()->stmt_)->cond_ = bin_op;
    // }
    // // ak je v compounde
    // if (this->clang_locationStack.back()->stmt_ && llvm::dyn_cast<clang::CompoundStmt>(this->clang_locationStack.back()->stmt_)) {
    //     ((CompoundStmt*)this->astfri_locationStack.back()->stmt_)->stmts_.push_back(
    //         this->stmt_factory_->mk_expr(bin_op)
    //     );
    //     isStmt = true;
    // }
    
    // prepisanie AST location
    this->astfri_location.expr_ = bin_op;
    this->clang_location.bin_op_ = BO;
    
    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // ak je expr_as_stmt flag true, nech sa pre ostatne vypne
    bool expr_as_stmt_changed = false;
    if (this->expr_as_stmt) {
        this->expr_as_stmt = false;
        expr_as_stmt_changed = true;
    }

    // lavy operator
    TraverseStmt(BO->getLHS());
    bin_op->left_ = this->astfri_location.expr_;
    // pravy operator
    TraverseStmt(BO->getRHS());
    bin_op->right_ = this->astfri_location.expr_;

    // vratenie expr_as_stmt naspat ak je toto node ktory ho zmenil
    if (expr_as_stmt_changed) {
        this->expr_as_stmt = true;
    }

    // vratenie tohto bin op naspat
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    if (this->expr_as_stmt) {
        ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(
            this->stmt_factory_->mk_expr(bin_op)
        );
    }

    return true;
}
bool ClangVisitor::TraverseUnaryOperator(clang::UnaryOperator *UO) {
    llvm::outs() << "Unary operator " << UO->getOpcode() << "\n";

    // dolezite operandy
    // UO_Plus     +
    // UO_Minus    -
    // UO_Not      !
    // UO_LNot     logicke !
    // UO_PreInc   ++x
    // UO_PostInc  x++
    // UO_PreDec   --x
    // UO_PostDec  x--
    // UO_AddrOf   &x
    // UO_Deref    *x

    // akcia na tomto vrchole
    // nastavenie operatora
    UnaryOpType op;
    switch (UO->getOpcode()) {
        case clang::UO_Plus: op = UnaryOpType::Plus;
            break;
        case clang::UO_Minus: op = UnaryOpType::Minus;
            break;
        case clang::UO_Not: op = UnaryOpType::LogicalNot;
            break;
        case clang::UO_LNot:op = UnaryOpType::LogicalNot; // TODO: spytat sa na rozdiel
            break; 
        case clang::UO_PreInc: op = UnaryOpType::PreIncrement;
            break;
        case clang::UO_PostInc: op = UnaryOpType::PostIncrement;
            break;
        case clang::UO_PreDec: op = UnaryOpType::PreDecrement;
            break;
        case clang::UO_PostDec: op = UnaryOpType::PostDecrement;
            break;
        case clang::UO_AddrOf: op = UnaryOpType::AddressOf;
            break;
        case clang::UO_Deref: op = UnaryOpType::Dereference;
            break;
        default: {
            std::cout << "Chyba pri nastavovani unary operatora!, koncim\n";
            return 1;
        }
            break;
    }

    Expr* arg = nullptr;
    // TODO: treba dorobit normalne
    // nastavenie operandu
    arg = this->expr_factory_->mk_local_var_ref(
        llvm::dyn_cast<clang::DeclRefExpr>(UO->getSubExpr())->getDecl()->getNameAsString()
    );

    // vytvorenie unary operator vrchol
    UnaryOpExpr* un_op = this->expr_factory_->mk_unary_op(
        op,
        arg
    );

    // ak je expression nad compound stmt, tak sa vytvori ako stmt a nie expr
    if (llvm::dyn_cast<clang::CompoundStmt>(this->clang_location.stmt_)) {
        ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(
            this->stmt_factory_->mk_expr(un_op)
        );
    }

    this->astfri_location.expr_ = un_op;
    this->clang_location.un_op_ = UO;

    llvm::outs() << "Unary operator koniec\n";
    return true;
}
} // namespace libastfri::cpp