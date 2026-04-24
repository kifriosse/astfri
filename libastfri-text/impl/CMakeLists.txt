add_library(astfri-text SHARED)

target_sources(astfri-text PRIVATE
  general/AbstractBuilder.cpp
  general/AbstractVisitor.cpp
  pseudocode/AbstractTextBuilder.cpp
  pseudocode/PlainTextBuilder.cpp
  pseudocode/PseudocodeVisitor.cpp
  settings/BaseStructure.cpp
  settings/GeneralConfig.cpp
  settings/OutputFileManager.cpp
  settings/TextConfig.cpp
)

target_include_directories(astfri-text PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(astfri-text PUBLIC astfri-core rapidjson)