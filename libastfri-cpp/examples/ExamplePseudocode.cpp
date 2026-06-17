#include <astfri-cpp/AstfriCpp.hpp>
#include <astfri-text/TextLibManager.hpp>

#include <iostream>


int main(int argc, const char** argv) {
    if (argc < 2) {
        std::cerr << "Provide path to a file with C++ source code.\n";
        return 1;
    }
    astfri::TranslationUnit tu = astfri::cpp_in::load_file(argv[1], astfri::cpp::Config());
    astfri::text::TextLibManager visitor;
    visitor.process_and_export_ast(tu, &std::cout);
}
