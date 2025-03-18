add_library(astfri-text SHARED
  ASTVisitor.cpp
  Configurator.cpp
  TextConfigurator.cpp
  Exporter.cpp
  HtmlFileExporter.cpp
  TxtFileExporter.cpp
)

target_compile_options(astfri-text PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(astfri-text PRIVATE astfri-core)
target_include_directories(astfri-text PRIVATE ${PROJECT_SOURCE_DIR}/lib)