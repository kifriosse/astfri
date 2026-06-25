#ifndef ASTFRI_JAVA_IMPL_NODE_MAPPER_HPP
#define ASTFRI_JAVA_IMPL_NODE_MAPPER_HPP

#include <astfri/impl/ExprFwd.hpp>
#include <astfri/impl/StmtFwd.hpp>
#include <astfri/impl/TypeFwd.hpp>

#include <map>
#include <optional>
#include <string_view>


namespace astfri::java {


class NodeMapper {
public:
    NodeMapper(astfri::TypeFactory *typeFactory);

    std::optional<astfri::Type*> map_type(std::string_view key) const;
    std::optional<astfri::BinOpType> map_binary_op(std::string_view key) const;
    std::optional<astfri::UnaryOpType> map_unary_op(std::string_view key) const;
    std::optional<astfri::AccessModifier> map_access_mod(std::string_view key) const;

private:
    astfri::TypeFactory *m_typeFactory;
    std::map<std::string, astfri::Type*, std::less<>> m_typeMap;
    std::map<std::string, astfri::BinOpType, std::less<>> m_binOpMap;
    std::map<std::string, astfri::UnaryOpType, std::less<>> m_unaryOpMap;
    std::map<std::string, astfri::AccessModifier, std::less<>> m_modMap;
};


} // namespace astfri::java

#endif // ASTFRI_JAVA_IMPL_NODE_MAPPER_HPP
