#include <libastfri-cs/inc/ASTBuilder.hpp>
#include <libastfri-uml/inc/UMLLibWrapper.hpp>

int main()
{
    using namespace astfri;

    csharp::ASTBuilder builder;
    builder.load_src("../../random/MonteCarlo");

    TranslationUnit* tu = builder.mk_ast();

    astfri::uml::Config conf;
    astfri::uml::PlantUMLOutputter op;
    astfri::uml::UMLLibWrapper umlout;
    umlout.init(conf, op);
    umlout.run(*tu);
}
