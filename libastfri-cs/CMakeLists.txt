add_library(astfri-cs)

target_sources(astfri-cs PRIVATE
        astfri-cs/impl/ASTBuilder.cpp
        astfri-cs/impl/Config.cpp
        astfri-cs/impl/SDKProfile.cpp
        astfri-cs/impl/data/CSModifiers.cpp
        astfri-cs/impl/data/Identifiers.cpp
        astfri-cs/impl/data/Metadata.cpp
        astfri-cs/impl/data/SourceFile.cpp
        astfri-cs/impl/dispatchers/SrcCodeTransformer.cpp
        astfri-cs/impl/dispatchers/SrcCodeTransformerDef.cpp
        astfri-cs/impl/dispatchers/SrcCodeTransformerExpr.cpp
        astfri-cs/impl/dispatchers/SrcCodeTransformerStmt.cpp
        astfri-cs/impl/dispatchers/SrcCodeTransformerUtil.cpp
        astfri-cs/impl/dispatchers/SymbTableBuilder.cpp
        astfri-cs/impl/dispatchers/TypeTranslator.cpp
        astfri-cs/impl/registries/Maps.cpp
        astfri-cs/impl/registries/QueryRegistry.cpp
        astfri-cs/impl/symbols/SemanticContext.cpp
        astfri-cs/impl/symbols/SymbolTable.cpp
        astfri-cs/impl/symbols/SymbolTree.cpp
        astfri-cs/impl/util/AstfriUtil.cpp
        astfri-cs/impl/util/TSUtil.cpp
        astfri-cs/impl/util/Utils.cpp
)

target_sources(astfri-cs PUBLIC FILE_SET headers TYPE HEADERS FILES
        astfri-cs/ASTBuilder.hpp
        astfri-cs/Config.hpp
        astfri-cs/SDKProfile.hpp
        astfri-cs/impl/ASTBuilder.inl
        astfri-cs/impl/CSFwd.hpp
        astfri-cs/impl/SemanticContext.inl
        astfri-cs/impl/data/AccessType.hpp
        astfri-cs/impl/data/CSModifiers.hpp
        astfri-cs/impl/data/Identifiers.hpp
        astfri-cs/impl/data/Identifiers.inl
        astfri-cs/impl/data/Metadata.hpp
        astfri-cs/impl/data/SourceFile.hpp
        astfri-cs/impl/dispatchers/SrcCodeTransformer.hpp
        astfri-cs/impl/dispatchers/SymbTableBuilder.hpp
        astfri-cs/impl/dispatchers/TypeTranslator.hpp
        astfri-cs/impl/registries/Maps.hpp
        astfri-cs/impl/registries/Maps.inl
        astfri-cs/impl/registries/NodeType.hpp
        astfri-cs/impl/registries/QueryRegistry.hpp
        astfri-cs/impl/symbols/SemanticContext.hpp
        astfri-cs/impl/symbols/SymbolTable.hpp
        astfri-cs/impl/symbols/SymbolTable.inl
        astfri-cs/impl/symbols/SymbolTree.hpp
        astfri-cs/impl/symbols/SymbolTree.inl
        astfri-cs/impl/util/AstfriUtil.hpp
        astfri-cs/impl/util/Common.hpp
        astfri-cs/impl/util/Common.inl
        astfri-cs/impl/util/RapidJsonUtil.hpp
        astfri-cs/impl/util/RapidJsonUtil.inl
        astfri-cs/impl/util/TSUtil.hpp
        astfri-cs/impl/util/TSUtil.inl
        astfri-cs/impl/util/Utils.hpp
)

target_compile_definitions(astfri-cs PRIVATE
        ASTFRI_CS_RESOURCES="${CMAKE_BINARY_DIR}/libastfri-cs/resources/external-types/" # TODO
)

target_include_directories(astfri-cs PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(astfri-cs PUBLIC
        astfri-core
        astfri-common
        rapidjson
        tree-sitter
        tree-sitter-c-sharp
)

apply_common_properties(astfri-cs)

# TODO
add_custom_command(
        TARGET astfri-cs POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/libastfri-cs/resources/"
        "${CMAKE_BINARY_DIR}/libastfri-cs/resources"
        COMMENT "Coping astfri-cs resources"
)

if (ASTFRI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
