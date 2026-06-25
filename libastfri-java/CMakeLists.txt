set(ASTFRI_JAVA_VERSION "0.1.0")

add_library(astfri-java)

target_sources(astfri-java PRIVATE
    astfri-java/impl/ASTBuilder.cpp
    astfri-java/impl/Config.cpp
    astfri-java/impl/ExpressionTransformer.cpp
    astfri-java/impl/NodeMapper.cpp
    astfri-java/impl/StatementTransformer.cpp
)

target_sources(astfri-java PUBLIC FILE_SET headers TYPE HEADERS FILES
    astfri-java/impl/NodeMapper.hpp
    astfri-java/impl/StatementTransformer.hpp
    astfri-java/impl/ExpressionTransformer.hpp
    astfri-java/ASTBuilder.hpp
)

target_compile_definitions(astfri-java PUBLIC ASTFRI_JAVA_VERSION="${ASTFRI_JAVA_VERSION}")

target_link_libraries(astfri-java PUBLIC
    astfri-core
    tree-sitter
    tree-sitter-java
    rapidjson
)

target_include_directories(astfri-java PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (ASTFRI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
