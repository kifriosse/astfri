#ifndef LIBASTFRI_EXPRESSION_HPP
#    include <libastfri/inc/Expression.hpp>
#endif

namespace astfri
{
template<typename T>
Literal<T>::Literal(T val) : val_(std::move(val))
{
}
} // namespace astfri