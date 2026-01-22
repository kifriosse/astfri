#include <libastfri-cs/inc/ASTBuilder.hpp>
#include <libastfri-text/inc/TextLibManager.hpp>
#include <libastfri/inc/Astfri.hpp>

int main()
{
    const astfri::csharp::ASTBuilder astBuilder;
    astfri::text::TextLibManager& manager
        = astfri::text::TextLibManager::get_instance();
    const astfri::TranslationUnit* ast = astBuilder.make_ast(
        "/home/andrew/Coding/astfri/resources/samples/csharp"
    );
    manager.visit_and_export(*ast);
}
