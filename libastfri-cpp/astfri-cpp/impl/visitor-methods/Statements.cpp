#include <astfri-cpp/impl/visitor-methods/ClangVisitor.hpp>


namespace astfri::cpp {


bool ClangVisitor::TraverseDeclStmt(clang::DeclStmt* DS) {
    // akcia na tomto vrchole
    auto new_def_stmt = this->m_stmt_factory->mk_def();
    ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.push_back(new_def_stmt);

    // zapamatanie si AST Location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // prepisanie AST location
    this->m_astfri_location.stmt = new_def_stmt;
    this->m_clang_location.stmt  = DS;

    // prejdenie deklaracii
    for (auto decl : DS->decls()) {
        TraverseDecl(decl);
    }

    // vratenie AST location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseCompoundStmt(clang::CompoundStmt* CS) {
    // akcia na tomto vrchole
    CompoundStmt* new_compound = this->m_stmt_factory->mk_compound(std::vector<Stmt*>{});

    // prepisanie AST location
    this->m_astfri_location.stmt = new_compound;
    this->m_clang_location.stmt  = CS;

    for (auto stmt : CS->body()) {
        // ak je binary operator, tak sa zoberie ako stmt
        if (llvm::dyn_cast<clang::BinaryOperator>(stmt)
            || llvm::dyn_cast<clang::UnaryOperator>(stmt)) {
            this->m_expr_as_stmt = true;
            TraverseStmt(stmt);
            this->m_expr_as_stmt = false;
        }
        else if (llvm::dyn_cast<clang::CXXThrowExpr>(stmt)) {
            // je to expr ale v ASTFRI je to stmt, tak aby sa vyhadzovana expr neulozila znova
            TraverseStmt(stmt);
        }
        else if (llvm::dyn_cast<clang::Expr>(stmt)) {
            TraverseStmt(stmt);
            new_compound->stmts.push_back(this->m_stmt_factory->mk_expr(this->m_astfri_location.expr)
            );
        }
        else {
            TraverseStmt(stmt);
        }
    }

    return true;
}

bool ClangVisitor::TraverseReturnStmt(clang::ReturnStmt* RS) {
    // akcia na tomto vrchole
    ReturnStmt* new_return = this->m_stmt_factory->mk_return(nullptr);
    ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.push_back(new_return);

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // prepisanie location
    this->m_astfri_location.stmt = new_return;
    this->m_clang_location.stmt  = RS;

    TraverseStmt(RS->getRetValue()); // ->IgnoreCasts()
    // naplnenie return stmt
    new_return->val = this->m_astfri_location.expr;

    // vratenie ast location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseIfStmt(clang::IfStmt* IS) {
    // akcia na tomto vrchole
    auto new_if = this->m_stmt_factory->mk_if(nullptr, nullptr, nullptr);

    // TODO: prepoklada sa ze je v compounde a nie v else vetve
    ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.push_back(new_if);

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // prepisanie AST Location
    this->m_astfri_location.stmt = new_if;
    this->m_clang_location.stmt  = IS;

    // priradenie podmienky
    TraverseStmt(IS->getCond());
    new_if->cond = this->m_astfri_location.expr;

    // priradenie true vetvy
    TraverseStmt(IS->getThen());
    new_if->iftrue = this->m_astfri_location.stmt;

    // else vetva
    if (auto else_stmt = IS->getElse()) {
        // ak je else compound
        if (auto compound = llvm::dyn_cast<clang::CompoundStmt>(IS->getElse())) {
            TraverseStmt(compound);
            new_if->iffalse = this->m_astfri_location.stmt;
        }
        else {
            // ak je to iba jeden prikaz
            CompoundStmt* tempCmpd      = this->m_stmt_factory->mk_compound(std::vector<Stmt*>{});
            this->m_astfri_location.stmt = tempCmpd;
            TraverseStmt(else_stmt);
            new_if->iffalse = tempCmpd->stmts[0];
        }
    }

    // vratenie AST location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseForStmt(clang::ForStmt* FS) {
    // akcia na tomto vrchole
    auto new_for = this->m_stmt_factory->mk_for(nullptr, nullptr, nullptr, nullptr);
    ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.push_back(new_for);

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // prepisanie AST location
    this->m_astfri_location.stmt = new_for;
    this->m_clang_location.stmt  = FS;

    // naplnenie new_for
    TraverseStmt(FS->getCond());
    new_for->cond = this->m_astfri_location.expr;

    // naplnenie tela
    TraverseStmt(FS->getBody());
    new_for->body = (CompoundStmt*)this->m_astfri_location.stmt;

    // vytvorim si compound stmt a do toho pojdu init statementy, potom ich hodim do vardef
    auto init_compound          = this->m_stmt_factory->mk_compound(std::vector<Stmt*>{});
    this->m_astfri_location.stmt = init_compound;
    TraverseStmt(FS->getInit());
    new_for->init = ((CompoundStmt*)this->m_astfri_location.stmt)->stmts[0];

    auto step     = FS->getInc();
    TraverseStmt(step);
    new_for->step = this->m_stmt_factory->mk_expr(this->m_astfri_location.expr);

    // vratenie AST location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseWhileStmt(clang::WhileStmt* WS) {
    // akcia na tomto vrchole
    auto new_while = this->m_stmt_factory->mk_while(nullptr, nullptr);
    ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.push_back(new_while);

    // zapamatanie si AST Location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // prepisanie AST location
    this->m_astfri_location.stmt = new_while;
    this->m_clang_location.stmt  = WS;

    // naplnenie new_while
    TraverseStmt(WS->getCond());
    new_while->cond = this->m_astfri_location.expr;
    TraverseStmt(WS->getBody());
    new_while->body = (CompoundStmt*)this->m_astfri_location.stmt;

    // vratenie AST Location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseDoStmt(clang::DoStmt* DS) {
    // akcia na tomto vrchole
    auto new_do_while = this->m_stmt_factory->mk_do_while(nullptr, nullptr);
    ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.push_back(new_do_while);

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // prepisanie AST location
    this->m_astfri_location.stmt = new_do_while;
    this->m_clang_location.stmt  = DS;

    // naplnenie new_do_while
    TraverseStmt(DS->getCond());
    new_do_while->cond = this->m_astfri_location.expr;
    TraverseStmt(DS->getBody());
    new_do_while->body = (CompoundStmt*)this->m_astfri_location.stmt;

    // vratenie AST location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseSwitchStmt(clang::SwitchStmt* SS) {
    // akcia na tomto vrchole
    auto new_switch = this->m_stmt_factory->mk_switch(nullptr, {});
    ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.push_back(new_switch);

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // prepisanie AST location
    this->m_astfri_location.stmt = new_switch;
    this->m_clang_location.stmt  = SS;

    // Nastavenie podmienky
    TraverseStmt(SS->getCond());
    new_switch->expr = this->m_astfri_location.expr;

    // naplnenie new_switch
    // akcia na kazdom case
    for (auto sw_case = SS->getSwitchCaseList(); sw_case; sw_case = sw_case->getNextSwitchCase()) {
        // llvm::outs() << "Sw case: " << sw_case->getStmtClassName() << "\n";
        // ak je to case stmt a nie default
        if (auto case_stmt = llvm::dyn_cast<clang::CaseStmt>(sw_case)) {
            auto new_case = this->m_stmt_factory->mk_case(std::vector<Expr*>{}, nullptr);
            // naplnenie case-u
            auto case_cond = case_stmt->getLHS(); //->IgnoreCasts()
            TraverseStmt(case_cond);
            new_case->exprs.push_back(this->m_astfri_location.expr);

            auto case_body = case_stmt->getSubStmt();
            // ak je v compounde
            if (auto CS_body = llvm::dyn_cast<clang::CompoundStmt>(case_body)) {
                TraverseStmt(CS_body);
                new_case->body = this->m_astfri_location.stmt;
            }
            else {
                // nieje compound, iba jeden prikaz (dam do compoundu a potom vyberiem)
                auto temp_compund = this->m_stmt_factory->mk_compound(std::vector<Stmt*>{});
                this->m_astfri_location.stmt = temp_compund;
                TraverseStmt(case_body);
                if (! ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.empty())
                    new_case->body = temp_compund->stmts[0];
            }
            new_switch->cases.push_back(new_case);
        }
        else if (auto default_stmt = llvm::dyn_cast<clang::DefaultStmt>(sw_case)) {
            // ak je to default
            auto new_default = this->m_stmt_factory->mk_default_case(nullptr);
            // naplnenie default-u
            auto default_body = default_stmt->getSubStmt();
            // ak je v compounde
            if (auto CS_body = llvm::dyn_cast<clang::CompoundStmt>(default_body)) {
                TraverseStmt(CS_body);
                new_default->body = this->m_astfri_location.stmt;
            }
            else {
                // nieje compound, iba jeden prikaz (dam do compoundu a potom vyberiem)
                auto temp_compund = this->m_stmt_factory->mk_compound(std::vector<Stmt*>{});
                this->m_astfri_location.stmt = temp_compund;
                TraverseStmt(default_body);
                if (! ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.empty())
                    new_default->body = temp_compund->stmts[0];
            }
            new_switch->defaultCase = new_default;
        }
    }

    // vratenie AST location
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseContinueStmt([[maybe_unused]] clang::ContinueStmt* CS) {
    // akcia na tomto vrchole
    auto new_continue = this->m_stmt_factory->mk_continue();
    ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.push_back(new_continue);

    return true;
}

bool ClangVisitor::TraverseBreakStmt([[maybe_unused]] clang::BreakStmt* BS) {
    // akcia na tomto vrchole
    auto new_break = this->m_stmt_factory->mk_break();
    ((CompoundStmt*)this->m_astfri_location.stmt)->stmts.push_back(new_break);

    return true;
}


} // namespace astfri::cpp
