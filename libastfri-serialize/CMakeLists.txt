set(ASTFRI_SERIALIZE_VERSION "0.0.2")

add_library(astfri-serialize)

target_sources(astfri-serialize PRIVATE
    astfri-serialize/impl/AstFriDeSerializer.cpp
    astfri-serialize/impl/JsonFormatChecker.cpp
)

target_sources(astfri-serialize PUBLIC FILE_SET headers TYPE HEADERS FILES
    astfri-serialize/AstFriDeSerializer.hpp
    astfri-serialize/Data.hpp
    astfri-serialize/JsonFormatChecker.hpp
)

target_compile_definitions(astfri-serialize PUBLIC ASTFRI_SERIALIZE_VERSION="${ASTFRI_SERIALIZE_VERSION}")

target_include_directories(astfri-serialize PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(astfri-serialize PUBLIC astfri-core rapidjson)
