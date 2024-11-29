#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <iostream>
#include <vector>
#include "libastfri/inc/Expr.hpp"
#include "libastfri/inc/Stmt.hpp"

int largest_even_number(int lowerBound, int upperBound)
{
    int result = 0;
    for (int i = lowerBound; i <= upperBound; ++i) {
        if (i % 2 == 0) {
            result = i;
        }
    }
    return result;
}

int main()
{
  auto& statements = astfri::StmtFactory::get_instance();
  auto& expressions = astfri::ExprFactory::get_instance();
  auto& types = astfri::TypeFactory::get_instance();

  //difference between: ?
  //<astfri::ParamVarDefStmt *>
  //<astfri::ParamVarDefStmt*>
  std::vector<astfri::ParamVarDefStmt *> params = {
    statements.mk_param_var_def("lowerBound", types.mk_int(), nullptr),
    statements.mk_param_var_def("upperBound", types.mk_int(), nullptr)
  };

  astfri::Expr* forCond = expressions.mk_if(
    expressions.mk_bin_on(
      expressions.mk_local_var_ref("i"),
      astfri::BinOpType::LessEqual,
      expressions.mk_param_var_ref("upperBound")
    ),
    nullptr,
    nullptr
  );

  astfri::Stmt* forStep = statements.mk_expr(expressions.mk_int_literal(1));

  astfri::Stmt* forInit = statements.mk_expr(
    expressions.mk_assign(
      expressions.mk_local_var_ref("i"),
      expressions.mk_param_var_ref("lowerBound")
    )
  );
  
  astfri::CompoundStmt* forBody = statements.mk_compound({
    statements.mk_if(
      expressions.mk_bin_on(
        expressions.mk_bin_on(
          expressions.mk_local_var_ref("i"),
          astfri::BinOpType::Modulo,
          expressions.mk_int_literal(2)
        ),
        astfri::BinOpType::Equal, expressions.mk_int_literal(0)
        ),
      statements.mk_expr(expressions.mk_assign(expressions.mk_local_var_ref("result"), expressions.mk_local_var_ref("i"))),
      nullptr)
  });

  astfri::CompoundStmt* body = statements.mk_compound({
    statements.mk_local_var_def("result", types.mk_int(), nullptr),
    statements.mk_for(forInit, forCond, forStep, forBody),
    statements.mk_return(expressions.mk_local_var_ref("result"))
  });

  astfri::FunctionDefStmt* func = statements.mk_function_def(
    "largest_even_number",
    std::move(params),
    types.mk_int(),
    body
  );

  std::cout << "Hello, world!" << std::endl;
  std::cout << largest_even_number(0, 100) << std::endl;

  return 0;
}