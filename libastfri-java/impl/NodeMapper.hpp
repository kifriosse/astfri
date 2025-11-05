#ifndef NODE_MAPPER_CLASS_HPP
#define NODE_MAPPER_CLASS_HPP

#include <libastfri/inc/Astfri.hpp>

#include <map>

namespace astfri::java
{
class NodeMapper
{
private:
    astfri::TypeFactory& typeFactory;
    astfri::StmtFactory& stmtFactory;
    astfri::ExprFactory& exprFactory;

    std::map<std::string, astfri::Type*> const typeMap;
    std::map<std::string, astfri::BinOpType> const binOpMap;
    std::map<std::string, astfri::UnaryOpType> const unaryOpMap;
    std::map<std::string, astfri::AccessModifier> const modMap;

public:
    NodeMapper();
    std::map<std::string, astfri::Type*> get_typeMap();
    std::map<std::string, astfri::BinOpType> get_binOpMap();
    std::map<std::string, astfri::UnaryOpType> get_unaryOpMap();
    std::map<std::string, astfri::AccessModifier> get_modMap();
};
} // namespace astfri::java
#endif // NODE_MAPPER_CLASS_HPP