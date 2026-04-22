add_library(astfri-text SHARED)

target_sources(astfri-text PRIVATE
  code/AbstractCodeBuilder.cpp
  code/AbstractCodeVisitor.cpp
  code/CxxCodeBuilder.cpp
  code/CxxCodeVisitor.cpp
  code/JavaCodeBuilder.cpp
  code/JavaCodeVisitor.cpp
  general/AbstractBuilder.cpp
  general/AbstractVisitor.cpp
  pseudocode/AbstractTextBuilder.cpp
  pseudocode/HtmlTextBuilder.cpp
  pseudocode/PlainTextBuilder.cpp
  pseudocode/PlainTextExporter.cpp
  pseudocode/PseudocodeVisitor.cpp
  settings/BaseStructure.cpp
  settings/GeneralConfig.cpp
  settings/OutputFileManager.cpp
  settings/TextConfig.cpp
  TextLibManager.cpp
)

target_include_directories(astfri-text PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(astfri-text PUBLIC astfri-core rapidjson)
