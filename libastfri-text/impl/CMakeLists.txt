add_library(astfri-text)

target_sources(astfri-text PRIVATE
  pseudocode/PlainTextBuilder.cpp
  pseudocode/PseudocodeBuilder.cpp
  pseudocode/PseudocodeStmtVisitor.cpp
  pseudocode/PseudocodeVisitor.cpp
  tools/AbstractBuilder.cpp
  tools/AbstractVisitor.cpp
  tools/Exporter.cpp
  TextLibConfig.cpp
  TextLibManager.cpp
)

target_include_directories(astfri-text PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(astfri-text PUBLIC astfri-core rapidjson)