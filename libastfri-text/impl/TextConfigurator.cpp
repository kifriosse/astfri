#include <libastfri-text/inc/TextConfigurator.hpp>
#include <lib/rapidjson/filereadstream.h>
#include <lib/rapidjson/document.h>
#include <filesystem>
#include <iostream>

TextConfigurator::TextConfigurator() : Configurator() {
    load_new_config_file();
}

void TextConfigurator::load_new_config_file() {
    set_defaults();
    std::string input;
    std::cout << "Chceš použiť konfiguračný súbor? [\"yes\"/...]: ";
    std::getline(std::cin, input);
    if (input != "yes") {
        std::cout << "Použijem predvolenú konfiguráciu.\n";
        Configurator::set_defaults();
        return;
    }
    std::cout << "Chceš použiť vlastný konfiguračný súbor? [\"yes\"/...]: ";
    std::getline(std::cin, input);
    if (input == "yes") {
        std::cout << "Zadaj celú cestu ku .json súboru : ";
        std::getline(std::cin, input);
        if (!input.ends_with(".json")) {
            std::cout << "Zlý typ súboru! Použijem predvolenú konfiguráciu.\n";
            Configurator::set_defaults();
            return;
        }
    } else {
        input = std::filesystem::current_path().parent_path().parent_path().parent_path() / "libastfri-text" / "impl" / "conf.json";
    }
    FILE* configFile = fopen(input.c_str(), "r");
    if (!configFile) {
        std::cout << "Zlá cesta! Použijem predvolenú konfiguráciu.\n";
        Configurator::set_defaults();
        return;
    }
    char readBuffer[65536];
    namespace rj = rapidjson;
    rj::FileReadStream inputStream(configFile, readBuffer, sizeof(std::move(readBuffer)));
    rj::Document doc;
    doc.ParseStream(inputStream);
    if (doc.HasParseError()) {
        std::cout << "Chyba v súbore! Použijem predvolenú konfiguráciu.\n";
        Configurator::set_defaults();
        return;
    }
    if (doc.HasMember("basic_format") && doc["basic_format"].IsObject() && !doc["basic_format"].IsNull()) {
        const rj::Value& basForm = std::move(doc["basic_format"]);
        if (basForm.HasMember("format") && basForm["format"].IsString() && basForm["format"].GetStringLength() > 0) {
            outputFileFormat_ = std::make_unique<std::stringstream>(std::move(basForm["format"].GetString()));
        }
        if (basForm.HasMember("show_row_num") && basForm["show_row_num"].IsBool()) {
            showRowNumber_ = std::move(basForm["show_row_num"].GetBool());
        }
        if (basForm.HasMember("show_op_br_new_line") && basForm["show_op_br_new_line"].IsBool()) {
            showOpeningBracketOnNewLine_ = std::move(basForm["show_op_br_new_line"].GetBool());
        }
        if (basForm.HasMember("show_class_decl") && basForm["show_class_decl"].IsBool()) {
            showClassDecl_ = std::move(basForm["show_class_decl"].GetBool());
        }
        if (basForm.HasMember("show_class_body") && basForm["show_class_body"].IsBool()) {
            showClassBody_ = std::move(basForm["show_class_body"].GetBool());
        }
        if (basForm.HasMember("show_attribute") && basForm["show_attribute"].IsBool()) {
            showAttribute_ = std::move(basForm["show_attribute"].GetBool());
        }
        if (basForm.HasMember("show_method_body") && basForm["show_method_body"].IsBool()) {
            showMethodBody_ = std::move(basForm["show_method_body"].GetBool());
        }
        if (basForm.HasMember("show_method_decl") && basForm["show_method_decl"].IsBool()) {
            showMethodDecl_ = std::move(basForm["show_method_decl"].GetBool());
        }
        if (basForm.HasMember("show_method_defin") && basForm["show_method_defin"].IsBool()) {
            showMethodDefin_ = std::move(basForm["show_method_defin"].GetBool());
        }
        if (basForm.HasMember("show_method_defin_inline") && basForm["show_method_defin_inline"].IsBool()) {
            showMethodDefinInline_ = std::move(basForm["show_method_defin_inline"].GetBool());
        }
        if (basForm.HasMember("show_function_decl") && basForm["show_function_decl"].IsBool()) {
            showFunctionDecl_ = std::move(basForm["show_function_decl"].GetBool());
        }
        if (basForm.HasMember("show_function_body") && basForm["show_function_body"].IsBool()) {
            showFunctionBody_ = std::move(basForm["show_function_body"].GetBool());
        }
        if (basForm.HasMember("tab_word_length") && basForm["tab_word_length"].IsInt()) {
            int val = std::move(basForm["tab_word_length"].GetInt());
            if (val >= 0 && val <= 8) {
                lengthOfTabWord_ = std::move(val);
            }
        }
        if (basForm.HasMember("margin_left") && basForm["margin_left"].IsInt()) {
            int val = std::move(basForm["margin_left"].GetInt());
            if (val >= 0 && val <= 8) {
                lengthOfLeftMargin_ = std::move(val);
            }
        }
        if (basForm.HasMember("row_num_size") && basForm["row_num_size"].IsInt()) {
            int val = std::move(basForm["row_num_size"].GetInt());
            if (val >= 0 && val <= 30) {
                sizeOfRowNumber_ = std::move(val);
            }
        }
    }
    if (doc.HasMember("system_names") && doc["system_names"].IsObject() && !doc["system_names"].IsNull()) {
        const rj::Value& sysNam = std::move(doc["system_names"]);
        if (sysNam.HasMember("class") && sysNam["class"].IsString() && sysNam["class"].GetStringLength() > 0) {
            classWord_ = std::make_unique<std::stringstream>(std::move(sysNam["class"].GetString()));
        }
        if (sysNam.HasMember("this") && sysNam["this"].IsString() && sysNam["this"].GetStringLength() > 0) {
            thisWord_ = std::make_unique<std::stringstream>(std::move(sysNam["this"].GetString()));
        }
        if (sysNam.HasMember("return") && sysNam["return"].IsString() && sysNam["return"].GetStringLength() > 0) {
            returnWord_ = std::make_unique<std::stringstream>(std::move(sysNam["return"].GetString()));
        }
        if (sysNam.HasMember("throw") && sysNam["throw"].IsString() && sysNam["throw"].GetStringLength() > 0) {
            throwWord_ = std::make_unique<std::stringstream>(std::move(sysNam["throw"].GetString()));
        }
        if (sysNam.HasMember("if") && sysNam["if"].IsString() && sysNam["if"].GetStringLength() > 0) {
            ifWord_ = std::make_unique<std::stringstream>(std::move(sysNam["if"].GetString()));
        }
        if (sysNam.HasMember("else") && sysNam["else"].IsString() && sysNam["else"].GetStringLength() > 0) {
            elseWord_ = std::make_unique<std::stringstream>(std::move(sysNam["else"].GetString()));
        }
        if (sysNam.HasMember("for") && sysNam["for"].IsString() && sysNam["for"].GetStringLength() > 0) {
            forWord_ = std::make_unique<std::stringstream>(std::move(sysNam["for"].GetString()));
        }
        if (sysNam.HasMember("while") && sysNam["while"].IsString() && sysNam["while"].GetStringLength() > 0) {
            whileWord_ = std::make_unique<std::stringstream>(std::move(sysNam["while"].GetString()));
        }
        if (sysNam.HasMember("do") && sysNam["do"].IsString() && sysNam["do"].GetStringLength() > 0) {
            doWord_ = std::make_unique<std::stringstream>(std::move(sysNam["do"].GetString()));
        }
        if (sysNam.HasMember("switch") && sysNam["switch"].IsString() && sysNam["switch"].GetStringLength() > 0) {
            switchWord_ = std::make_unique<std::stringstream>(std::move(sysNam["switch"].GetString()));
        }
        if (sysNam.HasMember("case") && sysNam["case"].IsString() && sysNam["case"].GetStringLength() > 0) {
            caseWord_ = std::make_unique<std::stringstream>(std::move(sysNam["case"].GetString()));
        }
    }
    fclose(std::move(configFile));
    Configurator::set_input_path(input);
    Configurator::load_new_config_file();
}

void TextConfigurator::set_defaults() {
    outputFileFormat_ = std::make_unique<std::stringstream>("txt");
    showRowNumber_ = true;
    showOpeningBracketOnNewLine_ = true;
    showClassDecl_ = true;
    showClassBody_ = true;
    showAttribute_ = true;
    showMethodBody_ = true;
    showMethodDecl_ = true;
    showMethodDefin_ = true;
    showMethodDefinInline_ = false;
    showFunctionDecl_ = true;
    showFunctionBody_ = true;
    lengthOfTabWord_ = 4;
    lengthOfLeftMargin_ = 2;
    sizeOfRowNumber_ = 15;
    classWord_ = std::make_unique<std::stringstream>("class");
    thisWord_ = std::make_unique<std::stringstream>("this");
    returnWord_ = std::make_unique<std::stringstream>("return");
    throwWord_ = std::make_unique<std::stringstream>("throw");
    ifWord_ = std::make_unique<std::stringstream>("if");
    elseWord_ = std::make_unique<std::stringstream>("else");
    forWord_ = std::make_unique<std::stringstream>("for");
    whileWord_ = std::make_unique<std::stringstream>("while");
    doWord_ = std::make_unique<std::stringstream>("do");
    switchWord_ = std::make_unique<std::stringstream>("switch");
    caseWord_ = std::make_unique<std::stringstream>("case");
}