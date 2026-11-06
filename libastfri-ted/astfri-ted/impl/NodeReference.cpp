#include <astfri-ted/impl/NodeReference.hpp>

namespace astfri::ted {

NodeReference::NodeReference(astfri::Expr* expr) : m_node(expr){}

NodeReference::NodeReference(astfri::Stmt* stmt) : m_node(stmt){}

NodeReference::NodeReference(astfri::Type* type) : m_node(type){}

bool NodeReference::same_family(NodeReference& other) const{
    return this->m_node.index() == other.m_node.index();
}

astfri::Expr* NodeReference::get_as_expr() const {
    if (std::holds_alternative<astfri::Expr*>(this->m_node))
        return std::get<astfri::Expr*>(this->m_node);
    return nullptr;
}

astfri::Stmt* NodeReference::get_as_stmt() const {
    if (std::holds_alternative<astfri::Stmt*>(this->m_node))
        return std::get<astfri::Stmt*>(this->m_node);
    return nullptr;
}

astfri::Type* NodeReference::get_as_type() const {
    if (std::holds_alternative<astfri::Type*>(this->m_node))
        return std::get<astfri::Type*>(this->m_node);
    return nullptr;
}

} // namespace astfri::ted
