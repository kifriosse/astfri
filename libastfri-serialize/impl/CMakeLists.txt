add_library(astfri-serialize SHARED AstFriSerializer.cpp
                                    JsonFormatChecker.cpp)


target_compile_options(astfri-serialize PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(astfri-serialize PUBLIC astfri-core)
target_include_directories(astfri-serialize PUBLIC ${PROJECT_SOURCE_DIR}/lib)
