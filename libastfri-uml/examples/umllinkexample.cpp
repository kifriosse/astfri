#include <libastfri-cpp/inc/ClangManagement.hpp>
#include <libastfri-uml/inc/UMLLibWrapper.hpp>

int main(int argc, char const** argv)
{
    astfri::TranslationUnit tu;

    if (astfri::astfri_cpp::fill_translation_unit(tu, argv[1]) != 0)
    {
        std::cout << "chyba pri fill_translation_unit\n";
        return 1;
    }

    astfri::uml::Config conf;
    astfri::uml::PlantUMLOutputter op;
    astfri::uml::TypeBeforeConvention tc;
    astfri::uml::UMLLibWrapper uml;
    uml.init(conf, op, tc);
    uml.run(tu);

    return 0;
}
