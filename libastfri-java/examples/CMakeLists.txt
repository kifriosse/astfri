# Modified example from tree-sitter documentation ---
# links libastfri-java which links tree-sitter and tree-sitter-java
add_executable(ts-java java.cpp)
target_compile_options(ts-java PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(ts-java PRIVATE astfri-java tree-sitter tree-sitter-java astfri-text astfri-uml)

add_executable(example_ak main.cpp)
target_link_libraries(example_ak PRIVATE astfri-java)

add_executable(example_to example_text_output.cpp)
target_link_libraries(example_to PRIVATE astfri-java tree-sitter tree-sitter-java astfri-text)

add_executable(example_umlo example_uml_output.cpp)
target_link_libraries(example_umlo PRIVATE astfri-java tree-sitter tree-sitter-java astfri-uml)