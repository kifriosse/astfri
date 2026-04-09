#ifndef ASTFRI_IMPL_AST_NODE_HPP
#    include <astfri/impl/ASTNode.hpp>
#endif


namespace astfri {


template<typename KindType>
void ASTNode<KindType>::accept(Visitor &visitor) {
    m_accept_ptr(this, visitor);
}


} // namespace astfri
