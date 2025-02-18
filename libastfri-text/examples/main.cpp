#include <libastfri-text/inc/ASTVisitor.hpp>
#include <libastfri-text/examples/ASTLoader.cpp>

int main() {
    std::unique_ptr<ASTVisitor> visitor = std::make_unique<ASTVisitor>();
    my_ast_trees::load_ast_tree_2(*visitor);
    visitor->write_file();
}