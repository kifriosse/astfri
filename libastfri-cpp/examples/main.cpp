// #include <iostream>

// astfri headers
#include <libastfri/inc/Astfri.hpp>

// ASTFRI visitor
#include <libastfri-text/inc/TextLibManager.hpp>

// ASTFRI UML
#include <libastfri-uml/inc/UMLLibWrapper.hpp>

// to co je treba na moje
#include <libastfri-cpp/inc/ClangManagement.hpp>

int main(int argc, const char** argv) {
    (void)argc;

    // vytvorenie Translation unit zo súboru
    astfri::cpp::cpp_in input;
    astfri::TranslationUnit tu = input.load_file(argv[1]);

    // pseudocode
    astfri::text::TextLibManager& man = astfri::text::TextLibManager::get_instance();
    man.visit_and_export(tu);

    // UML
    astfri::uml::UMLLibWrapper wrapper;
    astfri::uml::Config config;
    astfri::uml::PlantUMLOutputter op;
    config.separator_ = ':';
    wrapper.init(config, op);
    wrapper.run(tu);

    return 0;
}
