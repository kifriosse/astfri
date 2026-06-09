#include <astfri/impl/ASTNode.hpp>


namespace astfri {


namespace detail {


template<typename KindType>
void ASTNode<KindType>::accept(Visitor &visitor) {
    m_accept_ptr(this, visitor);
}


template<typename SelfType, typename KindType>
void initialize(ASTNode<KindType> &node) {
    // The specialization must exist, otherwise we get a compile error.
    node.kind = KindOf<SelfType>::value;

    // Implicit conversion of lambda to function pointer.
    node.m_accept_ptr = +[](void *self, Visitor &visitor){
        visitor.visit(*static_cast<SelfType*>(self));
    };
}


template<typename BaseType, typename SelfType>
MakeA<BaseType, SelfType>::MakeA() {
    initialize<SelfType>(*this);
}


} // namespace detail


} // namespace astfri
