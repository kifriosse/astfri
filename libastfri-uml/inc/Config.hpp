#pragma once

#include <libastfri-uml/inc/TypeConvention.hpp>

namespace astfri::uml {
    struct Config {
        // type var, var : type
        TypeConvention* typeConvention_;
        bool innerView_ = true;
        bool writeToFile_ = false;
        char indirectIndicator_ = '*';
        char destructorIndicator_ = '~';
        char separator_ = ' ';
        char accessPrefix_[4] = {'+', '-', '#', '~'};
        std::string relationArrows_[4] = {"<--", "*--", "<|--", "<|.."};
        std::string intTypeName_ = "int";
        std::string floatTypeName_ = "float";
        std::string charTypeName_ = "char";
        std::string boolTypeName_ = "bool";
        std::string voidTypeName_ = "void";

        std::string outputFilePath_ = "/tmp/class_diagram";

        void parse_json(const char* path);

        void save_json(const char* path);
    };

} // namespace astfri::uml
