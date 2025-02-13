#include <libastfri-text/inc/HtmlFileExporter.hpp>
#include <libastfri-text/inc/PdfFileExporter.hpp>
#include <libastfri-text/inc/RtfFileExporter.hpp>
#include <libastfri-text/inc/TxtFileExporter.hpp>
#include <libastfri-text/inc/Configurator.hpp>
#include <libastfri-text/inc/ASTVisitor.hpp>
#include <libastfri-text/inc/Exporter.hpp>
#include <libastfri-text/inl/ASTLoader.inl>

int main() {
    std::stringstream* output = new std::stringstream();
    const Configurator* config = new Configurator("conf.json");
    ASTVisitor* visitor = new ASTVisitor(config, output);

    my_ast_trees::load_ast_tree_2(*visitor);
    visitor->write_file();

    delete visitor;
    delete config;
    delete output;
}