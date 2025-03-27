#ifndef NODE_MAPPER_CLASS_HPP
#define NODE_MAPPER_CLASS_HPP

#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <map>

namespace astfri::java
{
    class NodeMapper
    {
    private:
        astfri::TypeFactory& typeFactory;
        const std::map<std::string, astfri::Type*> typeMap;

        astfri::StmtFactory& stmtFactory;
        astfri::ExprFactory& exprFactory;

        const std::map<std::string, astfri::BinOpType> binOpMap;
        const std::map<std::string, astfri::UnaryOpType> unaryOpMap;
        const std::map<std::string, astfri::AccessModifier> modMap;

    public:
        NodeMapper();
        std::map<std::string, astfri::Type*> get_typeMap ();
        std::map<std::string, astfri::BinOpType> get_binOpMap ();
        std::map<std::string, astfri::UnaryOpType> get_unaryOpMap ();
        std::map<std::string, astfri::AccessModifier> get_modMap ();
    };
} // namespace astfri::java
#endif // NODE_MAPPER_CLASS_HPP