cmake_minimum_required(VERSION 3.23)

SET(ASTFRI_VERSION "0.1.1")

project(
    astfri
    VERSION     ${ASTFRI_VERSION}
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

add_subdirectory(libastfri-common)

# List of targets that are build used for the installation.
set(ASTFRI_TARGETS "")
list(APPEND ASTFRI_TARGETS astfri-core)
list(APPEND ASTFRI_TARGETS astfri-common)

if (ASTFRI_BUILD_CLI)
    add_subdirectory(astfri-cli)
    list(APPEND ASTFRI_TARGETS astfri-cli)
endif()

if (ASTFRI_BUILD_SERVICE)
    add_subdirectory(astfri-service)
endif()

if (ASTFRI_BUILD_IN_CPP)
    add_subdirectory(libastfri-cpp)
    list(APPEND ASTFRI_TARGETS astfri-cpp)
endif()

if (ASTFRI_BUILD_IN_JAVA)
    add_subdirectory(libastfri-java)
    list(APPEND ASTFRI_TARGETS astfri-java)
endif()

if (ASTFRI_BUILD_IN_CSHARP)
    add_subdirectory(libastfri-cs)
    list(APPEND ASTFRI_TARGETS astfri-cs)
endif()

if (ASTFRI_BUILD_OUT_TEXT)
    add_subdirectory(libastfri-text)
    list(APPEND ASTFRI_TARGETS astfri-text)
endif()

if (ASTFRI_BUILD_OUT_UML)
    add_subdirectory(libastfri-uml)
    list(APPEND ASTFRI_TARGETS astfri-uml)
endif()

if (ASTFRI_BUILD_SERIALIZE)
    add_subdirectory(libastfri-serialize)
    list(APPEND ASTFRI_TARGETS astfri-serialize)
endif()


include(GNUInstallDirs)

include(CMakePackageConfigHelpers)

# Installs artifacts defined by the targets.
# Installs `headers` file set defined for each target.
# Creates AstfriTargets in-memory containing list of installed targets.
install(TARGETS ${ASTFRI_TARGETS}
    EXPORT AstfriTargets
    FILE_SET headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# TODO install resources dir using install dir for each lib into:
#     DESTINATION ${CMAKE_INSTALL_DATADIR}/astfri-cpp/<here>

# Materializes AstfriTargets to a file and installs it.
install(EXPORT AstfriTargets
    FILE AstfriTargets.cmake
    NAMESPACE astfri:: # TODO chceme tu toto?
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/astfri"
)

# Writes file with the version of the project.
write_basic_package_version_file(
    AstfriConfigVersion.cmake
    VERSION ${ASTFRI_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Installs generated version file and project config file.
install(FILES
    "${PROJECT_SOURCE_DIR}/cmake/AstfriConfig.cmake"
    "${PROJECT_BINARY_DIR}/AstfriConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/astfri"
)
