// #include <iostream>

// astfri headers
#include <astfri/Astfri.hpp>

// ASTFRI visitor
#include <astfri-text/TextLibManager.hpp>

// ASTFRI UML
#include <astfri-uml/UMLLibWrapper.hpp>

#include <astfri-cpp/impl/ClangManagement.hpp>

// to co je treba na moje
#include <astfri-cpp/AstfriCpp.hpp>

int main(int argc, const char** argv) {
    (void)argc;

    // vytvorenie Translation unit zo súboru
    astfri::TranslationUnit tu = astfri::cpp_in::load_file(argv[1], astfri::cpp::Config());

    // pseudocode
    astfri::text::TextLibManager man;
    man.process_and_export_ast(tu, nullptr);

    // UML
    astfri::uml::UMLLibWrapper wrapper;
    astfri::uml::Config config = astfri::uml::Config::createDefault();
    astfri::uml::PlantUMLOutputter op;
    wrapper.init(config, op);
    wrapper.run(tu);

    return 0;
}
