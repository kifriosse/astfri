add_library(astfri-cs SHARED)

target_sources(astfri-cs PRIVATE
    CSharpTSTreeVisitor.cpp
    utils.cpp
    NodeRegistry.cpp
    CSharpASTBuilder.cpp
    TypeContext.cpp
)

target_link_directories(astfri-cs PUBLIC
    ${TREE_SITTER_LIBRARY_DIRS}
    ${TREE_SITTER_C_SHARP_LIBRARY_DIRS}
)

target_link_libraries(astfri-cs PUBLIC
    astfri-core
    ${TREE_SITTER_LIBRARIES}
    ${TREE_SITTER_C_SHARP_LIBRARIES}
)

target_include_directories(astfri-cs PUBLIC ${TREE_SITTER_C_SHARP_INCLUDE_DIRS})
