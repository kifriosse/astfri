#include <astfri-cpp/impl/visitor-methods/ClangVisitor.hpp>


namespace astfri::cpp {


// visit expression
bool ClangVisitor::TraverseCXXConstructExpr(clang::CXXConstructExpr* Ctor) {
    // akcia na tomto vrchole
    auto new_ctor_expr = this->m_expr_factory->mk_constructor_call(nullptr, std::vector<Expr*>{});

    // treba nastaviť typ
    // TODO: je to momentálne len pre ClassDefStmt, mne sa neda použiť is_a (dyn_cast), tak bude
    // treba niećo vymyslieť
    new_ctor_expr->type = ((astfri::ClassDefStmt*)this->m_astfri_location.stmt)->type;

    for (auto arg : Ctor->arguments()) {
        TraverseStmt(arg);
        new_ctor_expr->args.push_back(this->m_astfri_location.expr);
    }

    this->m_astfri_location.expr = new_ctor_expr;
    this->m_clang_location.expr  = Ctor;

    return true;
}

bool ClangVisitor::TraverseDeclRefExpr(clang::DeclRefExpr* DRE) {
    // akcia na tomto vrchole
    Expr* new_ref_expr = nullptr;
    if (auto PVD = llvm::dyn_cast<clang::ParmVarDecl>(DRE->getDecl())) {
        // pre parameter
        new_ref_expr = this->m_expr_factory->mk_param_var_ref(PVD->getNameAsString());
    }
    else if (auto VD = llvm::dyn_cast<clang::VarDecl>(DRE->getDecl())) {
        // pre lokalnu premennu
        new_ref_expr = this->m_expr_factory->mk_local_var_ref(VD->getNameAsString());
    }
    else if (auto FD = llvm::dyn_cast<clang::FunctionDecl>(DRE->getDecl())) {
        // pre funkciu
        new_ref_expr
            = this->m_expr_factory->mk_function_call(FD->getNameAsString(), std::vector<Expr*>{});
    }

    this->m_astfri_location.expr = new_ref_expr;
    this->m_clang_location.expr  = DRE;

    return true;
}

bool ClangVisitor::TraverseLambdaExpr(clang::LambdaExpr* LBD) {
    // akcia na tomto vrchole
    // získam si potrebné veci na lambda uzol
    std::vector<ParamVarDefStmt*> params{};
    astfri::Stmt* body;

    // získanie parametrov
    for (auto parameter : (LBD->getCallOperator()->parameters())) {
        TraverseDecl(parameter);
        params.push_back((astfri::ParamVarDefStmt*)m_astfri_location.stmt);
    }

    // získanie tela lambdy
    TraverseStmt(LBD->getBody());
    body = this->m_astfri_location.stmt;

    // vytvorenie uzla
    std::string name       = LBD->getLambdaClass()->getQualifiedNameAsString();
    LambdaExpr* lambdaExpr = this->m_expr_factory->mk_lambda_expr(params, body, name);

    // nastavenie location
    this->m_astfri_location.expr = lambdaExpr;
    this->m_clang_location.expr  = LBD;

    return true;
}

bool ClangVisitor::TraverseMemberExpr(clang::MemberExpr* ME) {
    // akcia na tomto vrchole
    TraverseStmt(ME->getBase());
    auto new_mem_expr = this->m_expr_factory->mk_member_var_ref(
        this->m_astfri_location.expr,
        ME->getMemberNameInfo().getAsString().c_str()
    );
    this->m_astfri_location.expr = new_mem_expr;
    this->m_clang_location.expr  = ME;

    return true;
}

bool ClangVisitor::TraverseCallExpr(clang::CallExpr* CE) {
    // co sa ma stat ak je zavolana metoda triedy
    if (auto metoda
        = llvm::dyn_cast<clang::CXXDependentScopeMemberExpr>(CE->getCallee()->IgnoreImpCasts())) {
        TraverseStmt(metoda->getBase());
        auto new_method_call = this->m_expr_factory->mk_method_call(
            this->m_astfri_location.expr,
            metoda->getMemberNameInfo().getAsString(),
            std::vector<Expr*>{}
        );
        for (auto arg : CE->arguments()) {
            TraverseStmt(arg);
            new_method_call->args.push_back(this->m_astfri_location.expr);
        }

        this->m_astfri_location.expr = new_method_call;
    }

    // co sa ma stat ak je zavolana funkcia
    if (auto funkcia = llvm::dyn_cast<clang::DeclRefExpr>(CE->getCallee()->IgnoreImpCasts())) {
        TraverseStmt(funkcia);
        FunctionCallExpr* fun = (FunctionCallExpr*)this->m_astfri_location.expr;
        for (auto arg : CE->arguments()) {
            TraverseStmt(arg);
            fun->args.push_back(this->m_astfri_location.expr);
        }
        this->m_astfri_location.expr = fun;
    }
    return true;
}

bool ClangVisitor::TraverseCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr* DSME) {
    // akcia na tomto vrchole
    TraverseStmt(DSME->getBase());
    auto new_mem_expr = this->m_expr_factory->mk_member_var_ref(
        this->m_astfri_location.expr,
        DSME->getMemberNameInfo().getAsString().c_str()
    );
    this->m_astfri_location.expr = new_mem_expr;
    this->m_clang_location.expr  = DSME;

    return true;
}

bool ClangVisitor::TraverseCXXThisExpr(clang::CXXThisExpr* TE) {
    this->m_astfri_location.expr = this->m_expr_factory->mk_this();
    this->m_clang_location.expr  = TE;

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
        args.push_back(this->m_astfri_location.expr);
    }

    // std::cout << "Metoda je " << MCE->getMethodDecl()->getNameAsString().c_str() << std::endl;

    TraverseStmt(MCE->getImplicitObjectArgument()); // owner
    auto new_mem_call = this->m_expr_factory->mk_method_call(
        this->m_astfri_location.expr,
        MCE->getMethodDecl()->getNameAsString().c_str(),
        args
    );

    this->m_astfri_location.expr = new_mem_call;
    this->m_clang_location.expr  = MCE;

    return true;
}

bool ClangVisitor::TraverseCXXNewExpr(clang::CXXNewExpr* NE) {
    // akcia na tomto vrchole
    NewExpr* new_new;
    // ak je alokovany typ builtin typ
    if (NE->getAllocatedType().getTypePtr()->isBuiltinType()) {
        // vytvori sa newExpr a potom sa priradia argumenty construktora
        new_new = this->m_expr_factory->mk_new(this->m_expr_factory->mk_constructor_call(
            this->get_astfri_type(NE->getAllocatedType()),
            std::vector<Expr*>{}
        ));
        // TODO: dorobit aby sa argumenty priradovali pri builtin new>  int* cislo = new int(5);
    }
    else {
        TraverseCXXConstructExpr((clang::CXXConstructExpr*)NE->getConstructExpr());
        new_new = this->m_expr_factory->mk_new((ConstructorCallExpr*)this->m_astfri_location.expr);
    }
    this->m_astfri_location.expr = new_new;
    this->m_clang_location.expr  = NE;

    return true;
}

bool ClangVisitor::TraverseCXXDeleteExpr(clang::CXXDeleteExpr* DE) {
    // akcia na tomto vrchole
    TraverseStmt(DE->getArgument());
    auto new_delete             = this->m_expr_factory->mk_delete(this->m_astfri_location.expr);

    this->m_astfri_location.expr = new_delete;
    this->m_clang_location.expr  = DE;

    return true;
}

bool ClangVisitor::TraverseCXXThrowExpr(clang::CXXThrowExpr* TE) {
    // akcia na tomto vrchole
    TraverseStmt(TE->getSubExpr());
    auto new_throw = this->m_stmt_factory->mk_throw(this->m_astfri_location.expr);
    ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.push_back(new_throw);

    return true;
}

bool ClangVisitor::TraverseCXXOperatorCallExpr(clang::CXXOperatorCallExpr* COCE) {
    // co sa ma stat ak sa zavola lambda
    if (auto record = llvm::dyn_cast<clang::CXXRecordDecl>(COCE->getDirectCallee()->getParent())) {
        if (record && record->isLambda()) {
            std::string name        = record->getQualifiedNameAsString();
            Expr* lambda            = this->m_expr_factory->get_lambda_expr(name);
            std::vector<Expr*> args = {};
            // z nejakého dôvodu sa ako prvý argument berie samotná inštancia lambdy
            // takýto foreach jednoducho preskočí prvý element
            for (auto* arg : llvm::drop_begin(COCE->arguments(), 1)) {
                TraverseStmt(arg);
                args.push_back(this->m_astfri_location.expr);
            }
            LambdaCallExpr* lambdaCall  = this->m_expr_factory->mk_lambda_call(lambda, args);
            this->m_astfri_location.expr = lambdaCall;
        }
    }
    return true;
}

bool ClangVisitor::TraverseParenExpr(clang::ParenExpr *PE) {
    
    // zapamatanie si ast location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp = this->m_clang_location;
    
    // akcia na tomto vrchole
    TraverseStmt(PE->getSubExpr());
    // teraz by malo byt v astlocation atribute expr co je v zatvorke
    BracketExpr* bracket = this->m_expr_factory->mk_bracket(this->m_astfri_location.expr);

    // po tom co su zatvorky prejdene, tak sa povodna
    // verzia zapise naspat (pre istotu) ale so zatvorkami
    astfri_temp.expr = bracket;
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}


} // namespace astfri::cpp
