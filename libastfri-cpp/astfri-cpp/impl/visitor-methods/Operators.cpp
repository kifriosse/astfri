#include <astfri-cpp/impl/visitor-methods/ClangVisitor.hpp>


namespace astfri::cpp {


bool ClangVisitor::TraverseCompoundAssignOperator(clang::CompoundAssignOperator* CAO) {
    BinOpExpr* bin_op = nullptr;

    bin_op            = this->m_expr_factory
                 ->mk_bin_on(nullptr, this->get_astfri_bin_op_type(CAO->getOpcode()), nullptr);

    // prepisanie AST location
    this->m_astfri_location.expr  = bin_op;
    this->m_clang_location.bin_op = CAO;

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // ak je expr_as_stmt flag true, nech sa pre ostatne vypne
    bool expr_as_stmt_changed = false;
    if (this->m_expr_as_stmt) {
        this->m_expr_as_stmt   = false;
        expr_as_stmt_changed = true;
    }

    // lavy operator
    TraverseStmt(CAO->getLHS());
    bin_op->left = this->m_astfri_location.expr;
    // pravy operator
    TraverseStmt(CAO->getRHS());
    bin_op->right = this->m_astfri_location.expr;

    // vratenie expr_as_stmt naspat ak je toto node ktory ho zmenil
    if (expr_as_stmt_changed) {
        this->m_expr_as_stmt = true;
    }

    // vratenie tohto bin op naspat
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    if (this->m_expr_as_stmt) {
        ((CompoundStmt*)this->m_astfri_location.stmt)
            ->stmts.push_back(this->m_stmt_factory->mk_expr(bin_op));
    }

    return true;
}

bool ClangVisitor::TraverseBinaryOperator(clang::BinaryOperator* BO) {
    BinOpExpr* bin_op = nullptr;

    bin_op            = this->m_expr_factory
                 ->mk_bin_on(nullptr, this->get_astfri_bin_op_type(BO->getOpcode()), nullptr);

    // prepisanie AST location
    this->m_astfri_location.expr  = bin_op;
    this->m_clang_location.bin_op = BO;

    // zapamatanie AST location
    AstfriASTLocation astfri_temp = this->m_astfri_location;
    ClangASTLocation clang_temp   = this->m_clang_location;

    // ak je expr_as_stmt flag true, nech sa pre ostatne vypne
    bool expr_as_stmt_changed = false;
    if (this->m_expr_as_stmt) {
        this->m_expr_as_stmt   = false;
        expr_as_stmt_changed = true;
    }

    // lavy operator
    TraverseStmt(BO->getLHS());
    bin_op->left = this->m_astfri_location.expr;
    // pravy operator
    TraverseStmt(BO->getRHS());
    bin_op->right = this->m_astfri_location.expr;

    // vratenie expr_as_stmt naspat ak je toto node ktory ho zmenil
    if (expr_as_stmt_changed) {
        this->m_expr_as_stmt = true;
    }

    // vratenie tohto bin op naspat
    this->m_astfri_location = astfri_temp;
    this->m_clang_location  = clang_temp;

    if (this->m_expr_as_stmt) {
        ((CompoundStmt*)this->m_astfri_location.stmt)
            ->stmts.push_back(this->m_stmt_factory->mk_expr(bin_op));
    }

    return true;
}

bool ClangVisitor::TraverseUnaryOperator(clang::UnaryOperator* UO) {
    // akcia na tomto vrchole
    UnaryOpType op = this->get_astfri_un_op_type(UO->getOpcode());

    // nastavenie operandu
    TraverseStmt(UO->getSubExpr());
    Expr* arg = this->m_astfri_location.expr;

    // vytvorenie unary operator vrchol
    UnaryOpExpr* un_op = this->m_expr_factory->mk_unary_op(op, arg);

    // ak je expression nad compound stmt, tak sa vytvori ako stmt a nie expr a rovno sa tam vlozi
    if (this->m_expr_as_stmt) {
        ((CompoundStmt*)this->m_astfri_location.stmt)
            ->stmts.push_back(this->m_stmt_factory->mk_expr(un_op));
    }

    this->m_astfri_location.expr = un_op;
    this->m_clang_location.un_op = UO;

    return true;
}


} // namespace astfri::cpp
