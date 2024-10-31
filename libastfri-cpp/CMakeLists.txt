find_package(Clang REQUIRED)

add_library(
    astfri-cpp STATIC
        ClangManagement.cpp
        ClangVisitor.cpp
)

target_compile_options(
    astfri-cpp PUBLIC ${Clang_OPTIONS}
)

target_compile_options(
    astfri-cpp PRIVATE "-Wall" "-Wextra" "-Wpedantic"
)

target_include_directories(
    astfri-cpp PUBLIC ${Clang_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}
)

target_link_libraries(
    astfri-cpp PUBLIC ${Clang_LIBRARIES} astfri-core
)
