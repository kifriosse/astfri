#include <astfri-ted/ZhangShasha.hpp>
#include <catch2/catch_test_macros.hpp>


// TEST_CASE( "TED tychto stromov je 2", "[ted]" ) {
//     astfri::ted::TedIndex tedIndex1{
//         .nodes{"", "a", "b", "c", "d", "e", "f"},
//         .l{-1, 1, 2, 2, 1, 5, 1},
//         .keyroots{-1, 3, 5, 6}};
//     astfri::ted::TedIndex tedIndex2{
//         .nodes{"", "a", "b", "c", "d", "e", "f"},
//         .l{-1, 1, 2, 1, 1, 5, 1},
//         .keyroots{-1, 2, 5, 6}};
//     astfri::ted::Costs costs{.remove = 1, .insert = 1, .rename = 1};

//     astfri::ted::ZhangShasha zs(tedIndex1, tedIndex2, costs);

//     REQUIRE(zs.calculate_treedistance() == 2);
// }

// TEST_CASE( "TED rovnakych stromov je 0", "[ted]" ) {
//     astfri::ted::TedIndex tedIndex1{
//         .nodes{"", "a", "b", "c", "d", "e", "f"},
//         .l{-1, 1, 2, 2, 1, 5, 1},
//         .keyroots{-1, 3, 5, 6}};

//     astfri::ted::Costs costs{.remove = 1, .insert = 1, .rename = 1};

//     astfri::ted::ZhangShasha zs(tedIndex1, tedIndex1, costs);

//     REQUIRE(zs.calculate_treedistance() == 0);
// }

// TEST_CASE( "TED tychto stromov je 3", "[ted]" ) {
//     astfri::ted::TedIndex tedIndex1{
//         .nodes{"", "a"},
//         .l{-1, 1},
//         .keyroots{-1, 1}};
//     astfri::ted::TedIndex tedIndex2{
//         .nodes{"", "b", "b", "a", "b"},
//         .l{-1, 1, 2, 2, 1},
//         .keyroots{-1, 3, 4}};
//     astfri::ted::Costs costs{.remove = 1, .insert = 1, .rename = 1};

//     astfri::ted::ZhangShasha zs(tedIndex1, tedIndex2, costs);

//     REQUIRE(zs.calculate_treedistance() == 3);
// }