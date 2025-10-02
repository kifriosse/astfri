add_executable(uml-example)

target_sources(uml-example PRIVATE umlexample.cpp)

target_link_libraries(uml-example PRIVATE astfri-uml)


add_executable(uml-example-link)

target_sources(uml-example-link PRIVATE umllinkexample.cpp)

target_link_libraries(uml-example-link PRIVATE astfri-uml astfri-cpp)
