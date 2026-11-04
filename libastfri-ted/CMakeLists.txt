add_library(astfri-ted)

target_sources(astfri-ted PRIVATE
  astfri-ted/impl/TedIndex.cpp
  astfri-ted/impl/ZhangShasha.cpp
)

target_sources(astfri-ted
  PUBLIC FILE_SET headers
    TYPE HEADERS
    FILES
      astfri-ted/Costs.hpp
      astfri-ted/ZhangShasha.hpp
      astfri-ted/impl/TedIndex.hpp
)

target_include_directories(astfri-ted PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

apply_common_properties(astfri-ted)

if (ASTFRI_BUILD_TESTS)
  add_subdirectory(tests)
endif()