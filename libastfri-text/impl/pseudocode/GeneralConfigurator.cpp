#include <libastfri-text/inc/pseudocode/GeneralConfigurator.hpp>

using namespace astfri::text;

GeneralConfigurator::GeneralConfigurator()
{
    fmanager_ = &OutputFileManager::get_instance();
    set_defaults();
}

//
// -----
//

void GeneralConfigurator::set_defaults()
{
    // FILE_SETTINGS
    overwriteFile_ = false;
    outputFileName_ = std::make_unique<std::stringstream>("output");
    outputFilePath_ = std::make_unique<std::stringstream>(
        fmanager_->default_folder_path());
    outputFileFormat_ = std::make_unique<std::stringstream>("txt");
    // UNKNOWN_PHRASES
    unknownTypeWord_ = std::make_unique<std::stringstream>("UNKNOWN TYPE");
    unknownExprWord_ = std::make_unique<std::stringstream>("UNKNOWN EXPRESSION");
    unknownStmtWord_ = std::make_unique<std::stringstream>("UNKNOWN STATEMENT");
    invalidTypeWord_ = std::make_unique<std::stringstream>("INVALID TYPE");
    invalidExprWord_ = std::make_unique<std::stringstream>("INVALID EXPRESSION");
    invalidStmtWord_ = std::make_unique<std::stringstream>("INVALID STATEMENT");
    // STYLE
    defaultPhraseStyle_ = std::make_unique<std::stringstream>();
    unknownTypeWordStyle_ = std::make_unique<std::stringstream>();
    unknownExprWordStyle_ = std::make_unique<std::stringstream>();
    unknownStmtWordStyle_ = std::make_unique<std::stringstream>();
    invalidTypeWordStyle_ = std::make_unique<std::stringstream>();
    invalidExprWordStyle_ = std::make_unique<std::stringstream>();
    invalidStmtWordStyle_ = std::make_unique<std::stringstream>();
    // ACCESS_MODIFIERS
    useInnerView_ = true;
    publicWord_ = std::make_unique<std::stringstream>("public");
    protectedWord_ = std::make_unique<std::stringstream>("protected");
    privateWord_ = std::make_unique<std::stringstream>("private");
    internalWord_ = std::make_unique<std::stringstream>("internal");
    attributesWord_ = std::make_unique<std::stringstream>("attributes");
    constructorsWord_ = std::make_unique<std::stringstream>("constructors");
    destructorsWord_ = std::make_unique<std::stringstream>("destructors");
    methodsWord_ = std::make_unique<std::stringstream>("methods");
    accessModifierStyle_ = std::make_unique<std::stringstream>();
    // DATA_TYPES
    dynamicTypeWord_ = std::make_unique<std::stringstream>("auto");
    intTypeWord_ = std::make_unique<std::stringstream>("int");
    floatTypeWord_ = std::make_unique<std::stringstream>("float");
    charTypeWord_ = std::make_unique<std::stringstream>("char");
    boolTypeWord_ = std::make_unique<std::stringstream>("bool");
    voidTypeWord_ = std::make_unique<std::stringstream>("void");
    // STYLE
    defaultTypeWordStyle_ = std::make_unique<std::stringstream>();
    dynamicTypeWordStyle_ = std::make_unique<std::stringstream>();
    intTypeWordStyle_ = std::make_unique<std::stringstream>();
    floatTypeWordStyle_ = std::make_unique<std::stringstream>();
    charTypeWordStyle_ = std::make_unique<std::stringstream>();
    boolTypeWordStyle_ = std::make_unique<std::stringstream>();
    voidTypeWordStyle_ = std::make_unique<std::stringstream>();
    userTypeStyle_ = std::make_unique<std::stringstream>();
    // REFERENCE_NAMES
    defaultRefNameStyle_ = std::make_unique<std::stringstream>();
    genericParamNameStyle_ = std::make_unique<std::stringstream>();
    classNameStyle_ = std::make_unique<std::stringstream>();
    interfaceNameStyle_ = std::make_unique<std::stringstream>();
    methodNameStyle_ = std::make_unique<std::stringstream>();
    functionNameStyle_ = std::make_unique<std::stringstream>();
    defaultVarNameStyle_ = std::make_unique<std::stringstream>();
    globalVarNameStyle_ = std::make_unique<std::stringstream>();
    memberVarNameStyle_ = std::make_unique<std::stringstream>();
    localVarNameStyle_ = std::make_unique<std::stringstream>();
    paramVarNameStyle_ = std::make_unique<std::stringstream>();
    // OPERATORS
    assignOpWord_ = std::make_unique<std::stringstream>("=");
    moduloOpWord_ = std::make_unique<std::stringstream>("%");
    addressOpWord_ = std::make_unique<std::stringstream>("&");
    derefOpWord_ = std::make_unique<std::stringstream>("*");
    operatorStyle_ = std::make_unique<std::stringstream>();
    // SEPARATORS
    separatorStyle_ = std::make_unique<std::stringstream>();
    // VALUES
    trueValWord_ = std::make_unique<std::stringstream>("true");
    falseValWord_ = std::make_unique<std::stringstream>("false");
    nullValWord_ = std::make_unique<std::stringstream>("NULL");
    // STYLE
    defaultValStyle_ = std::make_unique<std::stringstream>();
    intValStyle_ = std::make_unique<std::stringstream>();
    floatValStyle_ = std::make_unique<std::stringstream>();
    charValStyle_ = std::make_unique<std::stringstream>();
    stringValStyle_ = std::make_unique<std::stringstream>();
    boolValStyle_ = std::make_unique<std::stringstream>();
    nullValStyle_ = std::make_unique<std::stringstream>();
}

void GeneralConfigurator::process_document(rj::Value const*& doc)
{
    rj::Value const* genConf;
    // GENERAL_CONFIGURATOR
    if (is_object("GENERAL_CONFIGURATOR", doc, genConf))
    {
        rj::Value const* tmp;
        // FILE_SETTINGS
        if (is_object("FILE_SETTINGS", genConf, tmp))
        {
            process_file_settings(tmp);
        }
        // UNKNOWN_PHRASES
        if (is_object("UNKNOWN_PHRASES", genConf, tmp))
        {
            process_unknown_phrases(tmp);
        }
        // ACCESS_MODIFIERS
        if (is_object("ACCESS_MODIFIERS", genConf, tmp))
        {
            process_access_modifiers(tmp);
        }
        // DATA_TYPES
        if (is_object("DATA_TYPES", genConf, tmp))
        {
            process_data_types(tmp);
        }
        // REFERENCE_NAMES
        if (is_object("REFERENCE_NAMES", genConf, tmp))
        {
            process_reference_names(tmp);
        }
        // OPERATORS
        if (is_object("OPERATORS", genConf, tmp))
        {
            process_ops_and_seps(tmp);
        }
        // SEPARATORS
        if (is_object("SEPARATORS", genConf, tmp))
        {
            process_ops_and_seps(tmp);
        }
        // VALUES
        if (is_object("VALUES", genConf, tmp))
        {
            process_values(tmp);
        }
    }
}

bool GeneralConfigurator::is_string(STRING name, rj::Value const*& rjval,
    std::string& getString, bool checkStrLen)
{
    auto const& iter = rjval->FindMember(name);
    if (iter != rjval->MemberEnd() && iter->value.IsString())
    {
        if (checkStrLen && iter->value.GetStringLength() < 1)
        {
            return false;
        }
        getString = iter->value.GetString();
        return true;
    }
    return false;
}

bool GeneralConfigurator::is_object(STRING name, rj::Value const*& rjval,
    rj::Value const*& getObject)
{
    auto const& iter = rjval->FindMember(name);
    if (iter == rjval->MemberEnd())
    {
        return false;
    }
    getObject = &iter->value;
    return getObject->IsObject() && !getObject->ObjectEmpty();
}

bool GeneralConfigurator::is_array(STRING name, rj::Value const*& rjval,
    rj::Value const*& getArray)
{
    auto const& iter = rjval->FindMember(name);
    if (iter != rjval->MemberEnd() && iter->value.IsArray())
    {
        getArray = &iter->value;
        return true;
    }
    return false;
}

bool GeneralConfigurator::is_bool(STRING name, rj::Value const*& rjval, bool& getBool)
{
    auto const& iter = rjval->FindMember(name);
    if (iter != rjval->MemberEnd() && iter->value.IsBool())
    {
        getBool = iter->value.GetBool();
        return true;
    }
    return false;
}

bool GeneralConfigurator::is_int(STRING name, rj::Value const*& rjval, int& getInt)
{
    auto const& iter = rjval->FindMember(name);
    if (iter != rjval->MemberEnd() && iter->value.IsInt())
    {
        getInt = iter->value.GetInt();
        return getInt >= 0;
    }
    return false;
}

void GeneralConfigurator::process_file_settings(rj::Value const*& settings)
{
    if (is_bool("overwrite_file", settings, overwriteFile_))
    { }
    std::string stringVal;
    if (is_string("output_file_name", settings, stringVal, true))
    {
        fmanager_->check_and_set_file_name(stringVal);
        outputFileName_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("output_file_path", settings, stringVal, false))
    {
        fmanager_->check_and_set_file_path(stringVal);
        outputFilePath_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("output_file_format", settings, stringVal, false))
    {
        outputFileFormat_ = std::make_unique<std::stringstream>(stringVal);
    }
}

void GeneralConfigurator::process_unknown_phrases(rj::Value const*& phrases)
{
    std::string stringVal;
    if (is_string("unknown_type_word", phrases, stringVal, true))
    {
        unknownTypeWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("unknown_expression_word", phrases, stringVal, true))
    {
        unknownExprWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("unknown_statement_word", phrases, stringVal, true))
    {
        unknownStmtWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("invalid_type_word", phrases, stringVal, true))
    {
        invalidTypeWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("invalid_expression_word", phrases, stringVal, true))
    {
        invalidExprWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("invalid_statement_word", phrases, stringVal, true))
    {
        invalidStmtWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    rj::Value const* style;
    if (is_object("STYLE", phrases, style))
    {
        if (is_string("default_phrase_style", style, stringVal, false))
        {
            defaultPhraseStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("unknown_type_word_style", style, stringVal, false))
        {
            unknownTypeWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("unknown_expression_word_style", style, stringVal, false))
        {
            unknownExprWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("unknown_statement_word_style", style, stringVal, false))
        {
            unknownStmtWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("invalid_type_word_style", style, stringVal, false))
        {
            invalidTypeWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("invalid_expression_word_style", style, stringVal, false))
        {
            invalidExprWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("invalid_statement_word_style", style, stringVal, false))
        {
            invalidStmtWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
    }
}

void GeneralConfigurator::process_access_modifiers(rj::Value const*& modifiers)
{
    if (is_bool("use_inner_view", modifiers, useInnerView_))
    { }
    std::string stringVal;
    if (is_string("public_word", modifiers, stringVal, true))
    {
        publicWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("protected_word", modifiers, stringVal, true))
    {
        protectedWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("private_word", modifiers, stringVal, true))
    {
        privateWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("internal_word", modifiers, stringVal, true))
    {
        internalWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("attributes_word", modifiers, stringVal, true))
    {
        attributesWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("constructors_word", modifiers, stringVal, true))
    {
        constructorsWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("destructors_word", modifiers, stringVal, true))
    {
        destructorsWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("methods_word", modifiers, stringVal, true))
    {
        methodsWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("access_modifier_style", modifiers, stringVal, false))
    {
        accessModifierStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
}

void GeneralConfigurator::process_data_types(rj::Value const*& types)
{
    std::string stringVal;
    if (is_string("dynamic_type_word", types, stringVal, true))
    {
        dynamicTypeWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("int_type_word", types, stringVal, true))
    {
        intTypeWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("float_type_word", types, stringVal, true))
    {
        floatTypeWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("char_type_word", types, stringVal, true))
    {
        charTypeWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("bool_type_word", types, stringVal, true))
    {
        boolTypeWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("void_type_word", types, stringVal, true))
    {
        voidTypeWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    rj::Value const* style;
    if (is_object("STYLE", types, style))
    {
        if (is_string("default_type_word_style", style, stringVal, false))
        {
            defaultTypeWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("dynamic_type_word_style", style, stringVal, false))
        {
            dynamicTypeWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("int_type_word_style", style, stringVal, false))
        {
            intTypeWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("float_type_word_style", style, stringVal, false))
        {
            floatTypeWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("char_type_word_style", style, stringVal, false))
        {
            charTypeWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("bool_type_word_style", style, stringVal, false))
        {
            boolTypeWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("void_type_word_style", style, stringVal, false))
        {
            voidTypeWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("user_type_style", style, stringVal, false))
        {
            userTypeStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
    }
}

void GeneralConfigurator::process_reference_names(rj::Value const*& names)
{
    std::string stringVal;
    if (is_string("default_ref_name_style", names, stringVal, false))
    {
        defaultRefNameStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("generic_param_name_style", names, stringVal, false))
    {
        genericParamNameStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("class_name_style", names, stringVal, false))
    {
        classNameStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("interface_name_style", names, stringVal, false))
    {
        interfaceNameStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("method_name_style", names, stringVal, false))
    {
        methodNameStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("function_name_style", names, stringVal, false))
    {
        functionNameStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("default_var_name_style", names, stringVal, false))
    {
        defaultVarNameStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("global_var_name_style", names, stringVal, false))
    {
        globalVarNameStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("member_var_name_style", names, stringVal, false))
    {
        memberVarNameStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("local_var_name_style", names, stringVal, false))
    {
        localVarNameStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("param_var_name_style", names, stringVal, false))
    {
        paramVarNameStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
}

void GeneralConfigurator::process_ops_and_seps(rj::Value const*& signs)
{
    std::string stringVal;
    if (is_string("assign_operator_word", signs, stringVal, true))
    {
        assignOpWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("modulo_operator_word", signs, stringVal, true))
    {
        moduloOpWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("address_operator_word", signs, stringVal, true))
    {
        addressOpWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("dereferencing_operator_word", signs, stringVal, true))
    {
        derefOpWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("operator_style", signs, stringVal, false))
    {
        operatorStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("separator_style", signs, stringVal, false))
    {
        separatorStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
}

void GeneralConfigurator::process_values(rj::Value const*& values)
{
    std::string stringVal;
    if (is_string("true_value_word", values, stringVal, true))
    {
        trueValWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("false_value_word", values, stringVal, true))
    {
        falseValWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("null_value_word", values, stringVal, true))
    {
        nullValWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    rj::Value const* style;
    if (is_object("STYLE", values, style))
    {
        if (is_string("default_value_style", style, stringVal, false))
        {
            defaultValStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("int_value_style", style, stringVal, false))
        {
            intValStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("float_value_style", style, stringVal, false))
        {
            floatValStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("char_value_style", style, stringVal, false))
        {
            charValStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("string_value_style", style, stringVal, false))
        {
            stringValStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("bool_value_style", style, stringVal, false))
        {
            boolValStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("null_value_style", style, stringVal, false))
        {
            nullValStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
    }
}