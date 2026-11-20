#include <astfri/impl/ASTNode.hpp>
#include <astfri/impl/TypeInfo.hpp>


namespace astfri {


namespace detail {


bool is_a_impl(ASTNodeType node_type, ASTNodeType parent_type);


} // namespace detail


template<typename T>
bool is(ASTNode *node) {
    return as<T>(node) != nullptr;
}

template<typename B>
bool is_a(ASTNode *node) {
    return detail::is_a_impl(node->type(), detail::TypeOf<B>::value);
}

template<typename T>
T *as(ASTNode *node) {
    return node && node->type() == detail::TypeOf<T>::value
        ? static_cast<T*>(node)
        : nullptr;
}


} // namespace astfri
