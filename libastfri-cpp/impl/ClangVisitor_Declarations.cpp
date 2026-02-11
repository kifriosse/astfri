#include <libastfri-cpp/inc/ClangVisitor.hpp>

namespace astfri::astfri_cpp
{
bool ClangVisitor::VisitNamespaceDecl(clang::NamespaceDecl* ND)
{
    (void)ND; // aby nevyskakoval warning o unused premennej
    // std::cout << "Traversing namespace: " << ND->getNameAsString() << "\n";
    return true; // Pokračujeme v prechádzaní poduzlov
}

bool ClangVisitor::TraverseCXXConstructorDecl(clang::CXXConstructorDecl* Ctor)
{
    // aby sa viac krat nevytvaral
    if (! Ctor->isFirstDecl())
    {
        return true;
    }
    
    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;
    
    // akcia na tomto vrchole
    // ziskanie ownera
    auto owner = this->get_existing_class(Ctor->getParent()->getNameAsString());
    if (Ctor->hasBody())
    {
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
        for (auto init : Ctor->inits())
        {

            // ak je to base init
            if (init->isBaseInitializer())
            {
                std::vector<Expr*> args{};
                auto ce = llvm::dyn_cast<clang::CXXConstructExpr>(init->getInit());
                for (auto arg : ce->arguments())
                {
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
            auto right    = this->astfri_location.expr_;
            auto new_init = this->stmt_factory_->mk_expr(
                this->expr_factory_->mk_bin_on(left, BinOpType::Assign, right)
            );
            new_ctor->body_->stmts_.emplace(
                new_ctor->body_->stmts_.begin() + place_in_vector,
                new_init
            );
            place_in_vector++;
        }

        for (auto parm : Ctor->parameters())
        {
            TraverseDecl(parm);
            new_ctor->params_.push_back((ParamVarDefStmt*)this->astfri_location.stmt_);
        }
    }

    // vratenie naspat AST location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseCXXDestructorDecl(clang::CXXDestructorDecl* Dtor)
{
    // ak nema telo tak sa znova skipne
    if (! Dtor->isFirstDecl())
    {
        return true;
    }

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // akcia na tomto vrchole
    // ziskanie ownera
    auto owner    = this->get_existing_class(Dtor->getParent()->getNameAsString());

    auto new_dtor = this->stmt_factory_->mk_destructor_def(owner, nullptr);
    owner->destructors_.push_back(new_dtor);
    TraverseStmt(Dtor->getBody());
    new_dtor->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseFunctionDecl(clang::FunctionDecl* FD)
{
    // ak nema telo (je to len declaracia), tak sa skipne, vytvori sa az ked pride na jej definiciu
    if (! FD->isFirstDecl())
    {
        return true;
    }

    Type* retType = this->get_astfri_type(FD->getReturnType());

    // akcia na tomto vrchole
    auto new_function = this->stmt_factory_->mk_function_def(
        FD->getNameAsString(),
        std::vector<ParamVarDefStmt*>{},
        retType,
        nullptr
    );
    this->tu_->functions_.push_back(new_function);

    // zapamatanie si AST Location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_function;
    this->clang_location.decl_  = FD;

    // parametre
    for (auto parm : FD->parameters())
    {
        TraverseDecl(parm);
        new_function->params_.push_back((ParamVarDefStmt*)this->astfri_location.stmt_);
    }

    // prejdenie tela funkcie
    auto body = FD->getBody();
    TraverseStmt(body);

    // priradenie comopund statementu funkcii
    new_function->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vratenie naspat location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseCXXMethodDecl(clang::CXXMethodDecl* MD)
{
    // ak nema telo (je to len declaracia), tak sa skipne, vytvori sa az ked pride na jej definiciu
    if (! MD->isFirstDecl())
    {
        return true;
    }

    // akcia na tomto vrchole
    auto owner      = this->get_existing_class(MD->getParent()->getNameAsString());

    auto virtuality = Virtuality::NotVirtual;
    if (MD->isVirtual())
    {
        virtuality = Virtuality::Virtual;
    }

    auto new_method = this->stmt_factory_->mk_method_def(
        owner,
        this->stmt_factory_->mk_function_def(
            MD->getNameAsString(),
            std::vector<ParamVarDefStmt*>{},
            this->get_astfri_type(MD->getReturnType()),
            nullptr
        ),
        this->getAccessModifier(MD),
        virtuality
    );
    owner->methods_.push_back(new_method);

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_method;
    this->clang_location.decl_  = MD;

    // parametre
    for (auto parm : MD->parameters())
    {
        TraverseDecl(parm);
        new_method->func_->params_.push_back((ParamVarDefStmt*)this->astfri_location.stmt_);
    }

    TraverseStmt(MD->getBody());
    new_method->func_->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vratenie naspat AST Location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseCXXRecordDecl(clang::CXXRecordDecl* RD)
{
    // ak je to trieda, ktorú vygenerovala lambda, tak skipnem
    if (RD->isLambda()) {
        return true;
    }
    
    // akcia na vrchole
    // vytvorí sa scope
    std::vector<std::string> layers = {};
    clang::DeclContext* context = RD->getDeclContext();
    while (context) {
        if (auto named = llvm::dyn_cast<clang::NamedDecl>(context)) {
            layers.push_back(named->getNameAsString());
        }
        context = context->getParent();
    }
    layers = {layers.rbegin(), layers.rend()};
    
    // vytvorenie triedy
    auto new_class = this->stmt_factory_->mk_class_def(
        RD->getNameAsString(),
        {{layers.rbegin(), layers.rend()}});
    this->tu_->classes_.push_back(new_class);
    
    // nastavenie bases
    for (auto base : RD->bases())
    {
        new_class->bases_.push_back(
            this->get_existing_class(base.getType().getBaseTypeIdentifier()->getName().str())
        );
    }
    
    // zapamatanie si predoslich location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;
    
    // prepisanie AST location
    this->astfri_location.stmt_ = new_class;
    this->clang_location.decl_  = RD;
    
    for (auto field : RD->fields())
    {
        TraverseDecl(field);
    }
    
    for (auto method : RD->methods())
    {
        TraverseDecl(method);
    }
    
    if (auto tparams = RD->getDescribedTemplateParams())
    {
        // std::cout << "Som v triede ktorá má template\n";
        for (unsigned int i = 0; i < tparams->size(); i++)
        {
            new_class->tparams_.push_back(
                this->stmt_factory_->mk_generic_param("", tparams->getParam(i)->getNameAsString())
            );
            // std::cout << tparams->getParam(i)->getNameAsString() << "\n"; // Vratilo T konecne
        }
    }

    // vratenie naspat na predosly location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseVarDecl(clang::VarDecl* VD)
{
    // akcia na tomto vrchole
    Type* type          = this->get_astfri_type(VD->getType());

    VarDefStmt* new_var = nullptr;
    if (this->astfri_location.stmt_)
    {
        // premenna v compounde
        new_var = this->stmt_factory_->mk_local_var_def(VD->getNameAsString(), type, nullptr);
        ((DefStmt*)this->astfri_location.stmt_)->defs_.push_back(new_var);
    }
    else
    {
        // globalna premenna
        new_var = this->stmt_factory_->mk_global_var_def(VD->getNameAsString(), type, nullptr);
        this->tu_->globals_.push_back((GlobalVarDefStmt*)new_var);
    }

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_var;
    this->clang_location.decl_  = VD;

    if (auto init = VD->getInit())
    {
        TraverseStmt(init);
        new_var->initializer_ = this->astfri_location.expr_;
    }

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseParmVarDecl(clang::ParmVarDecl* PVD)
{
    // akcia na tomto vrchole
    ParamVarDefStmt* new_par = this->stmt_factory_->mk_param_var_def(
        PVD->getNameAsString(),
        this->get_astfri_type(PVD->getType()),
        nullptr
    );

    // ak ma inicializator
    if (auto init = PVD->getInit())
    {
        TraverseStmt(init);
        new_par->initializer_ = this->astfri_location.expr_;
    }

    this->astfri_location.stmt_ = new_par;
    this->clang_location.decl_  = PVD;

    return true;
}

bool ClangVisitor::TraverseFieldDecl(clang::FieldDecl* FD)
{
    // akcia na tomto node
    // vytvorenie premennej triedy
    astfri::AccessModifier access = this->getAccessModifier(FD);
    auto new_member               = this->stmt_factory_->mk_member_var_def(
        FD->getNameAsString(),
        this->get_astfri_type(FD->getType()),
        nullptr,
        access
    );
    ((ClassDefStmt*)this->astfri_location.stmt_)->vars_.push_back(new_member);

    // zapamatanie si predoslich location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_member;
    this->clang_location.decl_  = FD;

    // ak ma inicializator
    if (auto init = FD->getInClassInitializer())
    {
        TraverseStmt(init);
        new_member->initializer_ = this->astfri_location.expr_;
    }

    // vratenie naspat na predosly location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}
}