find_package(TreeSitter REQUIRED)
find_package(TreeSitterJava REQUIRED)

add_library(libastfri-java SHARED dummy.cpp)

target_link_libraries(libastfri-java PUBLIC tree-sitter tree-sitter-java)
