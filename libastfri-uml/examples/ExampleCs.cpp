#include <astfri-cs/ASTBuilder.hpp>
#include <astfri-uml/UMLLibWrapper.hpp>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return -1;
    }

    auto astBuilder = astfri::csharp::ASTBuilder::create(astfri::csharp::Config::create_default());
    astfri::TranslationUnit tu = astBuilder.load_file(argv[1]);

    astfri::uml::Config conf = astfri::uml::Config::create_default();
    astfri::uml::PlantUMLOutputter op;
    astfri::uml::UMLLibWrapper umlout;
    umlout.init(conf, op);
    umlout.run(tu);

    return 0;
}
