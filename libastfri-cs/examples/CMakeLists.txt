add_executable(cs-example-pseudocode)

target_sources(cs-example-pseudocode PRIVATE ExamplePseudocode.cpp)

target_link_libraries(cs-example-pseudocode PRIVATE astfri-cs astfri-text)


add_executable(cs-example-uml)

target_sources(cs-example-uml PRIVATE ExampleUml.cpp)

target_link_libraries(cs-example-uml PRIVATE astfri-cs astfri-uml)
