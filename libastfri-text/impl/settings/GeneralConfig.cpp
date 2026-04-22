#include <libastfri-text/inc/settings/GeneralConfig.hpp>

using namespace astfri::text;

void GeneralConfigurator::change_to_default()
{
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
    useInnerView        = true;
    publicWord          = "public";
    protectedWord       = "protected";
    privateWord         = "private";
    internalWord        = "internal";
    attributesWord      = "attributes";
    constructorsWord    = "constructors";
    destructorsWord     = "destructors";
    methodsWord         = "methods";
    accessModifierStyle = "";
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
    defaultRefNameStyle   = "";
    genericParamNameStyle = "";
    classNameStyle        = "";
    interfaceNameStyle    = "";
    methodNameStyle       = "";
    functionNameStyle     = "";
    defaultVarNameStyle   = "";
    globalVarNameStyle    = "";
    memberVarNameStyle    = "";
    localVarNameStyle     = "";
    paramVarNameStyle     = "";
    // OPERATORS
    assignOpWord  = "=";
    moduloOpWord  = "%";
    addressOpWord = "&";
    derefOpWord   = "*";
    operatorStyle = "";
    // SEPARATORS
    separatorStyle = "";
    // VALUES
    trueValWord  = "true";
    falseValWord = "false";
    nullValWord  = "NULL";
    // STYLE
    defaultValStyle = "";
    intValStyle     = "";
    floatValStyle   = "";
    charValStyle    = "";
    stringValStyle  = "";
    boolValStyle    = "";
    nullValStyle    = "";
}

void GeneralConfigurator::load_from_json(jValue const& json)
{
    jValue const* config;
    // GENERAL_CONFIGURATOR
    if (is_object("GENERAL_CONFIG", json, config))
    {
        jValue const* tmp;
        // FILE_SETTINGS
        if (is_object("FILE_SETTINGS", *config, tmp))
        {
            process_file_settings(*tmp);
        }
        // UNKNOWN_PHRASES
        if (is_object("UNKNOWN_PHRASES", *config, tmp))
        {
            process_unknown_phrases(*tmp);
        }
        // ACCESS_MODIFIERS
        if (is_object("ACCESS_MODIFIERS", *config, tmp))
        {
            process_access_modifiers(*tmp);
        }
        // DATA_TYPES
        if (is_object("DATA_TYPES", *config, tmp))
        {
            process_data_types(*tmp);
        }
        // REFERENCE_NAMES
        if (is_object("REFERENCE_NAMES", *config, tmp))
        {
            process_reference_names(*tmp);
        }
        // OPERATORS
        if (is_object("OPERATORS", *config, tmp))
        {
            process_ops_and_seps(*tmp);
        }
        // SEPARATORS
        if (is_object("SEPARATORS", *config, tmp))
        {
            process_ops_and_seps(*tmp);
        }
        // VALUES
        if (is_object("VALUES", *config, tmp))
        {
            process_values(*tmp);
        }
    }
}

void GeneralConfigurator::process_file_settings(jValue const& settings)
{
    read_bool("overwrite_file", settings, overwriteFile);
    std::string val;
    read_string("output_file_name", settings, val);
    fmanager.check_and_set_file_name(val);
    outputFileName = val;
    read_string("output_file_path", settings, val);
    fmanager.check_and_set_file_path(val);
    outputFilePath = val;
    read_string("output_file_format", settings, outputFileFormat);
}

void GeneralConfigurator::process_unknown_phrases(jValue const& phrases)
{
    read_string("unknown_type_word", phrases, unknownTypeWord);
    read_string("unknown_expr_word", phrases, unknownExprWord);
    read_string("unknown_stmt_word", phrases, unknownStmtWord);
    jValue const* style;
    if (is_object("STYLE", phrases, style))
    {
        read_string("default_phrase_style", *style, defaultPhraseStyle);
        read_string("unknown_type_word_style", *style, unknownTypeWordStyle);
        read_string("unknown_expr_word_style", *style, unknownExprWordStyle);
        read_string("unknown_stmt_word_style", *style, unknownStmtWordStyle);
    }
}

void GeneralConfigurator::process_access_modifiers(jValue const& modifiers)
{
    read_bool("use_inner_view", modifiers, useInnerView);
    read_string("public_word", modifiers, publicWord);
    read_string("protected_word", modifiers, protectedWord);
    read_string("private_word", modifiers, privateWord);
    read_string("internal_word", modifiers, internalWord);
    read_string("attributes_word", modifiers, attributesWord);
    read_string("constructors_word", modifiers, constructorsWord);
    read_string("destructors_word", modifiers, destructorsWord);
    read_string("methods_word", modifiers, methodsWord);
    read_string("access_modifier_style", modifiers, accessModifierStyle);
}

void GeneralConfigurator::process_data_types(jValue const& types)
{
    read_string("dynamic_type_word", types, dynamicTypeWord);
    read_string("int_type_word", types, intTypeWord);
    read_string("float_type_word", types, floatTypeWord);
    read_string("char_type_word", types, charTypeWord);
    read_string("bool_type_word", types, boolTypeWord);
    read_string("void_type_word", types, voidTypeWord);
    jValue const* style;
    if (is_object("STYLE", types, style))
    {
        read_string("default_type_word_style", *style, defaultTypeWordStyle);
        read_string("dynamic_type_word_style", *style, dynamicTypeWordStyle);
        read_string("int_type_word_style", *style, intTypeWordStyle);
        read_string("float_type_word_style", *style, floatTypeWordStyle);
        read_string("char_type_word_style", *style, charTypeWordStyle);
        read_string("bool_type_word_style", *style, boolTypeWordStyle);
        read_string("void_type_word_style", *style, voidTypeWordStyle);
        read_string("user_type_style", *style, userTypeStyle);
    }
}

void GeneralConfigurator::process_reference_names(jValue const& names)
{
    read_string("default_ref_name_style", names, defaultRefNameStyle);
    read_string("generic_param_name_style", names, genericParamNameStyle);
    read_string("class_name_style", names, classNameStyle);
    read_string("interface_name_style", names, interfaceNameStyle);
    read_string("method_name_style", names, methodNameStyle);
    read_string("function_name_style", names, functionNameStyle);
    read_string("default_var_name_style", names, defaultVarNameStyle);
    read_string("global_var_name_style", names, globalVarNameStyle);
    read_string("member_var_name_style", names, memberVarNameStyle);
    read_string("local_var_name_style", names, localVarNameStyle);
    read_string("param_var_name_style", names, paramVarNameStyle);
}

void GeneralConfigurator::process_ops_and_seps(jValue const& signs)
{
    read_string("assign_operator_word", signs, assignOpWord);
    read_string("modulo_operator_word", signs, moduloOpWord);
    read_string("address_operator_word", signs, addressOpWord);
    read_string("dereferencing_operator_word", signs, derefOpWord);
    read_string("operator_style", signs, operatorStyle);
    read_string("separator_style", signs, separatorStyle);
}

void GeneralConfigurator::process_values(jValue const& values)
{
    read_string("true_value_word", values, trueValWord);
    read_string("false_value_word", values, falseValWord);
    read_string("null_value_word", values, nullValWord);
    jValue const* style;
    if (is_object("STYLE", values, style))
    {
        read_string("default_value_style", *style, defaultValStyle);
        read_string("int_value_style", *style, intValStyle);
        read_string("float_value_style", *style, floatValStyle);
        read_string("char_value_style", *style, charValStyle);
        read_string("string_value_style", *style, stringValStyle);
        read_string("bool_value_style", *style, boolValStyle);
        read_string("null_value_style", *style, nullValStyle);
    }
}