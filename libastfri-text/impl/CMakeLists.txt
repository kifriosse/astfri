add_library(astfri-text SHARED)

target_sources(astfri-text PRIVATE
  ASTTextVisitor.cpp
  Configurator.cpp
  Exporter.cpp
  HtmlFileExporter.cpp
  TextConfigurator.cpp
  GeneralConfigurator.cpp
  OutputFileManager.cpp
)

target_include_directories(astfri-text PUBLIC
    ${PROJECT_SOURCE_DIR}/external
)

target_link_libraries(astfri-text PUBLIC astfri-core)