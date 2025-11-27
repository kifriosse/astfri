#include <libastfri-cpp/inc/ClangVisitor.hpp>

namespace astfri::astfri_cpp
{
astfri::BinOpType ClangVisitor::get_astfri_bin_op_type(clang::BinaryOperatorKind clang_type)
{
    // TODO:
    // ඞ Fakt toto existuje?
    // {lhs} // {rhs}, {lhs} div {rhs}
    // FloorDivide,

    // {lhs} ** {rhs}
    // Exponentiation,

    // {lhs} //= {rhs}
    // FloorDivideAssign,

    // {lhs} **= {rhs}
    // ExponentiationAssign,

    // sú tam aj ďalšie srandy, ale to nepodporuje ASTFRI: .* ->*

    switch (clang_type)
    {
    case clang::BinaryOperatorKind::BO_Assign:
        return astfri::BinOpType::Assign;
        break; // =
    case clang::BinaryOperatorKind::BO_Add:
        return astfri::BinOpType::Add;
        break; // +
    case clang::BinaryOperatorKind::BO_Sub:
        return astfri::BinOpType::Subtract;
        break; // -
    case clang::BinaryOperatorKind::BO_Mul:
        return astfri::BinOpType::Multiply;
        break; // *
    case clang::BinaryOperatorKind::BO_Div:
        return astfri::BinOpType::Divide;
        break; // /
    case clang::BinaryOperatorKind::BO_AddAssign:
        return astfri::BinOpType::AddAssign;
        break; // +=
    case clang::BinaryOperatorKind::BO_SubAssign:
        return astfri::BinOpType::SubtractAssign;
        break; // -=
    case clang::BinaryOperatorKind::BO_MulAssign:
        return astfri::BinOpType::MultiplyAssign;
        break; // *=
    case clang::BinaryOperatorKind::BO_DivAssign:
        return astfri::BinOpType::DivideAssign;
        break; // /=
    case clang::BinaryOperatorKind::BO_GT:
        return astfri::BinOpType::Greater;
        break; // >
    case clang::BinaryOperatorKind::BO_GE:
        return astfri::BinOpType::GreaterEqual;
        break; // >=
    case clang::BinaryOperatorKind::BO_LT:
        return astfri::BinOpType::Less;
        break; // <
    case clang::BinaryOperatorKind::BO_LE:
        return astfri::BinOpType::LessEqual;
        break; // <=
    case clang::BinaryOperatorKind::BO_EQ:
        return astfri::BinOpType::Equal;
        break; // ==
    case clang::BinaryOperatorKind::BO_NE:
        return astfri::BinOpType::NotEqual;
        break; // !=
    case clang::BinaryOperatorKind::BO_LAnd:
        return astfri::BinOpType::LogicalAnd;
        break; // &&
    case clang::BinaryOperatorKind::BO_LOr:
        return astfri::BinOpType::LogicalOr;
        break; // ||
    case clang::BinaryOperatorKind::BO_Rem:
        return astfri::BinOpType::Modulo;
        break; // %
    case clang::BinaryOperatorKind::BO_RemAssign:
        return astfri::BinOpType::ModuloAssign;
        break; // %=
    case clang::BinaryOperatorKind::BO_Shr:
        return astfri::BinOpType::BitShiftRight;
        break; // >>
    case clang::BinaryOperatorKind::BO_Shl:
        return astfri::BinOpType::BitShiftLeft;
        break; // <<
    case clang::BinaryOperatorKind::BO_ShrAssign:
        return astfri::BinOpType::BitShiftRightAssign;
        break; // >>=
    case clang::BinaryOperatorKind::BO_ShlAssign:
        return astfri::BinOpType::BitShiftLeftAssign;
        break; // <<=
    case clang::BinaryOperatorKind::BO_And:
        return astfri::BinOpType::BitAnd;
        break; // &
    case clang::BinaryOperatorKind::BO_AndAssign:
        return astfri::BinOpType::BitAndAssign;
        break; // &=
    case clang::BinaryOperatorKind::BO_Or:
        return astfri::BinOpType::BitOr;
        break; // |
    case clang::BinaryOperatorKind::BO_OrAssign:
        return astfri::BinOpType::BitOrAssign;
        break; // |=
    case clang::BinaryOperatorKind::BO_Xor:
        return astfri::BinOpType::BitXor;
        break; // ^
    case clang::BinaryOperatorKind::BO_XorAssign:
        return astfri::BinOpType::BitXorAssign;
        break; // ^=
    case clang::BinaryOperatorKind::BO_Comma:
        return astfri::BinOpType::Comma;
        break; // ,
    default:
    {
        std::cerr << "\n\n\nErrorin finding right Binary operator: astfri_cpp project.\n\n\n";
    }
    break;
    }
    return BinOpType::Assign;
}

astfri::AccessModifier ClangVisitor::getAccessModifier(clang::Decl* decl)
{
    switch (decl->getAccess())
    {
    case clang::AccessSpecifier::AS_public:
        return astfri::AccessModifier::Public;
        break;
    case clang::AccessSpecifier::AS_protected:
        return astfri::AccessModifier::Protected;
        break;
    case clang::AccessSpecifier::AS_private:
        return astfri::AccessModifier::Private;
        break;
    default:
        return astfri::AccessModifier::Public;
        break;
    }
}

astfri::ClassDefStmt* ClangVisitor::get_existing_class(std::string name)
{
    for (auto cls : this->tu_->classes_)
    {
        if (cls->type_->name_.compare(name) == 0)
        {
            return cls;
        }
    }
    return nullptr;
}

astfri::UnaryOpType ClangVisitor::get_astfri_un_op_type(clang::UnaryOperatorKind clang_type)
{
    switch (clang_type)
    {
    case clang::UnaryOperatorKind::UO_Plus:
        return UnaryOpType::Plus;
        break;
    case clang::UnaryOperatorKind::UO_Minus:
        return UnaryOpType::Minus;
        break;
    case clang::UnaryOperatorKind::UO_LNot:
        return UnaryOpType::LogicalNot;
        break;
    case clang::UnaryOperatorKind::UO_PreInc:
        return UnaryOpType::PreIncrement;
        break;
    case clang::UnaryOperatorKind::UO_PostInc:
        return UnaryOpType::PostIncrement;
        break;
    case clang::UnaryOperatorKind::UO_PreDec:
        return UnaryOpType::PreDecrement;
        break;
    case clang::UnaryOperatorKind::UO_PostDec:
        return UnaryOpType::PostDecrement;
        break;
    case clang::UnaryOperatorKind::UO_AddrOf:
        return UnaryOpType::AddressOf;
        break;
    case clang::UnaryOperatorKind::UO_Deref:
        return UnaryOpType::Dereference;
        break;
    case clang::UnaryOperatorKind::UO_Not:
        return UnaryOpType::LogicalNot;
        break;
    default:
    {
        std::cerr << "\n\n\nError in finding right Unary operator: astfri_cpp project.\n\n\n";
    }
    break;
    }
    return UnaryOpType::Plus;
}
}

