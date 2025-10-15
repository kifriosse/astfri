#include <libastfri-serialize/inc/AstFriDeSerializer.hpp>
#include <libastfri-text/inc/TextLibManager.hpp>

#include <iostream>

int main(int argc, char** argv)
{

    if (argc < 2)
        throw std::runtime_error("No file name given!!");
    AstFriDeSerializer& serializer      = AstFriDeSerializer::get_instance();

    astfri::IVisitable* node            = serializer.deserialize(argv[1]);
    astfri::text::TextLibManager& visitor = astfri::text::TextLibManager::get_instance();


    // visitor.visit(*node);

    std::cout << "Parsing JSON file successfull";
    return 0;
}
