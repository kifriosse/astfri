add_library(astfri-text SHARED)

target_sources(astfri-text PRIVATE
  code/CxxCodeVisitor.cpp
  code/JavaCodeVisitor.cpp
  pseudocode/Exporter.cpp
  pseudocode/HtmlExporter.cpp
  pseudocode/PseudocodeVisitor.cpp
  AstfriVisitor.cpp
  GeneralConfigurator.cpp
  OutputFileManager.cpp
  TextConfigurator.cpp
  TextLibManager.cpp
)

target_include_directories(astfri-text PUBLIC
    ${PROJECT_SOURCE_DIR}/external
)

target_link_libraries(astfri-text PUBLIC astfri-core)