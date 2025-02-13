add_executable(app_mp
  main.cpp
  ASTVisitor.cpp
  Configurator.cpp
  Exporter.cpp
  HtmlFileExporter.cpp
  PdfFileExporter.cpp
  RtfFileExporter.cpp
  TxtFileExporter.cpp
)

target_compile_options(app_mp PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(app_mp PRIVATE astfri-core)
target_include_directories(app_mp PUBLIC ${PROJECT_SOURCE_DIR}/lib)