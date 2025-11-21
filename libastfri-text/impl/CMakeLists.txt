add_library(astfri-text SHARED)

target_sources(astfri-text PRIVATE
  code/AbstractCodeBuilder.cpp
  code/AbstractCodeVisitor.cpp
  code/CxxCodeBuilder.cpp
  code/CxxCodeVisitor.cpp
  code/JavaCodeBuilder.cpp
  code/JavaCodeVisitor.cpp
  pseudocode/AbstractTextBuilder.cpp
  pseudocode/GeneralConfigurator.cpp
  pseudocode/HtmlTextBuilder.cpp
  pseudocode/PlainTextBuilder.cpp
  pseudocode/PseudocodeVisitor.cpp
  pseudocode/TextConfigurator.cpp
  AbstractBuilder.cpp
  AbstractVisitor.cpp
  OutputFileManager.cpp
  TextLibManager.cpp
)

target_include_directories(astfri-text PUBLIC ${RAPIDJSON_INCLUDE_DIRS})

target_link_libraries(astfri-text PUBLIC astfri-core)
