#include <libastfri-cpp/inc/ClangVisitor.hpp>
#include "libastfri/inc/Expr.hpp"

namespace astfri::astfri_cpp
{
// visit expression
bool ClangVisitor::TraverseCXXConstructExpr(clang::CXXConstructExpr* Ctor)
{
    // akcia na tomto vrchole
    auto new_ctor_expr   = this->expr_factory_->mk_constructor_call(nullptr, std::vector<Expr*>{});

    // treba nastaviť typ
    // TODO: je to momentálne len pre ClassDefStmt, mne sa neda použiť is_a (dyn_cast), tak bude treba niećo vymyslieť
    new_ctor_expr->type_ = ((astfri::ClassDefStmt*)this->astfri_location.stmt_)->type_;

    for (auto arg : Ctor->arguments())
    {
        TraverseStmt(arg);
        new_ctor_expr->args_.push_back(this->astfri_location.expr_);
    }

    this->astfri_location.expr_ = new_ctor_expr;
    this->clang_location.expr_  = Ctor;

    return true;
}

bool ClangVisitor::TraverseDeclRefExpr(clang::DeclRefExpr* DRE)
{
    // akcia na tomto vrchole
    Expr* new_ref_expr = nullptr;
    if (auto PVD = llvm::dyn_cast<clang::ParmVarDecl>(DRE->getDecl()))
    {
        // pre parameter
        new_ref_expr = this->expr_factory_->mk_param_var_ref(PVD->getNameAsString());
    }
    else if (auto VD = llvm::dyn_cast<clang::VarDecl>(DRE->getDecl()))
    {
        // pre lokalnu premennu
        new_ref_expr = this->expr_factory_->mk_local_var_ref(VD->getNameAsString());
    }
    else if (auto FD = llvm::dyn_cast<clang::FunctionDecl>(DRE->getDecl()))
    {
        // pre funkciu
        new_ref_expr
            = this->expr_factory_->mk_function_call(FD->getNameAsString(), std::vector<Expr*>{});
    }

    this->astfri_location.expr_ = new_ref_expr;
    this->clang_location.expr_  = DRE;

    return true;
}

bool ClangVisitor::TraverseLambdaExpr(clang::LambdaExpr* LBD)
{
    // akcia na tomto vrchole
    // získam si potrebné veci na lambda uzol
    std::vector<ParamVarDefStmt*> params {};
    astfri::Stmt* body;

    // získanie parametrov
    for (auto parameter : (LBD->getCallOperator()->parameters())) {
        TraverseDecl(parameter);
        params.push_back((astfri::ParamVarDefStmt*)astfri_location.stmt_);
    }
    
    // získanie tela lambdy
    TraverseStmt(LBD->getBody());
    body = this->astfri_location.stmt_;

    // vytvorenie uzla
    std::string name = LBD->getLambdaClass()->getQualifiedNameAsString();
    LambdaExpr* lambdaExpr = this->expr_factory_->mk_lambda_expr(params, body, name);
    
    // nastavenie location
    this->astfri_location.expr_ = lambdaExpr;
    this->clang_location.expr_ = LBD;
    
    return true;
}

bool ClangVisitor::TraverseMemberExpr(clang::MemberExpr* ME)
{
    // akcia na tomto vrchole
    TraverseStmt(ME->getBase());
    auto new_mem_expr = this->expr_factory_->mk_member_var_ref(
        this->astfri_location.expr_,
        ME->getMemberNameInfo().getAsString().c_str()
    );
    this->astfri_location.expr_ = new_mem_expr;
    this->clang_location.expr_  = ME;

    return true;
}

bool ClangVisitor::TraverseCallExpr(clang::CallExpr* CE)
{
    // co sa ma stat ak je zavolana metoda triedy
    if (auto metoda
        = llvm::dyn_cast<clang::CXXDependentScopeMemberExpr>(CE->getCallee()->IgnoreImpCasts()))
    {
        TraverseStmt(metoda->getBase());
        auto new_method_call = this->expr_factory_->mk_method_call(
            this->astfri_location.expr_,
            metoda->getMemberNameInfo().getAsString(),
            std::vector<Expr*>{}
        );
        for (auto arg : CE->arguments())
        {
            TraverseStmt(arg);
            new_method_call->args_.push_back(this->astfri_location.expr_);
        }

        this->astfri_location.expr_ = new_method_call;
    }

    // co sa ma stat ak je zavolana funkcia
    if (auto funkcia = llvm::dyn_cast<clang::DeclRefExpr>(CE->getCallee()->IgnoreImpCasts()))
    {
        TraverseStmt(funkcia);
        FunctionCallExpr* fun = (FunctionCallExpr*)this->astfri_location.expr_;
        for (auto arg : CE->arguments())
        {
            TraverseStmt(arg);
            fun->args_.push_back(this->astfri_location.expr_);
        }
        this->astfri_location.expr_ = fun;
    }
    return true;
}

bool ClangVisitor::TraverseCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr* DSME)
{
    // akcia na tomto vrchole
    TraverseStmt(DSME->getBase());
    auto new_mem_expr = this->expr_factory_->mk_member_var_ref(
        this->astfri_location.expr_,
        DSME->getMemberNameInfo().getAsString().c_str()
    );
    this->astfri_location.expr_ = new_mem_expr;
    this->clang_location.expr_  = DSME;

    return true;
}

bool ClangVisitor::TraverseCXXThisExpr(clang::CXXThisExpr* TE)
{
    this->astfri_location.expr_ = this->expr_factory_->mk_this();
    this->clang_location.expr_  = TE;

    return true;
}

bool ClangVisitor::TraverseCXXMemberCallExpr(clang::CXXMemberCallExpr* MCE)
{
    // akcia na tomto vrchole
    std::vector<Expr*> args;
    for (auto arg : MCE->arguments())
    {
        if (arg->isDefaultArgument())
        {
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
    this->clang_location.expr_  = MCE;

    return true;
}

bool ClangVisitor::TraverseCXXNewExpr(clang::CXXNewExpr* NE)
{
    // akcia na tomto vrchole
    NewExpr* new_new;
    // ak je alokovany typ builtin typ
    if (NE->getAllocatedType().getTypePtr()->isBuiltinType())
    {
        // vytvori sa newExpr a potom sa priradia argumenty construktora
        new_new = this->expr_factory_->mk_new(this->expr_factory_->mk_constructor_call(
            this->get_astfri_type(NE->getAllocatedType()),
            std::vector<Expr*>{}
        ));
        // TODO: dorobit aby sa argumenty priradovali pri builtin new>  int* cislo = new int(5);
    }
    else
    {
        TraverseCXXConstructExpr((clang::CXXConstructExpr*)NE->getConstructExpr());
        new_new = this->expr_factory_->mk_new((ConstructorCallExpr*)this->astfri_location.expr_);
    }
    this->astfri_location.expr_ = new_new;
    this->clang_location.expr_  = NE;

    return true;
}

bool ClangVisitor::TraverseCXXDeleteExpr(clang::CXXDeleteExpr* DE)
{
    // akcia na tomto vrchole
    TraverseStmt(DE->getArgument());
    auto new_delete             = this->expr_factory_->mk_delete(this->astfri_location.expr_);

    this->astfri_location.expr_ = new_delete;
    this->clang_location.expr_  = DE;

    return true;
}

bool ClangVisitor::TraverseCXXThrowExpr(clang::CXXThrowExpr* TE)
{
    // akcia na tomto vrchole
    TraverseStmt(TE->getSubExpr());
    auto new_throw = this->stmt_factory_->mk_throw(this->astfri_location.expr_);
    ((CompoundStmt*)this->astfri_location.stmt_)->stmts_.push_back(new_throw);

    return true;
}

bool ClangVisitor::TraverseCXXOperatorCallExpr(clang::CXXOperatorCallExpr *COCE) {
    // co sa ma stat ak sa zavola lambda
     if (auto record = llvm::dyn_cast<clang::CXXRecordDecl>(COCE->getDirectCallee()->getParent())) {
        if (record && record->isLambda()) {
            std::string name = record->getQualifiedNameAsString();
            Expr* lambda = this->expr_factory_->get_lambda_expr(name);
            std::vector<Expr*> args = {};
            // z nejakého dôvodu sa ako prvý argument berie samotná inštancia lambdy
            // takýto foreach jednoducho preskočí prvý element
            for (auto *arg : llvm::drop_begin(COCE->arguments(), 1)) {
               TraverseStmt(arg);
               args.push_back(this->astfri_location.expr_);
            }
            LambdaCallExpr* lambdaCall = this->expr_factory_->mk_lambda_call(lambda, args);
            this->astfri_location.expr_ = lambdaCall;
        }
    }
    return true;
}
}