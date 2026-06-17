#include <astfri-java/ASTBuilder.hpp>
#include <astfri-uml/UMLLibWrapper.hpp>

#include <iostream>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Provide path to a file with Java source code.\n";
        return 1;
    }
    astfri::java::ASTBuilder astBuilder;
    std::string sourceCode = astBuilder.load_file(argv[1]);
    TSTree* tree = astBuilder.make_syntax_tree(sourceCode);
    astfri::TranslationUnit* tu = astBuilder.get_translation_unit(tree, sourceCode);
    astfri::uml::Config config = astfri::uml::Config::create_default();
    astfri::uml::PlantUMLOutputter plantUMLOut;
    astfri::uml::UMLLibWrapper umlLib;
    umlLib.init(config, plantUMLOut);
    umlLib.run(*tu);

    // freeing memory
    ts_tree_delete(tree); // TODO MM: RAII
}
