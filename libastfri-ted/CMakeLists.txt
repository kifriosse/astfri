add_library(astfri-ted)

target_sources(astfri-ted PRIVATE
  astfri-ted/impl/TedIndex.cpp
  astfri-ted/impl/ZhangShasha.cpp
  astfri-ted/impl/NodeReference.cpp
  astfri-ted/impl/Config.cpp
  astfri-ted/impl/NodeComparator.cpp
  astfri-ted/impl/TedIndexBuilder.cpp
)

target_sources(astfri-ted
  PUBLIC FILE_SET headers
    TYPE HEADERS
    FILES
      astfri-ted/Costs.hpp
      astfri-ted/ZhangShasha.hpp
      astfri-ted/Config.hpp

      astfri-ted/impl/TedIndex.hpp
      astfri-ted/impl/NodeReference.hpp
      astfri-ted/impl/AttributeConfig.hpp
      astfri-ted/impl/NodeComparator.hpp
      astfri-ted/impl/TedIndexBuilder.hpp
) 

target_include_directories(astfri-ted PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(astfri-ted PUBLIC astfri-core astfri-common rapidjson)

apply_common_properties(astfri-ted)

if (ASTFRI_BUILD_TESTS)
  add_subdirectory(tests)
endif()