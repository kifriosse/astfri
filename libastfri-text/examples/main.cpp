#include <iostream>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

int main() {

    std::cout << "Ahoj!" << std::endl;

    auto& statements = astfri::StmtFactory::get_instance();
    auto& expressions = astfri::ExprFactory::get_instance();
    auto& types = astfri::TypeFactory::get_instance();

    // int add(int a, int b) {
    //   return a + b;
    // }
    std::vector<astfri::ParamVarDefStmt *> params = {
        statements.mk_param_var_def("a", types.mk_int(), nullptr),
        statements.mk_param_var_def("b", types.mk_int(), nullptr)
    };
    astfri::CompoundStmt* body = statements.mk_compound({
        statements.mk_return(
            expressions.mk_bin_on(expressions.mk_param_var_ref("a"), astfri::BinOpType::Add, expressions.mk_param_var_ref("b"))
        )
    });
    astfri::FunctionDefStmt* func = statements.mk_function_def("add", std::move(params), types.mk_int(), body);


    // void cyklus(int cislo) {
    //     for (int i = 0; i < 5; ++i) {
    //         std::cout << i + cislo << std::endl;
    //     }
    // }
    std::vector<astfri::ParamVarDefStmt *> parametre = { statements.mk_param_var_def("cislo", types.mk_int(), nullptr) }; // params fcie

    astfri::CompoundStmt* telo = statements.mk_compound({ //telo fcie cyklus
        statements.mk_for( // vytvori for cyklus
            statements.mk_local_var_def("i", types.mk_int(), expressions.mk_int_literal(0)), // int i = 0
            expressions.mk_bin_on(expressions.mk_local_var_ref("i"), astfri::BinOpType::Less, expressions.mk_int_literal(5)), // i < 5
            statements.mk_expr(expressions.mk_bin_on(expressions.mk_local_var_ref("i"), astfri::BinOpType::Add, expressions.mk_int_literal(1))), // i++
            statements.mk_compound({
                statements.mk_expr( // i + cislo      ako spravit cout?
                    expressions.mk_bin_on(expressions.mk_local_var_ref("i"), astfri::BinOpType::Add, expressions.mk_param_var_ref("cislo"))
                )
            })
        )
    });

    astfri::FunctionDefStmt* funkcia = statements.mk_function_def("cyklus", std::move(parametre), types.mk_void(), telo); // definicia fcie
}