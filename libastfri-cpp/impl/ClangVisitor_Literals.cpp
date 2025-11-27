#include <libastfri-cpp/inc/ClangVisitor.hpp>

namespace astfri::astfri_cpp
{
bool ClangVisitor::TraverseIntegerLiteral(clang::IntegerLiteral* IL)
{
    // akcia na tomto vrchole
    auto int_literal = this->expr_factory_->mk_int_literal(IL->getValue().getSExtValue());

    // vytvorenie AST location
    this->astfri_location.expr_   = int_literal;
    this->clang_location.int_lit_ = IL;

    return true;
}

bool ClangVisitor::TraverseFloatingLiteral(clang::FloatingLiteral* FL)
{
    // akcia na tomto vrchole
    auto float_literal = this->expr_factory_->mk_float_literal(FL->getValue().convertToFloat());

    // vytvorenie AST location
    this->astfri_location.expr_     = float_literal;
    this->clang_location.float_lit_ = FL;

    return true;
}

bool ClangVisitor::TraverseStringLiteral(clang::StringLiteral* SL)
{
    // akcia na tomto vrchole
    auto string_literal = this->expr_factory_->mk_string_literal(SL->getString().str());

    // vytvorenie AST location
    this->astfri_location.expr_      = string_literal;
    this->clang_location.string_lit_ = SL;

    return true;
}

bool ClangVisitor::TraverseCXXBoolLiteralExpr(clang::CXXBoolLiteralExpr* BL)
{
    // akcia na tomto vrchole
    auto bool_literal = this->expr_factory_->mk_bool_literal(BL->getValue());

    // vytvorenie AST location
    this->astfri_location.expr_    = bool_literal;
    this->clang_location.bool_lit_ = BL;

    return true;
}

bool ClangVisitor::TraverseCharacterLiteral(clang::CharacterLiteral* CL)
{
    auto new_char                  = this->expr_factory_->mk_char_literal((char)CL->getValue());
    this->astfri_location.expr_    = new_char;
    this->clang_location.char_lit_ = CL;

    return true;
}

bool ClangVisitor::TraverseCXXNullPtrLiteralExpr(clang::CXXNullPtrLiteralExpr* NPLE)
{
    auto new_null                     = this->expr_factory_->mk_null_literal();
    this->astfri_location.expr_       = new_null;
    this->clang_location.nullptr_lit_ = NPLE;

    return true;
}
}