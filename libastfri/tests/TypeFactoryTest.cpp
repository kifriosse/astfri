#include <astfri/impl/TypeFactory.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace astfri;


TEST_CASE( "TypeFactory - singleton", "[singleton][factory]" ) {
    auto &factory1 = TypeFactory::get_instance();
    auto &factory2 = TypeFactory::get_instance();
    REQUIRE(&factory1 == &factory2);
}

TEST_CASE( "TypeFactory - singleton types", "[singleton][factory]" ) {
    auto &factory = TypeFactory::get_instance();

    SECTION("int") {
        IntType *const node1 = factory.mk_int();
        IntType *const node2 = factory.mk_int();
        REQUIRE(node1 == node2);
    }

    SECTION("float") {
        FloatType *const node1 = factory.mk_float();
        FloatType *const node2 = factory.mk_float();
        REQUIRE(node1 == node2);
    }

    SECTION("char") {
        CharType *const node1 = factory.mk_char();
        CharType *const node2 = factory.mk_char();
        REQUIRE(node1 == node2);
    }

    SECTION("bool") {
        BoolType *const node1 = factory.mk_bool();
        BoolType *const node2 = factory.mk_bool();
        REQUIRE(node1 == node2);
    }

    SECTION("void") {
        VoidType *const node1 = factory.mk_void();
        VoidType *const node2 = factory.mk_void();
        REQUIRE(node1 == node2);
    }

    SECTION("dynamic") {
        DynamicType *const node1 = factory.mk_dynamic();
        DynamicType *const node2 = factory.mk_dynamic();
        REQUIRE(node1 == node2);
    }
}

TEST_CASE( "TypeFactory - multiton types", "[multiton][factory]" ) {
    auto &factory = TypeFactory::get_instance();

    SECTION("indirection shared") {
        IntType *const intType = factory.mk_int();
        IndirectionType *const intPtr1 = factory.mk_indirect(intType);
        IndirectionType *const intPtr2 = factory.mk_indirect(intType);
        REQUIRE(intPtr1 == intPtr2);
    }

    SECTION("indirection different") {
        IntType *const intType = factory.mk_int();
        CharType *const charType = factory.mk_char();
        IndirectionType *const intPtr = factory.mk_indirect(intType);
        IndirectionType *const charPtr = factory.mk_indirect(charType);
        REQUIRE(intPtr != charPtr);
    }

    SECTION("incomplete shared") {
        const std::string foo = "foo";
        IncompleteType *const incompleteFoo1 = factory.mk_incomplete(foo);
        IncompleteType *const incompleteFoo2 = factory.mk_incomplete(foo);
        REQUIRE(incompleteFoo1 == incompleteFoo2);
    }

    SECTION("incomplete different") {
        const std::string foo = "foo";
        const std::string boo = "boo";
        IncompleteType *const incompleteFoo = factory.mk_incomplete(foo);
        IncompleteType *const incompleteBoo = factory.mk_incomplete(boo);
        REQUIRE(incompleteFoo != incompleteBoo);
    }

    SECTION("deduced shared") {
        IntType *const intType = factory.mk_int();
        DeducedType *const deducedInt1 = factory.mk_deduced(intType);
        DeducedType *const deducedInt2 = factory.mk_deduced(intType);
        REQUIRE(deducedInt1 == deducedInt2);
    }

    SECTION("deduced different") {
        IntType *const intType = factory.mk_int();
        CharType *const charType = factory.mk_char();
        DeducedType *const deducedInt = factory.mk_deduced(intType);
        DeducedType *const deducedChar = factory.mk_deduced(charType);
        REQUIRE(deducedInt != deducedChar);
    }

    // TODO deduced on nullptr
}

TEST_CASE( "TypeFactory - class type", "[multiton][factory]" ) {

}
