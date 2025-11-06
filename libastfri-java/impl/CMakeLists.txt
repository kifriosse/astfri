# find_package(TreeSitter REQUIRED)

# find_package(TreeSitterJava REQUIRED)

add_library(astfri-java SHARED)

target_sources(astfri-java PRIVATE
    ASTBuilder.cpp
    ExpressionTransformer.cpp
    NodeMapper.cpp
    StatementTransformer.cpp
)

target_link_libraries(astfri-java PUBLIC
    astfri-core
    tree-sitter
    tree-sitter-java
)
