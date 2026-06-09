cmake_minimum_required(VERSION 3.19)

project(
    astfri
    VERSION     0.0.1
    DESCRIPTION "Set of libraries for universal code representation and processing."
    LANGUAGES   C CXX
)

option(ASTFRI_BUILD_CLI "Build executable astfri-cli." OFF)
option(ASTFRI_BUILD_SERVICE "Build executable astfri-service." OFF)
option(ASTFRI_BUILD_IN_CPP "Build lib libastri-cpp." OFF)
option(ASTFRI_BUILD_IN_JAVA "Build lib libastri-java." OFF)
option(ASTFRI_BUILD_IN_CSHARP "Build lib libastri-cs." OFF)
option(ASTFRI_BUILD_OUT_TEXT "Build lib libastri-text." OFF)
option(ASTFRI_BUILD_OUT_UML "Build lib libastri-uml." OFF)
option(ASTFRI_BUILD_SERIALIZED "Build lib libastri-serialize." OFF)

option(ASTFRI_BUILD_TESTS "Build tests." OFF)
option(ASTFRI_BUILD_EXAMPLES "Build examples." OFF)

add_subdirectory(external)

add_subdirectory(libastfri-core)

if (ASTFRI_BUILD_CLI)
    add_subdirectory(astfri-cli)
endif()

if (ASTFRI_BUILD_CLI)
    add_subdirectory(astfri-service)
endif()

if (ASTFRI_BUILD_IN_CPP)
    add_subdirectory(libastfri-cpp)
endif()

if (ASTFRI_BUILD_IN_JAVA)
    add_subdirectory(libastfri-java)
endif()

if (ASTFRI_BUILD_IN_CSHARP)
    add_subdirectory(libastfri-cs)
endif()

if (ASTFRI_BUILD_OUT_TEXT)
    add_subdirectory(libastfri-text)
endif()

if (ASTFRI_BUILD_OUT_UML)
    add_subdirectory(libastfri-uml)
endif()

if (ASTFRI_BUILD_SERIALIZE)
    add_subdirectory(libastfri-serialize)
endif()
