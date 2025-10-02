add_executable(ts-java)

target_sources(ts-java PRIVATE java.cpp)

target_link_libraries(ts-java PRIVATE astfri-java astfri-text astfri-uml)


add_executable(example_ak)

target_sources(example_ak PRIVATE main.cpp)

target_link_libraries(example_ak PRIVATE astfri-java)


add_executable(example_to)

target_sources(example_to PRIVATE example_text_output.cpp)

target_link_libraries(example_to PRIVATE astfri-java astfri-text)


add_executable(example_umlo)

target_sources(example_umlo PRIVATE example_uml_output.cpp)

target_link_libraries(example_umlo PRIVATE astfri-java astfri-uml)
