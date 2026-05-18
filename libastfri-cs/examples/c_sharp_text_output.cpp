#include <libastfri-cs/inc/ASTBuilder.hpp>
#include <libastfri-text/inc/TextLibManager.hpp>
#include <astfri/Astfri.hpp>

int main() {
    using namespace astfri;
    csharp::ASTBuilder astBuilder;
    text::TextLibManager manager;
    // astBuilder.load_src("../../../libastfri-cs/resources/test_src/");
    astBuilder.load_src("../../../resources/samples/csharp/");
    // for loading file from stream
    // astBuilder.load_src(stream);
    const TranslationUnit* ast = astBuilder.mk_ast();
    manager.process_and_export_ast(*ast);
}
