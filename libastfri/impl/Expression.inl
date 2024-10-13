#ifndef LIBASTFRI_EXPRESSION_HPP
#include <libastfri/Expression.hpp>
#endif

namespace astfri
{
template<typename T>
Literal<T>::Literal(T val) : val_(std::move(val))
{
}
}