#include <libastfri-text/inc/Configurator.hpp>
#include <libastfri-text/rapidjson/filereadstream.h>
#include <libastfri-text/rapidjson/document.h>
#include <filesystem>

Configurator::Configurator(const std::string& configFileName) {
    namespace fs = std::filesystem;
    namespace rj = rapidjson;
    fs::path configFilePath = fs::current_path().parent_path().parent_path().parent_path() / "libastfri-text" / "impl" / configFileName;
    FILE* configFile = fopen(configFilePath.c_str(), "r");
    char readBuffer[65536];
    rj::FileReadStream inputStream(configFile, readBuffer, sizeof(readBuffer));
    rj::Document doc;
    doc.ParseStream(inputStream);
    if (doc.HasMember("basic_formatting")) {
        rj::Value& basicFormatting = doc["basic_formatting"];
        if (basicFormatting.HasMember("indentation")) {
            tabWord_ = new std::stringstream(basicFormatting["indentation"].GetString());
        }
    }
    if (doc.HasMember("data_types")) {
        rj::Value& dataTypes = doc["data_types"];
        if (dataTypes.HasMember("int")) {
            intWord_ = new std::stringstream(dataTypes["int"].GetString());
        }
    }
    fclose(configFile);
}

Configurator::~Configurator() {
    delete tabWord_;
    delete intWord_;
}