#pragma once

#include <string>

namespace uml {
    enum class TypeConvention {
        TYPE_BEFORE_NAME = 0,
        TYPE_AFTER_NAME
    };

    struct Config {
        // type var, var : type
        TypeConvention typeConvention_;
        std::string intTypeName_ = "int";
        std::string floatTypeName_ = "float";
        std::string charTypeName_ = "char";
        std::string boolTypeName_ = "bool";
        std::string voidTypeName_ = "void";

        void parse_json(const char* path);

        void save_json(const char* path);
    };

} // namespace uml