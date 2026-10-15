add_executable(astfri-text-example-standalone)

target_sources(astfri-text-example-standalone PRIVATE ExampleStandalone.cpp)

target_link_libraries(astfri-text-example-standalone PRIVATE astfri-text)

apply_common_properties(astfri-text-example-standalone)
