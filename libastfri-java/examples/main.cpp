#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

int main()
{
    auto& exprFactory = astfri::ExprFactory::get_instance();
    auto& stmtFactory = astfri::StmtFactory::get_instance();
    auto& typeFactory = astfri::TypeFactory::get_instance();

    std::vector<astfri::ParamVarDefStmt*> params
        = {stmtFactory.mk_param_var_def("count", typeFactory.mk_int(), nullptr),
           stmtFactory.mk_param_var_def("message", typeFactory.mk_char(), nullptr)};

    astfri::CompoundStmt* body = stmtFactory.mk_compound({
        stmtFactory.mk_if(
            exprFactory
                .mk_bin_on(exprFactory.mk_param_var_ref("count"), astfri::BinOpType::LessEqual, 0),
            stmtFactory.mk_return(nullptr),
            nullptr
        ),
        stmtFactory.mk_for(
            stmtFactory.mk_param_var_def("i", typeFactory.mk_int(), exprFactory.mk_int_literal(0)),
            exprFactory.mk_bin_on(
                exprFactory.mk_local_var_ref("i"),
                astfri::BinOpType::Less,
                exprFactory.mk_param_var_ref("count")
            ),
            stmtFactory.mk_expr(exprFactory.mk_bin_on(
                exprFactory.mk_local_var_ref("i"),
                astfri::BinOpType::Add,
                exprFactory.mk_int_literal(1)
            )),
            stmtFactory.mk_compound(
                {stmtFactory.mk_expr(exprFactory.mk_function_call(
                     "std::cout",
                     {exprFactory.mk_param_var_ref("message")}
                 )),
                 stmtFactory.mk_expr(exprFactory.mk_function_call("std::endl", {}))}
            )
        ),
    });

    astfri::FunctionDefStmt* func
        = stmtFactory
              .mk_function_def("repeatMessage", std::move(params), typeFactory.mk_void(), body);

    return 0;
}