#ifndef ASTFRI_IMPL_TYPE_FWD_INFO_HPP
#define ASTFRI_IMPL_TYPE_FWD_INFO_HPP


namespace astfri {


namespace detail {


template<typename KindType>
struct ASTNode;


} // namespace details


/**
 * @brief Returns true if @p t is @c T.
 * @param t Node to typecheck.
 * @return true if @p t is @c T.
 */
template<typename T, typename KindType>
bool is_a(detail::ASTNode<KindType> *t);

/**
 * @brief Returns @p t casted to @c T iff @p t is @c T, otherwise returns nullptr.
 * @param t Node to typecheck.
 * @return Typecasted @p t if @p t is @c T, otherwise nullptr.
 */
template<typename T, typename KindType>
T *as_a(detail::ASTNode<KindType> *t);


} // namespace astfri


#endif

