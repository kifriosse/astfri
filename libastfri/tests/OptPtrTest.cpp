#include <astfri/impl/tools/OptPtr.hpp>

#include <catch2/catch_test_macros.hpp>


TEST_CASE( "OptPtr const access throws on uninitialized", "[tools]" ) {
    using Dummy = struct {
        int val;
    };

    const astfri::OptPtr<Dummy> ptr;

    SECTION("dereferencing") {
      REQUIRE_THROWS_AS(*ptr, astfri::UninitializedException);
    }

    SECTION("conversion") {
      REQUIRE_THROWS_AS(static_cast<const Dummy*>(ptr), astfri::UninitializedException);
    }

    SECTION("arrow") {
      REQUIRE_THROWS_AS(ptr->val, astfri::UninitializedException);
    }
}

