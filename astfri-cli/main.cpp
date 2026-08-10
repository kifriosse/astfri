#include <astfri/Astfri.hpp>
#include <astfri-cpp/ASTBuilder.hpp>
#include <astfri-cs/ASTBuilder.hpp>
#include <astfri-java/ASTBuilder.hpp>
#include <astfri-text/TextLibManager.hpp>
#include <astfri-uml/UMLLibWrapper.hpp>

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
