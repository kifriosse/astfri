add_library(astfri-core SHARED)

add_subdirectory(astfri/impl)

target_compile_options(astfri-core PUBLIC
    -Wall
    -Wextra
    -Wpedantic
    -std=c++20
)

target_include_directories(astfri-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)
