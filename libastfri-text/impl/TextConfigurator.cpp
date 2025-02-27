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
    std::cout << std::move("Chceš použiť konfiguračný súbor? [\"yes\"/...]: ");
    std::getline(std::cin, input);
    if (input != "yes") {
        std::cout << std::move("Použijem predvolený formát.\n");
        Configurator::set_defaults();
        return;
    }
    std::cout << std::move("Chceš použiť vlastný konfiguračný súbor? [\"yes\"/...]: ");
    std::getline(std::cin, input);
    if (input == "yes") {
        std::cout << std::move("Zadaj celú cestu ku .json súboru: ");
        std::getline(std::cin, input);
        if (!input.ends_with(std::move(".json"))) {
            std::cout << std::move("Zlý typ súboru! Použijem predvolený formát.\n");
            Configurator::set_defaults();
            return;
        }
    } else {
        input = std::filesystem::current_path().parent_path().parent_path().parent_path() / "libastfri-text" / "impl" / "conf.json";
    }
    FILE* configFile = fopen(input.c_str(), std::move("r"));
    if (!configFile) {
        std::cout << std::move("Zlá cesta! Použijem predvolený formát.\n");
        Configurator::set_defaults();
        return;
    }
    char readBuffer[65536];
    namespace rj = rapidjson;
    rj::FileReadStream inputStream(configFile, readBuffer, sizeof(std::move(readBuffer)));
    rj::Document doc;
    doc.ParseStream(inputStream);
    if (doc.HasParseError()) {
        std::cout << std::move("Chyba pri čítaní súboru! Použijem predvolený formát.\n");
        Configurator::set_defaults();
        return;
    }
    if (doc.HasMember(std::move("TEXT_CONFIGURATOR")) && doc[std::move("TEXT_CONFIGURATOR")].IsObject()) {
        const rj::Value& tconf = std::move(doc[std::move("TEXT_CONFIGURATOR")]);
        if (tconf.HasMember(std::move("STRUCT_SETTING")) && tconf[std::move("STRUCT_SETTING")].IsObject()) {
            const rj::Value& sett = std::move(tconf[std::move("STRUCT_SETTING")]);
            if (sett.HasMember(std::move("show_row_num")) && sett[std::move("show_row_num")].IsBool()) {
                showRowNumber_ = std::move(sett[std::move("show_row_num")].GetBool());
            }
            if (sett.HasMember(std::move("show_op_br_new_line")) && sett[std::move("show_op_br_new_line")].IsBool()) {
                showOpeningBracketOnNewLine_ = std::move(sett[std::move("show_op_br_new_line")].GetBool());
            }
            if (sett.HasMember(std::move("show_other_exprs")) && sett[std::move("show_other_exprs")].IsBool()) {
                showOtherExprs_ = std::move(sett[std::move("show_other_exprs")].GetBool());
            }
            if (sett.HasMember(std::move("show_class_decl")) && sett[std::move("show_class_decl")].IsBool()) {
                showClassDecl_ = std::move(sett[std::move("show_class_decl")].GetBool());
            }
            if (sett.HasMember(std::move("show_class_body")) && sett[std::move("show_class_body")].IsBool()) {
                showClassBody_ = std::move(sett[std::move("show_class_body")].GetBool());
            }
            if (sett.HasMember(std::move("show_attribute")) && sett[std::move("show_attribute")].IsBool()) {
                showAttribute_ = std::move(sett[std::move("show_attribute")].GetBool());
            }
            if (sett.HasMember(std::move("show_method_body")) && sett[std::move("show_method_body")].IsBool()) {
                showMethodBody_ = std::move(sett[std::move("show_method_body")].GetBool());
            }
            if (sett.HasMember(std::move("show_method_decl")) && sett[std::move("show_method_decl")].IsBool()) {
                showMethodDecl_ = std::move(sett[std::move("show_method_decl")].GetBool());
            }
            if (sett.HasMember(std::move("show_method_defin")) && sett[std::move("show_method_defin")].IsBool()) {
                showMethodDefin_ = std::move(sett[std::move("show_method_defin")].GetBool());
            }
            if (sett.HasMember(std::move("show_method_defin_inline")) && sett[std::move("show_method_defin_inline")].IsBool()) {
                showMethodDefinInline_ = std::move(sett[std::move("show_method_defin_inline")].GetBool());
            }
            if (sett.HasMember(std::move("show_function_decl")) && sett[std::move("show_function_decl")].IsBool()) {
                showFunctionDecl_ = std::move(sett[std::move("show_function_decl")].GetBool());
            }
            if (sett.HasMember(std::move("show_function_body")) && sett[std::move("show_function_body")].IsBool()) {
                showFunctionBody_ = std::move(sett[std::move("show_function_body")].GetBool());
            }
            if (sett.HasMember(std::move("tab_word_length")) && sett[std::move("tab_word_length")].IsInt()) {
                int val = std::move(sett[std::move("tab_word_length")].GetInt());
                if (val >= 0 && val <= 10) {
                    lengthOfTabWord_ = std::move(val);
                }
            }
            if (sett.HasMember(std::move("margin_left")) && sett[std::move("margin_left")].IsInt()) {
                int val = std::move(sett[std::move("margin_left")].GetInt());
                if (val >= 0 && val <= 10) {
                    lengthOfLeftMargin_ = std::move(val);
                }
            }
            if (sett.HasMember(std::move("row_num_style")) && sett[std::move("row_num_style")].IsString()) {
                rowNumberStyle_ = std::make_unique<std::stringstream>(std::move(sett[std::move("row_num_style")].GetString()));
            }
        }
        if (tconf.HasMember(std::move("SYSTEM_EXPR")) && tconf[std::move("SYSTEM_EXPR")].IsObject()) {
            const rj::Value& ex = std::move(tconf[std::move("SYSTEM_EXPR")]);
            if (ex.HasMember(std::move("class_word")) && ex[std::move("class_word")].IsString() && ex[std::move("class_word")].GetStringLength() > 0) {
                classWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("class_word")].GetString()));
            }
            if (ex.HasMember(std::move("this_word")) && ex[std::move("this_word")].IsString() && ex[std::move("this_word")].GetStringLength() > 0) {
                thisWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("this_word")].GetString()));
            }
            if (ex.HasMember(std::move("return_word")) && ex[std::move("return_word")].IsString() && ex[std::move("return_word")].GetStringLength() > 0) {
                returnWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("return_word")].GetString()));
            }
            if (ex.HasMember(std::move("throw_word")) && ex[std::move("throw_word")].IsString() && ex[std::move("throw_word")].GetStringLength() > 0) {
                throwWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("throw_word")].GetString()));
            }
            if (ex.HasMember(std::move("if_word")) && ex[std::move("if_word")].IsString() && ex[std::move("if_word")].GetStringLength() > 0) {
                ifWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("if_word")].GetString()));
            }
            if (ex.HasMember(std::move("else_word")) && ex[std::move("else_word")].IsString() && ex[std::move("else_word")].GetStringLength() > 0) {
                elseWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("else_word")].GetString()));
            }
            if (ex.HasMember(std::move("do_word")) && ex[std::move("do_word")].IsString() && ex[std::move("do_word")].GetStringLength() > 0) {
                doWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("do_word")].GetString()));
            }
            if (ex.HasMember(std::move("while_word")) && ex[std::move("while_word")].IsString() && ex[std::move("while_word")].GetStringLength() > 0) {
                whileWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("while_word")].GetString()));
            }
            if (ex.HasMember(std::move("for_word")) && ex[std::move("for_word")].IsString() && ex[std::move("for_word")].GetStringLength() > 0) {
                forWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("for_word")].GetString()));
            }
            if (ex.HasMember(std::move("repeat_word")) && ex[std::move("repeat_word")].IsString() && ex[std::move("repeat_word")].GetStringLength() > 0) {
                repeatWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("repeat_word")].GetString()));
            }
            if (ex.HasMember(std::move("switch_word")) && ex[std::move("switch_word")].IsString() && ex[std::move("switch_word")].GetStringLength() > 0) {
                switchWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("switch_word")].GetString()));
            }
            if (ex.HasMember(std::move("case_word")) && ex[std::move("case_word")].IsString() && ex[std::move("case_word")].GetStringLength() > 0) {
                caseWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("case_word")].GetString()));
            }
            if (ex.HasMember(std::move("STYLE")) && ex[std::move("STYLE")].IsObject()) {
                const rj::Value& st = std::move(ex[std::move("STYLE")]);
                if (st.HasMember(std::move("general_style")) && st[std::move("general_style")].IsString()) {
                    systExprStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("general_style")].GetString()));
                }
                if (st.HasMember(std::move("class_word_style")) && st[std::move("class_word_style")].IsString()) {
                    classWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("class_word_style")].GetString()));
                }
                if (st.HasMember(std::move("this_word_style")) && st[std::move("this_word_style")].IsString()) {
                    thisWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("this_word_style")].GetString()));
                }
                if (st.HasMember(std::move("return_word_style")) && st[std::move("return_word_style")].IsString()) {
                    returnWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("return_word_style")].GetString()));
                }
                if (st.HasMember(std::move("throw_word_style")) && st[std::move("throw_word_style")].IsString()) {
                    throwWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("throw_word_style")].GetString()));
                }
                if (st.HasMember(std::move("if_word_style")) && st[std::move("if_word_style")].IsString()) {
                    ifWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("if_word_style")].GetString()));
                }
                if (st.HasMember(std::move("else_word_style")) && st[std::move("else_word_style")].IsString()) {
                    elseWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("else_word_style")].GetString()));
                }
                if (st.HasMember(std::move("do_word_style")) && st[std::move("do_word_style")].IsString()) {
                    doWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("do_word_style")].GetString()));
                }
                if (st.HasMember(std::move("while_word_style")) && st[std::move("while_word_style")].IsString()) {
                    whileWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("while_word_style")].GetString()));
                }
                if (st.HasMember(std::move("for_word_style")) && st[std::move("for_word_style")].IsString()) {
                    forWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("for_word_style")].GetString()));
                }
                if (st.HasMember(std::move("repeat_word_style")) && st[std::move("repeat_word_style")].IsString()) {
                    repeatWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("repeat_word_style")].GetString()));
                }
                if (st.HasMember(std::move("switch_word_style")) && st[std::move("switch_word_style")].IsString()) {
                    switchWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("switch_word_style")].GetString()));
                }
                if (st.HasMember(std::move("case_word_style")) && st[std::move("case_word_style")].IsString()) {
                    caseWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("case_word_style")].GetString()));
                }
            }
        }
        if (tconf.HasMember(std::move("OTHER_EXPR")) && tconf[std::move("OTHER_EXPR")].IsObject()) {
            const rj::Value& ex = std::move(tconf[std::move("OTHER_EXPR")]);
            if (ex.HasMember(std::move("method_word")) && ex[std::move("method_word")].IsString() && ex[std::move("method_word")].GetStringLength() > 0) {
                methodWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("method_word")].GetString()));
            }
            if (ex.HasMember(std::move("function_word")) && ex[std::move("function_word")].IsString() && ex[std::move("function_word")].GetStringLength() > 0) {
                functionWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("function_word")].GetString()));
            }
            if (ex.HasMember(std::move("lambda_word")) && ex[std::move("lambda_word")].IsString()) {
                lambdaWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("lambda_word")].GetString()));
            }
            if (ex.HasMember(std::move("call_word")) && ex[std::move("call_word")].IsString() && ex[std::move("call_word")].GetStringLength() > 0) {
                callWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("call_word")].GetString()));
            }
            if (ex.HasMember(std::move("define_word")) && ex[std::move("define_word")].IsString() && ex[std::move("define_word")].GetStringLength() > 0) {
                defineWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("define_word")].GetString()));
            }
            if (ex.HasMember(std::move("returns_word")) && ex[std::move("returns_word")].IsString() && ex[std::move("returns_word")].GetStringLength() > 0) {
                returnsWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("returns_word")].GetString()));
            }
            if (ex.HasMember(std::move("STYLE")) && ex[std::move("STYLE")].IsObject()) {
                const rj::Value& st = std::move(ex[std::move("STYLE")]);
                if (st.HasMember(std::move("general_style")) && st[std::move("general_style")].IsString()) {
                    otherExprStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("general_style")].GetString()));
                }
                if (st.HasMember(std::move("method_word_style")) && st[std::move("method_word_style")].IsString()) {
                    methodWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("method_word_style")].GetString()));
                }
                if (st.HasMember(std::move("function_word_style")) && st[std::move("function_word_style")].IsString()) {
                    functionWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("function_word_style")].GetString()));
                }
                if (st.HasMember(std::move("lambda_word_style")) && st[std::move("lambda_word_style")].IsString()) {
                    lambdaWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("lambda_word_style")].GetString()));
                }
                if (st.HasMember(std::move("call_word_style")) && st[std::move("call_word_style")].IsString()) {
                    callWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("call_word_style")].GetString()));
                }
                if (st.HasMember(std::move("define_word_style")) && st[std::move("define_word_style")].IsString()) {
                    defineWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("define_word_style")].GetString()));
                }
                if (st.HasMember(std::move("returns_word_style")) && st[std::move("returns_word_style")].IsString()) {
                    returnsWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("returns_word_style")].GetString()));
                }
            }
        }
    }
    fclose(std::move(configFile));
    Configurator::set_input_path(std::move(input));
    Configurator::load_new_config_file();
}

void TextConfigurator::set_defaults() {
    showRowNumber_ = true;
    showOpeningBracketOnNewLine_ = false;
    showOtherExprs_ = true;
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
    lengthOfLeftMargin_ = 3;
    rowNumberStyle_ = std::make_unique<std::stringstream>();
    classWord_ = std::make_unique<std::stringstream>(std::move("class"));
    thisWord_ = std::make_unique<std::stringstream>(std::move("this"));
    returnWord_ = std::make_unique<std::stringstream>(std::move("return"));
    throwWord_ = std::make_unique<std::stringstream>(std::move("throw"));
    ifWord_ = std::make_unique<std::stringstream>(std::move("if"));
    elseWord_ = std::make_unique<std::stringstream>(std::move("else"));
    doWord_ = std::make_unique<std::stringstream>(std::move("do"));
    whileWord_ = std::make_unique<std::stringstream>(std::move("while"));
    forWord_ = std::make_unique<std::stringstream>(std::move("for"));
    repeatWord_ = std::make_unique<std::stringstream>(std::move("repeat"));
    switchWord_ = std::make_unique<std::stringstream>(std::move("switch"));
    caseWord_ = std::make_unique<std::stringstream>(std::move("case"));
    systExprStyle_ = std::make_unique<std::stringstream>();
    classWordStyle_ = std::make_unique<std::stringstream>();
    thisWordStyle_ = std::make_unique<std::stringstream>();
    returnWordStyle_ = std::make_unique<std::stringstream>();
    throwWordStyle_ = std::make_unique<std::stringstream>();
    ifWordStyle_ = std::make_unique<std::stringstream>();
    elseWordStyle_ = std::make_unique<std::stringstream>();
    doWordStyle_ = std::make_unique<std::stringstream>();
    whileWordStyle_ = std::make_unique<std::stringstream>();
    forWordStyle_ = std::make_unique<std::stringstream>();
    repeatWordStyle_ = std::make_unique<std::stringstream>();
    switchWordStyle_ = std::make_unique<std::stringstream>();
    caseWordStyle_ = std::make_unique<std::stringstream>();
    methodWord_ = std::make_unique<std::stringstream>(std::move("method"));
    functionWord_ = std::make_unique<std::stringstream>(std::move("function"));
    lambdaWord_ = std::make_unique<std::stringstream>(std::move("lambda"));
    callWord_ = std::make_unique<std::stringstream>(std::move("call"));
    defineWord_ = std::make_unique<std::stringstream>(std::move("define"));
    returnsWord_ = std::make_unique<std::stringstream>(std::move("returns"));
    otherExprStyle_ = std::make_unique<std::stringstream>();
    methodWordStyle_ = std::make_unique<std::stringstream>();
    functionWordStyle_ = std::make_unique<std::stringstream>();
    lambdaWordStyle_ = std::make_unique<std::stringstream>();
    callWordStyle_ = std::make_unique<std::stringstream>();
    defineWordStyle_ = std::make_unique<std::stringstream>();
    returnsWordStyle_ = std::make_unique<std::stringstream>();
}