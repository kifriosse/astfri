#include <astfri-java/impl/NodeMapper.hpp>

#include <astfri/impl/ExprDef.hpp>
#include <astfri/impl/StmtDef.hpp>
#include <astfri/impl/TypeFactory.hpp>


namespace astfri::java {


NodeMapper::NodeMapper(
    astfri::TypeFactory *typeFactory
) :
    m_typeFactory(typeFactory),
    m_typeMap({
        {"byte",    m_typeFactory->mk_int()  },
        {"short",   m_typeFactory->mk_int()  },
        {"int",     m_typeFactory->mk_int()  },
        {"long",    m_typeFactory->mk_int()  },
        {"float",   m_typeFactory->mk_float()},
        {"double",  m_typeFactory->mk_float()},
        {"char",    m_typeFactory->mk_char() },
        {"boolean", m_typeFactory->mk_bool() },
        {"void",    m_typeFactory->mk_void() },}),
    m_binOpMap({
        {"=",   astfri::BinOpType::Assign},
        {"+",   astfri::BinOpType::Add},
        {"-",   astfri::BinOpType::Subtract},
        {"*",   astfri::BinOpType::Multiply},
        {"/",   astfri::BinOpType::Divide},
        {"%",   astfri::BinOpType::Modulo},
        {"**",  astfri::BinOpType::Exponentiation},
        {"==",  astfri::BinOpType::Equal},
        {"!=",  astfri::BinOpType::NotEqual},
        {"<",   astfri::BinOpType::Less},
        {"<=",  astfri::BinOpType::LessEqual},
        {">",   astfri::BinOpType::Greater},
        {">=",  astfri::BinOpType::GreaterEqual},
        {"&&",  astfri::BinOpType::LogicalAnd},
        {"||",  astfri::BinOpType::LogicalOr},
        {">>",  astfri::BinOpType::BitShiftRight},
        {"<<",  astfri::BinOpType::BitShiftLeft},
        {"&",   astfri::BinOpType::BitAnd},
        {"|",   astfri::BinOpType::BitOr},
        {"^",   astfri::BinOpType::BitXor},
        {",",   astfri::BinOpType::Comma},
        {"+=",  astfri::BinOpType::AddAssign},
        {"-=",  astfri::BinOpType::SubtractAssign},
        {"*=",  astfri::BinOpType::MultiplyAssign},
        {"/=",  astfri::BinOpType::DivideAssign},
        {"%=",  astfri::BinOpType::ModuloAssign},
        {"**=", astfri::BinOpType::ExponentiationAssign},}),
    m_unaryOpMap({
        {"!",  astfri::UnaryOpType::LogicalNot},
        {"-",  astfri::UnaryOpType::Minus},
        {"+",  astfri::UnaryOpType::Plus},
        {"++", astfri::UnaryOpType::PreIncrement},
        {"++", astfri::UnaryOpType::PostIncrement},
        {"--", astfri::UnaryOpType::PreDecrement},
        {"--", astfri::UnaryOpType::PostDecrement},
        {"~",  astfri::UnaryOpType::BitFlip},}),
    m_modMap({
        {"public",    astfri::AccessModifier::Public},
        {"private",   astfri::AccessModifier::Private},
        {"protected", astfri::AccessModifier::Protected},
        {"internal",  astfri::AccessModifier::Internal},})
{
}

std::optional<astfri::Type*> NodeMapper::map_type(std::string_view key) const {
    const auto it = m_typeMap.find(key);
    if (it == m_typeMap.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::optional<astfri::BinOpType> NodeMapper::map_binary_op(std::string_view key) const {
    const auto it = m_binOpMap.find(key);
    if (it == m_binOpMap.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::optional<astfri::UnaryOpType> NodeMapper::map_unary_op(std::string_view key) const {
    const auto it = m_unaryOpMap.find(key);
    if (it == m_unaryOpMap.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::optional<astfri::AccessModifier> NodeMapper::map_access_mod(std::string_view key) const {
    const auto it = m_modMap.find(key);
    if (it == m_modMap.end()) {
        return std::nullopt;
    }
    return it->second;
}


} // namespace astfri::java
