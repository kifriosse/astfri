add_executable(astfri-cpp-example-uml)

target_sources(astfri-cpp-example-uml PRIVATE ExampleUml.cpp)

target_link_libraries(astfri-cpp-example-uml PRIVATE astfri-cpp astfri-uml)

apply_common_properties(astfri-cpp-example-uml)


add_executable(astfri-cpp-example-pseudocode)

target_sources(astfri-cpp-example-pseudocode PRIVATE ExamplePseudocode.cpp)

target_link_libraries(astfri-cpp-example-pseudocode PRIVATE astfri-cpp astfri-text)

apply_common_properties(astfri-cpp-example-pseudocode)
