find_package(TreeSitter REQUIRED)

find_package(TreeSitterCSharp REQUIRED)

add_library(astfri-cs SHARED)

target_sources(astfri-cs PRIVATE
        CSharpTSTreeVisitor.cpp
        utils.cpp
        NodeRegistry.cpp
        CSharpASTBuilder.cpp
)

target_link_libraries(astfri-cs PUBLIC
        astfri-core
        tree-sitter
        tree-sitter-c-sharp
)