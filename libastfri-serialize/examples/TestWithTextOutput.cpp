#include <libastfri-serialize/inc/AstFriDeSerializer.hpp>
#include <libastfri-text/inc/TextLibManager.hpp>

#include <iostream>

int main(int argc, char** argv) {

    if (argc < 2)
        throw std::runtime_error("No file name given!!");
    AstFriDeSerializer& serializer        = AstFriDeSerializer::get_instance();

    astfri::Stmt* node               = serializer.deserialize(argv[1]);
    astfri::text::TextLibManager visitor;

    // visitor.visit(*node);

    std::cout << "Parsing JSON file successfull";
    return 0;
}
