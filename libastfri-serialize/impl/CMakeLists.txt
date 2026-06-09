set(ASTFRI_SERIALIZE_VERSION "0.0.2")

add_library(astfri-serialize)

target_compile_definitions(astfri-serialize PUBLIC ASTFRI_SERIALIZE_VERSION="${ASTFRI_SERIALIZE_VERSION}")

target_sources(astfri-serialize PRIVATE
    AstFriDeSerializer.cpp
    JsonFormatChecker.cpp
)

target_include_directories(astfri-serialize PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(astfri-serialize PUBLIC astfri-core rapidjson)
