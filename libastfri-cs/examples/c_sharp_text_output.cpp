#include <libastfri-cs/inc/ASTBuilder.hpp>
#include <libastfri-text/inc/TextLibManager.hpp>
#include <libastfri/inc/Astfri.hpp>

int main()
{
    using namespace astfri;
    csharp::ASTBuilder astBuilder;
    text::TextLibManager& manager = text::TextLibManager::get_instance();
    astBuilder.load_src("/home/andrew/Coding/astfri/resources/samples/csharp");
    // astBuilder.load_src("/home/andrew/Coding/astfri/resources/samples/csharp/UsingDirectives.cs");
    // for loading file from stream
    // astBuilder.load_src(stream);
    const TranslationUnit* ast = astBuilder.mk_ast();
    manager.visit_and_export(*ast);
}
