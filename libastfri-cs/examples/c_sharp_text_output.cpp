#include <libastfri-cs/inc/ASTBuilder.hpp>
#include <libastfri-text/inc/TextLibManager.hpp>
#include <libastfri/inc/Astfri.hpp>

int main()
{
    using namespace astfri;
    const csharp::ASTBuilder astBuilder;
    text::TextLibManager& manager = text::TextLibManager::get_instance();
    const TranslationUnit* ast    = astBuilder.make_ast(
        "/home/andrew/Coding/astfri/resources/samples/csharp"
    );
    manager.visit_and_export(*ast);
}
