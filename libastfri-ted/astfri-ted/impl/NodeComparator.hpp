#ifndef ASTFRI_TED_IMPL_TED_NODE_COMPARATOR_HPP
#define ASTFRI_TED_IMPL_TED_NODE_COMPARATOR_HPP

#include <astfri-ted/impl/AttributeConfig.hpp>
#include <astfri-ted/impl/NodeReference.hpp>

namespace astfri::ted {

class NodeComparator {
public:
    NodeComparator(const AttributeConfig& config);
    bool equal(const NodeReference& a, const NodeReference& b) const;

private:
    bool equal_expr(astfri::Expr* a, astfri::Expr* b) const;
    bool equal_stmt(astfri::Stmt* a, astfri::Stmt* b) const;
    bool equal_type(astfri::Type* a, astfri::Type* b) const;

private:
    const AttributeConfig m_config;
};

} // namespace astfri::ted

#endif
