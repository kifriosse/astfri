#include <libastfri-text/inc/Configurator.hpp>
#include <lib/rapidjson/filereadstream.h>
#include <lib/rapidjson/document.h>
#include <filesystem>

Configurator::Configurator() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    bool foundBuildFolder = false;
    while (currentPath.has_filename() || currentPath.parent_path().has_filename()) {
        if (currentPath.filename() == "build") {
            foundBuildFolder = true;
            break;
        }
        currentPath = currentPath.parent_path();
    }
    if (!foundBuildFolder) {
        currentPath.clear();
        std::string user = getenv(std::move("USER"));
        currentPath = std::move("/home/") + std::move(user);
    }
    currentPath.concat(std::move("/"));
    defFolderPath_ = std::make_unique<std::stringstream>(std::move(currentPath));
    set_defaults();
}

void Configurator::set_input_path(std::string path) {
    configFilePath_ = std::make_unique<std::stringstream>(std::move(path));
}

void Configurator::reset_def_path() {
    defaultOutputFilePath_ = std::make_unique<std::stringstream>(defFolderPath_->str() + outputFileName_->str());
}

void Configurator::reset_out_path() {
    outputFilePath_ = std::make_unique<std::stringstream>(outFolderPath_->str() + outputFileName_->str());
}

void Configurator::load_new_config_file() {
    FILE* configFile = fopen(std::move(configFilePath_->str().c_str()), std::move("r"));
    char readBuffer[65536];
    namespace rj = rapidjson;
    rj::FileReadStream inputStream(configFile, readBuffer, sizeof(std::move(readBuffer)));
    rj::Document doc;
    doc.ParseStream(inputStream);
    bool resetPath = false;
    if (doc.HasMember(std::move("CONFIGURATOR")) && doc[std::move("CONFIGURATOR")].IsObject()) {
        const rj::Value& conf = std::move(doc[std::move("CONFIGURATOR")]);
        if (conf.HasMember(std::move("FILE")) && conf[std::move("FILE")].IsObject()) {
            const rj::Value& file = std::move(conf[std::move("FILE")]);
            if (file.HasMember(std::move("name")) && file[std::move("name")].IsString() && file[std::move("name")].GetStringLength() > 0) {
                std::string name = std::move(file[std::move("name")].GetString());
                if (!name.starts_with(std::move(" ")) && !name.ends_with(std::move(" "))) {
                    outputFileName_ = std::make_unique<std::stringstream>(std::move(name));
                    resetPath = true;
                }
            }
            if (file.HasMember(std::move("path")) && file[std::move("path")].IsString() && file[std::move("path")].GetStringLength() > 0) {
                std::string path = std::move(file[std::move("path")].GetString());
                if (!path.starts_with(std::move(" ")) && !path.ends_with(std::move(" "))) {
                    if (path.at(std::move(path.length() - 1)) != '/') {
                        path.append(std::move("/"));
                    }
                    outFolderPath_ = std::make_unique<std::stringstream>(std::move(path));
                    resetPath = true;
                }
            }
            if (file.HasMember(std::move("format")) && file[std::move("format")].IsString()) {
                outputFileFormat_ = std::make_unique<std::stringstream>(std::move(file[std::move("format")].GetString()));
            }
        }
        if (conf.HasMember(std::move("DEFAULT_STYLE")) && conf[std::move("DEFAULT_STYLE")].IsString()) {
            defaultStyle_ = std::make_unique<std::stringstream>(std::move(conf[std::move("DEFAULT_STYLE")].GetString()));
        }
        if (conf.HasMember(std::move("UNKNOWN_WORD")) && conf[std::move("UNKNOWN_WORD")].IsString() && conf[std::move("UNKNOWN_WORD")].GetStringLength() > 0) {
            unknownWord_ = std::make_unique<std::stringstream>(std::move(conf[std::move("UNKNOWN_WORD")].GetString()));
        }
        if (conf.HasMember(std::move("UNKNOWN_WORD_STYLE")) && conf[std::move("UNKNOWN_WORD_STYLE")].IsString()) {
            unknownWordStyle_ = std::make_unique<std::stringstream>(std::move(conf[std::move("UNKNOWN_WORD_STYLE")].GetString()));
        }
        if (conf.HasMember(std::move("ACCESS_MOD")) && conf[std::move("ACCESS_MOD")].IsObject()) {
            const rj::Value& acc = std::move(conf[std::move("ACCESS_MOD")]);
            if (acc.HasMember(std::move("view")) && acc[std::move("view")].IsString()) {
                view_ = std::make_unique<std::stringstream>(std::move(acc[std::move("view")].GetString()));
            }
            if (acc.HasMember(std::move("public")) && acc[std::move("public")].IsString() && acc[std::move("public")].GetStringLength() > 0) {
                publicWord_ = std::make_unique<std::stringstream>(std::move(acc[std::move("public")].GetString()));
            }
            if (acc.HasMember(std::move("private")) && acc[std::move("private")].IsString() && acc[std::move("private")].GetStringLength() > 0) {
                privateWord_ = std::make_unique<std::stringstream>(std::move(acc[std::move("private")].GetString()));
            }
            if (acc.HasMember(std::move("protected")) && acc[std::move("protected")].IsString() && acc[std::move("protected")].GetStringLength() > 0) {
                protectedWord_ = std::make_unique<std::stringstream>(std::move(acc[std::move("protected")].GetString()));
            }
            if (acc.HasMember(std::move("style")) && acc[std::move("style")].IsString()) {
                accessModStyle_ = std::make_unique<std::stringstream>(std::move(acc[std::move("style")].GetString()));
            }
        }
        if (conf.HasMember(std::move("DATA_TYPE")) && conf[std::move("DATA_TYPE")].IsObject()) {
            const rj::Value& dt = std::move(conf[std::move("DATA_TYPE")]);
            if (dt.HasMember(std::move("dynamic")) && dt[std::move("dynamic")].IsString() && dt[std::move("dynamic")].GetStringLength() > 0) {
                dynamicWord_ = std::make_unique<std::stringstream>(std::move(dt[std::move("dynamic")].GetString()));
            }
            if (dt.HasMember(std::move("int")) && dt[std::move("int")].IsString() && dt[std::move("int")].GetStringLength() > 0) {
                intWord_ = std::make_unique<std::stringstream>(std::move(dt[std::move("int")].GetString()));
            }
            if (dt.HasMember(std::move("float")) && dt[std::move("float")].IsString() && dt[std::move("float")].GetStringLength() > 0) {
                floatWord_ = std::make_unique<std::stringstream>(std::move(dt[std::move("float")].GetString()));
            }
            if (dt.HasMember(std::move("char")) && dt[std::move("char")].IsString() && dt[std::move("char")].GetStringLength() > 0) {
                charWord_ = std::make_unique<std::stringstream>(std::move(dt[std::move("char")].GetString()));
            }
            if (dt.HasMember(std::move("bool")) && dt[std::move("bool")].IsString() && dt[std::move("bool")].GetStringLength() > 0) {
                boolWord_ = std::make_unique<std::stringstream>(std::move(dt[std::move("bool")].GetString()));
            }
            if (dt.HasMember(std::move("void")) && dt[std::move("void")].IsString() && dt[std::move("void")].GetStringLength() > 0) {
                voidWord_ = std::make_unique<std::stringstream>(std::move(dt[std::move("void")].GetString()));
            }
            if (dt.HasMember(std::move("STYLE")) && dt[std::move("STYLE")].IsObject()) {
                const rj::Value& st = std::move(dt[std::move("STYLE")]);
                if (st.HasMember(std::move("general_style")) && st[std::move("general_style")].IsString()) {
                    typeStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("general_style")].GetString()));
                }
                if (st.HasMember(std::move("dynamic_type_style")) && st[std::move("dynamic_type_style")].IsString()) {
                    dynamicTypeStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("dynamic_type_style")].GetString()));
                }
                if (st.HasMember(std::move("int_type_style")) && st[std::move("int_type_style")].IsString()) {
                    intTypeStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("int_type_style")].GetString()));
                }
                if (st.HasMember(std::move("float_type_style")) && st[std::move("float_type_style")].IsString()) {
                    floatTypeStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("float_type_style")].GetString()));
                }
                if (st.HasMember(std::move("char_type_style")) && st[std::move("char_type_style")].IsString()) {
                    charTypeStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("char_type_style")].GetString()));
                }
                if (st.HasMember(std::move("bool_type_style")) && st[std::move("bool_type_style")].IsString()) {
                    boolTypeStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("bool_type_style")].GetString()));
                }
                if (st.HasMember(std::move("void_type_style")) && st[std::move("void_type_style")].IsString()) {
                    voidTypeStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("void_type_style")].GetString()));
                }
                if (st.HasMember(std::move("user_type_style")) && st[std::move("user_type_style")].IsString()) {
                    userTypeStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("user_type_style")].GetString()));
                }
            }
        }
        if (conf.HasMember(std::move("REF_NAME_STYLE")) && conf[std::move("REF_NAME_STYLE")].IsObject()) {
            const rj::Value& ref = std::move(conf[std::move("REF_NAME_STYLE")]);
            if (ref.HasMember(std::move("def_ref_name_style")) && ref[std::move("def_ref_name_style")].IsString()) {
                defRefNameStyle_ = std::make_unique<std::stringstream>(std::move(ref[std::move("def_ref_name_style")].GetString()));
            }
            if (ref.HasMember(std::move("gen_param_name_style")) && ref[std::move("gen_param_name_style")].IsString()) {
                generParamNameStyle_ = std::make_unique<std::stringstream>(std::move(ref[std::move("gen_param_name_style")].GetString()));
            }
            if (ref.HasMember(std::move("gen_param_constr_style")) && ref[std::move("gen_param_constr_style")].IsString()) {
                generParamConstrStyle_ = std::make_unique<std::stringstream>(std::move(ref[std::move("gen_param_constr_style")].GetString()));
            }
            if (ref.HasMember(std::move("class_name_style")) && ref[std::move("class_name_style")].IsString()) {
                classNameStyle_ = std::make_unique<std::stringstream>(std::move(ref[std::move("class_name_style")].GetString()));
            }
            if (ref.HasMember(std::move("method_name_style")) && ref[std::move("method_name_style")].IsString()) {
                methodNameStyle_ = std::make_unique<std::stringstream>(std::move(ref[std::move("method_name_style")].GetString()));
            }
            if (ref.HasMember(std::move("function_name_style")) && ref[std::move("function_name_style")].IsString()) {
                functionNameStyle_ = std::make_unique<std::stringstream>(std::move(ref[std::move("function_name_style")].GetString()));
            }
            if (ref.HasMember(std::move("global_var_name_style")) && ref[std::move("global_var_name_style")].IsString()) {
                globalVarNameStyle_ = std::make_unique<std::stringstream>(std::move(ref[std::move("global_var_name_style")].GetString()));
            }
            if (ref.HasMember(std::move("member_var_name_style")) && ref[std::move("member_var_name_style")].IsString()) {
                memberVarNameStyle_ = std::make_unique<std::stringstream>(std::move(ref[std::move("member_var_name_style")].GetString()));
            }
            if (ref.HasMember(std::move("local_var_name_style")) && ref[std::move("local_var_name_style")].IsString()) {
                localVarNameStyle_ = std::make_unique<std::stringstream>(std::move(ref[std::move("local_var_name_style")].GetString()));
            }
            if (ref.HasMember(std::move("param_var_name_style")) && ref[std::move("param_var_name_style")].IsString()) {
                paramVarNameStyle_ = std::make_unique<std::stringstream>(std::move(ref[std::move("param_var_name_style")].GetString()));
            }
        }
        if (conf.HasMember(std::move("OPERATOR")) && conf[std::move("OPERATOR")].IsObject()) {
            const rj::Value& op = std::move(conf[std::move("OPERATOR")]);
            if (op.HasMember(std::move("assign")) && op[std::move("assign")].IsString() && op[std::move("assign")].GetStringLength() > 0) {
                assignOpWord_ = std::make_unique<std::stringstream>(std::move(op[std::move("assign")].GetString()));
            }
            if (op.HasMember(std::move("style")) && op[std::move("style")].IsString()) {
                operatorStyle_ = std::make_unique<std::stringstream>(std::move(op[std::move("style")].GetString()));
            }
        }
        if (conf.HasMember(std::move("SEPARATOR")) && conf[std::move("SEPARATOR")].IsObject()) {
            const rj::Value& sep = std::move(conf[std::move("SEPARATOR")]);
            if (sep.HasMember(std::move("style")) && sep[std::move("style")].IsString()) {
                separatorStyle_ = std::make_unique<std::stringstream>(std::move(sep[std::move("style")].GetString()));
            }
        }
        if (conf.HasMember(std::move("VALUE")) && conf[std::move("VALUE")].IsObject()) {
            const rj::Value& val = std::move(conf[std::move("VALUE")]);
            if (val.HasMember(std::move("true_val")) && val[std::move("true_val")].IsString() && val[std::move("true_val")].GetStringLength() > 0) {
                trueVal_ = std::make_unique<std::stringstream>(std::move(val[std::move("true_val")].GetString()));
            }
            if (val.HasMember(std::move("false_val")) && val[std::move("false_val")].IsString() && val[std::move("false_val")].GetStringLength() > 0) {
                falseVal_ = std::make_unique<std::stringstream>(std::move(val[std::move("false_val")].GetString()));
            }
            if (val.HasMember(std::move("null_val")) && val[std::move("null_val")].IsString() && val[std::move("null_val")].GetStringLength() > 0) {
                nullVal_ = std::make_unique<std::stringstream>(std::move(val[std::move("null_val")].GetString()));
            }
            if (val.HasMember(std::move("STYLE")) && val[std::move("STYLE")].IsObject()) {
                const rj::Value& st = std::move(val[std::move("STYLE")]);
                if (st.HasMember(std::move("general_val_style")) && st[std::move("general_val_style")].IsString()) {
                    valStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("general_val_style")].GetString()));
                }
                if (st.HasMember(std::move("int_val_style")) && st[std::move("int_val_style")].IsString()) {
                    intValStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("int_val_style")].GetString()));
                }
                if (st.HasMember(std::move("float_val_style")) && st[std::move("float_val_style")].IsString()) {
                    floatValStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("float_val_style")].GetString()));
                }
                if (st.HasMember(std::move("char_val_style")) && st[std::move("char_val_style")].IsString()) {
                    charValStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("char_val_style")].GetString()));
                }
                if (st.HasMember(std::move("string_val_style")) && st[std::move("string_val_style")].IsString()) {
                    stringValStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("string_val_style")].GetString()));
                }
                if (st.HasMember(std::move("bool_val_style")) && st[std::move("bool_val_style")].IsString()) {
                    boolValStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("bool_val_style")].GetString()));
                }
                if (st.HasMember(std::move("null_val_style")) && st[std::move("null_val_style")].IsString()) {
                    nullValStyle_ = std::make_unique<std::stringstream>(std::move(st[std::move("null_val_style")].GetString()));
                }
            }
        }
    }
    fclose(std::move(configFile));
    if (resetPath) {
        reset_def_path();
        reset_out_path();
    }
}

void Configurator::set_defaults() {
    outputFileName_ = std::make_unique<std::stringstream>(std::move("output"));
    outFolderPath_ = std::make_unique<std::stringstream>(defFolderPath_->str());
    defaultOutputFilePath_ = std::make_unique<std::stringstream>(defFolderPath_->str() + outputFileName_->str());
    outputFilePath_ = std::make_unique<std::stringstream>(defaultOutputFilePath_->str());
    outputFileFormat_ = std::make_unique<std::stringstream>(std::move("txt"));
    defaultStyle_ = std::make_unique<std::stringstream>(std::move("font-family:Consolas;font-size:18px"));
    unknownWord_ = std::make_unique<std::stringstream>(std::move("UNKNOWN EXPRESSION"));
    unknownWordStyle_ = std::make_unique<std::stringstream>();
    view_ = std::make_unique<std::stringstream>(std::move("inner"));
    publicWord_ = std::make_unique<std::stringstream>(std::move("public"));
    privateWord_ = std::make_unique<std::stringstream>(std::move("private"));
    protectedWord_ = std::make_unique<std::stringstream>(std::move("protected"));
    accessModStyle_ = std::make_unique<std::stringstream>();
    dynamicWord_ = std::make_unique<std::stringstream>(std::move("dynamic"));
    intWord_ = std::make_unique<std::stringstream>(std::move("int"));
    floatWord_ = std::make_unique<std::stringstream>(std::move("float"));
    charWord_ = std::make_unique<std::stringstream>(std::move("char"));
    boolWord_ = std::make_unique<std::stringstream>(std::move("bool"));
    voidWord_ = std::make_unique<std::stringstream>(std::move("void"));
    typeStyle_ = std::make_unique<std::stringstream>();
    dynamicTypeStyle_ = std::make_unique<std::stringstream>();
    intTypeStyle_ = std::make_unique<std::stringstream>();
    floatTypeStyle_ = std::make_unique<std::stringstream>();
    charTypeStyle_ = std::make_unique<std::stringstream>();
    boolTypeStyle_ = std::make_unique<std::stringstream>();
    voidTypeStyle_ = std::make_unique<std::stringstream>();
    userTypeStyle_ = std::make_unique<std::stringstream>();
    defRefNameStyle_ = std::make_unique<std::stringstream>();
    generParamNameStyle_ = std::make_unique<std::stringstream>();
    generParamConstrStyle_ = std::make_unique<std::stringstream>();
    classNameStyle_ = std::make_unique<std::stringstream>();
    methodNameStyle_ = std::make_unique<std::stringstream>();
    functionNameStyle_ = std::make_unique<std::stringstream>();
    globalVarNameStyle_ = std::make_unique<std::stringstream>();
    memberVarNameStyle_ = std::make_unique<std::stringstream>();
    localVarNameStyle_ = std::make_unique<std::stringstream>();
    paramVarNameStyle_ = std::make_unique<std::stringstream>();
    assignOpWord_ = std::make_unique<std::stringstream>(std::move("="));
    operatorStyle_ = std::make_unique<std::stringstream>();
    separatorStyle_ = std::make_unique<std::stringstream>();
    trueVal_ = std::make_unique<std::stringstream>(std::move("true"));
    falseVal_ = std::make_unique<std::stringstream>(std::move("false"));
    nullVal_ = std::make_unique<std::stringstream>(std::move("NULL"));
    valStyle_ = std::make_unique<std::stringstream>();
    intValStyle_ = std::make_unique<std::stringstream>();
    floatValStyle_ = std::make_unique<std::stringstream>();
    charValStyle_ = std::make_unique<std::stringstream>();
    stringValStyle_ = std::make_unique<std::stringstream>();
    boolValStyle_ = std::make_unique<std::stringstream>();
    nullValStyle_ = std::make_unique<std::stringstream>();
}