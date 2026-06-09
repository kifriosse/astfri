#include <astfri/impl/ExprFactory.hpp>
#include <astfri/impl/TypeInfo.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace astfri;


TEST_CASE( "Type queries", "[type-info]" ) {
    auto &f = ExprFactory::get_instance();
    Expr *const self = f.mk_this();
    Expr *const base = f.mk_base();

    SECTION("is_a true") {
      REQUIRE(is_a<ThisExpr>(self));
    }

    SECTION("as_a true") {
      REQUIRE(as_a<ThisExpr>(self) == self);
    }

    SECTION("is_a false") {
      REQUIRE_FALSE(is_a<ThisExpr>(base));
    }

    SECTION("as_a false") {
      REQUIRE(as_a<ThisExpr>(base) != base);
    }
}
