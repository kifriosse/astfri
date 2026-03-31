include(ExternalProject)
include(GNUInstallDirs)

set(
  EXTERNAL_INSTALL_DIR ${CMAKE_BINARY_DIR}/external-install
  CACHE PATH "External install directory"
)

# Create the directory to avoid errors during configuration.
file(MAKE_DIRECTORY ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR})

#############
# rapidjson #
#############
ExternalProject_Add(rapidjson-external
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/rapidjson
  BINARY_DIR ${CMAKE_BINARY_DIR}/external/rapidjson
  CMAKE_ARGS
    -DCMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -G ${CMAKE_GENERATOR}
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    -DRAPIDJSON_BUILD_TESTS=OFF
    -DRAPIDJSON_BUILD_EXAMPLES=OFF
    -DRAPIDJSON_BUILD_DOC=OFF
    -DRAPIDJSON_BUILD_THIRDPARTY_GTEST=OFF
  UPDATE_COMMAND ""
)

add_library(rapidjson INTERFACE IMPORTED GLOBAL)

set_target_properties(rapidjson PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}
)

add_dependencies(rapidjson rapidjson-external)

###############
# tree-sitter #
###############
ExternalProject_Add(tree-sitter-external
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/tree-sitter
  BINARY_DIR ${CMAKE_BINARY_DIR}/external/tree-sitter
  CMAKE_ARGS
    -DCMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -G ${CMAKE_GENERATOR}
  BUILD_BYPRODUCTS ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/libtree-sitter.so
  UPDATE_COMMAND ""
)

add_library(tree-sitter SHARED IMPORTED GLOBAL)

set_target_properties(tree-sitter PROPERTIES
  IMPORTED_LOCATION ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/libtree-sitter.so
  INTERFACE_INCLUDE_DIRECTORIES ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}
)

add_dependencies(tree-sitter tree-sitter-external)

####################
# tree-sitter-java #
####################
ExternalProject_Add(tree-sitter-java-external
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/tree-sitter-java
  BINARY_DIR ${CMAKE_BINARY_DIR}/external/tree-sitter-java
  CMAKE_ARGS
    -DCMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -G ${CMAKE_GENERATOR}
  BUILD_BYPRODUCTS ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/libtree-sitter-java.so
  UPDATE_COMMAND ""
)

add_library(tree-sitter-java SHARED IMPORTED GLOBAL)

set_target_properties(tree-sitter-java PROPERTIES
  IMPORTED_LOCATION ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/libtree-sitter-java.so
  INTERFACE_INCLUDE_DIRECTORIES ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}
)

add_dependencies(tree-sitter-java tree-sitter-java-external)

#######################
# tree-sitter-c-sharp #
#######################
ExternalProject_Add(tree-sitter-c-sharp-external
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/tree-sitter-c-sharp
  BINARY_DIR ${CMAKE_BINARY_DIR}/external/tree-sitter-c-sharp
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
    -DCMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  BUILD_BYPRODUCTS ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/libtree-sitter-c-sharp.so
  UPDATE_COMMAND ""
)

add_library(tree-sitter-c-sharp SHARED IMPORTED GLOBAL)

set_target_properties(tree-sitter-c-sharp PROPERTIES
  IMPORTED_LOCATION ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/libtree-sitter-c-sharp.so
  INTERFACE_INCLUDE_DIRECTORIES ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}
)

add_dependencies(tree-sitter-c-sharp tree-sitter-c-sharp-external)

###########
# cxxopts #
###########
ExternalProject_Add(cxxopts-external
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/cxxopts
  BINARY_DIR ${CMAKE_BINARY_DIR}/external/cxxopts
  CMAKE_ARGS
    -DCMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -G ${CMAKE_GENERATOR}
    -DCXXOPTS_BUILD_EXAMPLES=OFF
    -DCXXOPTS_BUILD_TESTS=OFF
    -DCXXOPTS_ENABLE_INSTALL=ON
  UPDATE_COMMAND ""
)

add_library(cxxopts STATIC IMPORTED GLOBAL)

set_target_properties(cxxopts PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}
)

add_dependencies(cxxopts cxxopts-external)

##########
# Catch2 #
##########
ExternalProject_Add(Catch2-external
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/Catch2
  BINARY_DIR ${CMAKE_BINARY_DIR}/external/Catch2
  CMAKE_ARGS
    -DCMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -G ${CMAKE_GENERATOR}
  BUILD_BYPRODUCTS
    ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/libCatch2d.a
    ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/libCatch2Maind.a
  UPDATE_COMMAND ""
)

set(
  CATCH2_INCLUDE_DIRS
    ${EXTERNAL_INSTALL_DIR}/include
  CACHE PATH "Catch2 include dir"
)

add_library(Catch2 INTERFACE IMPORTED GLOBAL)
add_library(Catch2Main INTERFACE IMPORTED GLOBAL)

set_target_properties(Catch2 PROPERTIES
    IMPORTED_LOCATION ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/libCatch2d.a
    INTERFACE_INCLUDE_DIRECTORIES ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}
)
set_target_properties(Catch2Main PROPERTIES
    IMPORTED_LOCATION ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/libCatch2Maind.a
    INTERFACE_INCLUDE_DIRECTORIES ${EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}
)

add_dependencies(Catch2 Catch2-external)
add_dependencies(Catch2Main Catch2-external)
