#include <libastfri-text/impl/HtmlFileExporter.cpp>
#include <libastfri-text/impl/PdfFileExporter.cpp>
#include <libastfri-text/impl/RtfFileExporter.cpp>
#include <libastfri-text/impl/TxtFileExporter.cpp>
#include <libastfri-text/impl/Configurator.cpp>
#include <libastfri-text/impl/ASTVisitor.cpp>
#include <libastfri-text/impl/Exporter.cpp>
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