add_executable(uml-example)

target_sources(uml-example PRIVATE umlexample.cpp)

target_link_libraries(uml-example PRIVATE astfri-uml)


add_executable(uml-example-link)

target_sources(uml-example-link PRIVATE umllinkexample.cpp)

target_link_libraries(uml-example-link PRIVATE astfri-uml astfri-cpp)

add_executable(uml-cs-link)
target_sources(uml-cs-link PRIVATE example_cs_link.cpp)
target_link_libraries(uml-cs-link PRIVATE astfri-uml astfri-cs)
