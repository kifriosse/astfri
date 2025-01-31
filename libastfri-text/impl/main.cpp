#include <libastfri-text/impl/ASTVisitor.cpp>
#include <libastfri-text/inl/ASTLoader.inl>

int main() {
    std::stringstream* output = new std::stringstream();
    const Configurator* config = new Configurator();
    ASTVisitor* visitor = new ASTVisitor(config, output);

    my_ast_trees::load_ast_tree_2(*visitor);
    visitor->write_file();

    delete visitor;
    delete config;
    delete output;
}