add_library(astfri-serialize SHARED)

target_sources(astfri-serialize PRIVATE
    AstFriDeSerializer.cpp
    JsonFormatChecker.cpp
)

target_include_directories(astfri-serialize PUBLIC ${RAPIDJSON_INCLUDE_DIRS})

target_link_libraries(astfri-serialize PUBLIC astfri-core)
