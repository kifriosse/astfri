set(ASTFRI_TEXT_VERSION "0.1.0")

add_library(astfri-text)

target_sources(astfri-text PRIVATE
    astfri-text/impl/PlainTextBuilder.cpp
    astfri-text/impl/TextLibConfig.cpp
    astfri-text/impl/AbstractVisitor.cpp
    astfri-text/impl/TextLibManager.cpp
    astfri-text/impl/PseudocodeBuilder.cpp
    astfri-text/impl/Exporter.cpp
    astfri-text/impl/PseudocodeStmtVisitor.cpp
    astfri-text/impl/AbstractBuilder.cpp
    astfri-text/impl/PseudocodeVisitor.cpp
)

target_sources(astfri-text PUBLIC FILE_SET headers TYPE HEADERS FILES
    astfri-text/TextLibConfig.hpp
    astfri-text/impl/PseudocodeVisitor.hpp
    astfri-text/impl/PseudocodeBuilder.hpp
    astfri-text/impl/PlainTextBuilder.hpp
    astfri-text/impl/AbstractBuilder.hpp
    astfri-text/impl/Exporter.hpp
    astfri-text/impl/AbstractVisitor.hpp
    astfri-text/TextLibManager.hpp
)

target_compile_definitions(astfri-text PUBLIC ASTFRI_TEXT_VERSION="${ASTFRI_TEXT_VERSION}")

target_include_directories(astfri-text PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(astfri-text PUBLIC astfri-core astfri-common rapidjson)

if (ASTFRI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
