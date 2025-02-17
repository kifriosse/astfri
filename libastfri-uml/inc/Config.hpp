#pragma once

#include <libastfri-uml/inc/TypeConvention.hpp>

namespace uml {
    struct Config {
        // type var, var : type
        TypeConvention* typeConvention_;
        char separator_ = ' ';
        char accessPrefix_[4] = {'+', '-', '#', '~'};
        std::string intTypeName_ = "int";
        std::string floatTypeName_ = "float";
        std::string charTypeName_ = "char";
        std::string boolTypeName_ = "bool";
        std::string voidTypeName_ = "void";

        void parse_json(const char* path);

        void save_json(const char* path);
    };

} // namespace uml