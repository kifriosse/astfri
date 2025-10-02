#include "ASTLoader.hpp"
#include <libastfri-text/inc/ASTVisitor.hpp>

int main()
{
    std::unique_ptr<ASTVisitor> visitor = std::make_unique<ASTVisitor>();
    my_ast_trees::load_ast_tree_1(*visitor);
    visitor->write_file();
    return 0;
}
