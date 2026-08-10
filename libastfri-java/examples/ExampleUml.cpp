#include <astfri-java/ASTBuilder.hpp>
#include <astfri-uml/UMLLibWrapper.hpp>

#include <iostream>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Provide path to a file with Java source code.\n";
        return 1;
    }
    auto astBuilder = astfri::java::ASTBuilder::create(astfri::java::Config::create_default());
    astfri::TranslationUnit tu = astBuilder.load_file(std::string(argv[1]));
    astfri::uml::Config config = astfri::uml::Config::create_default();
    astfri::uml::PlantUMLOutputter plantUMLOut;
    astfri::uml::UMLLibWrapper umlLib;
    umlLib.init(config, plantUMLOut);
    umlLib.run(tu);
}
