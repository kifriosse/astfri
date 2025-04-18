#include <iostream>
#include <libastfri-serialize/inc/AstFriDeSerializer.hpp>
#include <libastfri-text/inc/ASTVisitor.hpp>
int main(int argc, char** argv){

if (argc < 2) throw std::runtime_error("No file name given!!");
AstFriDeSerializer& serializer = AstFriDeSerializer::get_instance();    
    
astfri::IVisitable* node = serializer.deserialize(argv[1]);
std::unique_ptr<ASTVisitor> visitor = std::make_unique<ASTVisitor>();

node->accept(*visitor);
visitor->write_file();


std::cout << "Parsing JSON file successfull";


return 0;
}
