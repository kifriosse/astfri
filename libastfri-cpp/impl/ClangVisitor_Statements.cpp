#include <libastfri-cpp/inc/ClangVisitor.hpp>

namespace astfri::astfri_cpp
{
bool ClangVisitor::TraverseDeclStmt(clang::DeclStmt* DS)
{
    // akcia na tomto vrchole
    auto new_def_stmt = this->stmt_factory_->mk_def();
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_def_stmt);

    // zapamatanie si AST Location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_def_stmt;
    this->clang_location.stmt_  = DS;

    // prejdenie deklaracii
    for (auto decl : DS->decls())
    {
        TraverseDecl(decl);
    }

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseCompoundStmt(clang::CompoundStmt* CS)
{
    // akcia na tomto vrchole
    CompoundStmt* new_compound = this->stmt_factory_->mk_compound(std::vector<Stmt*>{});

    // prepisanie AST location
    this->astfri_location.stmt_ = new_compound;
    this->clang_location.stmt_  = CS;

    for (auto stmt : CS->body())
    {
        // ak je binary operator, tak sa zoberie ako stmt
        if (llvm::dyn_cast<clang::BinaryOperator>(stmt)
            || llvm::dyn_cast<clang::UnaryOperator>(stmt))
        {
            this->expr_as_stmt = true;
            TraverseStmt(stmt);
            this->expr_as_stmt = false;
        }
        else if (llvm::dyn_cast<clang::CXXThrowExpr>(stmt))
        {
            // je to expr ale v ASTFRI je to stmt, tak aby sa vyhadzovana expr neulozila znova
            TraverseStmt(stmt);
        }
        else if (llvm::dyn_cast<clang::Expr>(stmt))
        {
            TraverseStmt(stmt);
            new_compound->stmts_.push_back(this->stmt_factory_->mk_expr(this->astfri_location.expr_)
            );
        }
        else
        {
            TraverseStmt(stmt);
        }
    }

    return true;
}

bool ClangVisitor::TraverseReturnStmt(clang::ReturnStmt* RS)
{
    // akcia na tomto vrchole
    ReturnStmt* new_return = this->stmt_factory_->mk_return(nullptr);
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_return);

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // prepisanie location
    this->astfri_location.stmt_ = new_return;
    this->clang_location.stmt_  = RS;

    TraverseStmt(RS->getRetValue()); // ->IgnoreCasts()
    // naplnenie return stmt
    new_return->val_ = this->astfri_location.expr_;

    // vratenie ast location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseIfStmt(clang::IfStmt* IS)
{
    // akcia na tomto vrchole
    auto new_if = this->stmt_factory_->mk_if(nullptr, nullptr, nullptr);

    // TODO: prepoklada sa ze je v compounde a nie v else vetve
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_if);

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // prepisanie AST Location
    this->astfri_location.stmt_ = new_if;
    this->clang_location.stmt_  = IS;

    // priradenie podmienky
    TraverseStmt(IS->getCond());
    new_if->cond_ = this->astfri_location.expr_;

    // priradenie true vetvy
    TraverseStmt(IS->getThen());
    new_if->iftrue_ = this->astfri_location.stmt_;

    // else vetva
    if (auto else_stmt = IS->getElse())
    {
        // ak je else compound
        if (auto compound = llvm::dyn_cast<clang::CompoundStmt>(IS->getElse()))
        {
            TraverseStmt(compound);
            new_if->iffalse_ = this->astfri_location.stmt_;
        }
        else
        {
            // ak je to iba jeden prikaz
            CompoundStmt* tempCmpd      = this->stmt_factory_->mk_compound(std::vector<Stmt*>{});
            this->astfri_location.stmt_ = tempCmpd;
            TraverseStmt(else_stmt);
            new_if->iffalse_ = tempCmpd->stmts_[0];
        }
    }

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseForStmt(clang::ForStmt* FS)
{
    // akcia na tomto vrchole
    auto new_for = this->stmt_factory_->mk_for(nullptr, nullptr, nullptr, nullptr);
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_for);

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_for;
    this->clang_location.stmt_  = FS;

    // naplnenie new_for
    TraverseStmt(FS->getCond());
    new_for->cond_ = this->astfri_location.expr_;

    // naplnenie tela
    TraverseStmt(FS->getBody());
    new_for->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vytvorim si compound stmt a do toho pojdu init statementy, potom ich hodim do vardef
    auto init_compound          = this->stmt_factory_->mk_compound(std::vector<Stmt*>{});
    this->astfri_location.stmt_ = init_compound;
    TraverseStmt(FS->getInit());
    new_for->init_ = ((CompoundStmt*)this->astfri_location.stmt_)->stmts_[0];

    auto step      = FS->getInc();
    TraverseStmt(step);
    new_for->step_ = this->stmt_factory_->mk_expr(this->astfri_location.expr_);

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseWhileStmt(clang::WhileStmt* WS)
{
    // akcia na tomto vrchole
    auto new_while = this->stmt_factory_->mk_while(nullptr, nullptr);
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_while);

    // zapamatanie si AST Location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_while;
    this->clang_location.stmt_  = WS;

    // naplnenie new_while
    TraverseStmt(WS->getCond());
    new_while->cond_ = this->astfri_location.expr_;
    TraverseStmt(WS->getBody());
    new_while->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vratenie AST Location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseDoStmt(clang::DoStmt* DS)
{
    // akcia na tomto vrchole
    auto new_do_while = this->stmt_factory_->mk_do_while(nullptr, nullptr);
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_do_while);

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_do_while;
    this->clang_location.stmt_  = DS;

    // naplnenie new_do_while
    TraverseStmt(DS->getCond());
    new_do_while->cond_ = this->astfri_location.expr_;
    TraverseStmt(DS->getBody());
    new_do_while->body_ = (CompoundStmt*)this->astfri_location.stmt_;

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseSwitchStmt(clang::SwitchStmt* SS)
{
    // akcia na tomto vrchole
    auto new_switch = this->stmt_factory_->mk_switch(nullptr, std::vector<CaseBaseStmt*>{});
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_switch);

    // zapamatanie si AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // prepisanie AST location
    this->astfri_location.stmt_ = new_switch;
    this->clang_location.stmt_  = SS;

    // Nastavenie podmienky
    TraverseStmt(SS->getCond());
    new_switch->expr_ = this->astfri_location.expr_;

    // naplnenie new_switch
    // akcia na kazdom case
    for (auto sw_case = SS->getSwitchCaseList(); sw_case; sw_case = sw_case->getNextSwitchCase())
    {
        // llvm::outs() << "Sw case: " << sw_case->getStmtClassName() << "\n";
        // ak je to case stmt a nie default
        if (auto case_stmt = llvm::dyn_cast<clang::CaseStmt>(sw_case))
        {
            auto new_case = this->stmt_factory_->mk_case(std::vector<Expr*>{}, nullptr);
            // naplnenie case-u
            auto case_cond = case_stmt->getLHS(); //->IgnoreCasts()
            TraverseStmt(case_cond);
            new_case->exprs_.push_back(this->astfri_location.expr_);

            auto case_body = case_stmt->getSubStmt();
            // ak je v compounde
            if (auto CS_body = llvm::dyn_cast<clang::CompoundStmt>(case_body))
            {
                TraverseStmt(CS_body);
                new_case->body_ = this->astfri_location.stmt_;
            }
            else
            {
                // nieje compound, iba jeden prikaz (dam do compoundu a potom vyberiem)
                auto temp_compund = this->stmt_factory_->mk_compound(std::vector<Stmt*>{});
                this->astfri_location.stmt_ = temp_compund;
                TraverseStmt(case_body);
                if (! ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.empty())
                    new_case->body_ = temp_compund->stmts_[0];
            }
            new_switch->cases_.push_back(new_case);
        }
        else if (auto default_stmt = llvm::dyn_cast<clang::DefaultStmt>(sw_case))
        {
            // ak je to default
            auto new_default = this->stmt_factory_->mk_default_case(nullptr);
            // naplnenie default-u
            auto default_body = default_stmt->getSubStmt();
            // ak je v compounde
            if (auto CS_body = llvm::dyn_cast<clang::CompoundStmt>(default_body))
            {
                TraverseStmt(CS_body);
                new_default->body_ = this->astfri_location.stmt_;
            }
            else
            {
                // nieje compound, iba jeden prikaz (dam do compoundu a potom vyberiem)
                auto temp_compund = this->stmt_factory_->mk_compound(std::vector<Stmt*>{});
                this->astfri_location.stmt_ = temp_compund;
                TraverseStmt(default_body);
                if (! ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.empty())
                    new_default->body_ = temp_compund->stmts_[0];
            }
            new_switch->cases_.push_back(new_default);
        }
    }

    // vratenie AST location
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    return true;
}

bool ClangVisitor::TraverseContinueStmt([[maybe_unused]] clang::ContinueStmt* CS)
{
    // akcia na tomto vrchole
    auto new_continue = this->stmt_factory_->mk_continue();
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_continue);

    return true;
}

bool ClangVisitor::TraverseBreakStmt([[maybe_unused]] clang::BreakStmt* BS)
{
    // akcia na tomto vrchole
    auto new_break = this->stmt_factory_->mk_break();
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_break);

    return true;
}
}