add_executable(astfri-core-tests)

target_sources(astfri-core-tests PRIVATE astfri.test.cpp)

target_link_libraries(astfri-core-tests PRIVATE astfri-core Catch2)
