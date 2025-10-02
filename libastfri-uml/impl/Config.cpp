#include <libastfri-uml/inc/Config.hpp>

#include <fstream>

namespace astfri::uml {
    bool Config::parse_json(const char* path) {
        std::string jsonString;
        std::string line;
        std::ifstream fstream(path);
        while (std::getline(fstream, line)) {
            jsonString += line;
        }
        fstream.close();
        rapidjson::Document document;
        document.Parse(jsonString.c_str());
        if (!document.HasMember("FILE")) return false;
        if (!this->parse_file_info(document["FILE"])) return false;
        if (!document.HasMember("TYPES")) return false;
        if (!this->parse_types_info(document["TYPES"])) return false;
        if (!document.HasMember("ACCESS_MOD")) return false;
        if (!this->parse_access_info(document["ACCESS_MOD"])) return false;
        if (!document.HasMember("COLORS")) return false;
        if (!this->parse_colors_info(document["COLORS"])) return false;
        if (!document.HasMember("RELATIONS")) return false;
        if (!this->parse_relations_info(document["RELATIONS"])) return false;
        if (!document.HasMember("DESTRUCTOR")) return false;
        if (!this->parse_destructor_info(document["DESTRUCTOR"])) return false;
        return true;
    }

    bool Config::parse_file_info(const rapidjson::Value& val) {
        if (!val.HasMember("write_to_file")) return false;
        this->writeToFile_ = val["write_to_file"].GetBool();
        if (!val.HasMember("file_path")) return false;
        this->outputFilePath_ = val["file_path"].GetString();
        return true;
    }

    bool Config::parse_types_info(const rapidjson::Value& val) {
        if (!val.HasMember("int")) return false;
        this->intTypeName_ = val["int"].GetString();
        if (!val.HasMember("float")) return false;
        this->floatTypeName_ = val["float"].GetString();
        if (!val.HasMember("char")) return false;
        this->charTypeName_ = val["char"].GetString();
        if (!val.HasMember("bool")) return false;
        this->boolTypeName_ = val["bool"].GetString();
        if (!val.HasMember("void")) return false;
        this->voidTypeName_ = val["void"].GetString();
        if (!val.HasMember("indirect_indicator")) return false;
        this->indirectIndicator_ = val["indirect_indicator"].GetString()[0];
        if (!val.HasMember("separator")) return false;
        this->separator_ = val["separator"].GetString()[0];
        if (!val.HasMember("type_convention")) return false;
        std::string str = val["type_convention"].GetString();
        if (str.compare("TypeBeforeConvention") == 0); // allocate an instance of type convention and assign it to config
        if (str.compare("TypeAfterConvention") == 0); // allocate an instance of type convention and assign it to config
        return true;
    }

    bool Config::parse_access_info(const rapidjson::Value& val) {
        if (!val.HasMember("inner_view")) return false;
        this->innerView_ = val["inner_view"].GetBool();
        if (!val.HasMember("draw_icons")) return false;
        this->drawAccessModIcons_ = val["draw_icons"].GetBool();
        if (!val.HasMember("public")) return false;
        this->accessPrefix_[0] = val["public"].GetString()[0];
        if (!val.HasMember("private")) return false;
        this->accessPrefix_[1] = val["private"].GetString()[0];
        if (!val.HasMember("protected")) return false;
        this->accessPrefix_[2] = val["protected"].GetString()[0];
        if (!val.HasMember("package_private")) return false;
        this->accessPrefix_[3] = val["package_private"].GetString()[0];
        return true;
    }

    bool Config::parse_colors_info(const rapidjson::Value& val) {
        if (!val.HasMember("bg_diagram")) return false;
        this->diagramBG_ = val["bg_diagram"].GetString();
        if (!val.HasMember("bg_element")) return false;
        this->elementBG_ = val["bg_element"].GetString();
        if (!val.HasMember("element_border")) return false;
        this->elementBorder_ = val["element_border"].GetString();
        if (!val.HasMember("font_color")) return false;
        this->fontColor_ = val["font_color"].GetString();
        if (!val.HasMember("arrow_color")) return false;
        this->arrowColor_ = val["arrow_color"].GetString();
        return true;
    }

    bool Config::parse_relations_info(const rapidjson::Value& val) {
        if (!val.HasMember("association")) return false;
        this->relationArrows_[0] = val["association"].GetString();
        if (!val.HasMember("composition")) return false;
        this->relationArrows_[1] = val["composition"].GetString();
        if (!val.HasMember("extension")) return false;
        this->relationArrows_[2] = val["extension"].GetString();
        if (!val.HasMember("implementation")) return false;
        this->relationArrows_[3] = val["implementation"].GetString();
        return true;
    }

    bool Config::parse_destructor_info(const rapidjson::Value& val) {
        if (!val.HasMember("indicator")) return false;
        this->destructorIndicator_ = val["indicator"].GetString()[0];
        return true;
    }

    void Config::use_default_values() {

        this->innerView_ = true;
        this->writeToFile_ = false;
        this->drawAccessModIcons_ = true;

        this->indirectIndicator_ = '*';
        this->destructorIndicator_ = '~';
        this->separator_ = ' ';
        this->accessPrefix_[0] = '+';
        this->accessPrefix_[1] = '-';
        this->accessPrefix_[2] = '#';
        this->accessPrefix_[3] = '~';

        this->relationArrows_[0] = "<--";
        this->relationArrows_[1] = "*--";
        this->relationArrows_[2] = "<|--";
        this->relationArrows_[3] = "<|..";
        this->intTypeName_ = "int";
        this->floatTypeName_ = "float";
        this->charTypeName_ = "char";
        this->boolTypeName_ = "bool";
        this->voidTypeName_ = "void";

        this->diagramBG_ = "#FFFFFF";
        this->elementBG_ = "#FFDDDD";
        this->elementBorder_ = "#000000";
        this->fontColor_ = "#000000";
        this->arrowColor_ = "#000000";

        this->outputFilePath_ = "/tmp/class_diagram";
    }

} // namespace astfri::uml
