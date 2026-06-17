#include <astfri-java/ASTBuilder.hpp>
#include <astfri-text/TextLibManager.hpp>

#include <iostream>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Provide path to a file with Java source code.\n";
        return 1;
    }
    astfri::java::ASTBuilder tb;
    std::string sourceCode = tb.load_file(argv[1]);
    TSTree* tree = tb.make_syntax_tree(sourceCode);
    astfri::TranslationUnit* tu = tb.get_translation_unit(tree, sourceCode);
    astfri::text::TextLibManager visitor;
    visitor.process_and_export_ast(*tu, &std::cout);

    // freeing memory
    ts_tree_delete(tree); // TODO MM: RAII
}
