#pragma once

#include <fstream>
#include <libastfri-uml/inc/TypeConvention.hpp>
#include <lib/rapidjson/document.h>
#include <lib/rapidjson/writer.h>
#include <lib/rapidjson/stringbuffer.h>


namespace astfri::uml {
    struct Config {
        // type var, var : type
        TypeConvention* typeConvention_;
        bool innerView_ = true;
        bool writeToFile_ = false;
        bool drawAccessModIcons_ = true;

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

        std::string diagramBG_ = "#FFFFFF";
        std::string elementBG_ = "#FFDDDD";
        std::string elementBorder_ = "#000000";
        std::string fontColor_ = "#000000";
        std::string arrowColor_ = "#000000";

        std::string outputFilePath_ = "/tmp/class_diagram";

        bool parse_json(const char* path);
        void use_default_values();
    private:
        bool parse_file_info(const rapidjson::Value& val);
        bool parse_types_info(const rapidjson::Value& val);
        bool parse_access_info(const rapidjson::Value& val);
        bool parse_colors_info(const rapidjson::Value& val);
        bool parse_relations_info(const rapidjson::Value& val);
        bool parse_destructor_info(const rapidjson::Value& val);
    };

} // namespace astfri::uml
