add_executable(astfri-serialize-example-text)

target_sources(astfri-serialize-example-text PRIVATE TestWithTextOutput.cpp)

target_link_libraries(astfri-serialize-example-text PRIVATE astfri-serialize astfri-text)

apply_common_properties(astfri-serialize-example-text)


add_executable(astfri-serialize-example-uml)

target_sources(astfri-serialize-example-uml PRIVATE TestWithUmlOutput.cpp)

target_link_libraries(astfri-serialize-example-uml PRIVATE astfri-serialize astfri-uml)

apply_common_properties(astfri-serialize-example-uml)
