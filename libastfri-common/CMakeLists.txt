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

target_compile_options(astfri-common PUBLIC
  -Wall
  -Wextra
  -Wpedantic
  -std=c++20)

target_link_libraries(astfri-common PUBLIC rapidjson)
