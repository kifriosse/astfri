#include <astfri/impl/ASTNode.hpp>


namespace astfri {


template<typename KindType>
void ASTNode<KindType>::accept(Visitor &visitor) {
    m_accept_ptr(this, visitor);
}


} // namespace astfri
