#include <astfri-java/ASTBuilder.hpp>
#include <astfri-text/TextLibManager.hpp>

#include <iostream>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Provide path to a file with Java source code.\n";
        return 1;
    }
    auto astBuilder = astfri::java::ASTBuilder::create(astfri::java::Config::create_default());
    astfri::TranslationUnit tu = astBuilder.load_file(std::string(argv[1]));
    astfri::text::TextLibManager visitor;
    visitor.process_and_export_ast(tu, &std::cout);
}
