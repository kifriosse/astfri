#include <libastfri-cs/inc/ASTBuilder.hpp>
#include <libastfri-uml/inc/UMLLibWrapper.hpp>
#include <astfri/Astfri.hpp>

int main() {
    using namespace astfri;
    csharp::ASTBuilder astBuilder;
    astBuilder.load_src("../../../resources/samples/csharp/");
    // for loading file from stream
    // astBuilder.load_src(stream);
    TranslationUnit* ast = astBuilder.mk_ast();
    uml::UMLLibWrapper umlLibWrapper;
    uml::PlantUMLOutputter plantUMLOutputter;
    uml::Config config;
    config.innerView_ = true;
    umlLibWrapper.init(config, plantUMLOutputter);
    umlLibWrapper.run(*ast);
}
