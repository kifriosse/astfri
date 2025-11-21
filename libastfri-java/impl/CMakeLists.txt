add_library(astfri-java SHARED)

target_sources(astfri-java PRIVATE
    ASTBuilder.cpp
    ExpressionTransformer.cpp
    NodeMapper.cpp
    StatementTransformer.cpp
)

target_link_directories(astfri-java PUBLIC
    ${TREE_SITTER_LIBRARY_DIRS}
    ${TREE_SITTER_JAVA_LIBRARY_DIRS}
)

target_link_libraries(astfri-java PUBLIC
    astfri-core
    ${TREE_SITTER_LIBRARIES}
    ${TREE_SITTER_JAVA_LIBRARIES}
)

target_include_directories(astfri-java PUBLIC ${TREE_SITTER_JAVA_INCLUDE_DIRS})
