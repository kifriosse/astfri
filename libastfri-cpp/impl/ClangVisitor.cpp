#include <libastfri-cpp/inc/ClangVisitor.hpp>

#include <iostream>

namespace astfri::astfri_cpp
{
bool ClangVisitor::isInMainFile(clang::SourceLocation Loc) const {
    if (Loc.isInvalid()) return false;
    return this->SM->getFileID(this->SM->getExpansionLoc(Loc)) == this->MainFileID;
}
clang::NamespaceDecl* ClangVisitor::get_desired_namespace(clang::TranslationUnitDecl* TU) {
    for (auto decl : TU->decls()) {
        if (auto NSD = llvm::dyn_cast<clang::NamespaceDecl>(decl)) {
            if (this->isInMainFile(NSD->getLocation())) {
                return NSD;
            }
        }
    }
    return nullptr;
}
void ClangVisitor::setSM(clang::SourceManager* pSM) {
    this->SM = pSM;
}
clang::SourceManager* ClangVisitor::getSM() {
    return this->SM;
}
void ClangVisitor::setMainFileID(clang::FileID MFID) {
    this->MainFileID = MFID;
}
clang::FileID ClangVisitor::getMFID() {
    return this->MainFileID;
}

ClangVisitor::ClangVisitor(TranslationUnit& visitedTranslationUnit, clang::SourceManager* pSM) :
    tu_(&visitedTranslationUnit)
{
    this->SM = pSM;
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

astfri::ClassDefStmt* ClangVisitor::get_existing_class(std::string name) {
    for (auto cls : this->tu_->classes_) {
        if (cls->name_.compare(name) == 0) {
            return cls;
        }
    }
    return nullptr;
}

astfri::BinOpType ClangVisitor::get_astfri_bin_op_type(clang::BinaryOperatorKind clang_type) {
    // TODO: 
    // ඞ Fakt toto existuje?
    // {lhs} // {rhs}, {lhs} div {rhs}
    //FloorDivide,

    // {lhs} ** {rhs}
    // Exponentiation,

    // {lhs} //= {rhs}
    // FloorDivideAssign,

    // {lhs} **= {rhs}
    // ExponentiationAssign,
    
    // sú tam aj ďalšie srandy, ale to nepodporuje ASTFRI: .* ->* 

    switch (clang_type) {
        case clang::BinaryOperatorKind::BO_Assign: return astfri::BinOpType::Assign; break; // =
        case clang::BinaryOperatorKind::BO_Add: return astfri::BinOpType::Add; break; // +
        case clang::BinaryOperatorKind::BO_Sub: return astfri::BinOpType::Subtract; break; // -
        case clang::BinaryOperatorKind::BO_Mul: return astfri::BinOpType::Multiply; break; // *
        case clang::BinaryOperatorKind::BO_Div: return astfri::BinOpType::Divide; break; // /
        case clang::BinaryOperatorKind::BO_AddAssign: return astfri::BinOpType::AddAssign; break; // +=
        case clang::BinaryOperatorKind::BO_SubAssign: return astfri::BinOpType::SubtractAssign; break; // -=
        case clang::BinaryOperatorKind::BO_MulAssign: return astfri::BinOpType::MultiplyAssign; break; // *=
        case clang::BinaryOperatorKind::BO_DivAssign: return astfri::BinOpType::DivideAssign; break; // /=
        case clang::BinaryOperatorKind::BO_GT: return astfri::BinOpType::Greater; break; // >
        case clang::BinaryOperatorKind::BO_GE: return astfri::BinOpType::GreaterEqual; break; // >=
        case clang::BinaryOperatorKind::BO_LT: return astfri::BinOpType::Less; break; // <
        case clang::BinaryOperatorKind::BO_LE: return astfri::BinOpType::LessEqual; break; // <=
        case clang::BinaryOperatorKind::BO_EQ: return astfri::BinOpType::Equal; break; // ==
        case clang::BinaryOperatorKind::BO_NE: return astfri::BinOpType::NotEqual; break; // !=
        case clang::BinaryOperatorKind::BO_LAnd: return astfri::BinOpType::LogicalAnd; break; // &&
        case clang::BinaryOperatorKind::BO_LOr: return astfri::BinOpType::LogicalOr; break; // ||
        case clang::BinaryOperatorKind::BO_Rem: return astfri::BinOpType::Modulo; break; // %
        case clang::BinaryOperatorKind::BO_RemAssign: return astfri::BinOpType::ModuloAssign; break; // %=
        case clang::BinaryOperatorKind::BO_Shr: return astfri::BinOpType::BitShiftRight; break; // >>
        case clang::BinaryOperatorKind::BO_Shl: return astfri::BinOpType::BitShiftLeft; break; // <<
        case clang::BinaryOperatorKind::BO_ShrAssign: return astfri::BinOpType::BitShiftRightAssign; break; // >>=
        case clang::BinaryOperatorKind::BO_ShlAssign: return astfri::BinOpType::BitShiftLeftAssign; break; // <<=   
        case clang::BinaryOperatorKind::BO_And: return astfri::BinOpType::BitAnd; break; // &
        case clang::BinaryOperatorKind::BO_AndAssign: return astfri::BinOpType::BitAndAssign; break; // &=
        case clang::BinaryOperatorKind::BO_Or: return astfri::BinOpType::BitOr; break; // |
        case clang::BinaryOperatorKind::BO_OrAssign: return astfri::BinOpType::BitOrAssign; break; // |=
        case clang::BinaryOperatorKind::BO_Xor: return astfri::BinOpType::BitXor; break; // ^
        case clang::BinaryOperatorKind::BO_XorAssign: return astfri::BinOpType::BitXorAssign; break; // ^=
        case clang::BinaryOperatorKind::BO_Comma: return astfri::BinOpType::Comma; break; // ,
        default: {
            std::cerr << "\n\n\nErrorin finding right Binary operator: astfri_cpp project.\n\n\n";
        } break;
    }
    return BinOpType::Assign;
}

astfri::UnaryOpType ClangVisitor::get_astfri_un_op_type(clang::UnaryOperatorKind clang_type) {
    switch (clang_type) {
        case clang::UnaryOperatorKind::UO_Plus: return UnaryOpType::Plus; break;
        case clang::UnaryOperatorKind::UO_Minus: return UnaryOpType::Minus; break;
        case clang::UnaryOperatorKind::UO_LNot:return UnaryOpType::LogicalNot; break; 
        case clang::UnaryOperatorKind::UO_PreInc: return UnaryOpType::PreIncrement; break;
        case clang::UnaryOperatorKind::UO_PostInc: return UnaryOpType::PostIncrement; break;
        case clang::UnaryOperatorKind::UO_PreDec: return UnaryOpType::PreDecrement; break;
        case clang::UnaryOperatorKind::UO_PostDec: return UnaryOpType::PostDecrement; break;
        case clang::UnaryOperatorKind::UO_AddrOf: return UnaryOpType::AddressOf; break;
        case clang::UnaryOperatorKind::UO_Deref: return UnaryOpType::Dereference; break;
        case clang::UnaryOperatorKind::UO_Not: return UnaryOpType::LogicalNot; break;
        default: {
            std::cerr << "\n\n\nError in finding right Unary operator: astfri_cpp project.\n\n\n";
        } break;
    }
    return UnaryOpType::Plus;
}

// traverse deklaracie
bool ClangVisitor::VisitNamespaceDecl(clang::NamespaceDecl *ND) {
    std::cout << "Traversing namespace: " << ND->getNameAsString() << "\n";
    return true; // Pokračujeme v prechádzaní poduzlov
}
bool ClangVisitor::TraverseCXXConstructorDecl(clang::CXXConstructorDecl *Ctor) {
    // aby sa viac krat nevytvaral
    if (!Ctor->isFirstDecl()) {
        return true;
    }

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;
    
    // akcia na tomto vrchole
    // ziskanie ownera
    auto owner = this->get_existing_class(
        Ctor->getParent()->getNameAsString()
    );
    if (Ctor->hasBody()) {
        // vytvorenie konstruktora
        auto new_ctor = this->stmt_factory_->mk_constructor_def(
            owner,
            std::vector<ParamVarDefStmt*>{},
            std::vector<BaseInitializerStmt*>{},
            nullptr,
            this->getAccessModifier(Ctor)
        );
        owner->constructors_.push_back(new_ctor);

        TraverseStmt(Ctor->getBody());
        new_ctor->body_ = (CompoundStmt*)this->astfri_location.stmt_;
        

        // prejdenie CtorInitializers a vlozenie na zaciatok
        // pouzica sa na to, aby sa novy = stmt vlozil na spravne miesto,
        // inak by boli opacne vkladane (posledny by bol prvy)
        int place_in_vector = 0;
        for (auto init : Ctor->inits()) {

            // ak je to base init
            if (init->isBaseInitializer()) {
                std::vector<Expr *> args {};
                auto ce = llvm::dyn_cast<clang::CXXConstructExpr>(init->getInit());
                for (auto arg : ce->arguments()) {
                    TraverseStmt(arg);
                    args.push_back(this->astfri_location.expr_);
                }
                auto base_init = this->stmt_factory_->mak_base_initializer(
                    init->getBaseClass()->getAsCXXRecordDecl()->getNameAsString(),
                    args
                );
                new_ctor->baseInit_.push_back(base_init);
                continue;
            }

            // ak je to inicializator fieldu
            auto left = this->expr_factory_->mk_member_var_ref(
                this->expr_factory_->mk_this(),
                init->getMember()->getNameAsString()
            );
            TraverseStmt(init->getInit());
            auto right = this->astfri_location.expr_;
            auto new_init = this->stmt_factory_->mk_expr(
                this->expr_factory_->mk_bin_on(
                    left,
                    BinOpType::Assign,
                    right
                )
            );
            new_ctor->body_->stmts_.emplace(new_ctor->body_->stmts_.begin()
            + place_in_vector,
            new_init);
            place_in_vector++;
        }

        for (auto parm : Ctor->parameters()) {
            TraverseDecl(parm);
            new_ctor->params_.push_back((ParamVarDefStmt*)this->astfri_location.stmt_);
        }
    }

    // vratenie naspat AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    return true;
}
bool ClangVisitor::TraverseCXXDestructorDecl(clang::CXXDestructorDecl *Dtor) {
    // ak nema telo tak sa znova skipne
    if (!Dtor->isFirstDecl()) {
        return true;
    }

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // akcia na tomto vrchole
    // ziskanie ownera
    auto owner = this->get_existing_class(
        Dtor->getParent()->getNameAsString()
    );

    auto new_dtor = this->stmt_factory_->mk_destructor_def(
        owner,
        nullptr
    );
    owner->destructors_.push_back(new_dtor);
    TraverseStmt(Dtor->getBody());
    new_dtor->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    return true;
}
bool ClangVisitor::TraverseFunctionDecl(clang::FunctionDecl *FD) {
    // ak nema telo (je to len declaracia), tak sa skipne, vytvori sa az ked pride na jej definiciu
    if (!FD->isFirstDecl()) {
        return true;
    }

    Type* retType = this->get_astfri_type(FD->getReturnType());

    // akcia na tomto vrchole
    auto new_function = this->stmt_factory_->mk_function_def(
        FD->getNameAsString(),
        std::vector<ParamVarDefStmt*> {},
        retType,
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

    return true;
}
bool ClangVisitor::TraverseCXXMethodDecl(clang::CXXMethodDecl *MD) {
    // ak nema telo (je to len declaracia), tak sa skipne, vytvori sa az ked pride na jej definiciu
    if (!MD->isFirstDecl()) {
        return true;
    }
    
    // akcia na tomto vrchole
    auto owner = this->get_existing_class(
        MD->getParent()->getNameAsString()
    );

    auto virtuality = Virtuality::NotVirtual;
    if (MD->isVirtual()) {
        virtuality = Virtuality::Virtual;
    }

    auto new_method = this->stmt_factory_->mk_method_def(
        owner,
        this->stmt_factory_->mk_function_def(
            MD->getNameAsString(),
            std::vector<ParamVarDefStmt*> {},
            this->get_astfri_type(MD->getReturnType()),
            nullptr),
            this->getAccessModifier(MD),
            virtuality
    );
    owner->methods_.push_back(new_method);

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

    TraverseStmt(MD->getBody());
    new_method->func_->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vratenie naspat AST Location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    return true;
}
bool ClangVisitor::TraverseCXXRecordDecl(clang::CXXRecordDecl *RD) {
    // akcia na vrchole
    auto new_class = this->stmt_factory_->mk_class_def(
        RD->getNameAsString()
    );
    this->tu_->classes_.push_back(new_class);

    // nastavenie bases
    for (auto base : RD->bases()) {
        new_class->bases_.push_back(this->get_existing_class(base.getType().getBaseTypeIdentifier()->getName().str()));
    }

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

    if(auto tparams = RD->getDescribedTemplateParams()) {
        // std::cout << "Som v triede ktorá má template\n";
        for (unsigned int i = 0; i < tparams->size(); i++) {
            new_class->tparams_.push_back(
                this->stmt_factory_->mk_generic_param(
                    "",
                    tparams->getParam(i)->getNameAsString()
                )
            );
            // std::cout << tparams->getParam(i)->getNameAsString() << "\n"; // Vratilo T konecne
        }
    }

    // vratenie naspat na predosly location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    return true;
}
bool ClangVisitor::TraverseVarDecl(clang::VarDecl *VD) {
    // akcia na tomto vrchole
    Type* type = this->get_astfri_type(VD->getType());

    VarDefStmt* new_var = nullptr;
    if (this->astfri_location.stmt_) {
        // premenna v compounde premenna
        new_var = this->stmt_factory_->mk_local_var_def(
            VD->getNameAsString(),
            type,
            nullptr
        );
        ((DefStmt*)this->astfri_location.stmt_)->defs_.push_back(new_var);
    } else {
        // globalna premenna
        new_var = this->stmt_factory_->mk_global_var_def(
            VD->getNameAsString(),
            type,
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

    return true;
}
bool ClangVisitor::TraverseParmVarDecl(clang::ParmVarDecl *PVD) {
    // akcia na tomto vrchole
    ParamVarDefStmt* new_par = this->stmt_factory_->mk_param_var_def(
        PVD->getNameAsString(),
        this->get_astfri_type(PVD->getType()),
        nullptr
    );

    // ak ma inicializator
    if(auto init = PVD->getInit()) {
        TraverseStmt(init);
        new_par->initializer_ = this->astfri_location.expr_;
    }

    this->astfri_location.stmt_ = new_par;
    this->clang_location.decl_ = PVD;

    return true;
}
bool ClangVisitor::TraverseFieldDecl(clang::FieldDecl *FD) {
    // akcia na tomto node
    // vytvorenie premennej triedy
    astfri::AccessModifier access = this->getAccessModifier(FD);
    auto new_member = this->stmt_factory_->mk_member_var_def(
        FD->getNameAsString(),
        this->get_astfri_type(FD->getType()),
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

    return true;
}
// // traverse statementy
bool ClangVisitor::TraverseDeclStmt(clang::DeclStmt *DS) {
    // akcia na tomto vrchole
    auto new_def_stmt = this->stmt_factory_->mk_def();
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_def_stmt);

    // zapamatanie si AST Location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_def_stmt;
    this->clang_location.stmt_ = DS;

    // prejdenie deklaracii
    for (auto decl : DS->decls()) {
        TraverseDecl(decl);
    }

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    return true;
}
bool ClangVisitor::TraverseCompoundStmt(clang::CompoundStmt *CS) {
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
        } else if (llvm::dyn_cast<clang::CXXThrowExpr>(stmt)) {
            // je to expr ale v ASTFRI je to stmt, tak aby sa vyhadzovana expr neulozila znova
            TraverseStmt(stmt);
        } else if (llvm::dyn_cast<clang::Expr>(stmt)) {
            TraverseStmt(stmt);
            new_compound->stmts_.push_back(
                this->stmt_factory_->mk_expr(this->astfri_location.expr_)
            );
        } else {
            TraverseStmt(stmt);
        }
    }

    return true;
}
bool ClangVisitor::TraverseReturnStmt(clang::ReturnStmt *RS) {
    // akcia na tomto vrchole
    ReturnStmt* new_return = this->stmt_factory_->mk_return(
        nullptr
    );
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_return);

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie location
    this->astfri_location.stmt_ = new_return;
    this->clang_location.stmt_ = RS;

    TraverseStmt(RS->getRetValue()); // ->IgnoreCasts()
    // naplnenie return stmt
    new_return->val_ = this->astfri_location.expr_;

    // vratenie ast location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    return true;
}
bool ClangVisitor::TraverseIfStmt(clang::IfStmt *IS) {
    // akcia na tomto vrchole
    auto new_if = this->stmt_factory_->mk_if(
        nullptr,
        nullptr,
        nullptr
    );

    // TODO: prepoklada sa ze je v compounde a nie v else vetve
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_if);

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST Location
    this->astfri_location.stmt_ = new_if;
    this->clang_location.stmt_ = IS;

    // priradenie podmienky
    TraverseStmt(IS->getCond());
    new_if->cond_ = this->astfri_location.expr_;

    // priradenie true vetvy
    TraverseStmt(IS->getThen());
    new_if->iftrue_ = this->astfri_location.stmt_;

    // else vetva
    if (auto else_stmt = IS->getElse()) {
        // ak je else compound
        if (auto compound = llvm::dyn_cast<clang::CompoundStmt>(IS->getElse())) {
            TraverseStmt(compound);
            new_if->iffalse_ = this->astfri_location.stmt_;
        } else {
            // ak je to iba jeden prikaz
            CompoundStmt* tempCmpd = this->stmt_factory_->mk_compound(std::vector<Stmt *> {});
            this->astfri_location.stmt_ = tempCmpd;
            TraverseStmt(else_stmt);
            new_if->iffalse_ = tempCmpd->stmts_[0];
        }
    }

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    return true;
}
bool ClangVisitor::TraverseForStmt(clang::ForStmt *FS) {
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
    TraverseStmt(FS->getCond());
    new_for->cond_ = this->astfri_location.expr_;

    //naplnenie tela
    TraverseStmt(FS->getBody());
    new_for->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vytvorim si compound stmt a do toho pojdu init statementy, potom ich hodim do vardef
    auto init_compound = this->stmt_factory_->mk_compound(
        std::vector<Stmt *> {}
    );
    this->astfri_location.stmt_ = init_compound;
    TraverseStmt(FS->getInit());
    new_for->init_ = ((CompoundStmt*)this->astfri_location.stmt_)->stmts_[0];

    auto step = FS->getInc();
    TraverseStmt(step);
    new_for->step_ = this->stmt_factory_->mk_expr(
        this->astfri_location.expr_
    );

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location = clang_temp;

    return true;
}
bool ClangVisitor::TraverseWhileStmt(clang::WhileStmt *WS) {
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

    return true;
}
bool ClangVisitor::TraverseDoStmt(clang::DoStmt *DS) {
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

    return true;
}
bool ClangVisitor::TraverseSwitchStmt(clang::SwitchStmt *SS) {
    // akcia na tomto vrchole
    auto new_switch = this->stmt_factory_->mk_switch(
        nullptr,
        std::vector<CaseBaseStmt*> {}
    );
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_switch);

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_switch;
    this->clang_location.stmt_ = SS;

    // Nastavenie podmienky
    TraverseStmt(SS->getCond());
    new_switch->expr_ = this->astfri_location.expr_;

    // naplnenie new_switch
    // akcia na kazdom case
    for (auto sw_case = SS->getSwitchCaseList(); sw_case; sw_case = sw_case->getNextSwitchCase()) {
        // llvm::outs() << "Sw case: " << sw_case->getStmtClassName() << "\n";
        // ak je to case stmt a nie default
        if (auto case_stmt = llvm::dyn_cast<clang::CaseStmt>(sw_case)) {
            auto new_case = this->stmt_factory_->mk_case(
                std::vector<Expr*> {},
                nullptr
            );
            // naplnenie case-u
            auto case_cond = case_stmt->getLHS(); //->IgnoreCasts()
            TraverseStmt(case_cond);
            new_case->exprs_.push_back(this->astfri_location.expr_);

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
            auto new_default = this->stmt_factory_->mk_default_case(
                nullptr
            );
            // naplnenie default-u
            auto default_body = default_stmt->getSubStmt();
            // ak je v compounde
            if (auto CS_body = llvm::dyn_cast<clang::CompoundStmt>(default_body)) {
                TraverseStmt(CS_body);
                new_default->body_ = this->astfri_location.stmt_;
            } else {
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

    return true;
}
bool ClangVisitor::TraverseContinueStmt([[maybe_unused]] clang::ContinueStmt *CS) {
    // akcia na tomto vrchole
    auto new_continue = this->stmt_factory_->mk_continue();
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_continue);
    
    return true;
}
bool ClangVisitor::TraverseBreakStmt([[maybe_unused]] clang::BreakStmt *BS) {
    // akcia na tomto vrchole
    auto new_break = this->stmt_factory_->mk_break();
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_break);
    
    return true;
}
// visit expression
bool ClangVisitor::TraverseCXXConstructExpr(clang::CXXConstructExpr *Ctor) {
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

    return true;
}
bool ClangVisitor::TraverseDeclRefExpr(clang::DeclRefExpr *DRE) {
    // akcia na tomto vrchole
    Expr* new_ref_expr = nullptr;
    if (auto PVD = llvm::dyn_cast<clang::ParmVarDecl>(DRE->getDecl())) {
        // pre parameter
        new_ref_expr = this->expr_factory_->mk_param_var_ref(
            PVD->getNameAsString()
        );
    } else if (auto VD = llvm::dyn_cast<clang::VarDecl>(DRE->getDecl())) {
        // pre lokalnu premennu
        new_ref_expr = this->expr_factory_->mk_local_var_ref(
            VD->getNameAsString()
        );
    } else if (auto FD = llvm::dyn_cast<clang::FunctionDecl>(DRE->getDecl())) {
        // pre funkciu
        new_ref_expr = this->expr_factory_->mk_function_call(
            FD->getNameAsString(),
            std::vector<Expr *> {}
        );
    }

    this->astfri_location.expr_ = new_ref_expr;
    this->clang_location.expr_ = DRE;

    return true;
}
bool ClangVisitor::TraverseMemberExpr(clang::MemberExpr* ME) {
    // akcia na tomto vrchole
    TraverseStmt(ME->getBase());
    auto new_mem_expr = this->expr_factory_->mk_member_var_ref(
        this->astfri_location.expr_,
        ME->getMemberNameInfo().getAsString().c_str()
    );
    this->astfri_location.expr_ = new_mem_expr;
    this->clang_location.expr_ = ME;

    return true;
}
bool ClangVisitor::TraverseCallExpr(clang::CallExpr *CE) {
    // co sa ma stat ak je zavolana metoda triedy
    if (auto metoda = llvm::dyn_cast<clang::CXXDependentScopeMemberExpr>(CE->getCallee()->IgnoreImpCasts())) {
        TraverseStmt(metoda->getBase());
        auto new_method_call = this->expr_factory_->mk_method_call(
            this->astfri_location.expr_,
            metoda->getMemberNameInfo().getAsString(),
            std::vector<Expr *> {}
        );
        for (auto arg : CE->arguments()) {
            TraverseStmt(arg);
            new_method_call->args_.push_back(this->astfri_location.expr_);
        }

        this->astfri_location.expr_ = new_method_call;
    }
    
    // co sa ma stat ak je zavolana funkcia
    if (auto funkcia = llvm::dyn_cast<clang::DeclRefExpr>(CE->getCallee()->IgnoreImpCasts())) {
        TraverseStmt(funkcia);
        FunctionCallExpr* fun = (FunctionCallExpr*)this->astfri_location.expr_;
        for (auto arg : CE->arguments()) {
            TraverseStmt(arg);
            fun->args_.push_back(this->astfri_location.expr_);
        }
        this->astfri_location.expr_ = fun;
    }
    return true;
}
bool ClangVisitor::TraverseCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr *DSME) {
    // akcia na tomto vrchole
    TraverseStmt(DSME->getBase());
    auto new_mem_expr = this->expr_factory_->mk_member_var_ref(
        this->astfri_location.expr_,
        DSME->getMemberNameInfo().getAsString().c_str()
    );
    this->astfri_location.expr_ = new_mem_expr;
    this->clang_location.expr_ = DSME;

    return true;
}
bool ClangVisitor::TraverseCXXThisExpr(clang::CXXThisExpr *TE) {
    this->astfri_location.expr_ = this->expr_factory_->mk_this();
    this->clang_location.expr_ = TE;

    return true;
}
bool ClangVisitor::TraverseCXXMemberCallExpr(clang::CXXMemberCallExpr* MCE) {
    // akcia na tomto vrchole
    std::vector<Expr*> args;
    for (auto arg : MCE->arguments()) {
        if (arg->isDefaultArgument()) {
            continue;
        }
        TraverseStmt(arg);
        args.push_back(this->astfri_location.expr_);
    }

    // std::cout << "Metoda je " << MCE->getMethodDecl()->getNameAsString().c_str() << std::endl;

    TraverseStmt(MCE->getImplicitObjectArgument()); // owner
    auto new_mem_call = this->expr_factory_->mk_method_call(
        this->astfri_location.expr_,
        MCE->getMethodDecl()->getNameAsString().c_str(),
        args
    );

    this->astfri_location.expr_ = new_mem_call;
    this->clang_location.expr_ = MCE;

    return true;
}
bool ClangVisitor::TraverseCXXNewExpr(clang::CXXNewExpr *NE) {
    // akcia na tomto vrchole
    NewExpr* new_new;
    // ak je alokovany typ builtin typ
    if (NE->getAllocatedType().getTypePtr()->isBuiltinType()) {
        // vytvori sa newExpr a potom sa priradia argumenty construktora
        new_new = this->expr_factory_->mk_new(
            this->expr_factory_->mk_constructor_call(
                this->get_astfri_type(NE->getAllocatedType()),
                std::vector<Expr *> {}
            )
        );
        // TODO: dorobit aby sa argumenty priradovali pri builtin new>  int* cislo = new int(5);
    } else {
        TraverseCXXConstructExpr((clang::CXXConstructExpr*)NE->getConstructExpr());
        new_new = this->expr_factory_->mk_new(
            (ConstructorCallExpr*)this->astfri_location.expr_
        );
    }
    this->astfri_location.expr_ = new_new;
    this->clang_location.expr_ = NE;

    return true;
}
bool ClangVisitor::TraverseCXXDeleteExpr(clang::CXXDeleteExpr *DE) {
    // akcia na tomto vrchole
    TraverseStmt(DE->getArgument());
    auto new_delete = this->expr_factory_->mk_delete(
        this->astfri_location.expr_
    );

    this->astfri_location.expr_ = new_delete;
    this->clang_location.expr_ = DE;

    return true;
}
bool ClangVisitor::TraverseCXXThrowExpr(clang::CXXThrowExpr *TE) {
    // akcia na tomto vrchole
    TraverseStmt(TE->getSubExpr());
    auto new_throw = this->stmt_factory_->mk_throw(
        this->astfri_location.expr_
    );
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_throw);

    return true;
}
// bool ClangVisitor::TraverseCXXOperatorCallExpr(clang::CXXOperatorCallExpr *COCE) {
//     if (auto oper = llvm::dyn_cast<clang::BinaryOperatorKind>(COCE->getOperatorLoc())) {
//         oper
//         TraverseStmt(oper);
//     }
//     return true;
// }

// literaly
 bool ClangVisitor::TraverseIntegerLiteral(clang::IntegerLiteral *IL) {
    // akcia na tomto vrchole
    auto int_literal = this->expr_factory_->mk_int_literal(IL->getValue().getSExtValue());

    // vytvorenie AST location
    this->astfri_location.expr_ = int_literal;
    this->clang_location.int_lit_ = IL;

    return true;
 }
 bool ClangVisitor::TraverseFloatingLiteral(clang::FloatingLiteral *FL) {
    // akcia na tomto vrchole
    auto float_literal = this->expr_factory_->mk_float_literal(FL->getValue().convertToFloat());

    // vytvorenie AST location
    this->astfri_location.expr_ = float_literal;
    this->clang_location.float_lit_ = FL;

    return true;
 }
 bool ClangVisitor::TraverseStringLiteral(clang::StringLiteral *SL) {
    // akcia na tomto vrchole
    auto string_literal = this->expr_factory_->mk_string_literal(SL->getString().str());

    // vytvorenie AST location
    this->astfri_location.expr_ = string_literal;
    this->clang_location.string_lit_ = SL;

    return true;
 }
 bool ClangVisitor::TraverseCXXBoolLiteralExpr(clang::CXXBoolLiteralExpr *BL) {
    // akcia na tomto vrchole
    auto bool_literal = this->expr_factory_->mk_bool_literal(BL->getValue());

    // vytvorenie AST location
    this->astfri_location.expr_ = bool_literal;
    this->clang_location.bool_lit_ = BL;

    return true;
 }
 bool ClangVisitor::TraverseCharacterLiteral(clang::CharacterLiteral *CL) {
    auto new_char = this->expr_factory_->mk_char_literal((char)CL->getValue());
    this->astfri_location.expr_ = new_char;
    this->clang_location.char_lit_ = CL;

    return true;
 }
 bool ClangVisitor::TraverseCXXNullPtrLiteralExpr(clang::CXXNullPtrLiteralExpr *NPLE) {
    auto new_null = this->expr_factory_->mk_null_literal();
    this->astfri_location.expr_ = new_null;
    this->clang_location.nullptr_lit_ = NPLE;
    
    return true;
 }
 // operatory
 bool ClangVisitor::TraverseCompoundAssignOperator(clang::CompoundAssignOperator *CAO) {
    BinOpExpr* bin_op = nullptr;

    bin_op = this->expr_factory_->mk_bin_on(
        nullptr,
        this->get_astfri_bin_op_type(CAO->getOpcode()),
        nullptr
    );

    // prepisanie AST location
    this->astfri_location.expr_ = bin_op;
    this->clang_location.bin_op_ = CAO;

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
    TraverseStmt(CAO->getLHS());
    bin_op->left_ = this->astfri_location.expr_;
    // pravy operator
    TraverseStmt(CAO->getRHS());
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
 bool ClangVisitor::TraverseBinaryOperator(clang::BinaryOperator *BO) {
    BinOpExpr* bin_op = nullptr;

    bin_op = this->expr_factory_->mk_bin_on(
        nullptr,
        this->get_astfri_bin_op_type(BO->getOpcode()),
        nullptr
    );

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
    // akcia na tomto vrchole
    UnaryOpType op = this->get_astfri_un_op_type(UO->getOpcode());

    // nastavenie operandu
    TraverseStmt(UO->getSubExpr());
    Expr* arg = this->astfri_location.expr_;

    // vytvorenie unary operator vrchol
    UnaryOpExpr* un_op = this->expr_factory_->mk_unary_op(
        op,
        arg
    );

    // ak je expression nad compound stmt, tak sa vytvori ako stmt a nie expr a rovno sa tam vlozi
    if (this->expr_as_stmt) {
        ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(
            this->stmt_factory_->mk_expr(un_op)
        );
    }

    this->astfri_location.expr_ = un_op;
    this->clang_location.un_op_ = UO;

    return true;
}
} // namespace libastfri::astfri_cpp