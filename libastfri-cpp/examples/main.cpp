#include <clang-c/Index.h>
#include <iostream>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

int main(int argc, char** argv) {

    auto& StmtFactory = astfri::StmtFactory::get_instance();
    auto& ExprFactory = astfri::ExprFactory::get_instance();
    auto& TypeFactory = astfri::TypeFactory::get_instance();

    auto* funkcia = StmtFactory.mk_function_def("spocitaj",
    std::vector<astfri::ParamVarDefStmt *> {},
    TypeFactory.mk_int(),
    StmtFactory.mk_compound(std::vector<astfri::Stmt *> {
        StmtFactory.mk_if(
            ExprFactory.mk_int_literal(1),
            StmtFactory.mk_return(ExprFactory.mk_int_literal(1)),
            StmtFactory.mk_return(ExprFactory.mk_int_literal(0))
        )
    }));

    auto* transUnit = StmtFactory.mk_translation_unit(
        std::vector<astfri::ClassDefStmt*> {},
        std::vector<astfri::FunctionDefStmt*> { funkcia },
        std::vector<astfri::GlobalVarDefStmt*> {});

        std::cout << "Koniec mainu" << std::endl;

    return 0;

}