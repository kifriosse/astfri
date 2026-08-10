#include <astfri-cs/ASTBuilder.hpp>
#include <astfri-uml/UMLLibWrapper.hpp>

#include <iostream>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Provide path to a directory with C# source files.\n";
        return 1;
    }

    auto astBuilder = astfri::csharp::ASTBuilder::create(astfri::csharp::Config::create_default());
    astfri::TranslationUnit ast = astBuilder.load_project(argv[1])[0];

    astfri::uml::UMLLibWrapper umlLib;
    astfri::uml::PlantUMLOutputter plantUMLOut;
    astfri::uml::Config config = astfri::uml::Config::create_default();
    umlLib.init(config, plantUMLOut);
    umlLib.run(ast);
}
