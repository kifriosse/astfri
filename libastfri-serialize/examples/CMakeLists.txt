add_executable(example-serialize exampleFunction.cpp )
target_compile_options(example-serialize PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(example-serialize PRIVATE astfri-core)
