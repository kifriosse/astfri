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

target_include_directories(astfri-uml PUBLIC ${RAPIDJSON_INCLUDE_DIRS})

target_link_libraries(astfri-uml PUBLIC astfri-core)
