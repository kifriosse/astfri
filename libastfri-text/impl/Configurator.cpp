#include <libastfri-text/inc/Configurator.hpp>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

Configurator::Configurator() {
    fs::path currentPath = fs::current_path();
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
        std::string user = getenv("USER");
        currentPath = "/home/" + user;
    }
    currentPath.concat("/text_output/");
    defaultOutputFilePath_ = std::make_unique<std::stringstream>(std::move(currentPath));
    set_defaults();
}

void Configurator::set_input_path(const std::string& path) {
    configFilePath_ = std::make_unique<std::stringstream>(std::move(path));
}

void Configurator::load_new_config_file() {
    FILE* configFile = fopen(configFilePath_->str().c_str(), "r");
    char readBuffer[65536];
    namespace rj = rapidjson;
    rj::FileReadStream inputStream(configFile, readBuffer, sizeof(readBuffer));
    rj::Document doc;
    doc.ParseStream(inputStream);
    if (doc.HasMember("CONFIGURATOR") && doc["CONFIGURATOR"].IsObject()) {
        rj::Value& conf = doc["CONFIGURATOR"];
        if (conf.HasMember("FILE") && conf["FILE"].IsObject()) {
            rj::Value& file = conf["FILE"];
            if (file.HasMember("name") && file["name"].IsString() && file["name"].GetStringLength() > 0) {
                std::string name = file["name"].GetString();
                size_t pos = 0;
                while ((pos = name.find(" ", pos)) != std::string::npos) { // remove spaces from name
                    name.replace(pos, 1, "");
                }
                pos = 0;
                while ((pos = name.find("/", pos)) != std::string::npos) { // remove / from name
                    name.replace(pos, 1, "");
                }
                if (name.length() > 0) {
                    outputFileName_ = std::make_unique<std::stringstream>(std::move(name));
                }
            }
            if (file.HasMember("path") && file["path"].IsString()) {
                std::string path = file["path"].GetString();
                size_t pos = 0;
                while ((pos = path.find(" ", pos)) != std::string::npos) { // remove spaces from path
                    path.replace(pos, 1, "");
                }
                pos = 0;
                while ((pos = path.find("//", pos)) != std::string::npos) { // replace multiple /// by single /
                    path.replace(pos, 2, "/");
                }
                if (path == "build") { // path == "build" -> default path
                    outputFilePath_ = std::make_unique<std::stringstream>(defaultOutputFilePath_->str());
                } else if (path == "desktop") { // path == "desktop" -> active user desktop + text_folder
                    FILE* file = popen("cd /mnt/c && cmd.exe /c whoami", "r");
                    std::stringstream result;
                    char ch;
                    while ((ch = fgetc(file)) != EOF) {
                        result.put(ch);
                    }
                    pclose(file);
                    std::string user = result.str();
                    user.pop_back();
                    user.pop_back();
                    user = user.substr(user.find_last_of("\\") + 1);
                    outputFilePath_ = std::make_unique<std::stringstream>("/mnt/c/Users/" + user + "/Desktop/text_output/");
                } else if (path.length() > 0 && path.at(0) == '/') { // if path starts with / -> it is absolute path
                    path = path.substr(1);
                    std::stringstream ss(path);
                    std::string seg;
                    std::vector<std::string> segments;
                    while (std::getline(ss, seg, '/')) {
                        segments.push_back(seg);
                    }
                    if (!segments.empty()) { // if path contains any folder
                        fs::path newPath = "/";
                        bool useDef = false;
                        int level = 0;
                        for (size_t i = 0; i < segments.size(); ++i) {
                            newPath.concat(segments.at(i));
                            if (i != segments.size() - 1) {
                                newPath.concat("/");
                            }
                            try { // try to open folder
                                if (fs::exists(newPath) && fs::is_directory(newPath)) { // if partial path exists and it is folder
                                    if (i == segments.size() - 1) { // it is full path
                                        if (access(newPath.c_str(), W_OK) == 0) { // if folder has permission to write into itself
                                            std::string out = newPath;
                                            out.append("/");
                                            outputFilePath_ = std::make_unique<std::stringstream>(std::move(out));
                                        } else {
                                            useDef = true;
                                        }
                                    }
                                } else { // partial path does not exist
                                    fs::create_directory(newPath);
                                    ++level;
                                    if (i == segments.size() - 1) { // if it is full created path
                                        if (access(newPath.c_str(), W_OK) == 0) { // if new folder has permission to write into itself
                                            std::string out = newPath;
                                            out.append("/");
                                            outputFilePath_ = std::make_unique<std::stringstream>(std::move(out));
                                        } else {
                                            useDef = true;
                                        }
                                        while (level != 1) { // need to delete created folder for now
                                            --level;
                                            newPath = newPath.parent_path();
                                        }
                                        fs::remove_all(newPath);
                                    }
                                }
                            } catch (const std::exception&) { // error in entering folder
                                useDef = true;
                            }
                            if (useDef) { // should use default path?
                                if (level > 0) { // if directories were created, need to delete them
                                    while (level != 1) {
                                        --level;
                                        newPath = newPath.parent_path();
                                    }
                                    fs::remove_all(newPath);
                                }
                                outputFilePath_ = std::make_unique<std::stringstream>(defaultOutputFilePath_->str());
                                break;
                            }
                        }
                    } else { // if path does not contain anything except /
                        outputFilePath_ = std::make_unique<std::stringstream>(defaultOutputFilePath_->str());
                    }
                } else { // if path == "" or is relative -> program/path/text_folder
                    fs::path currentPath = fs::current_path();
                    currentPath.concat("/");
                    if (path.length() > 0 && !path.ends_with("/")) {
                        path.append("/");
                    }
                    path.append("text_output/");
                    currentPath.concat(path);
                    outputFilePath_ = std::make_unique<std::stringstream>(std::move(currentPath));
                }
            } else { // explicitly set default path, if not present
                outputFilePath_ = std::make_unique<std::stringstream>(defaultOutputFilePath_->str());
            }
            if (file.HasMember("format") && file["format"].IsString()) {
                outputFileFormat_ = std::make_unique<std::stringstream>(file["format"].GetString());
            }
        }
        if (conf.HasMember("DEFAULT_STYLE") && conf["DEFAULT_STYLE"].IsString()) {
            defaultStyle_ = std::make_unique<std::stringstream>(conf["DEFAULT_STYLE"].GetString());
        }
        if (conf.HasMember("UNKNOWN_WORD") && conf["UNKNOWN_WORD"].IsString() && conf["UNKNOWN_WORD"].GetStringLength() > 0) {
            unknownWord_ = std::make_unique<std::stringstream>(conf["UNKNOWN_WORD"].GetString());
        }
        if (conf.HasMember("UNKNOWN_WORD_STYLE") && conf["UNKNOWN_WORD_STYLE"].IsString()) {
            unknownWordStyle_ = std::make_unique<std::stringstream>(conf["UNKNOWN_WORD_STYLE"].GetString());
        }
        if (conf.HasMember("INVALID_WORD") && conf["INVALID_WORD"].IsString() && conf["INVALID_WORD"].GetStringLength() > 0) {
            invalidWord_ = std::make_unique<std::stringstream>(conf["INVALID_WORD"].GetString());
        }
        if (conf.HasMember("INVALID_WORD_STYLE") && conf["INVALID_WORD_STYLE"].IsString()) {
            invalidWordStyle_ = std::make_unique<std::stringstream>(conf["INVALID_WORD_STYLE"].GetString());
        }
        if (conf.HasMember("ACCESS_MOD") && conf["ACCESS_MOD"].IsObject()) {
            rj::Value& acc = conf["ACCESS_MOD"];
            if (acc.HasMember("view") && acc["view"].IsString()) {
                view_ = std::make_unique<std::stringstream>(acc["view"].GetString());
            }
            if (acc.HasMember("public") && acc["public"].IsString() && acc["public"].GetStringLength() > 0) {
                publicWord_ = std::make_unique<std::stringstream>(acc["public"].GetString());
            }
            if (acc.HasMember("private") && acc["private"].IsString() && acc["private"].GetStringLength() > 0) {
                privateWord_ = std::make_unique<std::stringstream>(acc["private"].GetString());
            }
            if (acc.HasMember("protected") && acc["protected"].IsString() && acc["protected"].GetStringLength() > 0) {
                protectedWord_ = std::make_unique<std::stringstream>(acc["protected"].GetString());
            }
            if (acc.HasMember("internal") && acc["internal"].IsString() && acc["internal"].GetStringLength() > 0) {
                internalWord_ = std::make_unique<std::stringstream>(acc["internal"].GetString());
            }
            if (acc.HasMember("attribs") && acc["attribs"].IsString() && acc["attribs"].GetStringLength() > 0) {
                accAtribWord_ = std::make_unique<std::stringstream>(acc["attribs"].GetString());
            }
            if (acc.HasMember("constructors") && acc["constructors"].IsString() && acc["constructors"].GetStringLength() > 0) {
                accConstrWord_ = std::make_unique<std::stringstream>(acc["constructors"].GetString());
            }
            if (acc.HasMember("destructors") && acc["destructors"].IsString() && acc["destructors"].GetStringLength() > 0) {
                accDestrWord_ = std::make_unique<std::stringstream>(acc["destructors"].GetString());
            }
            if (acc.HasMember("methods") && acc["methods"].IsString() && acc["methods"].GetStringLength() > 0) {
                accMethWord_ = std::make_unique<std::stringstream>(acc["methods"].GetString());
            }
            if (acc.HasMember("style") && acc["style"].IsString()) {
                accStyle_ = std::make_unique<std::stringstream>(acc["style"].GetString());
            }
        }
        if (conf.HasMember("DATA_TYPE") && conf["DATA_TYPE"].IsObject()) {
            rj::Value& dt = conf["DATA_TYPE"];
            if (dt.HasMember("dynamic") && dt["dynamic"].IsString() && dt["dynamic"].GetStringLength() > 0) {
                dynamicWord_ = std::make_unique<std::stringstream>(dt["dynamic"].GetString());
            }
            if (dt.HasMember("int") && dt["int"].IsString() && dt["int"].GetStringLength() > 0) {
                intWord_ = std::make_unique<std::stringstream>(dt["int"].GetString());
            }
            if (dt.HasMember("float") && dt["float"].IsString() && dt["float"].GetStringLength() > 0) {
                floatWord_ = std::make_unique<std::stringstream>(dt["float"].GetString());
            }
            if (dt.HasMember("char") && dt["char"].IsString() && dt["char"].GetStringLength() > 0) {
                charWord_ = std::make_unique<std::stringstream>(dt["char"].GetString());
            }
            if (dt.HasMember("bool") && dt["bool"].IsString() && dt["bool"].GetStringLength() > 0) {
                boolWord_ = std::make_unique<std::stringstream>(dt["bool"].GetString());
            }
            if (dt.HasMember("void") && dt["void"].IsString() && dt["void"].GetStringLength() > 0) {
                voidWord_ = std::make_unique<std::stringstream>(dt["void"].GetString());
            }
            if (dt.HasMember("STYLE") && dt["STYLE"].IsObject()) {
                rj::Value& st = dt["STYLE"];
                if (st.HasMember("general_style") && st["general_style"].IsString()) {
                    typeStyle_ = std::make_unique<std::stringstream>(st["general_style"].GetString());
                }
                if (st.HasMember("dynamic_type_style") && st["dynamic_type_style"].IsString()) {
                    dynamicTypeStyle_ = std::make_unique<std::stringstream>(st["dynamic_type_style"].GetString());
                }
                if (st.HasMember("int_type_style") && st["int_type_style"].IsString()) {
                    intTypeStyle_ = std::make_unique<std::stringstream>(st["int_type_style"].GetString());
                }
                if (st.HasMember("float_type_style") && st["float_type_style"].IsString()) {
                    floatTypeStyle_ = std::make_unique<std::stringstream>(st["float_type_style"].GetString());
                }
                if (st.HasMember("char_type_style") && st["char_type_style"].IsString()) {
                    charTypeStyle_ = std::make_unique<std::stringstream>(st["char_type_style"].GetString());
                }
                if (st.HasMember("bool_type_style") && st["bool_type_style"].IsString()) {
                    boolTypeStyle_ = std::make_unique<std::stringstream>(st["bool_type_style"].GetString());
                }
                if (st.HasMember("void_type_style") && st["void_type_style"].IsString()) {
                    voidTypeStyle_ = std::make_unique<std::stringstream>(st["void_type_style"].GetString());
                }
                if (st.HasMember("user_type_style") && st["user_type_style"].IsString()) {
                    userTypeStyle_ = std::make_unique<std::stringstream>(st["user_type_style"].GetString());
                }
            }
        }
        if (conf.HasMember("REF_NAME_STYLE") && conf["REF_NAME_STYLE"].IsObject()) {
            rj::Value& ref = conf["REF_NAME_STYLE"];
            if (ref.HasMember("def_ref_name_style") && ref["def_ref_name_style"].IsString()) {
                defRefNameStyle_ = std::make_unique<std::stringstream>(ref["def_ref_name_style"].GetString());
            }
            if (ref.HasMember("gen_param_name_style") && ref["gen_param_name_style"].IsString()) {
                generParamNameStyle_ = std::make_unique<std::stringstream>(ref["gen_param_name_style"].GetString());
            }
            if (ref.HasMember("class_name_style") && ref["class_name_style"].IsString()) {
                classNameStyle_ = std::make_unique<std::stringstream>(ref["class_name_style"].GetString());
            }
            if (ref.HasMember("interface_name_style") && ref["interface_name_style"].IsString()) {
                interfaceNameStyle_ = std::make_unique<std::stringstream>(ref["interface_name_style"].GetString());
            }
            if (ref.HasMember("method_name_style") && ref["method_name_style"].IsString()) {
                methodNameStyle_ = std::make_unique<std::stringstream>(ref["method_name_style"].GetString());
            }
            if (ref.HasMember("function_name_style") && ref["function_name_style"].IsString()) {
                functionNameStyle_ = std::make_unique<std::stringstream>(ref["function_name_style"].GetString());
            }
            if (ref.HasMember("global_var_name_style") && ref["global_var_name_style"].IsString()) {
                globalVarNameStyle_ = std::make_unique<std::stringstream>(ref["global_var_name_style"].GetString());
            }
            if (ref.HasMember("member_var_name_style") && ref["member_var_name_style"].IsString()) {
                memberVarNameStyle_ = std::make_unique<std::stringstream>(ref["member_var_name_style"].GetString());
            }
            if (ref.HasMember("local_var_name_style") && ref["local_var_name_style"].IsString()) {
                localVarNameStyle_ = std::make_unique<std::stringstream>(ref["local_var_name_style"].GetString());
            }
            if (ref.HasMember("param_var_name_style") && ref["param_var_name_style"].IsString()) {
                paramVarNameStyle_ = std::make_unique<std::stringstream>(ref["param_var_name_style"].GetString());
            }
        }
        if (conf.HasMember("OPERATOR") && conf["OPERATOR"].IsObject()) {
            rj::Value& op = conf["OPERATOR"];
            if (op.HasMember("assign") && op["assign"].IsString() && op["assign"].GetStringLength() > 0) {
                assignOpWord_ = std::make_unique<std::stringstream>(op["assign"].GetString());
            }
            if (op.HasMember("modulo") && op["modulo"].IsString() && op["modulo"].GetStringLength() > 0) {
                moduloOpWord_ = std::make_unique<std::stringstream>(op["modulo"].GetString());
            }
            if (op.HasMember("address") && op["address"].IsString() && op["address"].GetStringLength() > 0) {
                addressOpWord_ = std::make_unique<std::stringstream>(op["address"].GetString());
            }
            if (op.HasMember("deref") && op["deref"].IsString() && op["deref"].GetStringLength() > 0) {
                derefOpWord_ = std::make_unique<std::stringstream>(op["deref"].GetString());
            }
            if (op.HasMember("style") && op["style"].IsString()) {
                operatorStyle_ = std::make_unique<std::stringstream>(op["style"].GetString());
            }
        }
        if (conf.HasMember("SEPARATOR") && conf["SEPARATOR"].IsObject()) {
            rj::Value& sep = conf["SEPARATOR"];
            if (sep.HasMember("style") && sep["style"].IsString()) {
                separatorStyle_ = std::make_unique<std::stringstream>(sep["style"].GetString());
            }
        }
        if (conf.HasMember("VALUE") && conf["VALUE"].IsObject()) {
            rj::Value& val = conf["VALUE"];
            if (val.HasMember("true_val") && val["true_val"].IsString() && val["true_val"].GetStringLength() > 0) {
                trueVal_ = std::make_unique<std::stringstream>(val["true_val"].GetString());
            }
            if (val.HasMember("false_val") && val["false_val"].IsString() && val["false_val"].GetStringLength() > 0) {
                falseVal_ = std::make_unique<std::stringstream>(val["false_val"].GetString());
            }
            if (val.HasMember("null_val") && val["null_val"].IsString() && val["null_val"].GetStringLength() > 0) {
                nullVal_ = std::make_unique<std::stringstream>(val["null_val"].GetString());
            }
            if (val.HasMember("STYLE") && val["STYLE"].IsObject()) {
                const rj::Value& st = val["STYLE"];
                if (st.HasMember("general_val_style") && st["general_val_style"].IsString()) {
                    valStyle_ = std::make_unique<std::stringstream>(st["general_val_style"].GetString());
                }
                if (st.HasMember("int_val_style") && st["int_val_style"].IsString()) {
                    intValStyle_ = std::make_unique<std::stringstream>(st["int_val_style"].GetString());
                }
                if (st.HasMember("float_val_style") && st["float_val_style"].IsString()) {
                    floatValStyle_ = std::make_unique<std::stringstream>(st["float_val_style"].GetString());
                }
                if (st.HasMember("char_val_style") && st["char_val_style"].IsString()) {
                    charValStyle_ = std::make_unique<std::stringstream>(st["char_val_style"].GetString());
                }
                if (st.HasMember("string_val_style") && st["string_val_style"].IsString()) {
                    stringValStyle_ = std::make_unique<std::stringstream>(st["string_val_style"].GetString());
                }
                if (st.HasMember("bool_val_style") && st["bool_val_style"].IsString()) {
                    boolValStyle_ = std::make_unique<std::stringstream>(st["bool_val_style"].GetString());
                }
                if (st.HasMember("null_val_style") && st["null_val_style"].IsString()) {
                    nullValStyle_ = std::make_unique<std::stringstream>(st["null_val_style"].GetString());
                }
            }
        }
    }
    fclose(configFile);
}

void Configurator::set_defaults() {
    outputFileName_ = std::make_unique<std::stringstream>("output");
    outputFilePath_ = std::make_unique<std::stringstream>(defaultOutputFilePath_->str());
    outputFileFormat_ = std::make_unique<std::stringstream>("txt");
    defaultStyle_ = std::make_unique<std::stringstream>("font-family:Consolas;font-size:18px");
    unknownWord_ = std::make_unique<std::stringstream>("UNKNOWN EXPRESSION");
    unknownWordStyle_ = std::make_unique<std::stringstream>();
    invalidWord_ = std::make_unique<std::stringstream>("INVALID EXPRESSION");
    invalidWordStyle_ = std::make_unique<std::stringstream>();
    view_ = std::make_unique<std::stringstream>("inner");
    publicWord_ = std::make_unique<std::stringstream>("public");
    privateWord_ = std::make_unique<std::stringstream>("private");
    protectedWord_ = std::make_unique<std::stringstream>("protected");
    internalWord_ = std::make_unique<std::stringstream>("internal");
    accAtribWord_ = std::make_unique<std::stringstream>("attributes");
    accConstrWord_ = std::make_unique<std::stringstream>("constructors");
    accDestrWord_ = std::make_unique<std::stringstream>("destructors");
    accMethWord_ = std::make_unique<std::stringstream>("methods");
    accStyle_ = std::make_unique<std::stringstream>();
    dynamicWord_ = std::make_unique<std::stringstream>("auto");
    intWord_ = std::make_unique<std::stringstream>("int");
    floatWord_ = std::make_unique<std::stringstream>("float");
    charWord_ = std::make_unique<std::stringstream>("char");
    boolWord_ = std::make_unique<std::stringstream>("bool");
    voidWord_ = std::make_unique<std::stringstream>("void");
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
    classNameStyle_ = std::make_unique<std::stringstream>();
    interfaceNameStyle_ = std::make_unique<std::stringstream>();
    methodNameStyle_ = std::make_unique<std::stringstream>();
    functionNameStyle_ = std::make_unique<std::stringstream>();
    globalVarNameStyle_ = std::make_unique<std::stringstream>();
    memberVarNameStyle_ = std::make_unique<std::stringstream>();
    localVarNameStyle_ = std::make_unique<std::stringstream>();
    paramVarNameStyle_ = std::make_unique<std::stringstream>();
    assignOpWord_ = std::make_unique<std::stringstream>("=");
    moduloOpWord_ = std::make_unique<std::stringstream>("%");
    addressOpWord_ = std::make_unique<std::stringstream>("&");
    derefOpWord_ = std::make_unique<std::stringstream>("*");
    operatorStyle_ = std::make_unique<std::stringstream>();
    separatorStyle_ = std::make_unique<std::stringstream>();
    trueVal_ = std::make_unique<std::stringstream>("true");
    falseVal_ = std::make_unique<std::stringstream>("false");
    nullVal_ = std::make_unique<std::stringstream>("NULL");
    valStyle_ = std::make_unique<std::stringstream>();
    intValStyle_ = std::make_unique<std::stringstream>();
    floatValStyle_ = std::make_unique<std::stringstream>();
    charValStyle_ = std::make_unique<std::stringstream>();
    stringValStyle_ = std::make_unique<std::stringstream>();
    boolValStyle_ = std::make_unique<std::stringstream>();
    nullValStyle_ = std::make_unique<std::stringstream>();
}