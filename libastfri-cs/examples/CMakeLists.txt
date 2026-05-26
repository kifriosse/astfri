add_executable(astfri-cs-example-text c_sharp_text_output.cpp)

add_executable(astfri-cs-example-uml c_sharp_uml_output.cpp)

target_sources(astfri-cs-example-text PRIVATE
    c_sharp_text_output.cpp
)

target_sources(astfri-cs-example-uml PRIVATE
    c_sharp_uml_output.cpp
)

target_link_libraries(astfri-cs-example-text PRIVATE
    astfri-cs
    astfri-text
)

target_link_libraries(astfri-cs-example-uml PRIVATE
    astfri-cs
    astfri-uml
)