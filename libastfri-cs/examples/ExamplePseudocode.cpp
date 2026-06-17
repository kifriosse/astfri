#include <astfri-cs/ASTBuilder.hpp>
#include <astfri-text/TextLibManager.hpp>
#include <astfri/Astfri.hpp>

#include <iostream>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Provide path to a directory with C# source files.\n";
        return 1;
    }
    astfri::csharp::ASTBuilder astBuilder;
    astBuilder.load_src(argv[1]);
    const astfri::TranslationUnit* ast = astBuilder.mk_ast();
    astfri::text::TextLibManager manager;
    manager.process_and_export_ast(*ast, &std::cout);
}
