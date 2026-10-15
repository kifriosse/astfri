include(ExternalProject)
include(GNUInstallDirs)

##########
# common #
##########
set(
  ASTFRI_EXTERNAL_INSTALL_DIR ${CMAKE_BINARY_DIR}/external-install
  CACHE PATH "External install directory"
)
set(LIBPATH "${ASTFRI_EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(BINPATH "${ASTFRI_EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_BINDIR}")
set(INCPATH "${ASTFRI_EXTERNAL_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}")

set(ASTFRI_EXTERNAL_CMAKE_ARGS
  "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
  "-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}"
  "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
  "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
  "-DCMAKE_INSTALL_PREFIX=${ASTFRI_EXTERNAL_INSTALL_DIR}"
  "-DCMAKE_INSTALL_BINDIR=${CMAKE_INSTALL_BINDIR}"
  "-DCMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}"
  "-DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_INCLUDEDIR}"
)

file(MAKE_DIRECTORY "${LIBPATH}")
file(MAKE_DIRECTORY "${BINPATH}")
file(MAKE_DIRECTORY "${INCPATH}")

#############
# rapidjson #
#############
ExternalProject_Add(rapidjson-external
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/external/rapidjson"
  BINARY_DIR "${CMAKE_BINARY_DIR}/external/rapidjson"
  CMAKE_ARGS
    ${ASTFRI_EXTERNAL_CMAKE_ARGS}
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    -DRAPIDJSON_BUILD_TESTS=OFF
    -DRAPIDJSON_BUILD_EXAMPLES=OFF
    -DRAPIDJSON_BUILD_DOC=OFF
    -DRAPIDJSON_BUILD_THIRDPARTY_GTEST=OFF
  UPDATE_COMMAND ""
)

add_library(rapidjson INTERFACE IMPORTED GLOBAL)

set_target_properties(rapidjson PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}")

add_dependencies(rapidjson rapidjson-external)

###############
# tree-sitter #
###############
if (BUILD_SHARED_LIBS AND WIN32)
  set(TS_DLL    "${BINPATH}/libtree-sitter${CMAKE_SHARED_LIBRARY_SUFFIX}")
  set(TS_IMPLIB "${LIBPATH}/libtree-sitter${CMAKE_IMPORT_LIBRARY_SUFFIX}")
  set(TS_BYPRODUCTS ${TS_DLL} ${TS_IMPLIB})
elseif (BUILD_SHARED_LIBS)
  set(TS_LOCATION "${LIBPATH}/libtree-sitter${CMAKE_SHARED_LIBRARY_SUFFIX}")
  set(TS_BYPRODUCTS ${TS_LOCATION})
else()
  set(TS_LOCATION "${LIBPATH}/libtree-sitter${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(TS_BYPRODUCTS ${TS_LOCATION})
endif()

ExternalProject_Add(tree-sitter-external
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/external/tree-sitter"
  BINARY_DIR "${CMAKE_BINARY_DIR}/external/tree-sitter"
  CMAKE_ARGS ${ASTFRI_EXTERNAL_CMAKE_ARGS}
  BUILD_BYPRODUCTS ${TS_BYPRODUCTS}
  UPDATE_COMMAND ""
)

if (BUILD_SHARED_LIBS)
  add_library(tree-sitter SHARED IMPORTED GLOBAL)
else()
  add_library(tree-sitter STATIC IMPORTED GLOBAL)
endif()

if (BUILD_SHARED_LIBS AND WIN32)
  set_target_properties(tree-sitter PROPERTIES
    IMPORTED_LOCATION "${TS_DLL}"
    IMPORTED_IMPLIB   "${TS_IMPLIB}"
    INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}"
  )
else()
  set_target_properties(tree-sitter PROPERTIES
    IMPORTED_LOCATION "${TS_LOCATION}"
    INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}"
  )
endif()

add_dependencies(tree-sitter tree-sitter-external)

####################
# tree-sitter-java #
####################
if (BUILD_SHARED_LIBS AND WIN32)
  set(TSJAVA_DLL    "${BINPATH}/libtree-sitter-java${CMAKE_SHARED_LIBRARY_SUFFIX}")
  set(TSJAVA_IMPLIB "${LIBPATH}/libtree-sitter-java${CMAKE_IMPORT_LIBRARY_SUFFIX}")
  set(TSJAVA_BYPRODUCTS ${TSJAVA_DLL} ${TSJAVA_IMPLIB})
elseif (BUILD_SHARED_LIBS)
  set(TSJAVA_LOCATION "${LIBPATH}/libtree-sitter-java${CMAKE_SHARED_LIBRARY_SUFFIX}")
  set(TSJAVA_BYPRODUCTS ${TSJAVA_LOCATION})
else()
  set(TSJAVA_LOCATION "${LIBPATH}/libtree-sitter-java${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(TSJAVA_BYPRODUCTS ${TSJAVA_LOCATION})
endif()

ExternalProject_Add(tree-sitter-java-external
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/external/tree-sitter-java"
  BINARY_DIR "${CMAKE_BINARY_DIR}/external/tree-sitter-java"
  CMAKE_ARGS ${ASTFRI_EXTERNAL_CMAKE_ARGS}
  BUILD_BYPRODUCTS ${TSJAVA_BYPRODUCTS}
  UPDATE_COMMAND ""
)

if (BUILD_SHARED_LIBS)
  add_library(tree-sitter-java SHARED IMPORTED GLOBAL)
else()
  add_library(tree-sitter-java STATIC IMPORTED GLOBAL)
endif()

if (BUILD_SHARED_LIBS AND WIN32)
  set_target_properties(tree-sitter-java PROPERTIES
    IMPORTED_LOCATION "${TSJAVA_DLL}"
    IMPORTED_IMPLIB   "${TSJAVA_IMPLIB}"
    INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}"
  )
else()
  set_target_properties(tree-sitter-java PROPERTIES
    IMPORTED_LOCATION "${TSJAVA_LOCATION}"
    INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}"
  )
endif()

add_dependencies(tree-sitter-java tree-sitter-java-external)

#######################
# tree-sitter-c-sharp #
#######################
if (BUILD_SHARED_LIBS AND WIN32)
  set(TSCSHARP_DLL    "${BINPATH}/libtree-sitter-c-sharp${CMAKE_SHARED_LIBRARY_SUFFIX}")
  set(TSCSHARP_IMPLIB "${LIBPATH}/libtree-sitter-c-sharp${CMAKE_IMPORT_LIBRARY_SUFFIX}")
  set(TSCSHARP_BYPRODUCTS ${TSCSHARP_DLL} ${TSCSHARP_IMPLIB})
elseif (BUILD_SHARED_LIBS)
  set(TSCSHARP_LOCATION "${LIBPATH}/libtree-sitter-c-sharp${CMAKE_SHARED_LIBRARY_SUFFIX}")
  set(TSCSHARP_BYPRODUCTS ${TSCSHARP_LOCATION})
else()
  set(TSCSHARP_LOCATION "${LIBPATH}/libtree-sitter-c-sharp${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(TSCSHARP_BYPRODUCTS ${TSCSHARP_LOCATION})
endif()

ExternalProject_Add(tree-sitter-c-sharp-external
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/external/tree-sitter-c-sharp"
  BINARY_DIR "${CMAKE_BINARY_DIR}/external/tree-sitter-c-sharp"
  CMAKE_ARGS ${ASTFRI_EXTERNAL_CMAKE_ARGS}
  BUILD_BYPRODUCTS ${TSCSHARP_BYPRODUCTS}
  UPDATE_COMMAND ""
)

if (BUILD_SHARED_LIBS)
  add_library(tree-sitter-c-sharp SHARED IMPORTED GLOBAL)
else()
  add_library(tree-sitter-c-sharp STATIC IMPORTED GLOBAL)
endif()

if (BUILD_SHARED_LIBS AND WIN32)
  set_target_properties(tree-sitter-c-sharp PROPERTIES
    IMPORTED_LOCATION "${TSCSHARP_DLL}"
    IMPORTED_IMPLIB   "${TSCSHARP_IMPLIB}"
    INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}"
  )
else()
  set_target_properties(tree-sitter-c-sharp PROPERTIES
    IMPORTED_LOCATION "${TSCSHARP_LOCATION}"
    INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}"
  )
endif()

add_dependencies(tree-sitter-c-sharp tree-sitter-c-sharp-external)

###########
# cxxopts #
###########
ExternalProject_Add(cxxopts-external
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/external/cxxopts"
  BINARY_DIR "${CMAKE_BINARY_DIR}/external/cxxopts"
  CMAKE_ARGS
    ${ASTFRI_EXTERNAL_CMAKE_ARGS}
    -DCXXOPTS_BUILD_EXAMPLES=OFF
    -DCXXOPTS_BUILD_TESTS=OFF
    -DCXXOPTS_ENABLE_INSTALL=ON
  UPDATE_COMMAND ""
)

add_library(cxxopts INTERFACE IMPORTED GLOBAL)

set_target_properties(cxxopts PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}")

add_dependencies(cxxopts cxxopts-external)

##########
# Catch2 #
##########
if (ASTFRI_BUILD_TESTS)
    if (BUILD_SHARED_LIBS AND WIN32)
        set(CATCH2_DLL "${BINPATH}/libCatch2d${CMAKE_SHARED_LIBRARY_SUFFIX}")
        set(CATCH2_IMPLIB "${LIBPATH}/libCatch2d${CMAKE_IMPORT_LIBRARY_SUFFIX}")
        set(CATCH2MAIN_DLL "${BINPATH}/libCatch2Maind${CMAKE_SHARED_LIBRARY_SUFFIX}")
        set(CATCH2MAIN_IMPLIB "${LIBPATH}/libCatch2Maind${CMAKE_IMPORT_LIBRARY_SUFFIX}")
        set(CATCH2_BYPRODUCTS
            ${CATCH2_DLL}
            ${CATCH2MAIN_DLL}
            ${CATCH2_IMPLIB}
            ${CATCH2MAIN_IMPLIB}
        )
    elseif (BUILD_SHARED_LIBS)
        set(CATCH2_LIB "${LIBPATH}/libCatch2d${CMAKE_SHARED_LIBRARY_SUFFIX}")
        set(CATCH2MAIN_LIB "${LIBPATH}/libCatch2Maind${CMAKE_SHARED_LIBRARY_SUFFIX}")
        set(CATCH2_BYPRODUCTS ${CATCH2_LIB} ${CATCH2MAIN_LIB})
    else()
        set(CATCH2_LIB "${LIBPATH}/libCatch2d${CMAKE_STATIC_LIBRARY_SUFFIX}")
        set(CATCH2MAIN_LIB "${LIBPATH}/libCatch2Maind${CMAKE_STATIC_LIBRARY_SUFFIX}")
        set(CATCH2_BYPRODUCTS ${CATCH2_LIB} ${CATCH2MAIN_LIB})
    endif()

    ExternalProject_Add(Catch2-external
      SOURCE_DIR "${PROJECT_SOURCE_DIR}/external/Catch2"
      BINARY_DIR "${CMAKE_BINARY_DIR}/external/Catch2"
      CMAKE_ARGS
          ${ASTFRI_EXTERNAL_CMAKE_ARGS}
          -DCATCH_INSTALL_DOCS=OFF
          -DCATCH_DEVELOPMENT_BUILD=OFF
      BUILD_BYPRODUCTS ${CATCH2_BYPRODUCTS}
      UPDATE_COMMAND ""
    )

    if (BUILD_SHARED_LIBS)
        add_library(Catch2 SHARED IMPORTED GLOBAL)
        add_library(Catch2Main SHARED IMPORTED GLOBAL)
    else()
        add_library(Catch2 STATIC IMPORTED GLOBAL)
        add_library(Catch2Main STATIC IMPORTED GLOBAL)
    endif()

    if (BUILD_SHARED_LIBS AND WIN32)
        set_target_properties(Catch2 PROPERTIES
            IMPORTED_LOCATION "${CATCH2_DLL}"
            IMPORTED_IMPLIB   "${CATCH2_IMPLIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}"
        )
        set_target_properties(Catch2Main PROPERTIES
            IMPORTED_LOCATION "${CATCH2MAIN_DLL}"
            IMPORTED_IMPLIB   "${CATCH2MAIN_IMPLIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}"
        )
    else()
        set_target_properties(Catch2 PROPERTIES
            IMPORTED_LOCATION "${CATCH2_LIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}"
        )
        set_target_properties(Catch2Main PROPERTIES
            IMPORTED_LOCATION "${CATCH2MAIN_LIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${INCPATH}"
        )
    endif()

    target_link_libraries(Catch2Main INTERFACE Catch2)

    add_dependencies(Catch2Main Catch2-external)
    add_dependencies(Catch2 Catch2-external)
endif(ASTFRI_BUILD_TESTS)
