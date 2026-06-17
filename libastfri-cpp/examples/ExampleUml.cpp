#include <astfri-cpp/AstfriCpp.hpp>
#include <astfri-uml/UMLLibWrapper.hpp>

#include <iostream>


int main(int argc, const char** argv) {
    if (argc < 2) {
        std::cerr << "Provide path to a file with C++ source code.\n";
        return 1;
    }
    astfri::TranslationUnit tu = astfri::cpp_in::load_file(argv[1], astfri::cpp::Config());
    astfri::uml::Config config = astfri::uml::Config::create_default();
    astfri::uml::PlantUMLOutputter plantUMLOut;
    astfri::uml::UMLLibWrapper umlLib;
    umlLib.init(config, plantUMLOut);
    umlLib.run(tu);
}
