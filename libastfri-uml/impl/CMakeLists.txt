set(ASTFRI_UML_VERSION "0.0.2")

add_library(astfri-uml)

target_sources(astfri-uml PRIVATE
    ClassVisitor.cpp
    Config.cpp
    ElementStructs.cpp
    UMLLibWrapper.cpp
    UMLOutputter.cpp
    PlantUMLOutputter.cpp
    TypeConvention.cpp
)

target_compile_definitions(astfri-uml PUBLIC ASTFRI_UML_VERSION="${ASTFRI_UML_VERSION}")

target_include_directories(astfri-uml PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(astfri-uml PUBLIC astfri-core rapidjson)
