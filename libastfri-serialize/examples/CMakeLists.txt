add_executable(example-serialize exampleFunction.cpp )
target_compile_options(example-serialize PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(example-serialize PRIVATE astfri-core)


add_executable(classExample classExample.cpp)
target_compile_options(classExample PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(classExample PRIVATE astfri-core)