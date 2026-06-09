#ifndef ASTFRI_CLI_ARGS_HPP
#define ASTFRI_CLI_ARGS_HPP

#include <cxxopts.hpp>

#include <ostream>


namespace astfri {


enum class InputLanguage {
    UNINITIALIZED,
    CPP,
    JAVA,
    CSHARP
};


enum class OutputFormat {
    UNINITIALIZED,
    UML,
    PSEUDOCODE
};


cxxopts::Options prepare_options();


cxxopts::ParseResult parse_args(int argc, char *argv[], cxxopts::Options &options);


void print_help(std::ostream &ost, const cxxopts::Options &options);


} // namespace astfri

#endif
