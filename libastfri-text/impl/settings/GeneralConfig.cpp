#include <libastfri-text/inc/settings/GeneralConfig.hpp>

using namespace astfri::text;

void GeneralConfig::change_to_default()
{
    BaseStructure::change_to_default();
    // FILE_SETTINGS
    overwriteFile    = false;
    outputFileName   = "output";
    outputFilePath   = fmanager.default_folder_path();
    outputFileFormat = "txt";
    // UNKNOWN_PHRASES
    unknownTypeWord = "UNKNOWN TYPE";
    unknownExprWord = "UNKNOWN EXPRESSION";
    unknownStmtWord = "UNKNOWN STATEMENT";
    // STYLE
    defaultPhraseStyle   = "";
    unknownTypeWordStyle = "";
    unknownExprWordStyle = "";
    unknownStmtWordStyle = "";
    // ACCESS_MODIFIERS
    useInnerView     = true;
    publicWord       = "public";
    protectedWord    = "protected";
    privateWord      = "private";
    internalWord     = "internal";
    attributesWord   = "attributes";
    constructorsWord = "constructors";
    destructorsWord  = "destructors";
    methodsWord      = "methods";
    accessModifStyle = "";
    // DATA_TYPES
    dynamicTypeWord = "auto";
    intTypeWord     = "int";
    floatTypeWord   = "float";
    charTypeWord    = "char";
    boolTypeWord    = "bool";
    voidTypeWord    = "void";
    // STYLE
    defaultTypeWordStyle = "";
    dynamicTypeWordStyle = "";
    intTypeWordStyle     = "";
    floatTypeWordStyle   = "";
    charTypeWordStyle    = "";
    boolTypeWordStyle    = "";
    voidTypeWordStyle    = "";
    userTypeStyle        = "";
    // REFERENCE_NAMES
    defaultRefNameStyle = "";
    templateNameStyle   = "";
    classNameStyle      = "";
    interfaceNameStyle  = "";
    methodNameStyle     = "";
    functionNameStyle   = "";
    defaultVarNameStyle = "";
    globalVarNameStyle  = "";
    memberVarNameStyle  = "";
    localVarNameStyle   = "";
    paramVarNameStyle   = "";
    // OPERATORS
    assignWord  = "=";
    moduloWord  = "%";
    addressWord = "&";
    derefWord   = "*";
    opWordStyle = "";
    // SEPARATORS
    sepWordStyle = "";
    // VALUES
    trueWord  = "true";
    falseWord = "false";
    nullWord  = "NULL";
    // STYLE
    defaultValStyle = "";
    nullWordStyle   = "";
    intValStyle     = "";
    floatValStyle   = "";
    charValStyle    = "";
    stringValStyle  = "";
    boolValStyle    = "";
}

void GeneralConfig::load_from_json(rapidjson::Document const& doc)
{
    BaseStructure::load_from_json(doc);
    //
    jValue const* tmp;
    if (is_object("GENERAL_CONFIG", doc, tmp))
    {
        process_file_settings(*tmp);
        process_unknown_phrases(*tmp);
        process_access_modifiers(*tmp);
        process_data_types(*tmp);
        process_reference_names(*tmp);
        process_ops_and_seps(*tmp);
        process_values(*tmp);
    }
}

void GeneralConfig::process_file_settings(jValue const& settings)
{
    jValue const* tmp;
    if (is_object("FILE_SETTINGS", settings, tmp))
    {
        read_bool("overwrite_file", *tmp, overwriteFile);
        //
        std::string val;
        read_string("output_file_name", *tmp, val);
        fmanager.check_and_set_file_name(val);
        outputFileName = val;
        val.clear();
        read_string("output_file_path", *tmp, val);
        fmanager.check_and_set_file_path(val);
        outputFilePath = val;
        //
        read_string("output_file_format", *tmp, outputFileFormat);
    }
}

void GeneralConfig::process_unknown_phrases(jValue const& phrases)
{
    jValue const* tmp;
    if (is_object("UNKNOWN_PHRASES", phrases, tmp))
    {
        read_string("unknown_type_word", *tmp, unknownTypeWord);
        read_string("unknown_expr_word", *tmp, unknownExprWord);
        read_string("unknown_stmt_word", *tmp, unknownStmtWord);
        //
        if (is_object("STYLE", *tmp, tmp))
        {
            read_string("default_phrase_style", *tmp, defaultPhraseStyle);
            read_string("unknown_type_word_style", *tmp, unknownTypeWordStyle);
            read_string("unknown_expr_word_style", *tmp, unknownExprWordStyle);
            read_string("unknown_stmt_word_style", *tmp, unknownStmtWordStyle);
        }
    }
}

void GeneralConfig::process_access_modifiers(jValue const& modifiers)
{
    jValue const* tmp;
    if (is_object("ACCESS_MODIFIERS", modifiers, tmp))
    {
        read_bool("use_inner_view", *tmp, useInnerView);
        read_string("public_word", *tmp, publicWord);
        read_string("protected_word", *tmp, protectedWord);
        read_string("private_word", *tmp, privateWord);
        read_string("internal_word", *tmp, internalWord);
        read_string("attributes_word", *tmp, attributesWord);
        read_string("constructors_word", *tmp, constructorsWord);
        read_string("destructors_word", *tmp, destructorsWord);
        read_string("methods_word", *tmp, methodsWord);
        read_string("access_modifier_style", *tmp, accessModifStyle);
    }
}

void GeneralConfig::process_data_types(jValue const& types)
{
    jValue const* tmp;
    if (is_object("DATA_TYPES", types, tmp))
    {
        read_string("dynamic_type_word", *tmp, dynamicTypeWord);
        read_string("int_type_word", *tmp, intTypeWord);
        read_string("float_type_word", *tmp, floatTypeWord);
        read_string("char_type_word", *tmp, charTypeWord);
        read_string("bool_type_word", *tmp, boolTypeWord);
        read_string("void_type_word", *tmp, voidTypeWord);
        //
        if (is_object("STYLE", *tmp, tmp))
        {
            read_string("default_type_word_style", *tmp, defaultTypeWordStyle);
            read_string("dynamic_type_word_style", *tmp, dynamicTypeWordStyle);
            read_string("int_type_word_style", *tmp, intTypeWordStyle);
            read_string("float_type_word_style", *tmp, floatTypeWordStyle);
            read_string("char_type_word_style", *tmp, charTypeWordStyle);
            read_string("bool_type_word_style", *tmp, boolTypeWordStyle);
            read_string("void_type_word_style", *tmp, voidTypeWordStyle);
            read_string("user_type_style", *tmp, userTypeStyle);
        }
    }
}

void GeneralConfig::process_reference_names(jValue const& names)
{
    jValue const* tmp;
    if (is_object("REFERENCE_NAMES", names, tmp))
    {
        read_string("default_ref_name_style", *tmp, defaultRefNameStyle);
        read_string("template_name_style", *tmp, templateNameStyle);
        read_string("class_name_style", *tmp, classNameStyle);
        read_string("interface_name_style", *tmp, interfaceNameStyle);
        read_string("method_name_style", *tmp, methodNameStyle);
        read_string("function_name_style", *tmp, functionNameStyle);
        read_string("default_var_name_style", *tmp, defaultVarNameStyle);
        read_string("global_var_name_style", *tmp, globalVarNameStyle);
        read_string("member_var_name_style", *tmp, memberVarNameStyle);
        read_string("local_var_name_style", *tmp, localVarNameStyle);
        read_string("param_var_name_style", *tmp, paramVarNameStyle);
    }
}

void GeneralConfig::process_ops_and_seps(jValue const& signs)
{
    jValue const* tmp;
    if (is_object("OPERATORS", signs, tmp))
    {
        read_string("assign", *tmp, assignWord);
        read_string("modulo", *tmp, moduloWord);
        read_string("address", *tmp, addressWord);
        read_string("dereferencing", *tmp, derefWord);
        read_string("operator_style", *tmp, opWordStyle);
    }
    //
    if (is_object("SEPARATORS", signs, tmp))
    {
        read_string("separator_style", *tmp, sepWordStyle);
    }
}

void GeneralConfig::process_values(jValue const& values)
{
    jValue const* tmp;
    if (is_object("VALUES", values, tmp))
    {
        read_string("true_word", *tmp, trueWord);
        read_string("false_word", *tmp, falseWord);
        read_string("null_word", *tmp, nullWord);
        //
        if (is_object("STYLE", *tmp, tmp))
        {
            read_string("default_value_style", *tmp, defaultValStyle);
            read_string("null_word_style", *tmp, nullWordStyle);
            read_string("int_value_style", *tmp, intValStyle);
            read_string("float_value_style", *tmp, floatValStyle);
            read_string("char_value_style", *tmp, charValStyle);
            read_string("string_value_style", *tmp, stringValStyle);
            read_string("bool_value_style", *tmp, boolValStyle);
        }
    }
}