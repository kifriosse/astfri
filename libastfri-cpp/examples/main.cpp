// astfri headers
#include <libastfri/inc/Expr.hpp>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/Stmt.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

#include <iostream>

// ASTFRI visitor
#include <libastfri-text/inc/TextLibManager.hpp>

// ASTFRI UML
#include <libastfri-uml/inc/UMLLibWrapper.hpp>

// to co je treba na moje
#include <libastfri-cpp/inc/ClangManagement.hpp>

int main(int argc, char const** argv)
{
    (void)argc;
    astfri::TranslationUnit tu;
    if (astfri::astfri_cpp::fill_translation_unit(tu, argv[1]) != 0)
    {
        std::cout << "chyba pri fill_translation_unit\n";
        return 1;
    }
    // koniec mojho
    // std::cout << "Ill be back!" << std::endl;
    // AST Visitor - nice
    
    // ako toto rozbehať?
    //astfri::text::TextLibManager& man = astfri::text::TextLibManager::get_instance();
    //man.visit(tu);

    // UML
    astfri::uml::UMLLibWrapper wrapper;
    astfri::uml::Config config;
    astfri::uml::PlantUMLOutputter op;
    config.separator_ = ':';
    wrapper.init(config, op);
    wrapper.run(tu);

    return 0;
}
