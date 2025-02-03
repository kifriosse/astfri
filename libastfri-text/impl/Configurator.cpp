#include <libastfri-text/inc/Configurator.hpp>
#include <libastfri-text/rapidjson/filereadstream.h>
#include <libastfri-text/rapidjson/document.h>
#include <filesystem>

Configurator::Configurator(const std::string& configFileName) {
    set_defaults();
    namespace fs = std::filesystem;
    namespace rj = rapidjson;
    fs::path configFilePath = fs::current_path().parent_path().parent_path().parent_path() / "libastfri-text" / "impl" / configFileName;
    FILE* configFile = fopen(configFilePath.c_str(), "r");
    if (!configFile) {
        return;
    }
    char readBuffer[65536];
    rj::FileReadStream inputStream(configFile, readBuffer, sizeof(readBuffer));
    rj::Document doc;
    doc.ParseStream(inputStream);
    if (doc.HasMember("general_output") && doc["general_output"].IsObject() && !doc["general_output"].IsNull()) {//----------general_output----------
        rj::Value& genOutput = doc["general_output"];
        if (genOutput.HasMember("file_name") && genOutput["file_name"].IsString() && genOutput["file_name"].GetStringLength() > 0) {
            delete outputFileName_;
            outputFileName_ = new std::stringstream(genOutput["file_name"].GetString());
        }
        if (genOutput.HasMember("type") && genOutput["type"].IsString() && !genOutput["type"].IsNull()) {
            delete outputType_;
            outputType_ = new std::stringstream(genOutput["type"].GetString());
        }
        if (genOutput.HasMember("view") && genOutput["view"].IsString() && !genOutput["view"].IsNull()) {
            delete view_;
            view_ = new std::stringstream(genOutput["view"].GetString());
        }
        if (genOutput.HasMember("data_types") && genOutput["data_types"].IsObject() && !genOutput["data_types"].IsNull()) {//-----data_types-----
            rj::Value& datTypes = genOutput["data_types"];
            if (datTypes.HasMember("int") && datTypes["int"].IsString() && !datTypes["int"].IsNull()) {
                delete intWord_;
                intWord_ = new std::stringstream(datTypes["int"].GetString());
            }
            if (datTypes.HasMember("char") && datTypes["char"].IsString() && !datTypes["char"].IsNull()) {
                delete charWord_;
                charWord_ = new std::stringstream(datTypes["char"].GetString());
            }
        }
    }
    if (doc.HasMember("basic_format") && doc["basic_format"].IsObject() && !doc["basic_format"].IsNull()) {//----------basic_format----------
        rj::Value& basForm = doc["basic_format"];
        if (basForm.HasMember("format") && basForm["format"].IsString() && !basForm["format"].IsNull()) {
            delete outputFormat_;
            outputFormat_ = new std::stringstream(basForm["format"].GetString());
        }
        if (basForm.HasMember("indentation") && basForm["indentation"].IsString() && !basForm["indentation"].IsNull()) {
            delete tabWord_;
            tabWord_ = new std::stringstream(basForm["indentation"].GetString());
        }
        if (basForm.HasMember("show_class_body") && basForm["show_class_body"].IsBool() && !basForm["show_class_body"].IsNull()) {
            showClassBody_ = basForm["show_class_body"].GetBool();
        }
        if (basForm.HasMember("show_method_body") && basForm["show_method_body"].IsBool() && !basForm["show_method_body"].IsNull()) {
            showMethodBody_ = basForm["show_method_body"].GetBool();
        }
        if (basForm.HasMember("show_function_body") && basForm["show_function_body"].IsBool() && !basForm["show_function_body"].IsNull()) {
            showFunctionBody_ = basForm["show_function_body"].GetBool();
        }
        if (basForm.HasMember("open_bracket_new_line") && basForm["open_bracket_new_line"].IsBool() && !basForm["open_bracket_new_line"].IsNull()) {
            openBracketOnNewLine_ = basForm["open_bracket_new_line"].GetBool();
        }
    }
    if (doc.HasMember("system_names") && doc["system_names"].IsObject() && !doc["system_names"].IsNull()) {//----------system_names----------
        rj::Value& sysNames = doc["system_names"];
        if (sysNames.HasMember("class") && sysNames["class"].IsString() && !sysNames["class"].IsNull()) {
            delete classWord_;
            classWord_ = new std::stringstream(sysNames["class"].GetString());
        }
        if (sysNames.HasMember("if") && sysNames["if"].IsString() && !sysNames["if"].IsNull()) {
            delete ifWord_;
            ifWord_ = new std::stringstream(sysNames["if"].GetString());
        }
        if (sysNames.HasMember("else") && sysNames["else"].IsString() && !sysNames["else"].IsNull()) {
            delete elseWord_;
            elseWord_ = new std::stringstream(sysNames["else"].GetString());
        }
        if (sysNames.HasMember("for") && sysNames["for"].IsString() && !sysNames["for"].IsNull()) {
            delete forWord_;
            forWord_ = new std::stringstream(sysNames["for"].GetString());
        }
        if (sysNames.HasMember("while") && sysNames["while"].IsString() && !sysNames["while"].IsNull()) {
            delete whileWord_;
            whileWord_ = new std::stringstream(sysNames["while"].GetString());
        }
    }
    fclose(configFile);
}

Configurator::~Configurator() {
    delete outputFileName_;
    delete outputType_;
    delete view_;
    delete intWord_;
    delete charWord_;
    delete outputFormat_;
    delete tabWord_;
    delete classWord_;
    delete ifWord_;
    delete elseWord_;
    delete forWord_;
    delete whileWord_;
}

void Configurator::set_defaults() {
    outputFileName_ = new std::stringstream("output");
    outputType_ = new std::stringstream("text");
    view_ = new std::stringstream("vnutorny");
    intWord_ = new std::stringstream("int");
    charWord_ = new std::stringstream("char");
    outputFormat_ = new std::stringstream("txt");
    tabWord_ = new std::stringstream("   ");
    showClassBody_ = true;
    showMethodBody_ = true;
    showFunctionBody_ = true;
    openBracketOnNewLine_ = false;
    classWord_ = new std::stringstream("class");
    ifWord_ = new std::stringstream("if");
    elseWord_ = new std::stringstream("else");
    forWord_ = new std::stringstream("for");
    whileWord_ = new std::stringstream("while");
}