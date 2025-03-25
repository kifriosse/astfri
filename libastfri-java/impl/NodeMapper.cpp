#include "NodeMapper.hpp"

NodeMapper::NodeMapper() : typeFactory(astfri::TypeFactory::get_instance()), 
                           typeMap({
                            {"int", typeFactory.mk_int()},
                            {"float", typeFactory.mk_float()},
                            {"double", typeFactory.mk_float()},
                            {"char", typeFactory.mk_char()},
                            {"boolean", typeFactory.mk_bool()},
                            {"void", typeFactory.mk_void()},
                         }), 
                           stmtFactory(astfri::StmtFactory::get_instance()), 
                           exprFactory(astfri::ExprFactory::get_instance()),
                           binOpMap({
                               {"=", astfri::BinOpType::Assign},
                               {"+", astfri::BinOpType::Add},
                               {"-", astfri::BinOpType::Subtract},
                               {"*", astfri::BinOpType::Multiply},
                               {"/", astfri::BinOpType::Divide},
                               {"%", astfri::BinOpType::Modulo},
                               {"**", astfri::BinOpType::Exponentiation},
                               {"==", astfri::BinOpType::Equal},
                               {"!=", astfri::BinOpType::NotEqual},
                               {"<", astfri::BinOpType::Less},
                               {"<=", astfri::BinOpType::LessEqual},
                               {">", astfri::BinOpType::Greater},
                               {">=", astfri::BinOpType::GreaterEqual},
                               {"&&", astfri::BinOpType::LogicalAnd},
                               {"||", astfri::BinOpType::LogicalOr},
                               {">>", astfri::BinOpType::BitShiftRight},
                               {"<<", astfri::BinOpType::BitShiftLeft},
                               {"&", astfri::BinOpType::BitAnd},
                               {"|", astfri::BinOpType::BitOr},
                               {"^", astfri::BinOpType::BitXor},
                               {",", astfri::BinOpType::Comma},
                               {"+=", astfri::BinOpType::AddAssign},
                               {"-=", astfri::BinOpType::SubtractAssign},
                               {"*=", astfri::BinOpType::MultiplyAssign},
                               {"/=", astfri::BinOpType::DivideAssign},
                               {"%=", astfri::BinOpType::ModuloAssign},
                               {"**=", astfri::BinOpType::ExponentiationAssign},
                            }),
                            unaryOpMap({
                                {"!", astfri::UnaryOpType::LogicalNot},
                                {"-", astfri::UnaryOpType::Minus},
                                {"+", astfri::UnaryOpType::Plus},
                                {"++", astfri::UnaryOpType::PreIncrement},
                                {"++", astfri::UnaryOpType::PostIncrement},
                                {"--", astfri::UnaryOpType::PreDecrement},
                                {"--", astfri::UnaryOpType::PostDecrement},
                                {"~", astfri::UnaryOpType::BitFlip},
                            }),
                            modMap({
                                {"public", astfri::AccessModifier::Public},
                                {"private", astfri::AccessModifier::Private},
                                {"protected", astfri::AccessModifier::Protected},
                                {"internal", astfri::AccessModifier::Internal},
                            })
{    
}

std::map<std::string, astfri::Type*> NodeMapper::get_typeMap() {
    return this->typeMap;
}

std::map<std::string, astfri::BinOpType> NodeMapper::get_binOpMap() {
    return this->binOpMap;
}

std::map<std::string, astfri::UnaryOpType> NodeMapper::get_unaryOpMap() {
    return this->unaryOpMap;
}

std::map<std::string, astfri::AccessModifier> NodeMapper::get_modMap() {
    return this->modMap;
}