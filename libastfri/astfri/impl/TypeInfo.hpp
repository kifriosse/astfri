#ifndef ASTFRI_IMPL_TYPE_INFO_HPP
#define ASTFRI_IMPL_TYPE_INFO_HPP

#include <astfri/impl/ExprFwd.hpp>
#include <astfri/impl/StmtFwd.hpp>
#include <astfri/impl/TypeFwd.hpp>

#include <concepts>


namespace astfri {


/**
 * @brief returns true if @p t is-a T
 */
template<typename T>
requires(std::derived_from<T, Type>)
bool is_a(Type* t);

/**
 * @brief returns @p t casted to T iff T is-a Type, otherwise returns nullptr
 */
template<typename T>
requires(std::derived_from<T, Type>)
T* as_a(Type* t);

/**
 * @brief returns true if @p t is-a T
 */
template<typename T>
requires(std::derived_from<T, Stmt>)
bool is_a(Stmt* t);

/**
 * @brief returns @p t casted to T iff T is-a Stmt, otherwise returns nullptr
 */
template<typename T>
requires(std::derived_from<T, Stmt>)
T* as_a(Stmt* t);

/**
 * @brief returns true if @p t is-a T
 */
template<typename T>
requires(std::derived_from<T, Expr>)
bool is_a(Expr* t);

/**
 * @brief returns @p t casted to T iff T is-a Expr, otherwise returns nullptr
 */
template<typename T>
requires(std::derived_from<T, Expr>)
T* as_a(Expr* t);


} // namespace astfri


#include <astfri/impl/TypeInfo.inl>


#endif
