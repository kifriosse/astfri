add_executable(uml-example-standalone)

target_sources(uml-example-standalone PRIVATE ExampleStandalone.cpp)

target_link_libraries(uml-example-standalone PRIVATE astfri-uml)


add_executable(uml-example-cpp)

target_sources(uml-example-cpp PRIVATE ExampleCpp.cpp)

target_link_libraries(uml-example-cpp PRIVATE astfri-uml astfri-cpp)


add_executable(uml-example-cs)

target_sources(uml-example-cs PRIVATE ExampleCs.cpp)

target_link_libraries(uml-example-cs PRIVATE astfri-uml astfri-cs)
