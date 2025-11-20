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

# TODO tmp solution
set(RAPIDJSON_INCLUDE_DIRS ${EXTERNAL_INSTALL_DIR}/include)

target_include_directories(astfri-text PUBLIC ${RAPIDJSON_INCLUDE_DIRS})

target_link_libraries(astfri-text PUBLIC astfri-core)
