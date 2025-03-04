#ifndef LIBASTFRI_INC_TYPE_QUERY_HPP
#define LIBASTFRI_INC_TYPE_QUERY_HPP

#include <libastfri/impl/TypeFwd.hpp>
#include <libastfri/impl/ExprFwd.hpp>
#include <libastfri/impl/StmtFwd.hpp>

namespace astfri
{

/**
  * @brief Checks if @p e is exactly of type @tparam T
  */
template<class T>
bool is_type(Type* e);

/**
  * @brief Checks if @p e is exactly of type @tparam T
  */
template<class T>
bool is_type(Expr* e);

/**
  * @brief Checks if @p e is exactly of type @tparam T
  */
template<class T>
bool is_type(Stmt* e);

}  // namespace astfri

#include <libastfri/impl/TypeQuery.inl>

#endif
