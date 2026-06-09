#include <astfri/impl/TypeInfo.hpp>
#include <astfri/impl/ASTNode.hpp>
#include <astfri/impl/Kind.hpp>


namespace astfri {


template<typename T, typename KindType>
bool is(detail::ASTNode<KindType> *t) {
    return as<T>(t) != nullptr;
}

template<typename T, typename KindType>
T *as(detail::ASTNode<KindType> *t) {
    return t->kind == KindOf<T>::value ? static_cast<T*>(t) : nullptr;
}


} // namespace astfri
