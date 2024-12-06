#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include "libastfri/inc/Expr.hpp"
#include "libastfri/inc/Stmt.hpp"
#include <iostream>

int main() {
std::cout << "prisiel som sem" << std::endl;

auto& statements = astfri::StmtFactory::get_instance();
auto& expressions = astfri::ExprFactory::get_instance();
auto& types = astfri::TypeFactory::get_instance();

// AST 
// int get_distance(int a, int c) {
//   int x = 0;
//   while (a < c) {
//     a = a + 1;
//     x = x + 1;
//   }
//   return x;
// }
// ja viem ze tuto metodu by nepouzil nik, ale je to len na otestovanie :D
astfri::CompoundStmt* get_distance_body = statements.mk_compound(
{
statements.mk_local_var_def("x", types.mk_int(), expressions.mk_int_literal(0)),
statements.mk_while(
  expressions.mk_bin_on(
    expressions.mk_param_var_ref("a"),
    astfri::BinOpType::Less,
    expressions.mk_param_var_ref("c")),
  statements.mk_compound(
  {
    statements.mk_expr(
      expressions.mk_assign(
        expressions.mk_param_var_ref("a"),
        expressions.mk_bin_on(
          expressions.mk_param_var_ref("a"),
          astfri::BinOpType::Add,
          expressions.mk_int_literal(1)
        )
      )
    ),
    statements.mk_expr(
      expressions.mk_assign(
        expressions.mk_param_var_ref("x"),
        expressions.mk_bin_on(
          expressions.mk_param_var_ref("x"),
          astfri::BinOpType::Add,
          expressions.mk_int_literal(1)
        )
      )
    )
  }
  )
),
 statements.mk_return(expressions.mk_param_var_ref("x"))
});

astfri::FunctionDefStmt* get_distance_Func = statements.mk_function_def(
"get_distance",
{
  statements.mk_param_var_def("a", types.mk_int(), nullptr),
  statements.mk_param_var_def("c", types.mk_int(), nullptr)
},
types.mk_int(),
get_distance_body);
// AST int get_distance(int a, int c) Koniec


// AST
// int add(int a, int b) {
//   return a + b;
// }
std::vector<astfri::ParamVarDefStmt *> params = {
  statements.mk_param_var_def("a", types.mk_int(), nullptr),
  statements.mk_param_var_def("b", types.mk_int(), nullptr)
};

astfri::CompoundStmt* body = statements.mk_compound({
  statements.mk_return(
    expressions.mk_bin_on(
      expressions.mk_param_var_ref("a"),
      astfri::BinOpType::Add,
      expressions.mk_param_var_ref("b")
    )
  )
});

astfri::FunctionDefStmt* func = statements.mk_function_def(
  "add",
  std::move(params),
  types.mk_int(),
  body
);

}