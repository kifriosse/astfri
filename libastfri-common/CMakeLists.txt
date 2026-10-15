add_library(astfri-common)

target_sources(astfri-common PRIVATE
  astfri-common/impl/RapidjsonUtils.cpp
)

target_sources(astfri-common PUBLIC FILE_SET headers TYPE HEADERS FILES
  astfri-common/RapidjsonUtils.hpp
)

target_include_directories(astfri-common PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(astfri-common PUBLIC rapidjson)

apply_common_properties(astfri-common)
