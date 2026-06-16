add_executable(text-example-standalone)

target_sources(text-example-standalone PRIVATE ExampleStandalone.cpp)

target_link_libraries(text-example-standalone PRIVATE astfri-text)
