set(ASTFRI_CS_VERSION "0.1.0")

add_library(astfri-cs)

target_sources(astfri-cs PRIVATE
    astfri-cs/impl/ASTBuilder.cpp
    astfri-cs/impl/SemanticContext.cpp
    astfri-cs/impl/Config.cpp
    astfri-cs/impl/regs/Maps.cpp
    astfri-cs/impl/regs/QueryRegistry.cpp
    astfri-cs/impl/util/TSUtil.cpp
    astfri-cs/impl/util/Utils.cpp
    astfri-cs/impl/util/AstfriUtil.cpp
    astfri-cs/impl/data/SymbolTable.cpp
    astfri-cs/impl/data/CSModifiers.cpp
    astfri-cs/impl/data/SourceFile.cpp
    astfri-cs/impl/data/Metadata.cpp
    astfri-cs/impl/data/SymbolTree.cpp
    astfri-cs/impl/data/Identifiers.cpp
    astfri-cs/impl/visitors/SymbTableBuilder.cpp
    astfri-cs/impl/visitors/TypeTranslator.cpp
    astfri-cs/impl/visitors/src_code/SrcCodeTransformerExpr.cpp
    astfri-cs/impl/visitors/src_code/SrcCodeTransformer.cpp
    astfri-cs/impl/visitors/src_code/SrcCodeTransformerUtil.cpp
    astfri-cs/impl/visitors/src_code/SrcCodeTransformerStmt.cpp
    astfri-cs/impl/visitors/src_code/SrcCodeTransformerDef.cpp
)

target_sources(astfri-cs PUBLIC FILE_SET headers TYPE HEADERS FILES
    astfri-cs/impl/CSFwd.hpp
    astfri-cs/impl/regs/QueryRegistry.hpp
    astfri-cs/impl/regs/NodeType.hpp
    astfri-cs/impl/regs/Maps.hpp
    astfri-cs/impl/regs/Maps.inl
    astfri-cs/impl/SemanticContext.inl
    astfri-cs/impl/util/TSUtil.hpp
    astfri-cs/impl/util/Common.inl
    astfri-cs/impl/util/AstfriUtil.hpp
    astfri-cs/impl/util/Common.hpp
    astfri-cs/impl/util/Utils.hpp
    astfri-cs/impl/util/TSUtil.inl
    astfri-cs/impl/util/RapidJsonUtil.inl
    astfri-cs/impl/util/RapidJsonUtil.hpp
    astfri-cs/impl/data/SourceFile.hpp
    astfri-cs/impl/data/Identifiers.inl
    astfri-cs/impl/data/SymbolTable.hpp
    astfri-cs/impl/data/Identifiers.hpp
    astfri-cs/impl/data/SymbolTree.inl
    astfri-cs/impl/data/SymbolTree.hpp
    astfri-cs/impl/data/SymbolTable.inl
    astfri-cs/impl/data/AccessType.hpp
    astfri-cs/impl/data/Metadata.hpp
    astfri-cs/impl/data/CSModifiers.hpp
    astfri-cs/impl/ASTBuilder.inl
    astfri-cs/impl/visitors/SymbTableBuilder.hpp
    astfri-cs/impl/visitors/TypeTranslator.hpp
    astfri-cs/impl/visitors/src_code/SrcCodeTransformer.hpp
    astfri-cs/impl/SemanticContext.hpp
    astfri-cs/SDKProfile.hpp
    astfri-cs/ASTBuilder.hpp
    astfri-cs/Config.hpp
)

target_compile_definitions(astfri-cs PUBLIC
    ASTFRI_CS_VERSION="${ASTFRI_CS_VERSION}"
)

target_compile_definitions(astfri-cs PRIVATE
    ASTFRI_CS_RESOURCES="${CMAKE_BINARY_DIR}/libastfri-cs/resources/external-types/" # TODO
)

target_link_libraries(astfri-cs PUBLIC
    astfri-core
    rapidjson
    tree-sitter
    tree-sitter-c-sharp
)

target_include_directories(astfri-cs PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# TODO
add_custom_command(
    TARGET astfri-cs POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/libastfri-cs/resources/"
    "${CMAKE_BINARY_DIR}/libastfri-cs/resources"
    COMMENT "Coping astfri-cs resources"
)

if(ASTFRI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
