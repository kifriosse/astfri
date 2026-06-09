#include <astfri/Astfri.hpp>
#include <libastfri-text/inc/TextLibManager.hpp>
#include <libastfri-uml/inc/UMLLibWrapper.hpp>
#include <astfri-cpp/AstfriCpp.hpp>
#include <libastfri-cs/inc/ASTBuilder.hpp>
#include <libastfri-java/inc/ASTBuilder.hpp>

#include <cxxopts.hpp>

#include "args.hpp"
#include "status.hpp"
#include "version.hpp"

#include <iostream>



int main(int argc, char *argv[]) {
    cxxopts::Options options = astfri::prepare_options();
    const cxxopts::ParseResult args = astfri::parse_args(argc, argv, options);

    if (args.contains("help")) {
        astfri::print_help(std::cout, options);
        return astfri::EXIT_OK;
    }

    if (args.contains("version")) {
        astfri::print_version(std::cout);
        return astfri::EXIT_OK;
    }

    return astfri::EXIT_OK;
}
