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
    if (input != std::move("yes")) {
        std::cout << std::move("Použijem predvolený formát.\n");
        Configurator::set_defaults();
        return;
    }
    std::cout << std::move("Chceš použiť vlastný konfiguračný súbor? [\"yes\"/...]: ");
    std::getline(std::cin, input);
    if (input == std::move("yes")) {
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
            if (sett.HasMember(std::move("use_cpp_bracket")) && sett[std::move("use_cpp_bracket")].IsBool()) {
                useCppBr_ = std::move(sett[std::move("use_cpp_bracket")].GetBool());
            }
            if (sett.HasMember(std::move("use_br_color")) && sett[std::move("use_br_color")].IsBool()) {
                useBrCol_ = std::move(sett[std::move("use_br_color")].GetBool());
            }
            if (sett.HasMember(std::move("show_row")) && sett[std::move("show_row")].IsBool()) {
                shRow_ = std::move(sett[std::move("show_row")].GetBool());
            }
            if (sett.HasMember(std::move("show_other_expr")) && sett[std::move("show_other_expr")].IsBool()) {
                shOtherExpr_ = std::move(sett[std::move("show_other_expr")].GetBool());
            }
            if (sett.HasMember(std::move("show_class_dec")) && sett[std::move("show_class_dec")].IsBool()) {
                shClassDec_ = std::move(sett[std::move("show_class_dec")].GetBool());
            }
            if (sett.HasMember(std::move("show_class_def")) && sett[std::move("show_class_def")].IsBool()) {
                shClassDef_ = std::move(sett[std::move("show_class_def")].GetBool());
            }
            if (sett.HasMember(std::move("show_interf_dec")) && sett[std::move("show_interf_dec")].IsBool()) {
                shInterfDec_ = std::move(sett[std::move("show_interf_dec")].GetBool());
            }
            if (sett.HasMember(std::move("show_interf_def")) && sett[std::move("show_interf_def")].IsBool()) {
                shInterfDef_ = std::move(sett[std::move("show_interf_def")].GetBool());
            }
            if (sett.HasMember(std::move("show_gener_par")) && sett[std::move("show_gener_par")].IsBool()) {
                shGenerPar_ = std::move(sett[std::move("show_gener_par")].GetBool());
            }
            if (sett.HasMember(std::move("show_attribute")) && sett[std::move("show_attribute")].IsBool()) {
                shAttrib_ = std::move(sett[std::move("show_attribute")].GetBool());
            }
            if (sett.HasMember(std::move("show_meth_dec")) && sett[std::move("show_meth_dec")].IsBool()) {
                shMethDec_ = std::move(sett[std::move("show_meth_dec")].GetBool());
            }
            if (sett.HasMember(std::move("show_meth_def")) && sett[std::move("show_meth_def")].IsBool()) {
                shMethDef_ = std::move(sett[std::move("show_meth_def")].GetBool());
            }
            if (sett.HasMember(std::move("show_meth_owner")) && sett[std::move("show_meth_owner")].IsBool()) {
                shMethOwner_ = std::move(sett[std::move("show_meth_owner")].GetBool());
            }
            if (sett.HasMember(std::move("show_func_dec")) && sett[std::move("show_func_dec")].IsBool()) {
                shFuncDec_ = std::move(sett[std::move("show_func_dec")].GetBool());
            }
            if (sett.HasMember(std::move("show_func_def")) && sett[std::move("show_func_def")].IsBool()) {
                shFuncDef_ = std::move(sett[std::move("show_func_def")].GetBool());
            }
            if (sett.HasMember(std::move("show_global_par")) && sett[std::move("show_global_par")].IsBool()) {
                shGlobPar_ = std::move(sett[std::move("show_global_par")].GetBool());
            }
            if (sett.HasMember(std::move("tab_length")) && sett[std::move("tab_length")].IsInt()) {
                int val = std::move(sett[std::move("tab_length")].GetInt());
                if (val >= 0 && val <= 10) {
                    tabLen_ = std::move(val);
                }
            }
            if (sett.HasMember(std::move("margin_length")) && sett[std::move("margin_length")].IsInt()) {
                int val = std::move(sett[std::move("margin_length")].GetInt());
                if (val >= 0 && val <= 10) {
                    margLen_ = std::move(val);
                }
            }
            if (sett.HasMember(std::move("round_br_colors")) && sett[std::move("round_br_colors")].IsArray()) {
                const rj::Value& colors = std::move(sett[std::move("round_br_colors")]);
                for (size_t i = 0; i < colors.Size() && i < 4; ++i) {
                    if (colors[i].IsString()) {
                        roundBrCol_->push_back(std::stringstream(std::move(colors[i].GetString())));
                    }
                }
            }
            if (sett.HasMember(std::move("curl_br_colors")) && sett[std::move("curl_br_colors")].IsArray()) {
                const rj::Value& colors = std::move(sett[std::move("curl_br_colors")]);
                for (size_t i = 0; i < colors.Size() && i < 4; ++i) {
                    if (colors[i].IsString()) {
                        curlBrCol_->push_back(std::stringstream(std::move(colors[i].GetString())));
                    }
                }
            }
            if (sett.HasMember(std::move("row_style")) && sett[std::move("row_style")].IsString()) {
                rowStyle_ = std::make_unique<std::stringstream>(std::move(sett[std::move("row_style")].GetString()));
            }
        }
        if (tconf.HasMember(std::move("SYSTEM_EXPR")) && tconf[std::move("SYSTEM_EXPR")].IsObject()) {
            const rj::Value& ex = std::move(tconf[std::move("SYSTEM_EXPR")]);
            if (ex.HasMember(std::move("class_word")) && ex[std::move("class_word")].IsString() && ex[std::move("class_word")].GetStringLength() > 0) {
                classWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("class_word")].GetString()));
            }
            if (ex.HasMember(std::move("interface_word")) && ex[std::move("interface_word")].IsString() && ex[std::move("interface_word")].GetStringLength() > 0) {
                interfaceWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("interface_word")].GetString()));
            }
            if (ex.HasMember(std::move("implement_word")) && ex[std::move("implement_word")].IsString() && ex[std::move("implement_word")].GetStringLength() > 0) {
                implementWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("implement_word")].GetString()));
            }
            if (ex.HasMember(std::move("extend_word")) && ex[std::move("extend_word")].IsString() && ex[std::move("extend_word")].GetStringLength() > 0) {
                extendWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("extend_word")].GetString()));
            }
            if (ex.HasMember(std::move("this_word")) && ex[std::move("this_word")].IsString() && ex[std::move("this_word")].GetStringLength() > 0) {
                thisWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("this_word")].GetString()));
            }
            if (ex.HasMember(std::move("return_word")) && ex[std::move("return_word")].IsString() && ex[std::move("return_word")].GetStringLength() > 0) {
                returnWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("return_word")].GetString()));
            }
            if (ex.HasMember(std::move("continue_word")) && ex[std::move("continue_word")].IsString() && ex[std::move("continue_word")].GetStringLength() > 0) {
                continueWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("continue_word")].GetString()));
            }
            if (ex.HasMember(std::move("break_word")) && ex[std::move("break_word")].IsString() && ex[std::move("break_word")].GetStringLength() > 0) {
                breakWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("break_word")].GetString()));
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
            if (ex.HasMember(std::move("default_word")) && ex[std::move("default_word")].IsString() && ex[std::move("default_word")].GetStringLength() > 0) {
                defaultWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("default_word")].GetString()));
            }
            if (ex.HasMember(std::move("new_word")) && ex[std::move("new_word")].IsString() && ex[std::move("new_word")].GetStringLength() > 0) {
                newWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("new_word")].GetString()));
            }
            if (ex.HasMember(std::move("delete_word")) && ex[std::move("delete_word")].IsString() && ex[std::move("delete_word")].GetStringLength() > 0) {
                deleteWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("delete_word")].GetString()));
            }
            if (ex.HasMember(std::move("pointer_word")) && ex[std::move("pointer_word")].IsString() && ex[std::move("pointer_word")].GetStringLength() > 0) {
                pointerWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("pointer_word")].GetString()));
            }
            if (ex.HasMember(std::move("virtual_word")) && ex[std::move("virtual_word")].IsString() && ex[std::move("virtual_word")].GetStringLength() > 0) {
                virtualWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("virtual_word")].GetString()));
            }
            if (ex.HasMember(std::move("template_word")) && ex[std::move("template_word")].IsString() && ex[std::move("template_word")].GetStringLength() > 0) {
                templateWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("template_word")].GetString()));
            }
            if (ex.HasMember(std::move("STYLE")) && ex[std::move("STYLE")].IsObject()) {
                const rj::Value& st = std::move(ex[std::move("STYLE")]);
                if (st.HasMember(std::move("general_style")) && st[std::move("general_style")].IsString()) {
                    systExprStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("general_style")].GetString()));
                }
                if (st.HasMember(std::move("class_word_style")) && st[std::move("class_word_style")].IsString()) {
                    classWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("class_word_style")].GetString()));
                }
                if (st.HasMember(std::move("interface_word_style")) && st[std::move("interface_word_style")].IsString()) {
                    interfaceWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("interface_word_style")].GetString()));
                }
                if (st.HasMember(std::move("implement_word_style")) && st[std::move("implement_word_style")].IsString()) {
                    implementWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("implement_word_style")].GetString()));
                }
                if (st.HasMember(std::move("extend_word_style")) && st[std::move("extend_word_style")].IsString()) {
                    extendWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("extend_word_style")].GetString()));
                }
                if (st.HasMember(std::move("this_word_style")) && st[std::move("this_word_style")].IsString()) {
                    thisWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("this_word_style")].GetString()));
                }
                if (st.HasMember(std::move("return_word_style")) && st[std::move("return_word_style")].IsString()) {
                    returnWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("return_word_style")].GetString()));
                }
                if (st.HasMember(std::move("continue_word_style")) && st[std::move("continue_word_style")].IsString()) {
                    continueWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("continue_word_style")].GetString()));
                }
                if (st.HasMember(std::move("break_word_style")) && st[std::move("break_word_style")].IsString()) {
                    breakWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("break_word_style")].GetString()));
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
                if (st.HasMember(std::move("default_word_style")) && st[std::move("default_word_style")].IsString()) {
                    defaultWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("default_word_style")].GetString()));
                }
                if (st.HasMember(std::move("new_word_style")) && st[std::move("new_word_style")].IsString()) {
                    newWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("new_word_style")].GetString()));
                }
                if (st.HasMember(std::move("delete_word_style")) && st[std::move("delete_word_style")].IsString()) {
                    deleteWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("delete_word_style")].GetString()));
                }
                if (st.HasMember(std::move("pointer_word_style")) && st[std::move("pointer_word_style")].IsString()) {
                    pointerWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("pointer_word_style")].GetString()));
                }
                if (st.HasMember(std::move("virtual_word_style")) && st[std::move("virtual_word_style")].IsString()) {
                    virtualWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("virtual_word_style")].GetString()));
                }
                if (st.HasMember(std::move("template_word_style")) && st[std::move("template_word_style")].IsString()) {
                    templateWordStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("template_word_style")].GetString()));
                }
            }
        }
        if (tconf.HasMember(std::move("OTHER_EXPR")) && tconf[std::move("OTHER_EXPR")].IsObject()) {
            const rj::Value& ex = std::move(tconf[std::move("OTHER_EXPR")]);
            if (ex.HasMember(std::move("constructor_word")) && ex[std::move("constructor_word")].IsString() && ex[std::move("constructor_word")].GetStringLength() > 0) {
                constrWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("constructor_word")].GetString()));
            }
            if (ex.HasMember(std::move("destructor_word")) && ex[std::move("destructor_word")].IsString() && ex[std::move("destructor_word")].GetStringLength() > 0) {
                destrWord_ = std::make_unique<std::stringstream>(std::move(ex[std::move("destructor_word")].GetString()));
            }
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
                if (st.HasMember(std::move("constructor_style")) && st[std::move("constructor_style")].IsString()) {
                    constrStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("constructor_style")].GetString()));
                }
                if (st.HasMember(std::move("destructor_style")) && st[std::move("destructor_style")].IsString()) {
                    destrStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("destructor_style")].GetString()));
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
    shFuncDec_ = true;
    shFuncDef_ = true;
    shGlobPar_ = true;
    tabLen_ = 4;
    margLen_ = 3;
    roundBrCol_ = std::make_unique<std::vector<std::stringstream>>();
    curlBrCol_ = std::make_unique<std::vector<std::stringstream>>();
    rowStyle_ = std::make_unique<std::stringstream>();
    classWord_ = std::make_unique<std::stringstream>(std::move("class"));
    interfaceWord_ = std::make_unique<std::stringstream>(std::move("interface"));
    implementWord_ = std::make_unique<std::stringstream>(std::move("implements"));
    extendWord_ = std::make_unique<std::stringstream>(std::move("extends"));
    thisWord_ = std::make_unique<std::stringstream>(std::move("this"));
    returnWord_ = std::make_unique<std::stringstream>(std::move("return"));
    continueWord_ = std::make_unique<std::stringstream>(std::move("continue"));
    breakWord_ = std::make_unique<std::stringstream>(std::move("break"));
    throwWord_ = std::make_unique<std::stringstream>(std::move("throw"));
    ifWord_ = std::make_unique<std::stringstream>(std::move("if"));
    elseWord_ = std::make_unique<std::stringstream>(std::move("else"));
    doWord_ = std::make_unique<std::stringstream>(std::move("do"));
    whileWord_ = std::make_unique<std::stringstream>(std::move("while"));
    forWord_ = std::make_unique<std::stringstream>(std::move("for"));
    repeatWord_ = std::make_unique<std::stringstream>(std::move("repeat"));
    switchWord_ = std::make_unique<std::stringstream>(std::move("switch"));
    caseWord_ = std::make_unique<std::stringstream>(std::move("case"));
    defaultWord_ = std::make_unique<std::stringstream>(std::move("default"));
    newWord_ = std::make_unique<std::stringstream>(std::move("new"));
    deleteWord_ = std::make_unique<std::stringstream>(std::move("delete"));
    pointerWord_ = std::make_unique<std::stringstream>(std::move("↑"));
    virtualWord_ = std::make_unique<std::stringstream>(std::move("is virtual"));
    templateWord_ = std::make_unique<std::stringstream>(std::move("template"));
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
    templateWordStyle_ = std::make_unique<std::stringstream>();
    constrWord_ = std::make_unique<std::stringstream>(std::move("constructor"));
    destrWord_ = std::make_unique<std::stringstream>(std::move("destructor"));
    methodWord_ = std::make_unique<std::stringstream>(std::move("method"));
    functionWord_ = std::make_unique<std::stringstream>(std::move("function"));
    lambdaWord_ = std::make_unique<std::stringstream>(std::move("lambda"));
    callWord_ = std::make_unique<std::stringstream>(std::move("call"));
    defineWord_ = std::make_unique<std::stringstream>(std::move("define"));
    returnsWord_ = std::make_unique<std::stringstream>(std::move("returns"));
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