#include "args.hpp"

#include "status.hpp"

#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <istream>
#include <stdexcept>


namespace astfri {


std::istream &operator>>(std::istream &ist, InputLanguage &inLang) {
    std::string token;
    ist >> token;
    if (token == "cpp") {
        inLang = InputLanguage::CPP;
    } else if (token == "java") {
        inLang = InputLanguage::JAVA;
    } else if (token == "csharp") {
        inLang = InputLanguage::CSHARP;
    } else {
        throw std::invalid_argument(std::format("Invalid input language: {}.", token));
    }
    return ist;
}


std::istream &operator>>(std::istream &ist, OutputFormat &inLang) {
    std::string token;
    ist >> token;
    if (token == "uml") {
        inLang = OutputFormat::UML;
    } else if (token == "pseudocode") {
        inLang = OutputFormat::PSEUDOCODE;
    } else {
        throw std::invalid_argument(std::format("Invalid output format: {}.", token));
    }
    return ist;
}


std::istream &operator>>(std::istream &ist, std::filesystem::path &path) {
    std::string token;
    ist >> token;
    try {
        std::filesystem::path p(token);
        path = p;
    } catch (const std::exception &e) {
        throw std::invalid_argument(std::format("Error when parsing path: {}.", token));
    }
    return ist;
}


cxxopts::Options prepare_options() {
    cxxopts::Options options(
        "astfri",
        "astfri -- cli interface for the astfri libraries");

    options.add_options()
        ("h,help", "Print help.")
        ("v,version", "Print version info.")
        ("f,from", "Input language.", cxxopts::value<InputLanguage>())
        ("t,to", "Output format.", cxxopts::value<OutputFormat>())
        ("i,in-config", "Input config file.", cxxopts::value<std::filesystem::path>())
        ("o,out-config", "Output config file.", cxxopts::value<std::filesystem::path>())
        ("input", "Input file or directory.", cxxopts::value<std::filesystem::path>())
    ;
    options.parse_positional("input");

    return options;
}


cxxopts::ParseResult parse_args(int argc, char **argv, cxxopts::Options &options) {
    cxxopts::ParseResult args;

    try {
        args = options.parse(argc, argv);
    } catch (const std::exception &e) {
        std::cerr
            << "Argument error:\n"
            << "  " << e.what() << "\n";
        std::exit(EXIT_ARG_ERROR);
    }

    if (args.contains("help")) {
        return args;
    }

    if (args.contains("version")) {
        return args;
    }

    if (args.count("from") + args.count("to") != 2) {
        std::cerr
            << "Argument error:\n"
            << "  --from and --to must be specified.\n";
        std::exit(EXIT_ARG_ERROR);
    }

    if (! args.contains("input")) {
        std::cerr
            << "Argument error:\n"
            << "  input positional argument must be specified.\n";
        std::exit(EXIT_ARG_ERROR);
    }

    return args;
}


void print_help(std::ostream &ost, const cxxopts::Options &options) {
    (void)options;
    ost <<
        "NAME\n"
        "    astfri-cli - cli interface for the astfri libraries\n\n"
        "SYNOPSIS\n"
        "    astfri-cli --from in_language --to out_format\n"
        "               [--in-config in_config] [--out-config out_config] input\n"
        "    astfri-cli --version\n"
        "    astfri-cli --help\n\n"
        "OPTIONS\n"
        "    -h, --help\n"
        "            Print help.\n\n"
        "    -v, --version\n"
        "            Print version info.\n\n"
        "    -f, --from\n"
        "            Input language. Valid languages are:\n"
        "              cpp     C++.\n"
        "              java    Java.\n"
        "              csharp  C#.\n\n"
        "    -t, --to\n"
        "            Output format. Valid formats are:\n"
        "              uml        UML diagram.\n"
        "              pseudocode Pseudocode.\n\n"
        "    -i, --in-config path\n"
        "            Input config file.\n\n"
        "    -o, --out-config path\n"
        "            Output config file.\n\n"
    ;
}


} // namespace astfri
