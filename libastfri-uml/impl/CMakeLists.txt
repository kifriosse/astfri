add_library(astfri-uml)
target_sources(astfri-uml PRIVATE
    UMLOutputter.cpp
    PlantUMLOutputter.cpp
    ClassVisitor.cpp
    TypeConvention.cpp
)
target_link_libraries(astfri-uml PRIVATE astfri-core)