target_sources(astfri-cs PRIVATE
    ASTBuilder.cpp
    SemanticContext.cpp
    CSFwd.hpp
)

add_subdirectory(data)
add_subdirectory(regs)
add_subdirectory(util)
add_subdirectory(visitors)

target_link_libraries(astfri-cs PUBLIC
    astfri-core
    rapidjson
    tree-sitter
    tree-sitter-c-sharp
)

target_include_directories(astfri-cs PUBLIC ${PROJECT_SOURCE_DIR})
