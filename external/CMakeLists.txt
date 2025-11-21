# External project installation
include(ExternalProject)

#############
# rapidjson #
#############
ExternalProject_Add(rapidjson-external
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/rapidjson
  BINARY_DIR ${PROJECT_SOURCE_DIR}/external/bin/build/rapidjson
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    -DRAPIDJSON_BUILD_TESTS=OFF
  BUILD_BYPRODUCTS ${EXTERNAL_INSTALL_DIR}
)

set(
  RAPIDJSON_INCLUDE_DIRS
    ${EXTERNAL_INSTALL_DIR}/include
  CACHE PATH "rapidjson include dir"
)

###############
# tree-sitter #
###############

# TODO also switch this to external project
add_subdirectory(tree-sitter)

####################
# tree-sitter-java #
####################
ExternalProject_Add(tree-sitter-java-external
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/tree-sitter-java
  BINARY_DIR ${PROJECT_SOURCE_DIR}/external/bin/build/tree-sitter-java
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  BUILD_BYPRODUCTS ${EXTERNAL_INSTALL_DIR}
)

set(
  TREE_SITTER_JAVA_INCLUDE_DIRS
    ${EXTERNAL_INSTALL_DIR}/include
  CACHE PATH "tree-sitter-java include dir"
)

set(
  TREE_SITTER_JAVA_LIBRARIES
    ${EXTERNAL_INSTALL_DIR}/lib64/libtree-sitter-java.so
  CACHE PATH "tree-sitter-java library"
)

#######################
# tree-sitter-c-sharp #
#######################
ExternalProject_Add(tree-sitter-c-sharp-external
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/tree-sitter-c-sharp
  BINARY_DIR ${PROJECT_SOURCE_DIR}/external/bin/build/tree-sitter-c-sharp
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  BUILD_BYPRODUCTS ${EXTERNAL_INSTALL_DIR}
)

set(
  TREE_SITTER_C_SHARP_INCLUDE_DIRS
    ${EXTERNAL_INSTALL_DIR}/include
  CACHE PATH "tree-sitter-c-sharp include dir"
)

set(
  TREE_SITTER_C_SHARP_LIBRARIES
    ${EXTERNAL_INSTALL_DIR}/lib64/libtree-sitter-c-sharp.so
  CACHE PATH "tree-sitter-c-sharp library"
)
