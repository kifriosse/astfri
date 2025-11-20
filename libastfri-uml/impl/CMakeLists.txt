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

# TODO tmp solution
set(RAPIDJSON_INCLUDE_DIRS ${EXTERNAL_INSTALL_DIR}/include)

target_include_directories(astfri-uml PUBLIC ${RAPIDJSON_INCLUDE_DIRS})

target_link_libraries(astfri-uml PUBLIC astfri-core)
