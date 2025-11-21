#include <libastfri-cs/impl/CSharpASTBuilder.hpp>
#include <libastfri-cs/tests/ExpressionTests.hpp>

#include "libastfri-text/inc/TextLibManager.hpp"

int main()
{
    astfri::csharp::CSharpASTBuilder const ast_builder;
    astfri::text::TextLibManager& manager = astfri::text::TextLibManager::get_instance();
    astfri::TranslationUnit const* ast
        = ast_builder.make_ast("/home/andrew/Coding/astfri/libastfri-cs/resources/TestProject");
    manager.visit_and_export(*ast);
}
