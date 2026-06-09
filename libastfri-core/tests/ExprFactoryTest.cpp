#include <astfri/impl/ExprFactory.hpp>

#include <catch2/catch_test_macros.hpp>


TEST_CASE( "ExprFactory is singleton", "[singleton][factory]" ) {
    astfri::ExprFactory &factory1 = astfri::ExprFactory::get_instance();
    astfri::ExprFactory &factory2 = astfri::ExprFactory::get_instance();
    REQUIRE(&factory1 == &factory2);
}
