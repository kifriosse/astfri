#include <libastfri-text/inc/TextConfigurator.hpp>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <filesystem>
#include <iostream>

TextConfigurator::TextConfigurator() : Configurator() {
    load_new_config_file();
}

void TextConfigurator::reset() {
    std::cout << " > Reloading configuration file.\n";
    Configurator::set_defaults();
    load_new_config_file();
}

void TextConfigurator::load_new_config_file() {
    set_defaults();
    std::string input;
    std::cout << " > Do you want to load configuration file? [\"y\"/...]: ";
    std::getline(std::cin, input);
    if (input != "y") {
        std::cout << " > Loading default output option.\n";
        Configurator::set_defaults();
        return;
    }
    std::cout << " > Do you want to load your own configuration file? [\"y\"/...]: ";
    std::getline(std::cin, input);
    if (input == "y") {
        std::cout << " > Put path to .json file: ";
        std::getline(std::cin, input);
        if (!input.ends_with(".json")) {
            std::cout << " > Wrong file format! Loading default output option.\n";
            Configurator::set_defaults();
            return;
        }
    } else {
        std::filesystem::path currentPath = std::filesystem::current_path();
        bool foundBuildFolder = false;
        while (currentPath.has_filename() || currentPath.parent_path().has_filename()) {
            if (currentPath.filename() == "build") {
                foundBuildFolder = true;
                break;
            }
            currentPath = currentPath.parent_path();
        }
        if (foundBuildFolder) {
            currentPath = currentPath.parent_path() / "libastfri-text" / "impl" / "conf.json";
            input = std::move(currentPath);
        } else {
            std::cout << " > Wrong path! Loading default output option.\n";
            Configurator::set_defaults();
            return;
        }
    }
    FILE* configFile = fopen(input.c_str(), "r");
    if (!configFile) {
        std::cout << " > Wrong path! Loading default output option.\n";
        Configurator::set_defaults();
        return;
    }
    char readBuffer[65536];
    namespace rj = rapidjson;
    rj::FileReadStream inputStream(configFile, readBuffer, sizeof(readBuffer));
    rj::Document doc;
    doc.ParseStream(inputStream);
    if (doc.HasParseError()) {
        std::cout << " > Error in parsing json file! Loading default output option.\n";
        Configurator::set_defaults();
        return;
    }
    if (doc.HasMember("TEXT_CONFIGURATOR") && doc["TEXT_CONFIGURATOR"].IsObject()) {
        rj::Value& tconf = doc["TEXT_CONFIGURATOR"];
        if (tconf.HasMember("STRUCT_SETTING") && tconf["STRUCT_SETTING"].IsObject()) {
            rj::Value& sett = tconf["STRUCT_SETTING"];
            if (sett.HasMember("use_cpp_bracket") && sett["use_cpp_bracket"].IsBool()) {
                useCppBr_ = sett["use_cpp_bracket"].GetBool();
            }
            if (sett.HasMember("use_br_color") && sett["use_br_color"].IsBool()) {
                useBrCol_ = sett["use_br_color"].GetBool();
            }
            if (sett.HasMember("show_row") && sett["show_row"].IsBool()) {
                shRow_ = sett["show_row"].GetBool();
            }
            if (sett.HasMember("show_other_expr") && sett["show_other_expr"].IsBool()) {
                shOtherExpr_ = sett["show_other_expr"].GetBool();
            }
            if (sett.HasMember("show_class_dec") && sett["show_class_dec"].IsBool()) {
                shClassDec_ = sett["show_class_dec"].GetBool();
            }
            if (sett.HasMember("show_class_def") && sett["show_class_def"].IsBool()) {
                shClassDef_ = sett["show_class_def"].GetBool();
            }
            if (sett.HasMember("show_interf_dec") && sett["show_interf_dec"].IsBool()) {
                shInterfDec_ = sett["show_interf_dec"].GetBool();
            }
            if (sett.HasMember("show_interf_def") && sett["show_interf_def"].IsBool()) {
                shInterfDef_ = sett["show_interf_def"].GetBool();
            }
            if (sett.HasMember("show_gener_par") && sett["show_gener_par"].IsBool()) {
                shGenerPar_ = sett["show_gener_par"].GetBool();
            }
            if (sett.HasMember("show_attribute") && sett["show_attribute"].IsBool()) {
                shAttrib_ = sett["show_attribute"].GetBool();
            }
            if (sett.HasMember("show_meth_dec") && sett["show_meth_dec"].IsBool()) {
                shMethDec_ = sett["show_meth_dec"].GetBool();
            }
            if (sett.HasMember("show_meth_def") && sett["show_meth_def"].IsBool()) {
                shMethDef_ = sett["show_meth_def"].GetBool();
            }
            if (sett.HasMember("show_meth_owner") && sett["show_meth_owner"].IsBool()) {
                shMethOwner_ = sett["show_meth_owner"].GetBool();
            }
            if (sett.HasMember("show_meth_templ") && sett["show_meth_templ"].IsBool()) {
                shMethTempl_ = sett["show_meth_templ"].GetBool();
            }
            if (sett.HasMember("show_func_dec") && sett["show_func_dec"].IsBool()) {
                shFuncDec_ = sett["show_func_dec"].GetBool();
            }
            if (sett.HasMember("show_func_def") && sett["show_func_def"].IsBool()) {
                shFuncDef_ = sett["show_func_def"].GetBool();
            }
            if (sett.HasMember("show_global_par") && sett["show_global_par"].IsBool()) {
                shGlobPar_ = sett["show_global_par"].GetBool();
            }
            if (sett.HasMember("tab_length") && sett["tab_length"].IsInt()) {
                int val = sett["tab_length"].GetInt();
                if (val >= 0 && val <= 10) {
                    tabLen_ = val;
                }
            }
            if (sett.HasMember("margin_length") && sett["margin_length"].IsInt()) {
                int val = sett["margin_length"].GetInt();
                if (val >= 0 && val <= 10) {
                    margLen_ = val;
                }
            }
            if (sett.HasMember("round_br_colors") && sett["round_br_colors"].IsArray()) {
                rj::Value& colors = sett["round_br_colors"];
                for (size_t i = 0; i < colors.Size() && i < 4; ++i) {
                    if (colors[i].IsString()) {
                        roundBrCol_->push_back(std::stringstream(colors[i].GetString()));
                    }
                }
            }
            if (sett.HasMember("curl_br_colors") && sett["curl_br_colors"].IsArray()) {
                rj::Value& colors = sett["curl_br_colors"];
                for (size_t i = 0; i < colors.Size() && i < 4; ++i) {
                    if (colors[i].IsString()) {
                        curlBrCol_->push_back(std::stringstream(colors[i].GetString()));
                    }
                }
            }
            if (sett.HasMember("row_style") && sett["row_style"].IsString()) {
                rowStyle_ = std::make_unique<std::stringstream>(sett["row_style"].GetString());
            }
        }
        if (tconf.HasMember("SYSTEM_EXPR") && tconf["SYSTEM_EXPR"].IsObject()) {
            rj::Value& ex = tconf["SYSTEM_EXPR"];
            if (ex.HasMember("class_word") && ex["class_word"].IsString() && ex["class_word"].GetStringLength() > 0) {
                classWord_ = std::make_unique<std::stringstream>(ex["class_word"].GetString());
            }
            if (ex.HasMember("interface_word") && ex["interface_word"].IsString() && ex["interface_word"].GetStringLength() > 0) {
                interfaceWord_ = std::make_unique<std::stringstream>(ex["interface_word"].GetString());
            }
            if (ex.HasMember("implement_word") && ex["implement_word"].IsString() && ex["implement_word"].GetStringLength() > 0) {
                implementWord_ = std::make_unique<std::stringstream>(ex["implement_word"].GetString());
            }
            if (ex.HasMember("extend_word") && ex["extend_word"].IsString() && ex["extend_word"].GetStringLength() > 0) {
                extendWord_ = std::make_unique<std::stringstream>(ex["extend_word"].GetString());
            }
            if (ex.HasMember("this_word") && ex["this_word"].IsString() && ex["this_word"].GetStringLength() > 0) {
                thisWord_ = std::make_unique<std::stringstream>(ex["this_word"].GetString());
            }
            if (ex.HasMember("return_word") && ex["return_word"].IsString() && ex["return_word"].GetStringLength() > 0) {
                returnWord_ = std::make_unique<std::stringstream>(ex["return_word"].GetString());
            }
            if (ex.HasMember("continue_word") && ex["continue_word"].IsString() && ex["continue_word"].GetStringLength() > 0) {
                continueWord_ = std::make_unique<std::stringstream>(ex["continue_word"].GetString());
            }
            if (ex.HasMember("break_word") && ex["break_word"].IsString() && ex["break_word"].GetStringLength() > 0) {
                breakWord_ = std::make_unique<std::stringstream>(ex["break_word"].GetString());
            }
            if (ex.HasMember("throw_word") && ex["throw_word"].IsString() && ex["throw_word"].GetStringLength() > 0) {
                throwWord_ = std::make_unique<std::stringstream>(ex["throw_word"].GetString());
            }
            if (ex.HasMember("if_word") && ex["if_word"].IsString() && ex["if_word"].GetStringLength() > 0) {
                ifWord_ = std::make_unique<std::stringstream>(ex["if_word"].GetString());
            }
            if (ex.HasMember("else_word") && ex["else_word"].IsString() && ex["else_word"].GetStringLength() > 0) {
                elseWord_ = std::make_unique<std::stringstream>(ex["else_word"].GetString());
            }
            if (ex.HasMember("do_word") && ex["do_word"].IsString() && ex["do_word"].GetStringLength() > 0) {
                doWord_ = std::make_unique<std::stringstream>(ex["do_word"].GetString());
            }
            if (ex.HasMember("while_word") && ex["while_word"].IsString() && ex["while_word"].GetStringLength() > 0) {
                whileWord_ = std::make_unique<std::stringstream>(ex["while_word"].GetString());
            }
            if (ex.HasMember("for_word") && ex["for_word"].IsString() && ex["for_word"].GetStringLength() > 0) {
                forWord_ = std::make_unique<std::stringstream>(ex["for_word"].GetString());
            }
            if (ex.HasMember("repeat_word") && ex["repeat_word"].IsString() && ex["repeat_word"].GetStringLength() > 0) {
                repeatWord_ = std::make_unique<std::stringstream>(ex["repeat_word"].GetString());
            }
            if (ex.HasMember("switch_word") && ex["switch_word"].IsString() && ex["switch_word"].GetStringLength() > 0) {
                switchWord_ = std::make_unique<std::stringstream>(ex["switch_word"].GetString());
            }
            if (ex.HasMember("case_word") && ex["case_word"].IsString() && ex["case_word"].GetStringLength() > 0) {
                caseWord_ = std::make_unique<std::stringstream>(ex["case_word"].GetString());
            }
            if (ex.HasMember("default_word") && ex["default_word"].IsString() && ex["default_word"].GetStringLength() > 0) {
                defaultWord_ = std::make_unique<std::stringstream>(ex["default_word"].GetString());
            }
            if (ex.HasMember("new_word") && ex["new_word"].IsString() && ex["new_word"].GetStringLength() > 0) {
                newWord_ = std::make_unique<std::stringstream>(ex["new_word"].GetString());
            }
            if (ex.HasMember("delete_word") && ex["delete_word"].IsString() && ex["delete_word"].GetStringLength() > 0) {
                deleteWord_ = std::make_unique<std::stringstream>(ex["delete_word"].GetString());
            }
            if (ex.HasMember("pointer_word") && ex["pointer_word"].IsString() && ex["pointer_word"].GetStringLength() > 0) {
                pointerWord_ = std::make_unique<std::stringstream>(ex["pointer_word"].GetString());
            }
            if (ex.HasMember("virtual_word") && ex["virtual_word"].IsString() && ex["virtual_word"].GetStringLength() > 0) {
                virtualWord_ = std::make_unique<std::stringstream>(ex["virtual_word"].GetString());
            }
            if (ex.HasMember("abstract_word") && ex["abstract_word"].IsString() && ex["abstract_word"].GetStringLength() > 0) {
                abstractWord_ = std::make_unique<std::stringstream>(ex["abstract_word"].GetString());
            }
            if (ex.HasMember("template_word") && ex["template_word"].IsString() && ex["template_word"].GetStringLength() > 0) {
                templateWord_ = std::make_unique<std::stringstream>(ex["template_word"].GetString());
            }
            if (ex.HasMember("STYLE") && ex["STYLE"].IsObject()) {
                rj::Value& st = ex["STYLE"];
                if (st.HasMember("general_style") && st["general_style"].IsString()) {
                    systExprStyle_ = std::make_unique<std::stringstream>(st["general_style"].GetString());
                }
                if (st.HasMember("class_word_style") && st["class_word_style"].IsString()) {
                    classWordStyle_ = std::make_unique<std::stringstream>(st["class_word_style"].GetString());
                }
                if (st.HasMember("interface_word_style") && st["interface_word_style"].IsString()) {
                    interfaceWordStyle_ = std::make_unique<std::stringstream>(st["interface_word_style"].GetString());
                }
                if (st.HasMember("implement_word_style") && st["implement_word_style"].IsString()) {
                    implementWordStyle_ = std::make_unique<std::stringstream>(st["implement_word_style"].GetString());
                }
                if (st.HasMember("extend_word_style") && st["extend_word_style"].IsString()) {
                    extendWordStyle_ = std::make_unique<std::stringstream>(st["extend_word_style"].GetString());
                }
                if (st.HasMember("this_word_style") && st["this_word_style"].IsString()) {
                    thisWordStyle_ = std::make_unique<std::stringstream>(st["this_word_style"].GetString());
                }
                if (st.HasMember("return_word_style") && st["return_word_style"].IsString()) {
                    returnWordStyle_ = std::make_unique<std::stringstream>(st["return_word_style"].GetString());
                }
                if (st.HasMember("continue_word_style") && st["continue_word_style"].IsString()) {
                    continueWordStyle_ = std::make_unique<std::stringstream>(st["continue_word_style"].GetString());
                }
                if (st.HasMember("break_word_style") && st["break_word_style"].IsString()) {
                    breakWordStyle_ = std::make_unique<std::stringstream>(st["break_word_style"].GetString());
                }
                if (st.HasMember("throw_word_style") && st["throw_word_style"].IsString()) {
                    throwWordStyle_ = std::make_unique<std::stringstream>(st["throw_word_style"].GetString());
                }
                if (st.HasMember("if_word_style") && st["if_word_style"].IsString()) {
                    ifWordStyle_ = std::make_unique<std::stringstream>(st["if_word_style"].GetString());
                }
                if (st.HasMember("else_word_style") && st["else_word_style"].IsString()) {
                    elseWordStyle_ = std::make_unique<std::stringstream>(st["else_word_style"].GetString());
                }
                if (st.HasMember("do_word_style") && st["do_word_style"].IsString()) {
                    doWordStyle_ = std::make_unique<std::stringstream>(st["do_word_style"].GetString());
                }
                if (st.HasMember("while_word_style") && st["while_word_style"].IsString()) {
                    whileWordStyle_ = std::make_unique<std::stringstream>(st["while_word_style"].GetString());
                }
                if (st.HasMember("for_word_style") && st["for_word_style"].IsString()) {
                    forWordStyle_ = std::make_unique<std::stringstream>(st["for_word_style"].GetString());
                }
                if (st.HasMember("repeat_word_style") && st["repeat_word_style"].IsString()) {
                    repeatWordStyle_ = std::make_unique<std::stringstream>(st["repeat_word_style"].GetString());
                }
                if (st.HasMember("switch_word_style") && st["switch_word_style"].IsString()) {
                    switchWordStyle_ = std::make_unique<std::stringstream>(st["switch_word_style"].GetString());
                }
                if (st.HasMember("case_word_style") && st["case_word_style"].IsString()) {
                    caseWordStyle_ = std::make_unique<std::stringstream>(st["case_word_style"].GetString());
                }
                if (st.HasMember("default_word_style") && st["default_word_style"].IsString()) {
                    defaultWordStyle_ = std::make_unique<std::stringstream>(st["default_word_style"].GetString());
                }
                if (st.HasMember("new_word_style") && st["new_word_style"].IsString()) {
                    newWordStyle_ = std::make_unique<std::stringstream>(st["new_word_style"].GetString());
                }
                if (st.HasMember("delete_word_style") && st["delete_word_style"].IsString()) {
                    deleteWordStyle_ = std::make_unique<std::stringstream>(st["delete_word_style"].GetString());
                }
                if (st.HasMember("pointer_word_style") && st["pointer_word_style"].IsString()) {
                    pointerWordStyle_ = std::make_unique<std::stringstream>(st["pointer_word_style"].GetString());
                }
                if (st.HasMember("virtual_word_style") && st["virtual_word_style"].IsString()) {
                    virtualWordStyle_ = std::make_unique<std::stringstream>(st["virtual_word_style"].GetString());
                }
                if (st.HasMember("abstract_word_style") && st["abstract_word_style"].IsString()) {
                    abstractWordStyle_ = std::make_unique<std::stringstream>(st["abstract_word_style"].GetString());
                }
                if (st.HasMember("template_word_style") && st["template_word_style"].IsString()) {
                    templateWordStyle_ = std::make_unique<std::stringstream>(st["template_word_style"].GetString());
                }
            }
        }
        if (tconf.HasMember("OTHER_EXPR") && tconf["OTHER_EXPR"].IsObject()) {
            rj::Value& ex = tconf["OTHER_EXPR"];
            if (ex.HasMember("constructor_word") && ex["constructor_word"].IsString() && ex["constructor_word"].GetStringLength() > 0) {
                constrWord_ = std::make_unique<std::stringstream>(ex["constructor_word"].GetString());
            }
            if (ex.HasMember("destructor_word") && ex["destructor_word"].IsString() && ex["destructor_word"].GetStringLength() > 0) {
                destrWord_ = std::make_unique<std::stringstream>(ex["destructor_word"].GetString());
            }
            if (ex.HasMember("method_word") && ex["method_word"].IsString() && ex["method_word"].GetStringLength() > 0) {
                methodWord_ = std::make_unique<std::stringstream>(ex["method_word"].GetString());
            }
            if (ex.HasMember("function_word") && ex["function_word"].IsString() && ex["function_word"].GetStringLength() > 0) {
                functionWord_ = std::make_unique<std::stringstream>(ex["function_word"].GetString());
            }
            if (ex.HasMember("lambda_word") && ex["lambda_word"].IsString()) {
                lambdaWord_ = std::make_unique<std::stringstream>(ex["lambda_word"].GetString());
            }
            if (ex.HasMember("call_word") && ex["call_word"].IsString() && ex["call_word"].GetStringLength() > 0) {
                callWord_ = std::make_unique<std::stringstream>(ex["call_word"].GetString());
            }
            if (ex.HasMember("define_word") && ex["define_word"].IsString() && ex["define_word"].GetStringLength() > 0) {
                defineWord_ = std::make_unique<std::stringstream>(ex["define_word"].GetString());
            }
            if (ex.HasMember("returns_word") && ex["returns_word"].IsString() && ex["returns_word"].GetStringLength() > 0) {
                returnsWord_ = std::make_unique<std::stringstream>(ex["returns_word"].GetString());
            }
            if (ex.HasMember("STYLE") && ex["STYLE"].IsObject()) {
                rj::Value& st = ex["STYLE"];
                if (st.HasMember("general_style") && st["general_style"].IsString()) {
                    otherExprStyle_ = std::make_unique<std::stringstream>(st["general_style"].GetString());
                }
                if (st.HasMember("constructor_style") && st["constructor_style"].IsString()) {
                    constrStyle_ = std::make_unique<std::stringstream>(st["constructor_style"].GetString());
                }
                if (st.HasMember("destructor_style") && st["destructor_style"].IsString()) {
                    destrStyle_ = std::make_unique<std::stringstream>(st["destructor_style"].GetString());
                }
                if (st.HasMember("method_word_style") && st["method_word_style"].IsString()) {
                    methodWordStyle_ = std::make_unique<std::stringstream>(st["method_word_style"].GetString());
                }
                if (st.HasMember("function_word_style") && st["function_word_style"].IsString()) {
                    functionWordStyle_ = std::make_unique<std::stringstream>(st["function_word_style"].GetString());
                }
                if (st.HasMember("lambda_word_style") && st["lambda_word_style"].IsString()) {
                    lambdaWordStyle_ = std::make_unique<std::stringstream>(st["lambda_word_style"].GetString());
                }
                if (st.HasMember("call_word_style") && st["call_word_style"].IsString()) {
                    callWordStyle_ = std::make_unique<std::stringstream>(st["call_word_style"].GetString());
                }
                if (st.HasMember("define_word_style") && st["define_word_style"].IsString()) {
                    defineWordStyle_ = std::make_unique<std::stringstream>(st["define_word_style"].GetString());
                }
                if (st.HasMember("returns_word_style") && st["returns_word_style"].IsString()) {
                    returnsWordStyle_ = std::make_unique<std::stringstream>(st["returns_word_style"].GetString());
                }
            }
        }
    }
    fclose(std::move(configFile));
    Configurator::set_input_path(std::move(input));
    Configurator::load_new_config_file();
}

void TextConfigurator::set_defaults() {
    useCppBr_ = true;
    useBrCol_ = true;
    shRow_ = true;
    shOtherExpr_ = true;
    shClassDec_ = true;
    shClassDef_ = true;
    shInterfDec_ = true;
    shInterfDef_ = true;
    shGenerPar_ = true;
    shAttrib_ = true;
    shMethDec_ = true;
    shMethDef_ = true;
    shMethOwner_ = true;
    shMethTempl_ = true;
    shFuncDec_ = true;
    shFuncDef_ = true;
    shGlobPar_ = true;
    tabLen_ = 4;
    margLen_ = 3;
    roundBrCol_ = std::make_unique<std::vector<std::stringstream>>();
    curlBrCol_ = std::make_unique<std::vector<std::stringstream>>();
    rowStyle_ = std::make_unique<std::stringstream>();
    classWord_ = std::make_unique<std::stringstream>("class");
    interfaceWord_ = std::make_unique<std::stringstream>("interface");
    implementWord_ = std::make_unique<std::stringstream>("implements");
    extendWord_ = std::make_unique<std::stringstream>("extends");
    thisWord_ = std::make_unique<std::stringstream>("this");
    returnWord_ = std::make_unique<std::stringstream>("return");
    continueWord_ = std::make_unique<std::stringstream>("continue");
    breakWord_ = std::make_unique<std::stringstream>("break");
    throwWord_ = std::make_unique<std::stringstream>("throw");
    ifWord_ = std::make_unique<std::stringstream>("if");
    elseWord_ = std::make_unique<std::stringstream>("else");
    doWord_ = std::make_unique<std::stringstream>("do");
    whileWord_ = std::make_unique<std::stringstream>("while");
    forWord_ = std::make_unique<std::stringstream>("for");
    repeatWord_ = std::make_unique<std::stringstream>("repeat");
    switchWord_ = std::make_unique<std::stringstream>("switch");
    caseWord_ = std::make_unique<std::stringstream>("case");
    defaultWord_ = std::make_unique<std::stringstream>("default");
    newWord_ = std::make_unique<std::stringstream>("new");
    deleteWord_ = std::make_unique<std::stringstream>("delete");
    pointerWord_ = std::make_unique<std::stringstream>("↑");
    virtualWord_ = std::make_unique<std::stringstream>("is virtual");
    abstractWord_ = std::make_unique<std::stringstream>("is abstract");
    templateWord_ = std::make_unique<std::stringstream>("template");
    systExprStyle_ = std::make_unique<std::stringstream>();
    classWordStyle_ = std::make_unique<std::stringstream>();
    interfaceWordStyle_ = std::make_unique<std::stringstream>();
    implementWordStyle_ = std::make_unique<std::stringstream>();
    extendWordStyle_ = std::make_unique<std::stringstream>();
    thisWordStyle_ = std::make_unique<std::stringstream>();
    returnWordStyle_ = std::make_unique<std::stringstream>();
    continueWordStyle_ = std::make_unique<std::stringstream>();
    breakWordStyle_ = std::make_unique<std::stringstream>();
    throwWordStyle_ = std::make_unique<std::stringstream>();
    ifWordStyle_ = std::make_unique<std::stringstream>();
    elseWordStyle_ = std::make_unique<std::stringstream>();
    doWordStyle_ = std::make_unique<std::stringstream>();
    whileWordStyle_ = std::make_unique<std::stringstream>();
    forWordStyle_ = std::make_unique<std::stringstream>();
    repeatWordStyle_ = std::make_unique<std::stringstream>();
    switchWordStyle_ = std::make_unique<std::stringstream>();
    caseWordStyle_ = std::make_unique<std::stringstream>();
    defaultWordStyle_ = std::make_unique<std::stringstream>();
    newWordStyle_ = std::make_unique<std::stringstream>();
    deleteWordStyle_ = std::make_unique<std::stringstream>();
    pointerWordStyle_ = std::make_unique<std::stringstream>();
    virtualWordStyle_ = std::make_unique<std::stringstream>();
    abstractWordStyle_ = std::make_unique<std::stringstream>();
    templateWordStyle_ = std::make_unique<std::stringstream>();
    constrWord_ = std::make_unique<std::stringstream>("constructor");
    destrWord_ = std::make_unique<std::stringstream>("destructor");
    methodWord_ = std::make_unique<std::stringstream>("method");
    functionWord_ = std::make_unique<std::stringstream>("function");
    lambdaWord_ = std::make_unique<std::stringstream>("λ");
    callWord_ = std::make_unique<std::stringstream>("call");
    defineWord_ = std::make_unique<std::stringstream>("define");
    returnsWord_ = std::make_unique<std::stringstream>("returns");
    otherExprStyle_ = std::make_unique<std::stringstream>();
    constrStyle_ = std::make_unique<std::stringstream>();
    destrStyle_ = std::make_unique<std::stringstream>();
    methodWordStyle_ = std::make_unique<std::stringstream>();
    functionWordStyle_ = std::make_unique<std::stringstream>();
    lambdaWordStyle_ = std::make_unique<std::stringstream>();
    callWordStyle_ = std::make_unique<std::stringstream>();
    defineWordStyle_ = std::make_unique<std::stringstream>();
    returnsWordStyle_ = std::make_unique<std::stringstream>();
}