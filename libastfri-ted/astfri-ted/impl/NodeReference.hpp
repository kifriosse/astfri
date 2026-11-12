#ifndef ASTFRI_TED_IMPL_NODE_REFERENCE_HPP
#define ASTFRI_TED_IMPL_NODE_REFERENCE_HPP

#include <astfri/impl/ExprFwd.hpp>
#include <astfri/impl/StmtFwd.hpp>
#include <astfri/impl/TypeFwd.hpp>

#include <variant>

namespace astfri::ted {

class NodeReference {
public:
    NodeReference(astfri::Expr* expr);
    NodeReference(astfri::Stmt* stmt);
    NodeReference(astfri::Type* type);

    bool same_family(NodeReference& other) const;

    astfri::Expr* get_as_expr() const;
    astfri::Stmt* get_as_stmt() const;
    astfri::Type* get_as_type() const;

private:
    // union
    // https://en.cppreference.com/cpp/utility/variant
    std::variant< astfri::Expr*, astfri::Stmt*, astfri::Type*> m_node;
};

} // namespace astfri::ted

#endif
