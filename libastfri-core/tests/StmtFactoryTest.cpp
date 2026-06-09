#include <astfri/impl/StmtFactory.hpp>

#include <catch2/catch_test_macros.hpp>


TEST_CASE( "StmtFactory is singleton", "[singleton][factory]" ) {
    astfri::StmtFactory &factory1 = astfri::StmtFactory::get_instance();
    astfri::StmtFactory &factory2 = astfri::StmtFactory::get_instance();
    REQUIRE(&factory1 == &factory2);
}

