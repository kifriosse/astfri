add_library(astfri-serialize SHARED)

target_sources(astfri-serialize PRIVATE
    AstFriDeSerializer.cpp
    JsonFormatChecker.cpp
)

target_include_directories(astfri-serialize PUBLIC
    ${PROJECT_SOURCE_DIR}/external
)

target_link_libraries(astfri-serialize PUBLIC astfri-core)
