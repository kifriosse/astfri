# Example from tree-sitter documentation
# add_executable(ts-json json.cpp)
# target_compile_options(ts-json PRIVATE -Wall -Wextra -Wpedantic)
# target_link_libraries(ts-json PRIVATE tree-sitter tree-sitter-json)

# Modified example from tree-sitter documentation ---
# links libastfri-java which links tree-sitter and tree-sitter-java
add_executable(ts-java java.cpp)
target_compile_options(ts-java PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(ts-java PRIVATE astfri-java tree-sitter tree-sitter-java)

add_executable(example_ak main.cpp)
target_link_libraries(example_ak PRIVATE astfri-java)
