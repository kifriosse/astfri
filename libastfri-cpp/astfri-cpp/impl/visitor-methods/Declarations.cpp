#include <astfri-cpp/impl/visitor-methods/ClangVisitor.hpp>
#include <clang/Basic/Specifiers.h>
#include <astfri/impl/StmtDef.hpp>


namespace astfri::cpp {


bool ClangVisitor::VisitNamespaceDecl(clang::NamespaceDecl* ND) {
    (void)ND; // aby nevyskakoval warning o unused premennej
    // std::cout << "Traversing namespace: " << ND->getNameAsString() << "\n";
    return true; // Pokračujeme v prechádzaní poduzlov
}

bool ClangVisitor::TraverseCXXConstructorDecl(clang::CXXConstructorDecl* Ctor) {
    // aby sa viac krat nevytvaral
    if (! Ctor->hasBody()) {
        return true;
    }

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // akcia na tomto vrchole
    // ziskanie ownera
    auto owner = this->get_existing_class(Ctor->getParent()->getNameAsString());
    if (Ctor->hasBody()) {
        // vytvorenie konstruktora
        auto new_ctor = this->m_stmt_factory->mk_constructor_def(
            owner,
            std::vector<ParamVarDefStmt*>{},
            std::vector<BaseInitializerStmt*>{},
            nullptr,
            this->getAccessModifier(Ctor)
        );
        owner->constructors.push_back(new_ctor);

        TraverseStmt(Ctor->getBody());
        new_ctor->body = (CompoundStmt*)this->m_astfri_location.stmt;

        // prejdenie CtorInitializers a vlozenie na zaciatok
        // pouzica sa na to, aby sa novy = stmt vlozil na spravne miesto,
        // inak by boli opacne vkladane (posledny by bol prvy)
        int place_in_vector = 0;
        for (auto init : Ctor->inits()) {

            // ak je to base init
            if (init->isBaseInitializer()) {
                std::vector<Expr*> args{};
                auto ce = llvm::dyn_cast<clang::CXXConstructExpr>(init->getInit());
                for (auto arg : ce->arguments()) {
                    TraverseStmt(arg);
                    args.push_back(this->m_astfri_location.expr);
                }

                // treba ziskat nazov base triedy, podla neho sa najde typ
                std::string base_class_name;
                const clang::Type* clang_type = init->getBaseClass();
                if (clang_type) {
                    base_class_name = clang::QualType(clang_type, 0).getAsString();
                }
                ClassType* type = this->get_existing_class(base_class_name)->type;

                // vytvorenie astfri vrchola base initializer, ked uz mam vsetko
                auto base_init = this->m_stmt_factory->mk_base_initializer(type, args);
                new_ctor->baseInit.push_back(base_init);
                continue;
            }

            // ak je to inicializator fieldu
            auto left = this->m_expr_factory->mk_member_var_ref(
                this->m_expr_factory->mk_this(),
                init->getMember()->getNameAsString()
            );
            TraverseStmt(init->getInit());
            auto right    = this->m_astfri_location.expr;
            auto new_init = this->m_stmt_factory->mk_expr(
                this->m_expr_factory->mk_bin_on(left, BinOpType::Assign, right)
            );
            new_ctor->body->stmts.emplace(
                new_ctor->body->stmts.begin() + place_in_vector,
                new_init
            );
            place_in_vector++;
        }

        for (auto parm : Ctor->parameters()) {
            TraverseDecl(parm);
            new_ctor->params.push_back((ParamVarDefStmt*)this->m_astfri_location.stmt);
        }
    }

    // vratenie naspat AST location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseCXXDestructorDecl(clang::CXXDestructorDecl* Dtor) {
    // ak nema telo tak sa znova skipne
    if (! Dtor->isFirstDecl()) {
        return true;
    }

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // akcia na tomto vrchole
    // ziskanie ownera
    auto owner    = this->get_existing_class(Dtor->getParent()->getNameAsString());

    auto new_dtor = this->m_stmt_factory->mk_destructor_def(owner, nullptr);
    owner->destructors.push_back(new_dtor);
    TraverseStmt(Dtor->getBody());
    new_dtor->body = (CompoundStmt*)this->m_astfri_location.stmt;

    // vratenie AST location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseFunctionDecl(clang::FunctionDecl* FD) {
    // ak nema telo (je to len declaracia), tak sa skipne, vytvori sa az ked pride na jej definiciu
    if (! FD->isFirstDecl()) {
        return true;
    }

    Type* retType = this->get_astfri_type(FD->getReturnType());

    // akcia na tomto vrchole
    auto new_function = this->m_stmt_factory->mk_function_def(
        FD->getNameAsString(),
        std::vector<ParamVarDefStmt*>{},
        retType,
        nullptr
    );
    this->m_tu->functions.push_back(new_function);

    // zapamatanie si AST Location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // prepisanie AST location
    this->m_astfri_location.stmt = new_function;
    this->m_clang_location.decl  = FD;

    // parametre
    for (auto parm : FD->parameters()) {
        TraverseDecl(parm);
        new_function->params.push_back((ParamVarDefStmt*)this->m_astfri_location.stmt);
    }

    // prejdenie tela funkcie
    auto body = FD->getBody();
    TraverseStmt(body);

    // priradenie comopund statementu funkcii
    new_function->body = (CompoundStmt*)this->m_astfri_location.stmt;

    // vratenie naspat location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseCXXMethodDecl(clang::CXXMethodDecl* MD) {
    // ak nema telo (je to len declaracia), tak sa skipne, vytvori sa az ked pride na jej definiciu
    if (! MD->isFirstDecl()) {
        return true;
    }

    // akcia na tomto vrchole
    auto owner      = this->get_existing_class(MD->getParent()->getNameAsString());

    auto virtuality = Virtuality::NotVirtual;
    if (MD->isVirtual()) {
        virtuality = Virtuality::Virtual;
    }

    // zistenie statickosti
    auto staticity = Staticity::NonStatic;
    if (MD->isStatic()) {
        staticity = Staticity::Static;
    }

    auto new_method = this->m_stmt_factory->mk_method_def(
        owner,
        this->m_stmt_factory->mk_function_def(
            MD->getNameAsString(),
            std::vector<ParamVarDefStmt*>{},
            this->get_astfri_type(MD->getReturnType()),
            nullptr
        ),
        this->getAccessModifier(MD),
        virtuality,
        staticity
    );
    owner->methods.push_back(new_method);

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // prepisanie AST location
    this->m_astfri_location.stmt = new_method;
    this->m_clang_location.decl  = MD;

    // parametre
    for (auto parm : MD->parameters()) {
        TraverseDecl(parm);
        new_method->func->params.push_back((ParamVarDefStmt*)this->m_astfri_location.stmt);
    }

    TraverseStmt(MD->getBody());
    new_method->func->body = (CompoundStmt*)this->m_astfri_location.stmt;

    // vratenie naspat AST Location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseCXXRecordDecl(clang::CXXRecordDecl* RD) {
    // ak je to trieda, ktorú vygenerovala lambda, tak skipnem
    if (RD->isLambda()) {
        return true;
    }

    // akcia na vrchole
    // vytvorí sa scope (linked list, do ktoreho ukladam na zaciatok,
    // aby bol scope zoradeny od vseobecnych namespaceov prcvych)
    std::list<std::string> linked_scope = {};
    clang::DeclContext* context    = RD->getDeclContext();
    while (context) {
        if (auto named = llvm::dyn_cast<clang::NamedDecl>(context)) {
            linked_scope.push_front(named->getNameAsString());
        }
        context = context->getParent();
    }
    
    // vytvorenie triedy
    auto new_class = this->m_stmt_factory->mk_class_def(
        RD->getNameAsString(),
        {{linked_scope.begin(), linked_scope.end()}}
    );
    this->m_tu->classes.push_back(new_class);

    // nastavenie bases
    for (auto base : RD->bases()) {
        new_class->bases.push_back(
            this->get_existing_class(base.getType().getBaseTypeIdentifier()->getName().str())
        );
    }

    // zapamatanie si predoslich location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    
    // prejdenie statickych atributov triedy
    // potrebujem nastavit aby sa ulozili do docasneho def stmt a potom ich odtial vyberiem, clang to tak modeluje
    this->m_clang_location.decl  = RD;
    // prejdenie vsetkych decls
    for(auto decl : RD->decls()) {
        if(auto varDecl = llvm::dyn_cast<clang::VarDecl>(decl)) {
            // ak nie je staticka, tak preskocim
            if (!varDecl->isStaticDataMember()) {
                continue;
            }
            // temp def stmt, do ktoreho sa naplni staticky field
            this->m_astfri_location.stmt = this->m_stmt_factory->mk_def();
            TraverseDecl(varDecl); // clang ich modeluje ako varDecl
            astfri::AccessModifier access = this->getAccessModifier(varDecl);
            // vytvorenie statickeho fieldu a naplnenie z temp def stmt
            // TODO: toto by mohlo byt v cykle, ak je ich viac v jednom def stmt -> static int first, second;
            // zatial ratam s tym ze je len jeden
            MemberVarDefStmt* newStaticField = this->m_stmt_factory->mk_member_var_def(
                ((MultiVarDefStmt*)this->m_astfri_location.stmt)->defs[0]->name,
                ((MultiVarDefStmt*)this->m_astfri_location.stmt)->defs[0]->type,
                ((MultiVarDefStmt*)this->m_astfri_location.stmt)->defs[0]->initializer,
                access,
                Staticity::Static
            );
            new_class->vars.push_back(newStaticField);
        }
   }

    // prepisanie AST location na triedu
    this->m_astfri_location.stmt = new_class;
    this->m_clang_location.decl  = RD;
    
    for (auto field : RD->fields()) {
        TraverseDecl(field);
    }

    for (auto method : RD->methods()) {
        TraverseDecl(method);
    }

    if (auto tparams = RD->getDescribedTemplateParams()) {
        // std::cout << "Som v triede ktorá má template\n";
        for (unsigned int i = 0; i < tparams->size(); i++) {
            new_class->tparams.push_back(
                this->m_stmt_factory->mk_generic_param("", tparams->getParam(i)->getNameAsString())
            );
            // std::cout << tparams->getParam(i)->getNameAsString() << "\n"; // Vratilo T konecne
        }
    }

    // vratenie naspat na predosly location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseVarDecl(clang::VarDecl* VD) {
    // akcia na tomto vrchole
    Type* type          = this->get_astfri_type(VD->getType());

    LocalVarDefStmt *local_var = nullptr;
    GlobalVarDefStmt *global_var = nullptr;
    if (this->m_astfri_location.stmt) {
        // premenna v compounde
        local_var = this->m_stmt_factory->mk_local_var_def(VD->getNameAsString(), type, nullptr);
        ((MultiVarDefStmt*)this->m_astfri_location.stmt)->defs.push_back(local_var);
    }
    else {
        // globalna premenna
        global_var = this->m_stmt_factory->mk_global_var_def(VD->getNameAsString(), type, nullptr);
        this->m_tu->globals.push_back(global_var);
    }

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // prepisanie AST location
    this->m_astfri_location.stmt = local_var ? (Stmt*)local_var : (Stmt*)global_var;
    this->m_clang_location.decl  = VD;

    if (auto init = VD->getInit()) {
        TraverseStmt(init);
        if (local_var) {
            local_var->initializer = this->m_astfri_location.expr;
        } else {
            global_var->initializer = this->m_astfri_location.expr;
        }
    }

    // vratenie AST location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseParmVarDecl(clang::ParmVarDecl* PVD) {
    // akcia na tomto vrchole
    ParamVarDefStmt* new_par = this->m_stmt_factory->mk_param_var_def(
        PVD->getNameAsString(),
        this->get_astfri_type(PVD->getType()),
        nullptr
    );

    // ak ma inicializator
    if (auto init = PVD->getInit()) {
        TraverseStmt(init);
        new_par->initializer = this->m_astfri_location.expr;
    }

    this->m_astfri_location.stmt = new_par;
    this->m_clang_location.decl  = PVD;

    return true;
}

bool ClangVisitor::TraverseFieldDecl(clang::FieldDecl* FD) {
    // akcia na tomto node
    // vytvorenie premennej triedy
    astfri::AccessModifier access = this->getAccessModifier(FD);
    auto staticity = Staticity::NonStatic;

    auto new_member               = this->m_stmt_factory->mk_member_var_def(
        FD->getNameAsString(),
        this->get_astfri_type(FD->getType()),
        nullptr,
        access,
        staticity
    );
    ((ClassDefStmt*)this->m_astfri_location.stmt)->vars.push_back(new_member);

    // zapamatanie si predoslich location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // prepisanie AST location
    this->m_astfri_location.stmt = new_member;
    this->m_clang_location.decl  = FD;

    // ak ma inicializator
    if (auto init = FD->getInClassInitializer()) {
        TraverseStmt(init);
        new_member->initializer = this->m_astfri_location.expr;
    }

    // vratenie naspat na predosly location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}


} // namespace astfri::cpp
