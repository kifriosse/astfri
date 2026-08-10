#include <astfri-cpp/ASTBuilder.hpp>
#include <astfri-text/TextLibManager.hpp>

#include <iostream>


int main(int argc, const char** argv) {
    if (argc < 2) {
        std::cerr << "Provide path to a file with C++ source code.\n";
        return 1;
    }
    auto astBuilder = astfri::cpp::ASTBuilder::create(astfri::cpp::Config::create_default());
    astfri::TranslationUnit tu = astBuilder.load_file(std::string(argv[1]));
    astfri::text::TextLibManager visitor;
    visitor.process_and_export_ast(tu, &std::cout);
}
