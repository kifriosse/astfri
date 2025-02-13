#include <libastfri-text/inc/Configurator.hpp>
#include <lib/rapidjson/filereadstream.h>
#include <lib/rapidjson/document.h>
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
        rj::Value& genOut = doc["general_output"];
        if (genOut.HasMember("file_name") && genOut["file_name"].IsString() && genOut["file_name"].GetStringLength() > 0) {
            delete outputFileName_;
            outputFileName_ = new std::stringstream(genOut["file_name"].GetString());
        }
        if (genOut.HasMember("type") && genOut["type"].IsString() && genOut["type"].GetStringLength() > 0) {
            delete outputType_;
            outputType_ = new std::stringstream(genOut["type"].GetString());
        }
        if (genOut.HasMember("view") && genOut["view"].IsString() && genOut["view"].GetStringLength() > 0) {
            delete view_;
            view_ = new std::stringstream(genOut["view"].GetString());
        }
        if (genOut.HasMember("data_types") && genOut["data_types"].IsObject() && !genOut["data_types"].IsNull()) {//-----data_types-----
            rj::Value& datTyp = genOut["data_types"];
            if (datTyp.HasMember("int") && datTyp["int"].IsString() && datTyp["int"].GetStringLength() > 0) {
                delete intWord_;
                intWord_ = new std::stringstream(datTyp["int"].GetString());
            }
            if (datTyp.HasMember("float") && datTyp["float"].IsString() && datTyp["float"].GetStringLength() > 0) {
                delete floatWord_;
                floatWord_ = new std::stringstream(datTyp["float"].GetString());
            }
            if (datTyp.HasMember("char") && datTyp["char"].IsString() && datTyp["char"].GetStringLength() > 0) {
                delete charWord_;
                charWord_ = new std::stringstream(datTyp["char"].GetString());
            }
            if (datTyp.HasMember("bool") && datTyp["bool"].IsString() && datTyp["bool"].GetStringLength() > 0) {
                delete boolWord_;
                boolWord_ = new std::stringstream(datTyp["bool"].GetString());
            }
            if (datTyp.HasMember("void") && datTyp["void"].IsString() && datTyp["void"].GetStringLength() > 0) {
                delete voidWord_;
                voidWord_ = new std::stringstream(datTyp["void"].GetString());
            }
        }
        if (genOut.HasMember("bin_ops") && genOut["bin_ops"].IsObject() && !genOut["bin_ops"].IsNull()) {//-----bin_ops-----
            rj::Value& binOp = genOut["bin_ops"];
            if (binOp.HasMember("assign") && binOp["assign"].IsString() && binOp["assign"].GetStringLength() > 0) {
                delete assignWord_;
                assignWord_ = new std::stringstream(binOp["assign"].GetString());
            }
        }
        if (genOut.HasMember("access_mods") && genOut["access_mods"].IsObject() && !genOut["access_mods"].IsNull()) {//-----access_mods-----
            rj::Value& accMod = genOut["access_mods"];
            if (accMod.HasMember("private") && accMod["private"].IsString() && accMod["private"].GetStringLength() > 0) {
                delete privateWord_;
                privateWord_ = new std::stringstream(accMod["private"].GetString());
            }
            if (accMod.HasMember("public") && accMod["public"].IsString() && accMod["public"].GetStringLength() > 0) {
                delete publicWord_;
                publicWord_ = new std::stringstream(accMod["public"].GetString());
            }
        }
        if (genOut.HasMember("formatting") && genOut["formatting"].IsObject() && !genOut["formatting"].IsNull()) {//-----formatting-----
            rj::Value& formm = genOut["formatting"];
            if (formm.HasMember("class_word_color") && formm["class_word_color"].IsString()) {
                delete classWordColor_;
                classWordColor_ = new std::stringstream(formm["class_word_color"].GetString());
            }
            if (formm.HasMember("class_name_color") && formm["class_name_color"].IsString()) {
                delete classNameColor_;
                classNameColor_ = new std::stringstream(formm["class_name_color"].GetString());
            }
        }
    }
    if (doc.HasMember("basic_format") && doc["basic_format"].IsObject() && !doc["basic_format"].IsNull()) {//----------basic_format----------
        rj::Value& basForm = doc["basic_format"];
        if (basForm.HasMember("format") && basForm["format"].IsString() && basForm["format"].GetStringLength() > 0) {
            delete outputFormat_;
            outputFormat_ = new std::stringstream(basForm["format"].GetString());
        }
        if (basForm.HasMember("indentation") && basForm["indentation"].IsString() && basForm["indentation"].GetStringLength() < 9) {
            delete tabWord_;
            tabWord_ = new std::stringstream(basForm["indentation"].GetString());
        }
        if (basForm.HasMember("show_class_body") && basForm["show_class_body"].IsBool()) {
            showClassBody_ = basForm["show_class_body"].GetBool();
        }
        if (basForm.HasMember("show_method_body") && basForm["show_method_body"].IsBool()) {
            showMethodBody_ = basForm["show_method_body"].GetBool();
        }
        if (basForm.HasMember("show_method_definition") && basForm["show_method_definition"].IsBool()) {
            showMethodDefinition_ = basForm["show_method_definition"].GetBool();
        }
        if (basForm.HasMember("show_function_body") && basForm["show_function_body"].IsBool()) {
            showFunctionBody_ = basForm["show_function_body"].GetBool();
        }
        if (basForm.HasMember("open_bracket_new_line") && basForm["open_bracket_new_line"].IsBool()) {
            openBracketOnNewLine_ = basForm["open_bracket_new_line"].GetBool();
        }
        if (basForm.HasMember("show_row_number") && basForm["show_row_number"].IsBool()) {
            showRowNumber_ = basForm["show_row_number"].GetBool();
        }
    }
    if (doc.HasMember("system_names") && doc["system_names"].IsObject() && !doc["system_names"].IsNull()) {//----------system_names----------
        rj::Value& sysNam = doc["system_names"];
        if (sysNam.HasMember("class") && sysNam["class"].IsString() && sysNam["class"].GetStringLength() > 0) {
            delete classWord_;
            classWord_ = new std::stringstream(sysNam["class"].GetString());
        }
        if (sysNam.HasMember("if") && sysNam["if"].IsString() && sysNam["if"].GetStringLength() > 0) {
            delete ifWord_;
            ifWord_ = new std::stringstream(sysNam["if"].GetString());
        }
        if (sysNam.HasMember("else") && sysNam["else"].IsString() && sysNam["else"].GetStringLength() > 0) {
            delete elseWord_;
            elseWord_ = new std::stringstream(sysNam["else"].GetString());
        }
        if (sysNam.HasMember("for") && sysNam["for"].IsString() && sysNam["for"].GetStringLength() > 0) {
            delete forWord_;
            forWord_ = new std::stringstream(sysNam["for"].GetString());
        }
        if (sysNam.HasMember("while") && sysNam["while"].IsString() && sysNam["while"].GetStringLength() > 0) {
            delete whileWord_;
            whileWord_ = new std::stringstream(sysNam["while"].GetString());
        }
        if (sysNam.HasMember("do") && sysNam["do"].IsString() && sysNam["do"].GetStringLength() > 0) {
            delete doWord_;
            doWord_ = new std::stringstream(sysNam["do"].GetString());
        }
        if (sysNam.HasMember("return") && sysNam["return"].IsString() && sysNam["return"].GetStringLength() > 0) {
            delete returnWord_;
            returnWord_ = new std::stringstream(sysNam["return"].GetString());
        }
        if (sysNam.HasMember("throw") && sysNam["throw"].IsString() && sysNam["throw"].GetStringLength() > 0) {
            delete throwWord_;
            throwWord_ = new std::stringstream(sysNam["throw"].GetString());
        }
        if (sysNam.HasMember("switch") && sysNam["switch"].IsString() && sysNam["switch"].GetStringLength() > 0) {
            delete switchWord_;
            switchWord_ = new std::stringstream(sysNam["switch"].GetString());
        }
        if (sysNam.HasMember("case") && sysNam["case"].IsString() && sysNam["case"].GetStringLength() > 0) {
            delete caseWord_;
            caseWord_ = new std::stringstream(sysNam["case"].GetString());
        }
        if (sysNam.HasMember("this") && sysNam["this"].IsString() && sysNam["this"].GetStringLength() > 0) {
            delete thisWord_;
            thisWord_ = new std::stringstream(sysNam["this"].GetString());
        }
    }
    fclose(configFile);
}

Configurator::~Configurator() {
    delete outputFileName_;
    delete outputType_;
    delete view_;
    delete intWord_;
    delete floatWord_;
    delete charWord_;
    delete boolWord_;
    delete voidWord_;
    delete assignWord_;
    delete privateWord_;
    delete publicWord_;
    delete outputFormat_;
    delete tabWord_;
    delete classWord_;
    delete ifWord_;
    delete elseWord_;
    delete forWord_;
    delete whileWord_;
    delete doWord_;
    delete returnWord_;
    delete throwWord_;
    delete switchWord_;
    delete caseWord_;
    delete thisWord_;
    delete classWordColor_;
    delete classNameColor_;
}

void Configurator::set_defaults() {
    outputFileName_ = new std::stringstream("output");
    outputType_ = new std::stringstream("text");
    view_ = new std::stringstream("vnutorny");
    intWord_ = new std::stringstream("int");
    floatWord_ = new std::stringstream("float");
    charWord_ = new std::stringstream("char");
    boolWord_ = new std::stringstream("bool");
    voidWord_ = new std::stringstream("void");
    assignWord_ = new std::stringstream("=");
    privateWord_ = new std::stringstream("private");
    publicWord_ = new std::stringstream("public");
    outputFormat_ = new std::stringstream("txt");
    tabWord_ = new std::stringstream("   ");
    showClassBody_ = true;
    showMethodBody_ = true;
    showMethodDefinition_ = true;
    showFunctionBody_ = true;
    openBracketOnNewLine_ = false;
    showRowNumber_ = true;
    classWord_ = new std::stringstream("class");
    ifWord_ = new std::stringstream("if");
    elseWord_ = new std::stringstream("else");
    forWord_ = new std::stringstream("for");
    whileWord_ = new std::stringstream("while");
    doWord_ = new std::stringstream("do");
    returnWord_ = new std::stringstream("return");
    throwWord_ = new std::stringstream("throw");
    switchWord_ = new std::stringstream("switch");
    caseWord_ = new std::stringstream("case");
    thisWord_ = new std::stringstream("this");
    classWordColor_ = new std::stringstream("black");
    classNameColor_ = new std::stringstream("black");
}