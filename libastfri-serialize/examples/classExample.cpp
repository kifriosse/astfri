#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <algorithm>
#include <iostream>

void printChar(char ch){
    for(int i = 0; i < 10; ++i)
    {
        std::cout << ch << " ";
    }
}
class Example{
    public:
    void print(){
        printChar(symbol);
    }
    private:
    const char symbol{'*'}; 

};

int main(){
  auto& statements = astfri::StmtFactory::get_instance();   
  auto& expressions = astfri::ExprFactory::get_instance();
  auto& types = astfri::TypeFactory::get_instance();


std::vector<astfri::MemberVarDefStmt*> attributes{ 
        statements.mk_member_var_def("symbol",types.mk_char(),
                                        expressions.mk_char_literal('*'))};

    
    
    astfri::ClassDefStmt* classDefinition = statements.mk_class_def("Example",std::move(attributes),
            std::vector<astfri::MethodDefStmt*>{statements.mk_method_def(classDefinition,statements.mk_function_def("print",
                std::vector<astfri::ParamVarDefStmt*>{},types.mk_void(),statements.mk_compound(
                    std::vector<astfri::Stmt*>{statements.mk_expr(expressions.mk_function_call("printChar",std::vector<astfri::Expr *>{
                        expressions.mk_local_var_ref("symbol")
                    }))})))},std::vector<astfri::GenericParam*>{});


    
  

  return 0;
}
