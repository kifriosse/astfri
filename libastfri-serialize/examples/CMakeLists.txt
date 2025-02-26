add_executable(example-serialize exampleFunction.cpp )
target_compile_options(example-serialize PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(example-serialize PRIVATE astfri-core)


add_executable(classExample classExample.cpp)
target_compile_options(classExample PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(classExample PRIVATE astfri-core)

add_executable(serialisationTest Test.cpp  
               ${PROJECT_SOURCE_DIR}/libastfri-serialize/impl/AstFriSerializer.cpp
               ${PROJECT_SOURCE_DIR}/libastfri-serialize/impl/JsonFormatChecker.cpp)

target_include_directories(serialisationTest PRIVATE    ${PROJECT_SOURCE_DIR}/)
          
target_compile_options(serialisationTest PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(serialisationTest PRIVATE astfri-core)
target_compile_options(serialisationTest PUBLIC -std=c++20)


