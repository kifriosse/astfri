#include <astfri-cpp/AstfriCpp.hpp>
#include <astfri-uml/UMLLibWrapper.hpp>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return -1;
    }

    astfri::TranslationUnit tu = astfri::cpp_in::load_file(argv[1], astfri::cpp::Config());

    astfri::uml::Config conf = astfri::uml::Config::create_default();
    astfri::uml::PlantUMLOutputter op;
    astfri::uml::UMLLibWrapper uml;
    uml.init(conf, op);
    uml.run(tu);

    return 0;
}
