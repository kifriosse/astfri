# TODO temporary solution
set(TREE_SITTER_C_SHARP_INCLUDE_DIRS ${EXTERNAL_INSTALL_DIR}/include)
set(TREE_SITTER_C_SHARP_LIBRARIES ${EXTERNAL_INSTALL_DIR}/lib/libtree-sitter-c-sharp.so)

add_library(astfri-cs SHARED)

target_sources(astfri-cs PRIVATE
        CSharpTSTreeVisitor.cpp
        utils.cpp
        NodeRegistry.cpp
        CSharpASTBuilder.cpp
        TypeContext.cpp
)

target_include_directories(astfri-cs PUBLIC ${TREE_SITTER_C_SHARP_INCLUDE_DIRS})

target_link_libraries(astfri-cs PUBLIC
        astfri-core
        tree-sitter
        ${TREE_SITTER_C_SHARP_LIBRARIES}
)