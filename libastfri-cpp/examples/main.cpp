// astfri headers
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri/inc/Expr.hpp>
#include <libastfri/inc/Stmt.hpp>
#include <iostream>

// ASTFRI visitor
#include <libastfri-text/inc/ASTVisitor.hpp>

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
    return 0;
}