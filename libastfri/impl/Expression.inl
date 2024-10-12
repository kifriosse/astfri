#ifndef LIBASTFRI_EXPRESSION_HPP
#include <libastfri/Expression.hpp>
#endif

namespace astfri
{
template<typename T>
Literal<T>::Literal(T value) : value_(std::move(value))
{
}
}