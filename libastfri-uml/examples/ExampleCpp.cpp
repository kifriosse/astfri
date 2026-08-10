#include <astfri-cpp/ASTBuilder.hpp>
#include <astfri-uml/UMLLibWrapper.hpp>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return -1;
    }

    auto astBuilder = astfri::cpp::ASTBuilder::create(astfri::cpp::Config::create_default());
    astfri::TranslationUnit tu = astBuilder.load_file(argv[1]);

    astfri::uml::Config conf = astfri::uml::Config::create_default();
    astfri::uml::PlantUMLOutputter op;
    astfri::uml::UMLLibWrapper uml;
    uml.init(conf, op);
    uml.run(tu);

    return 0;
}
