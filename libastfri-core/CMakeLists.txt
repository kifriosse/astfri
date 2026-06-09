set(ASTFRI_CORE_VERSION "0.0.2")

add_library(astfri-core)

add_subdirectory(astfri/impl)

target_compile_definitions(astfri-core PUBLIC ASTFRI_VERSION="${ASTFRI_VERSION}")
target_compile_definitions(astfri-core PUBLIC ASTFRI_CORE_VERSION="${ASTFRI_CORE_VERSION}")

target_compile_options(astfri-core PUBLIC
    -Wall
    -Wextra
    -Wpedantic
    -std=c++20
)

target_include_directories(astfri-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (ASTFRI_BUILD_TESTS)
    add_subdirectory(tests)
endif()
