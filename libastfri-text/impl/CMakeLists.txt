add_library(astfri-text SHARED)

target_sources(astfri-text PRIVATE
  pseudocode/PlainTextBuilder.cpp
  pseudocode/PseudocodeBuilder.cpp
  pseudocode/PseudoExprVis.cpp
  pseudocode/PseudoObjectStmtVis.cpp
  pseudocode/PseudoOtherStmtVis.cpp
  pseudocode/PseudoTypeVis.cpp
  tools/AbstractBuilder.cpp
  tools/AbstractVisitor.cpp
  TextLibConfig.cpp
  TextLibManager.cpp
)

target_include_directories(astfri-text PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(astfri-text PUBLIC astfri-core rapidjson)