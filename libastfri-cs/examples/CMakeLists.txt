add_executable(astfri-cs-example-pseudocode)

target_sources(astfri-cs-example-pseudocode PRIVATE ExamplePseudocode.cpp)

target_link_libraries(astfri-cs-example-pseudocode PRIVATE astfri-cs astfri-text)

apply_common_properties(astfri-cs-example-pseudocode)


add_executable(astfri-cs-example-uml)

target_sources(astfri-cs-example-uml PRIVATE ExampleUml.cpp)

target_link_libraries(astfri-cs-example-uml PRIVATE astfri-cs astfri-uml)

apply_common_properties(astfri-cs-example-uml)
