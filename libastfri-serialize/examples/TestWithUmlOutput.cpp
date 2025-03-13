#include <iostream>
#include <libastfri-serialize/inc/AstFriSerializer.hpp>
#include <libastfri-uml/inc/UMLLibWrapper.hpp>

int main(int argc, char** argv){

if (argc < 2) throw std::runtime_error("No file name given!!");
AstFriSerializer& serializer = AstFriSerializer::get_instance();    
    
astfri::IVisitable* node = serializer.serialize(argv[1]);

astfri::TranslationUnit* tu = dynamic_cast<astfri::TranslationUnit*>(node);

astfri::uml::Config conf;
    astfri::uml::PlantUMLOutputter op;
    astfri::uml::TypeBeforeConvention tc;

    astfri::uml::UMLLibWrapper uml;
    uml.init(conf, op, tc);
    uml.run(*tu);


std::cout << "Parsing JSON file successfull";


return 0;
}




