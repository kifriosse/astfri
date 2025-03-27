find_package(TreeSitter REQUIRED)
find_package(TreeSitterJava REQUIRED)

add_library(astfri-java SHARED NodeMapper.cpp
                               ExpressionTransformer.cpp
                               StatementTransformer.cpp
                               ASTBuilder.cpp)

target_compile_options(astfri-java PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(
  astfri-java PUBLIC
    astfri-core
    tree-sitter
    tree-sitter-java)
