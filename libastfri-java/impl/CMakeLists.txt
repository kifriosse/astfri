# TODO temporary solution
set(TREE_SITTER_JAVA_INCLUDE_DIRS ${EXTERNAL_INSTALL_DIR}/include)
set(TREE_SITTER_JAVA_LIBRARIES ${EXTERNAL_INSTALL_DIR}/lib64/libtree-sitter-java.so)

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
    ${TREE_SITTER_JAVA_LIBRARIES}
)

target_include_directories(astfri-java PUBLIC ${TREE_SITTER_JAVA_INCLUDE_DIRS})
