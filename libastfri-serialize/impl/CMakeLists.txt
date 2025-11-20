add_library(astfri-serialize SHARED)

target_sources(astfri-serialize PRIVATE
    AstFriDeSerializer.cpp
    JsonFormatChecker.cpp
)

# TODO tmp solution
set(RAPIDJSON_INCLUDE_DIRS ${EXTERNAL_INSTALL_DIR}/include)

target_include_directories(astfri-serialize PUBLIC ${RAPIDJSON_INCLUDE_DIRS})

target_link_libraries(astfri-serialize PUBLIC astfri-core)
