#include <libastfri-cpp/inc/ClangVisitor.hpp>

namespace astfri::astfri_cpp
{
bool ClangVisitor::TraverseCompoundAssignOperator(clang::CompoundAssignOperator* CAO)
{
    BinOpExpr* bin_op = nullptr;

    bin_op            = this->expr_factory_
                 ->mk_bin_on(nullptr, this->get_astfri_bin_op_type(CAO->getOpcode()), nullptr);

    // prepisanie AST location
    this->astfri_location.expr_  = bin_op;
    this->clang_location.bin_op_ = CAO;

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // ak je expr_as_stmt flag true, nech sa pre ostatne vypne
    bool expr_as_stmt_changed = false;
    if (this->expr_as_stmt)
    {
        this->expr_as_stmt   = false;
        expr_as_stmt_changed = true;
    }

    // lavy operator
    TraverseStmt(CAO->getLHS());
    bin_op->left_ = this->astfri_location.expr_;
    // pravy operator
    TraverseStmt(CAO->getRHS());
    bin_op->right_ = this->astfri_location.expr_;

    // vratenie expr_as_stmt naspat ak je toto node ktory ho zmenil
    if (expr_as_stmt_changed)
    {
        this->expr_as_stmt = true;
    }

    // vratenie tohto bin op naspat
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    if (this->expr_as_stmt)
    {
        ((CompoundStmt*)this->astfri_location.stmt_)
            ->stmts_.push_back(this->stmt_factory_->mk_expr(bin_op));
    }

    return true;
}

bool ClangVisitor::TraverseBinaryOperator(clang::BinaryOperator* BO)
{
    BinOpExpr* bin_op = nullptr;

    bin_op            = this->expr_factory_
                 ->mk_bin_on(nullptr, this->get_astfri_bin_op_type(BO->getOpcode()), nullptr);

    // prepisanie AST location
    this->astfri_location.expr_  = bin_op;
    this->clang_location.bin_op_ = BO;

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->astfri_location;
    ClangASTLocation clang_temp   = this->clang_location;

    // ak je expr_as_stmt flag true, nech sa pre ostatne vypne
    bool expr_as_stmt_changed = false;
    if (this->expr_as_stmt)
    {
        this->expr_as_stmt   = false;
        expr_as_stmt_changed = true;
    }

    // lavy operator
    TraverseStmt(BO->getLHS());
    bin_op->left_ = this->astfri_location.expr_;
    // pravy operator
    TraverseStmt(BO->getRHS());
    bin_op->right_ = this->astfri_location.expr_;

    // vratenie expr_as_stmt naspat ak je toto node ktory ho zmenil
    if (expr_as_stmt_changed)
    {
        this->expr_as_stmt = true;
    }

    // vratenie tohto bin op naspat
    this->astfri_location = astfri_temp;
    this->clang_location  = clang_temp;

    if (this->expr_as_stmt)
    {
        ((CompoundStmt*)this->astfri_location.stmt_)
            ->stmts_.push_back(this->stmt_factory_->mk_expr(bin_op));
    }

    return true;
}

bool ClangVisitor::TraverseUnaryOperator(clang::UnaryOperator* UO)
{
    // akcia na tomto vrchole
    UnaryOpType op = this->get_astfri_un_op_type(UO->getOpcode());

    // nastavenie operandu
    TraverseStmt(UO->getSubExpr());
    Expr* arg = this->astfri_location.expr_;

    // vytvorenie unary operator vrchol
    UnaryOpExpr* un_op = this->expr_factory_->mk_unary_op(op, arg);

    // ak je expression nad compound stmt, tak sa vytvori ako stmt a nie expr a rovno sa tam vlozi
    if (this->expr_as_stmt)
    {
        ((CompoundStmt*)this->astfri_location.stmt_)
            ->stmts_.push_back(this->stmt_factory_->mk_expr(un_op));
    }

    this->astfri_location.expr_ = un_op;
    this->clang_location.un_op_ = UO;

    return true;
}
}