#include <astfri-cpp/impl/visitor-methods/ClangVisitor.hpp>


namespace astfri::cpp {


bool ClangVisitor::TraverseIntegerLiteral(clang::IntegerLiteral* IL) {
    // akcia na tomto vrchole
    auto int_literal = this->m_expr_factory->mk_int_literal(IL->getValue().getSExtValue());

    // vytvorenie AST location
    this->m_astfri_location.expr   = int_literal;
    this->m_clang_location.int_lit = IL;

    return true;
}

bool ClangVisitor::TraverseFloatingLiteral(clang::FloatingLiteral* FL) {
    // akcia na tomto vrchole
    auto float_literal = this->m_expr_factory->mk_float_literal(FL->getValue().convertToFloat());

    // vytvorenie AST location
    this->m_astfri_location.expr     = float_literal;
    this->m_clang_location.float_lit = FL;

    return true;
}

bool ClangVisitor::TraverseStringLiteral(clang::StringLiteral* SL) {
    // akcia na tomto vrchole
    auto string_literal = this->m_expr_factory->mk_string_literal(SL->getString().str());

    // vytvorenie AST location
    this->m_astfri_location.expr      = string_literal;
    this->m_clang_location.string_lit = SL;

    return true;
}

bool ClangVisitor::TraverseCXXBoolLiteralExpr(clang::CXXBoolLiteralExpr* BL) {
    // akcia na tomto vrchole
    auto bool_literal = this->m_expr_factory->mk_bool_literal(BL->getValue());

    // vytvorenie AST location
    this->m_astfri_location.expr    = bool_literal;
    this->m_clang_location.bool_lit = BL;

    return true;
}

bool ClangVisitor::TraverseCharacterLiteral(clang::CharacterLiteral* CL) {
    auto new_char                  = this->m_expr_factory->mk_char_literal((char)CL->getValue());
    this->m_astfri_location.expr    = new_char;
    this->m_clang_location.char_lit = CL;

    return true;
}

bool ClangVisitor::TraverseCXXNullPtrLiteralExpr(clang::CXXNullPtrLiteralExpr* NPLE) {
    auto new_null                     = this->m_expr_factory->mk_null_literal();
    this->m_astfri_location.expr       = new_null;
    this->m_clang_location.nullptr_lit = NPLE;

    return true;
}


} // namespace astfri::cpp
