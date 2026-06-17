#include <astfri-cs/ASTBuilder.hpp>
#include <astfri-uml/UMLLibWrapper.hpp>
#include <astfri/Astfri.hpp>

#include <iostream>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Provide path to a directory with C# source files.\n";
        return 1;
    }
    astfri::csharp::ASTBuilder astBuilder;
    astfri::TranslationUnit* ast = astBuilder.mk_ast();
    astBuilder.load_src(argv[1]);
    astfri::uml::UMLLibWrapper umlLibWrapper;
    astfri::uml::PlantUMLOutputter plantUMLOutputter;
    astfri::uml::Config config = astfri::uml::Config::createDefault();
    umlLibWrapper.init(config, plantUMLOutputter);
    umlLibWrapper.run(*ast);
}
