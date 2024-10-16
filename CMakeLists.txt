cmake_minimum_required(
    VERSION 3.19
)

project(
    astfri
    VERSION     0.0.1
    DESCRIPTION "Library for universal code representation."
    LANGUAGES   CXX
)

option(ASTFRI_BUILD_CPP_FRONTEND "Build module libastri-cpp." OFF)
option(ASTFRI_BUILD_JAVA_FRONTEND "Build module libastri-java." OFF)

add_subdirectory(libastfri)

if (ASTFRI_BUILD_CPP_FRONTEND)
    add_subdirectory(libastfri-cpp)
endif()

if (ASTFRI_BUILD_JAVA_FRONTEND)
    add_subdirectory(libastfri-java)
endif()

# add_subdirectory(libastfri-output)

# add_subdirectory(examples)
