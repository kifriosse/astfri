#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri-serialize/JSonDeserialiser.cpp>

// int compare(int a,int b=10)
// {
//     if (a > b)
//     {
//         return a;
//     }
//     else{
//         return b;
//     }
// }

int main() {
  auto& statements = astfri::StmtFactory::get_instance();
  auto& expressions = astfri::ExprFactory::get_instance();
  auto& types = astfri::TypeFactory::get_instance();
 
std::vector<astfri::ParamVarDefStmt*> params = {
    statements.mk_param_var_def("a",types.mk_int(),nullptr),
    statements.mk_param_var_def("b",types.mk_int(),expressions.mk_int_literal(10))
};

astfri::IfStmt* condition = statements.mk_if(expressions.mk_bin_on(
    expressions.mk_param_var_ref("a"),
    astfri::BinOpType::Greater,
    expressions.mk_param_var_ref("b")),
    statements.mk_return(expressions.mk_param_var_ref("a")),
    statements.mk_return(expressions.mk_param_var_ref("b"))
    );

astfri::CompoundStmt* body = statements.mk_compound({condition});
astfri::FunctionDefStmt* function = statements.mk_function_def("compare",std::move(params),types.mk_int(),body);
 
  JsonDeSerialiser jsonOutput(std::cout);
  jsonOutput.visit(*function); 

 return 0;
  }