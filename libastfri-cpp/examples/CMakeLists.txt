add_executable(cpp-example-uml)

target_sources(cpp-example-uml PRIVATE ExampleUml.cpp)

target_link_libraries(cpp-example-uml PRIVATE astfri-cpp astfri-uml)


add_executable(cpp-example-pseudocode)

target_sources(cpp-example-pseudocode PRIVATE ExamplePseudocode.cpp)

target_link_libraries(cpp-example-pseudocode PRIVATE astfri-cpp astfri-text)
