add_executable(astfri-java-example-pseudocode)

target_sources(astfri-java-example-pseudocode PRIVATE ExamplePseudocode.cpp)

target_link_libraries(astfri-java-example-pseudocode PRIVATE astfri-java astfri-text)

apply_common_properties(astfri-java-example-pseudocode)


add_executable(astfri-java-example-uml)

target_sources(astfri-java-example-uml PRIVATE ExampleUml.cpp)

target_link_libraries(astfri-java-example-uml PRIVATE astfri-java astfri-uml)

apply_common_properties(astfri-java-example-uml)
