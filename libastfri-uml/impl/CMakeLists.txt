add_library(astfri-uml)
target_sources(astfri-uml PRIVATE
    UMLLibWrapper.cpp
    UMLOutputter.cpp
    PlantUMLOutputter.cpp
    ClassVisitor.cpp
    TypeConvention.cpp
    ElementStructs.cpp
)
target_link_libraries(astfri-uml PUBLIC astfri-core)