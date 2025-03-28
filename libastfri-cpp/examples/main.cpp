// astfri headers
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri/inc/Expr.hpp>
#include <libastfri/inc/Stmt.hpp>
#include <iostream>
#include "libastfri-uml/inc/Config.hpp"
#include "libastfri-uml/inc/PlantUMLOutputter.hpp"
#include "libastfri-uml/inc/TypeConvention.hpp"

// ASTFRI visitor
#include <libastfri-text/inc/ASTVisitor.hpp>

// ASTFRI UML
#include <libastfri-uml/inc/UMLLibWrapper.hpp>

// to co je treba na moje
#include <libastfri-cpp/inc/ClangManagement.hpp>

int main(int argc, const char **argv) {
    astfri::TranslationUnit tu;
    if (astfri::astfri_cpp::fill_translation_unit(tu, argv[1]) != 0) {
        std::cout << "chyba pri fill_translation_unit\n";
        return 1;
    }
    //koniec mojho
    std::cout << "Ill be back!" << std::endl;    
    // AST Visitor - nice
    ASTVisitor* visitor = new ASTVisitor();
    visitor->visit(tu);
    visitor->write_file();

    // UML
    astfri::uml::UMLLibWrapper wrapper;
    astfri::uml::Config config;
    astfri::uml::PlantUMLOutputter op;
    astfri::uml::TypeAfterConvention con;
	    config.separator_ = ':';
    wrapper.init(config, op, con);
    wrapper.run(tu);

    return 0;
}
