#ifndef LIBASTFRI_EXPRESSION_HPP
#    include <astfri/impl/Expr.hpp>
#endif

namespace astfri {


template<typename T>
Literal<T>::Literal(T val) :
    val(std::move(val)) {
}


} // namespace astfri
