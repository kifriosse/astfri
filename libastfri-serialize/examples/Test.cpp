#include <iostream>
#include <libastfri-serialize/inc/AstFriSerializer.hpp>

int main(int argc, char** argv){

if (argc < 2) throw std::runtime_error("No file name given!!");
AstFriSerializer& serializer = AstFriSerializer::get_instance();    
    
astfri::IVisitable* node = serializer.serialize(argv[1]);

astfri::MethodCallExpr* methodCallExpr = dynamic_cast<astfri::MethodCallExpr*>(node);

//astfri::ParamVarRefExpr* paramVarRefExpr = dynamic_cast<astfri::ParamVarRefExpr*>(methodCallExpr->args_[0]);


std::cout << "Method name: " << methodCallExpr->name_  << std::endl;


std::cout << "Parsing JSON file successfull";


return 0;
}
