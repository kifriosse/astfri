add_executable(astfri-uml-example-standalone)

target_sources(astfri-uml-example-standalone PRIVATE ExampleStandalone.cpp)

target_link_libraries(astfri-uml-example-standalone PRIVATE astfri-uml)

apply_common_properties(astfri-uml-example-standalone)


add_executable(astfri-uml-example-cpp)

target_sources(astfri-uml-example-cpp PRIVATE ExampleCpp.cpp)

target_link_libraries(astfri-uml-example-cpp PRIVATE astfri-uml astfri-cpp)

apply_common_properties(astfri-uml-example-cpp)


add_executable(astfri-uml-example-cs)

target_sources(astfri-uml-example-cs PRIVATE ExampleCs.cpp)

target_link_libraries(astfri-uml-example-cs PRIVATE astfri-uml astfri-cs)

apply_common_properties(astfri-uml-example-cs)
