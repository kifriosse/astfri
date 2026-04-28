#include <astfri/impl/TypeInfo.hpp>
#include <astfri/impl/Kind.hpp>


namespace astfri {


template<typename T, typename KindType>
bool is_a(ASTNode<KindType> *t) {
    return as_a<T>(t) != nullptr;
}

template<typename T, typename KindType>
T *as_a(ASTNode<KindType> *t) {
    return t->kind == KindOf<T>::value ? static_cast<T*>(t) : nullptr;
}


} // namespace astfri
