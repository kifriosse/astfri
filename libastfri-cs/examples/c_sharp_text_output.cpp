#include <libastfri-cs/inc/CSharpASTBuilder.hpp>
#include <libastfri-text/inc/TextLibManager.hpp>

int main()
{
    const astfri::csharp::CSharpASTBuilder ast_builder;
    astfri::text::TextLibManager& manager
        = astfri::text::TextLibManager::get_instance();
    const astfri::TranslationUnit* ast = ast_builder.make_ast(
        "/home/andrew/Coding/astfri/resources/samples/csharp"
    );
    manager.visit_and_export(*ast);
}
