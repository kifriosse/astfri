add_library(uml-class-visitor)
target_sources(uml-class-visitor PRIVATE
    ClassVisitor.cpp
)
target_link_libraries(uml-class-visitor PRIVATE astfri-core)

add_library(uml-outputter)
target_sources(uml-outputter PRIVATE
    UMLOutputter.cpp
    PlantUMLOutputter.cpp
)
target_link_libraries(uml-outputter PRIVATE astfri-core)