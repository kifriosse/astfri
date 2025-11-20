#include <libastfri-cs/impl/CSharpASTBuilder.hpp>
#include <libastfri-cs/tests/ExpressionTests.hpp>
#include <libastfri-text/inc/TextConfigurator.hpp>

#include "libastfri-text/inc/TextLibManager.hpp"

int main()
{
    astfri::csharp::CSharpASTBuilder ast_builder;
    astfri::text::TextLibManager& manager     = astfri::text::TextLibManager::get_instance();
    std::vector<astfri::TranslationUnit*> ast = ast_builder.make_ast(
        "/home/andrew/Coding/Github/astfri/libastfri-cs/resources/TestProject"
    );
    for (astfri::TranslationUnit const* comp_unit : ast)
    {
        manager.visit_and_export(*comp_unit);
    }
}
