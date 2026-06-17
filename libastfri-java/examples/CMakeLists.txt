add_executable(java-example-pseudocode)

target_sources(java-example-pseudocode PRIVATE ExamplePseudocode.cpp)

target_link_libraries(java-example-pseudocode PRIVATE astfri-java astfri-text)


add_executable(java-example-uml)

target_sources(java-example-uml PRIVATE ExampleUml.cpp)

target_link_libraries(java-example-uml PRIVATE astfri-java astfri-uml)
