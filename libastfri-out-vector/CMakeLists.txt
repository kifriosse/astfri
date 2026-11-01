add_library(astfri-vector)

target_sources(astfri-vector PRIVATE
    astfri-vector/impl/Config.cpp
    astfri-vector/impl/CharacteristicVector.cpp
)

target_sources(astfri-vector PUBLIC FILE_SET headers TYPE HEADERS FILES
    astfri-vector/Config.hpp
    astfri-vector/impl/CharacteristicVector.hpp
)

target_include_directories(astfri-vector PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(astfri-vector PUBLIC astfri-core astfri-common rapidjson)

apply_common_properties(astfri-vector)

# if (ASTFRI_BUILD_EXAMPLES)
#     add_subdirectory(examples)
# endif()