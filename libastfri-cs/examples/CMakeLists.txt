add_executable(astfri-cs-examples)

target_sources(astfri-cs-examples PRIVATE
        c_sharp_text_output.cpp
)

target_link_libraries(astfri-cs-examples PRIVATE
        astfri-cs
        astfri-text
)