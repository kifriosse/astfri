#include <astfri-ted/impl/NodeComparator.hpp>
#include <stdexcept>

namespace astfri::ted {


NodeComparator::NodeComparator(const AttributeConfig& config) : m_config(config) {}

bool NodeComparator::equal(const NodeReference& a, const NodeReference& b) const {
    (void)a;
    (void)b;
    throw std::logic_error("equal: not implemented yet!");
}


bool NodeComparator::equal_expr(astfri::Expr* a, astfri::Expr* b) const {
    (void)a;
    (void)b;
    throw std::logic_error("equal_expr: not implemented yet!");

}
bool NodeComparator::equal_stmt(astfri::Stmt* a, astfri::Stmt* b) const {
    (void)a;
    (void)b;
    throw std::logic_error("equal_stmt: not implemented yet!");

}
bool NodeComparator::equal_type(astfri::Type* a, astfri::Type* b) const {
    (void)a;
    (void)b;
    throw std::logic_error("equal_type: not implemented yet!");

}

} // namespace astfri::ted
