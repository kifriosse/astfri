add_library(astfri-text)

target_sources(astfri-text PRIVATE
  astfri-text/impl/AbstractBuilder.cpp
  astfri-text/impl/AbstractVisitor.cpp
  astfri-text/impl/Config.cpp
  astfri-text/impl/Exporter.cpp
  astfri-text/impl/TextLibManager.cpp
  astfri-text/impl/pseudocode/HtmlTextBuilder.cpp
  astfri-text/impl/pseudocode/PlainTextBuilder.cpp
  astfri-text/impl/pseudocode/PseudocodeBuilder.cpp
  astfri-text/impl/pseudocode/PseudocodeStmtVisitor.cpp
  astfri-text/impl/pseudocode/PseudocodeVisitor.cpp
)

target_sources(astfri-text PUBLIC FILE_SET headers TYPE HEADERS FILES
  astfri-text/impl/pseudocode/HtmlTextBuilder.hpp
  astfri-text/impl/pseudocode/PlainTextBuilder.hpp
  astfri-text/impl/pseudocode/PseudocodeBuilder.hpp
  astfri-text/impl/pseudocode/PseudocodeVisitor.hpp
  astfri-text/impl/AbstractBuilder.hpp
  astfri-text/impl/AbstractVisitor.hpp
  astfri-text/impl/Exporter.hpp
  astfri-text/TextLibManager.hpp
  astfri-text/Config.hpp
)

target_include_directories(astfri-text PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(astfri-text PUBLIC astfri-core astfri-common rapidjson)

apply_common_properties(astfri-text)

if (ASTFRI_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
