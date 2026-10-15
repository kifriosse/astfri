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

target_include_directories(astfri-serialize PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(astfri-serialize PUBLIC astfri-core rapidjson)

apply_common_properties(astfri-serialize)
