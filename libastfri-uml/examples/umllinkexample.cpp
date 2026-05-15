#include <astfri-cpp/AstfriCpp.hpp>
#include <libastfri-uml/inc/UMLLibWrapper.hpp>

int main(int argc, const char** argv) {
    if (argc != 2) {
        return -1;
    }
    astfri::TranslationUnit tu;

    tu = astfri::cpp_in::load_file(argv[1], astfri::cpp::Config());
    // if (astfri::astfri_cpp::fill_translation_unit(tu, argv[1]) != 0) {
    //     // std::cout << "chyba pri fill_translation_unit\n";
    //     return 1;
    // }

    astfri::uml::Config conf;
    astfri::uml::PlantUMLOutputter op;
    astfri::uml::UMLLibWrapper uml;
    uml.init(conf, op);
    uml.run(tu);

    return 0;
}
