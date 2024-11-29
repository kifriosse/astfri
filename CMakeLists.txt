cmake_minimum_required(
    VERSION 3.19
)

project(
    astfri
    VERSION     0.0.1
    DESCRIPTION "Library for universal code representation."
    LANGUAGES   CXX
)

set(
    CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${PROJECT_SOURCE_DIR}/cmake/modules"
)

option(ASTFRI_BUILD_CPP_INPUT "Build module libastri-cpp." OFF)
option(ASTFRI_BUILD_JAVA_INPUT "Build module libastri-java." OFF)
option(ASTFRI_BUILD_SERIALIZED_INPUT "Build module libastri-serialize." OFF)
option(ASTFRI_BUILD_TEXT_OUTPUT "Build module libastri-text." OFF)
option(ASTFRI_BUILD_UML_OUTPUT "Build module libastri-uml." OFF)

add_subdirectory(libastfri)

if (ASTFRI_BUILD_CPP_INPUT)
    add_subdirectory(libastfri-cpp)
endif()

if (ASTFRI_BUILD_JAVA_INPUT)
    add_subdirectory(libastfri-java)
endif()

if (ASTFRI_BUILD_SERIALIZED_INPUT)
    add_subdirectory(libastfri-serialize)
endif()

if (ASTFRI_BUILD_TEXT_OUTPUT)
    add_subdirectory(libastfri-text)
endif()

if (ASTFRI_BUILD_UML_OUTPUT)
    add_subdirectory(libastfri-uml)
endif()