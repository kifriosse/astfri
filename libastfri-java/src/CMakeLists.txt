find_package(TreeSitter REQUIRED)
find_package(TreeSitterJava REQUIRED)

add_library(astfri-java SHARED dummy.cpp)

target_link_libraries(
  astfri-java PUBLIC
    astfri-core
    tree-sitter
    tree-sitter-java)
