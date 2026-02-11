add_library(astfri-cs SHARED)

target_sources(astfri-cs PRIVATE
    ASTBuilder.cpp
    SemanticContext.cpp
    visitors/SymbolTableBuilder.cpp
    visitors/TypeTranslator.cpp
    CSFwd.hpp
)

add_subdirectory(data)
add_subdirectory(regs)
add_subdirectory(util)
add_subdirectory(visitors/src_code)

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
