cmake_minimum_required(VERSION 3.19)

project(
    astfri
    VERSION     0.0.1
    DESCRIPTION "Library for universal code representation."
    LANGUAGES   C CXX
)

option(ASTFRI_BUILD_CLI "Build executable astfri." OFF)
option(ASTFRI_BUILD_CPP_INPUT "Build lib libastri-cpp." OFF)
option(ASTFRI_BUILD_JAVA_INPUT "Build lib libastri-java." OFF)
option(ASTFRI_BUILD_C_SHARP_INPUT "Build lib libastri-cs." OFF)
option(ASTFRI_BUILD_SERIALIZED_INPUT "Build lib libastri-serialize." OFF)
option(ASTFRI_BUILD_TEXT_OUTPUT "Build lib libastri-text." OFF)
option(ASTFRI_BUILD_UML_OUTPUT "Build lib libastri-uml." OFF)

option(ASTFRI_BUILD_TESTS "Build tests." OFF)
option(ASTFRI_BUILD_EXAMPLES "Build examples." OFF)

add_subdirectory(external)

add_subdirectory(libastfri)

if (ASTFRI_BUILD_CLI)
    add_subdirectory(astfri)
endif()

if (ASTFRI_BUILD_CPP_INPUT)
    add_subdirectory(libastfri-cpp)
endif()

if (ASTFRI_BUILD_JAVA_INPUT)
    add_subdirectory(libastfri-java)
endif()

if (ASTFRI_BUILD_C_SHARP_INPUT)
    add_subdirectory(libastfri-cs)
endif()

if (ASTFRI_BUILD_SERIALIZE)
    add_subdirectory(libastfri-serialize)
endif()

if (ASTFRI_BUILD_TEXT_OUTPUT)
    add_subdirectory(libastfri-text)
endif()

if (ASTFRI_BUILD_UML_OUTPUT)
    add_subdirectory(libastfri-uml)
endif()
