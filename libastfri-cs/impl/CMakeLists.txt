add_library(astfri-cs SHARED
    CSAliases.hpp)

target_sources(astfri-cs PRIVATE
    ASTBuilder.cpp
    Registries.cpp
    SemanticContext.cpp
    SymbolTableBuilder.cpp
)

add_subdirectory(data)
add_subdirectory(util)
add_subdirectory(visitor)

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
