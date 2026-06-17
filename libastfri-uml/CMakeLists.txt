set(ASTFRI_UML_VERSION "0.1.0")

add_library(astfri-uml)

target_sources(astfri-uml PRIVATE
    astfri-uml/impl/Config.cpp
    astfri-uml/impl/UMLLibWrapper.cpp
    astfri-uml/impl/PlantUMLOutputter.cpp
    astfri-uml/impl/TypeConvention.cpp
    astfri-uml/impl/ElementStructs.cpp
    astfri-uml/impl/ClassVisitor.cpp
    astfri-uml/impl/UMLOutputter.cpp
)

target_sources(astfri-uml PUBLIC FILE_SET headers TYPE HEADERS FILES
    astfri-uml/impl/ElementStructs.hpp
    astfri-uml/impl/ClassVisitor.hpp
    astfri-uml/impl/TypeConvention.hpp
    astfri-uml/impl/PlantUMLOutputter.hpp
    astfri-uml/impl/Config.hpp
    astfri-uml/impl/UMLOutputter.hpp
    astfri-uml/UMLLibWrapper.hpp
)

target_compile_definitions(astfri-uml PUBLIC ASTFRI_UML_VERSION="${ASTFRI_UML_VERSION}")

target_include_directories(astfri-text PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(astfri-uml PUBLIC astfri-core astfri-common rapidjson)

if (ASTFRI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
