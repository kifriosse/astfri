cmake_minimum_required(
    VERSION 3.19
)

project(
    libast
    VERSION     1.0.0
    DESCRIPTION "Library for universal code representation."
    LANGUAGES   CXX
)

option(
    ASTFRI_BUILD_CPP_FRONTEND "Build module libastri-cpp." OFF
)

add_subdirectory(libastfri)

if (ASTFRI_BUILD_CPP_FRONTEND)
    add_subdirectory(libastfri-cpp)
endif()

add_subdirectory(libastfri-output)

add_subdirectory(examples)
