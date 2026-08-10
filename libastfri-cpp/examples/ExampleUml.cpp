#include <astfri-cpp/ASTBuilder.hpp>
#include <astfri-uml/UMLLibWrapper.hpp>

#include <iostream>


int main(int argc, const char** argv) {
    if (argc < 2) {
        std::cerr << "Provide path to a file with C++ source code.\n";
        return 1;
    }
    auto astBuilder = astfri::cpp::ASTBuilder::create(astfri::cpp::Config::create_default());
    astfri::TranslationUnit tu = astBuilder.load_file(std::string(argv[1]));
    astfri::uml::Config config = astfri::uml::Config::create_default();
    astfri::uml::PlantUMLOutputter plantUMLOut;
    astfri::uml::UMLLibWrapper umlLib;
    umlLib.init(config, plantUMLOut);
    umlLib.run(tu);
}
