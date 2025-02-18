#include <libastfri-text/inc/Configurator.hpp>
#include <lib/rapidjson/filereadstream.h>
#include <lib/rapidjson/document.h>
#include <filesystem>

void Configurator::set_input_path(std::string& path) {
    inputFilePath_ = std::make_unique<std::stringstream>(std::move(path));
}

void Configurator::load_new_config_file() {
    FILE* configFile = fopen(std::move(inputFilePath_->str().c_str()), "r");
    char readBuffer[65536];
    namespace rj = rapidjson;
    rj::FileReadStream inputStream(configFile, readBuffer, sizeof(std::move(readBuffer)));
    rj::Document doc;
    doc.ParseStream(inputStream);
    if (doc.HasMember("general_output") && doc["general_output"].IsObject() && !doc["general_output"].IsNull()) {
        const rj::Value& genOut = std::move(doc["general_output"]);
        if (genOut.HasMember("file_path") && genOut["file_path"].IsString() && genOut["file_path"].GetStringLength() > 0) {
            outputFilePath_ = std::make_unique<std::stringstream>(std::move(genOut["file_path"].GetString()));
        }
        if (genOut.HasMember("view") && genOut["view"].IsString() && genOut["view"].GetStringLength() > 0) {
            view_ = std::make_unique<std::stringstream>(std::move(genOut["view"].GetString()));
        }
        if (genOut.HasMember("data_types") && genOut["data_types"].IsObject() && !genOut["data_types"].IsNull()) {
            const rj::Value& datTyp = std::move(genOut["data_types"]);
            if (datTyp.HasMember("int") && datTyp["int"].IsString() && datTyp["int"].GetStringLength() > 0) {
                intWord_ = std::make_unique<std::stringstream>(std::move(datTyp["int"].GetString()));
            }
            if (datTyp.HasMember("float") && datTyp["float"].IsString() && datTyp["float"].GetStringLength() > 0) {
                floatWord_ = std::make_unique<std::stringstream>(std::move(datTyp["float"].GetString()));
            }
            if (datTyp.HasMember("char") && datTyp["char"].IsString() && datTyp["char"].GetStringLength() > 0) {
                charWord_ = std::make_unique<std::stringstream>(std::move(datTyp["char"].GetString()));
            }
            if (datTyp.HasMember("bool") && datTyp["bool"].IsString() && datTyp["bool"].GetStringLength() > 0) {
                boolWord_ = std::make_unique<std::stringstream>(std::move(datTyp["bool"].GetString()));
            }
            if (datTyp.HasMember("void") && datTyp["void"].IsString() && datTyp["void"].GetStringLength() > 0) {
                voidWord_ = std::make_unique<std::stringstream>(std::move(datTyp["void"].GetString()));
            }
        }
        if (genOut.HasMember("bin_ops") && genOut["bin_ops"].IsObject() && !genOut["bin_ops"].IsNull()) {
            const rj::Value& binOp = std::move(genOut["bin_ops"]);
            if (binOp.HasMember("assign") && binOp["assign"].IsString() && binOp["assign"].GetStringLength() > 0) {
                assignWord_ = std::make_unique<std::stringstream>(std::move(binOp["assign"].GetString()));
            }
        }
        if (genOut.HasMember("access_mods") && genOut["access_mods"].IsObject() && !genOut["access_mods"].IsNull()) {
            const rj::Value& accMod = std::move(genOut["access_mods"]);
            if (accMod.HasMember("public") && accMod["public"].IsString() && accMod["public"].GetStringLength() > 0) {
                publicWord_ = std::make_unique<std::stringstream>(std::move(accMod["public"].GetString()));
            }
            if (accMod.HasMember("private") && accMod["private"].IsString() && accMod["private"].GetStringLength() > 0) {
                privateWord_ = std::make_unique<std::stringstream>(std::move(accMod["private"].GetString()));
            }
            if (accMod.HasMember("protected") && accMod["protected"].IsString() && accMod["protected"].GetStringLength() > 0) {
                protectedWord_ = std::make_unique<std::stringstream>(std::move(accMod["protected"].GetString()));
            }
        }
        if (genOut.HasMember("formatting") && genOut["formatting"].IsObject() && !genOut["formatting"].IsNull()) {
            const rj::Value& formm = std::move(genOut["formatting"]);
            if (formm.HasMember("class_word_color") && formm["class_word_color"].IsString()) {
                classWordColor_ = std::make_unique<std::stringstream>(std::move(formm["class_word_color"].GetString()));
            }
            if (formm.HasMember("class_name_color") && formm["class_name_color"].IsString()) {
                classNameColor_ = std::make_unique<std::stringstream>(std::move(formm["class_name_color"].GetString()));
            }
        }
    }
    fclose(std::move(configFile));
}

void Configurator::set_defaults() {
    namespace fs = std::filesystem;
    defaultOutputFilePath_ = std::make_unique<std::stringstream>(fs::current_path().parent_path().parent_path() / "output");
    outputFilePath_ = std::make_unique<std::stringstream>(std::move("/"));
    view_ = std::make_unique<std::stringstream>(std::move("vnutorny"));
    intWord_ = std::make_unique<std::stringstream>(std::move("int"));
    floatWord_ = std::make_unique<std::stringstream>(std::move("float"));
    charWord_ = std::make_unique<std::stringstream>(std::move("char"));
    boolWord_ = std::make_unique<std::stringstream>(std::move("bool"));
    voidWord_ = std::make_unique<std::stringstream>(std::move("void"));
    assignWord_ = std::make_unique<std::stringstream>(std::move("="));
    publicWord_ = std::make_unique<std::stringstream>(std::move("public"));
    privateWord_ = std::make_unique<std::stringstream>(std::move("private"));
    protectedWord_ = std::make_unique<std::stringstream>(std::move("protected"));
    classWordColor_ = std::make_unique<std::stringstream>(std::move("black"));
    classNameColor_ = std::make_unique<std::stringstream>(std::move("black"));
}