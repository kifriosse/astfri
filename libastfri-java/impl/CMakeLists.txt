set(ASTFRI_JAVA_VERSION "0.0.2")

add_library(astfri-java)

target_compile_definitions(astfri-java PUBLIC ASTFRI_JAVA_VERSION="${ASTFRI_JAVA_VERSION}")

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

target_include_directories(astfri-java PUBLIC ${PROJECT_SOURCE_DIR})
