# https://github.com/firolino/clang-tool/tree/master

set(
    CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${PROJECT_SOURCE_DIR}/cmake/modules"
)

find_package(LibClangTooling REQUIRED)

add_library(
    astfri-cpp STATIC
        ClangManagement.cpp
        ClangVisitor.cpp
)

target_compile_options(
    astfri-cpp PUBLIC "-fno-rtti"
)

target_include_directories(
    astfri-cpp PUBLIC ${LibClangTooling_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}
)

target_link_libraries(
    astfri-cpp PUBLIC ${LibClangTooling_LIBRARIES} astfri-core
)