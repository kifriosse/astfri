#ifndef ASTFRI_IMPL_EXPR_KIND_HPP
#define ASTFRI_IMPL_EXPR_KIND_HPP

#include <astfri/impl/Kind.hpp>
#include <astfri/impl/ExprFwd.hpp>


namespace astfri {


enum class ExprKind {
    IntLiteral,
    FloatLiteral,
    // TODO
};


template<> struct KindOf<IntLiteralExpr> {
    static constexpr ExprKind value{ExprKind::IntLiteral};
};

// TODO


} // namespace astfri

#endif
