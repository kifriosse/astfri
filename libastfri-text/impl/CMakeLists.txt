add_library(astfri-text SHARED)

target_sources(astfri-text PRIVATE
  OutputFileManager.cpp
  GeneralConfigurator.cpp
  TextConfigurator.cpp
  Exporter.cpp
  HtmlFileExporter.cpp
  GeneralASTVisitor.cpp
  ASTTextVisitor.cpp
  TextLibManager.cpp
)

target_include_directories(astfri-text PUBLIC
    ${PROJECT_SOURCE_DIR}/external
)

target_link_libraries(astfri-text PUBLIC astfri-core)