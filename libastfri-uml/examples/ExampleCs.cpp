#include <astfri-cs/ASTBuilder.hpp>
#include <astfri-uml/UMLLibWrapper.hpp>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return -1;
    }

    astfri::csharp::ASTBuilder builder;
    builder.load_src(argv[1]);

    astfri::TranslationUnit *tu = builder.mk_ast();

    astfri::uml::Config conf = astfri::uml::Config::create_default();
    astfri::uml::PlantUMLOutputter op;
    astfri::uml::UMLLibWrapper umlout;
    umlout.init(conf, op);
    umlout.run(*tu);

    return 0;
}
