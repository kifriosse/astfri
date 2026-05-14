#include <libastfri-text/inc/TextLibConfig.hpp>

#include <rapidjson/istreamwrapper.h>

#include <fstream>

using namespace astfri::text;

TextLibConfig TextLibConfig::createDefault()
{
    return TextLibConfig();
}

TextLibConfig TextLibConfig::createFromArgs(int /*argc*/, char* /*argv*/[])
{
    throw std::logic_error("Not implemented yet!");
}

TextLibConfig TextLibConfig::createFromJson(rapidjson::Value const& node)
{
    TextLibConfig config;
    config.load_from_json(node);
    return config;
}

TextLibConfig TextLibConfig::createFromJson(std::filesystem::path const& path)
{
    TextLibConfig config;
    config.load_from_file(path);
    return config;
}

void TextLibConfig::change_to_default()
{
    // 1) CODE_STRUCTURE
    tabulatorLength              = 4;
    namespaceBlockBracketNewLine = false;
    useNamespaceTabulator        = false;
    objectBlockBracketNewLine    = false;
    functionBlockBracketNewLine  = false;
    loopBlockBracketNewLine      = false;
    dowhileConditionNewLine      = false;
    conditionBlockBracketNewLine = false;
    elseifConditionNewLine       = false;
    elseConditionNewLine         = false;
    switchBlockBracketNewLine    = false;
    trycatchBlockBracketNewLine  = false;
    catchConditionNewLine        = false;
    // 2) PSEUDOCODE_STRUCTURE
    textMarginLeft     = 3;
    rowNumMarginLeft   = 1;
    shBrColors         = true;
    shRowNum           = true;
    shDotAfterRowNum   = true;
    shRowNumOnEmptyRow = true;
    shGlobVarDeclar    = true;
    shTemplateDeclar   = true;
    shClassDeclar      = true;
    shClassDefin       = true;
    shClassDefinInl    = false;
    shInterfDeclar     = true;
    shInterfDefin      = true;
    shMembVarDeclar    = true;
    shCoDeMeDeclar     = true;
    shCoDeMeDefin      = true;
    shCoDeMeOwner      = true;
    shCoDeMeTemplate   = true;
    shFuncDeclar       = true;
    shFuncDefin        = true;
    // 3) PSEUDOCODE_TEXT
    // 3.1 GENERAL
    unknownTypeWord    = "UNKNOWN TYPE";
    unknownExprWord    = "UNKNOWN EXPRESSION";
    unknownStmtWord    = "UNKNOWN STATEMENT";
    defaultTextStyle   = "font-family:Consolas;font-size:16px";
    unknownPhraseStyle = "";
    rowNumStyle        = "";
    brColors = {"red", "green", "blue"};
    // 3.2 SYMBOLS
    // 3.2.1 OPERATORS
    pointerWord = "↑";
    assignWord  = "=";
    moduloWord  = "%";
    addressWord = "&";
    derefWord   = "*";
    opWordStyle = "";
    // 3.2.2 SEPARATORS
    semicolonWord = ";";
    sepWordStyle  = "";
    // 3.3 VALUES
    trueWord          = "true";
    falseWord         = "false";
    nullWord          = "nullptr";
    valueStyle        = "";
    numericValueStyle = "";
    stringValueStyle  = "";
    // 3.4 REFERENCE_NAMES
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
    // 3.5 SYSTEM_EXPRESSIONS
    // 3.5.1 ACCESS_MODIFIERS
    publicWord       = "public";
    protectedWord    = "protected";
    privateWord      = "private";
    internalWord     = "package-private";
    attributesWord   = "attributes";
    constructorsWord = "constructors";
    destructorsWord  = "destructors";
    methodsWord      = "methods";
    accessModifStyle = "";
    // 3.5.2 DATA_TYPES
    dynamicTypeWord      = "auto";
    intTypeWord          = "int";
    floatTypeWord        = "float";
    charTypeWord         = "char";
    boolTypeWord         = "bool";
    voidTypeWord         = "void";
    typeWordStyle        = "";
    numericTypeWordStyle = "";
    stringTypeWordStyle  = "";
    systemTypeWordStyle  = "";
    // 3.5.3 OBJECTS
    scopeWord       = "namespace";
    templateWord    = "template";
    classWord       = "class";
    interfaceWord   = "interface";
    implementWord   = "implements";
    extendWord      = "extends";
    virtualWord     = "virtual";
    abstractWord    = "abstract";
    staticWord      = "static";
    overrideWord    = "override";
    thisWord        = "this";
    objectWordStyle = "";
    // 3.5.4 CONDITIONS
    ifWord             = "if";
    elseifWord         = "else if";
    elseWord           = "else";
    switchWord         = "switch";
    caseWord           = "case";
    defaultWord        = "default";
    conditionWordStyle = "";
    // 3.5.5 LOOPS
    doWord        = "do";
    whileWord     = "while";
    forWord       = "for";
    foreachWord   = "for";
    loopWordStyle = "";
    // 3.5.6 OTHER
    returnWord     = "return";
    continueWord   = "continue";
    breakWord      = "break";
    tryWord        = "try";
    catchWord      = "catch";
    finallyWord    = "finally";
    throwWord      = "throw";
    newWord        = "new";
    deleteWord     = "delete";
    otherExprStyle = "";
    // 3.6 SUPPORT_EXPRESSIONS
    constructorWord  = "constructor";
    destructorWord   = "destructor";
    methodWord       = "method";
    functionWord     = "function";
    lambdaWord       = "λ";
    callWord         = "call";
    defineWord       = "define";
    returnsWord      = "returns ->";
    repeatWord       = "repeat";
    supportExprStyle = "";
}

void TextLibConfig::change_to_java_like()
{
    change_to_default();
}

void TextLibConfig::change_to_cxx_like()
{
    tabulatorLength              = 4;
    namespaceBlockBracketNewLine = true;
    useNamespaceTabulator        = true;
    objectBlockBracketNewLine    = true;
    functionBlockBracketNewLine  = true;
    loopBlockBracketNewLine      = true;
    dowhileConditionNewLine      = true;
    conditionBlockBracketNewLine = true;
    elseifConditionNewLine       = true;
    elseConditionNewLine         = true;
    switchBlockBracketNewLine    = true;
    trycatchBlockBracketNewLine  = true;
    catchConditionNewLine        = true;
}

bool TextLibConfig::try_create_json(std::filesystem::path const& path, rapidjson::Document& doc)
{
    std::ifstream jsonFile(path);
    if (!jsonFile)
    {
        return false;
    }
    rapidjson::IStreamWrapper wrapper(jsonFile);
    doc.ParseStream(wrapper);
    return !doc.HasParseError();
}

void TextLibConfig::load_from_file(std::filesystem::path const& path)
{
    rapidjson::Document doc;
    if (try_create_json(path, doc))
    {
        load_from_json(doc);
    }
}

void TextLibConfig::load_from_json(rapidjson::Value const& json)
{
    jValue const* tmp;
    if (is_object("CODE_STRUCTURE", json, tmp))
    {
        process_code_structure(*tmp);
    }
    if (is_object("PSEUDOCODE_STRUCTURE", json, tmp))
    {
        process_pseudocode_structure(*tmp);
    }
    if (is_object("PSEUDOCODE_TEXT", json, tmp))
    {
        process_pseudocode_text(*tmp);
    }
}

void TextLibConfig::process_code_structure(jValue const& structure)
{
    read_int("tabulator_length", structure, tabulatorLength);
    read_bool("namespace_block_bracket_on_new_line", structure, namespaceBlockBracketNewLine);
    read_bool("use_namespace_tabulator", structure, useNamespaceTabulator);
    read_bool("object_block_bracket_on_new_line", structure, objectBlockBracketNewLine);
    read_bool("function_block_bracket_on_new_line", structure, functionBlockBracketNewLine);
    read_bool("loop_block_bracket_on_new_line", structure, loopBlockBracketNewLine);
    read_bool("dowhile_condition_on_new_line", structure, dowhileConditionNewLine);
    read_bool("condition_block_bracket_on_new_line", structure, conditionBlockBracketNewLine);
    read_bool("elseif_condition_on_new_line", structure, elseifConditionNewLine);
    read_bool("else_condition_on_new_line", structure, elseConditionNewLine);
    read_bool("switch_block_bracket_on_new_line", structure, switchBlockBracketNewLine);
    read_bool("trycatch_block_bracket_on_new_line", structure, trycatchBlockBracketNewLine);
    read_bool("catch_condition_on_new_line", structure, catchConditionNewLine);
}

void TextLibConfig::process_pseudocode_structure(jValue const& structure)
{
    read_int("text_margin_left", structure, textMarginLeft);
    read_int("row_number_margin_left", structure, rowNumMarginLeft);
    read_bool("show_bracket_colors", structure, shBrColors);
    read_bool("show_row_number", structure, shRowNum);
    read_bool("show_dot_after_row_number", structure, shDotAfterRowNum);
    read_bool("show_row_number_on_empty_row", structure, shRowNumOnEmptyRow);
    read_bool("show_global_var_declaration", structure, shGlobVarDeclar);
    read_bool("show_template_declaration", structure, shTemplateDeclar);
    read_bool("show_class_declaration", structure, shClassDeclar);
    read_bool("show_class_definition", structure, shClassDefin);
    read_bool("show_class_definition_inline", structure, shClassDefinInl);
    read_bool("show_interface_declaration", structure, shInterfDeclar);
    read_bool("show_interface_definition", structure, shInterfDefin);
    read_bool("show_member_var_declaration", structure, shMembVarDeclar);
    read_bool("show_constr_destr_meth_declaration", structure, shCoDeMeDeclar);
    read_bool("show_constr_destr_meth_definition", structure, shCoDeMeDefin);
    read_bool("show_constr_destr_meth_owner", structure, shCoDeMeOwner);
    read_bool("show_constr_destr_meth_template", structure, shCoDeMeTemplate);
    read_bool("show_function_declaration", structure, shFuncDeclar);
    read_bool("show_function_definition", structure, shFuncDefin);
}

void TextLibConfig::process_pseudocode_text(jValue const& text)
{
    jValue const* tmp;
    if (is_object("GENERAL", text, tmp))
    {
        process_general_text(*tmp);
    }
    if (is_object("SYMBOLS", text, tmp))
    {
        process_symbols(*tmp);
    }
    if (is_object("VALUES", text, tmp))
    {
        process_values(*tmp);
    }
    if (is_object("REFERENCE_NAMES", text, tmp))
    {
        process_reference_names(*tmp);
    }
    if (is_object("SYSTEM_EXPRESSIONS", text, tmp))
    {
        process_system_expressions(*tmp);
    }
    if (is_object("SUPPORT_EXPRESSIONS", text, tmp))
    {
        process_support_expressions(*tmp);
    }
}

void TextLibConfig::process_general_text(jValue const& text)
{
    read_string("unknown_type_word", text, unknownTypeWord);
    read_string("unknown_expr_word", text, unknownExprWord);
    read_string("unknown_stmt_word", text, unknownStmtWord);
    read_string("default_style", text, defaultTextStyle);
    read_string("unknown_phrase_style", text, unknownPhraseStyle);
    read_string("row_number_style", text, rowNumStyle);
    jValue const* array = nullptr;
    read_array("bracket_colors", text, array);
    if (array)
    {
        brColors.clear();
        for (size_t i = 0; i < array->Size(); ++i)
        {
            if (array[i].IsString())
            {
                brColors.push_back(array[i].GetString());
            }
        }
    }
}

void TextLibConfig::process_symbols(jValue const& symbols)
{
    jValue const* tmp;
    if (is_object("OPERATORS", symbols, tmp))
    {
        read_string("pointer_word", *tmp, pointerWord);
        read_string("assign_word", *tmp, assignWord);
        read_string("modulo_word", *tmp, moduloWord);
        read_string("address_word", *tmp, addressWord);
        read_string("dereference_word", *tmp, derefWord);
        read_string("default_style", *tmp, opWordStyle);
    }
    if (is_object("SEPARATORS", symbols, tmp))
    {
        read_string("semicolon_word", *tmp, semicolonWord);
        read_string("default_style", *tmp, sepWordStyle);
    }
}

void TextLibConfig::process_values(jValue const& values)
{
    read_string("true_word", values, trueWord);
    read_string("false_word", values, falseWord);
    read_string("null_word", values, nullWord);
    read_string("default_style", values, valueStyle);
    read_string("numeric_value_style", values, numericValueStyle);
    read_string("string_value_style", values, stringValueStyle);
}

void TextLibConfig::process_reference_names(jValue const& names)
{
    read_string("default_style", names, defaultRefNameStyle);
    read_string("template_name_style", names, templateNameStyle);
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

void TextLibConfig::process_system_expressions(jValue const& expr)
{
    jValue const* tmp;
    if (is_object("ACCESS_MODIFIERS", expr, tmp))
    {
        process_access_modifiers(*tmp);
    }
    if (is_object("DATA_TYPES", expr, tmp))
    {
        process_data_types(*tmp);
    }
    if (is_object("OBJECTS", expr, tmp))
    {
        process_objects(*tmp);
    }
    if (is_object("CONDITIONS", expr, tmp))
    {
        process_conditions(*tmp);
    }
    if (is_object("LOOPS", expr, tmp))
    {
        process_loops(*tmp);
    }
    if (is_object("OTHER", expr, tmp))
    {
        process_other(*tmp);
    }
}

void TextLibConfig::process_access_modifiers(jValue const& modifiers)
{
    read_string("public_word", modifiers, publicWord);
    read_string("protected_word", modifiers, protectedWord);
    read_string("private_word", modifiers, privateWord);
    read_string("internal_word", modifiers, internalWord);
    read_string("attributes_word", modifiers, attributesWord);
    read_string("constructors_word", modifiers, constructorsWord);
    read_string("destructors_word", modifiers, destructorsWord);
    read_string("methods_word", modifiers, methodsWord);
    read_string("default_style", modifiers, accessModifStyle);
}

void TextLibConfig::process_data_types(jValue const& types)
{
    read_string("dynamic_type_word", types, dynamicTypeWord);
    read_string("int_type_word", types, intTypeWord);
    read_string("float_type_word", types, floatTypeWord);
    read_string("char_type_word", types, charTypeWord);
    read_string("bool_type_word", types, boolTypeWord);
    read_string("void_type_word", types, voidTypeWord);
    read_string("default_style", types, typeWordStyle);
    read_string("numeric_type_word_style", types, numericTypeWordStyle);
    read_string("string_type_word_style", types, stringTypeWordStyle);
    read_string("system_type_word_style", types, systemTypeWordStyle);
}

void TextLibConfig::process_objects(jValue const& objects)
{
    read_string("scope_word", objects, scopeWord);
    read_string("template_word", objects, templateWord);
    read_string("class_word", objects, classWord);
    read_string("interface_word", objects, interfaceWord);
    read_string("implement_word", objects, implementWord);
    read_string("extend_word", objects, extendWord);
    read_string("virtual_word", objects, virtualWord);
    read_string("abstract_word", objects, abstractWord);
    read_string("static_word", objects, staticWord);
    read_string("override_word", objects, overrideWord);
    read_string("this_word", objects, thisWord);
    read_string("default_style", objects, objectWordStyle);
}

void TextLibConfig::process_conditions(jValue const& conditions)
{
    read_string("if_word", conditions, ifWord);
    read_string("elseif_word", conditions, elseifWord);
    read_string("else_word", conditions, elseWord);
    read_string("switch_word", conditions, switchWord);
    read_string("case_word", conditions, caseWord);
    read_string("default_word", conditions, defaultWord);
    read_string("default_style", conditions, conditionWordStyle);
}

void TextLibConfig::process_loops(jValue const& loops)
{
    read_string("do_word", loops, doWord);
    read_string("while_word", loops, whileWord);
    read_string("for_word", loops, forWord);
    read_string("foreach_word", loops, foreachWord);
    read_string("default_style", loops, loopWordStyle);
}

void TextLibConfig::process_other(jValue const& other)
{
    read_string("return_word", other, returnWord);
    read_string("continue_word", other, continueWord);
    read_string("break_word", other, breakWord);
    read_string("try_word", other, tryWord);
    read_string("catch_word", other, catchWord);
    read_string("finally_word", other, finallyWord);
    read_string("throw_word", other, throwWord);
    read_string("new_word", other, newWord);
    read_string("delete_word", other, deleteWord);
    read_string("default_style", other, otherExprStyle);
}

void TextLibConfig::process_support_expressions(jValue const& expr)
{
    read_string("constructor_word", expr, constructorWord);
    read_string("destructor_word", expr, destructorWord);
    read_string("method_word", expr, methodWord);
    read_string("function_word", expr, functionWord);
    read_string("lambda_word", expr, lambdaWord);
    read_string("call_word", expr, callWord);
    read_string("define_word", expr, defineWord);
    read_string("returns_word", expr, returnsWord);
    read_string("repeat_word", expr, repeatWord);
    read_string("default_style", expr, supportExprStyle);
}

bool TextLibConfig::is_object(std::string_view name, jValue const& val, jValue const*& val_out)
{
    auto const& it = val.FindMember(name.data());
    if (it == val.MemberEnd()) {
        return false;
    }
    val_out = &it->value;
    return val_out->IsObject() && !val_out->ObjectEmpty();
}

void TextLibConfig::read_array(std::string_view name, jValue const& val, jValue const*& val_out)
{
    auto const& it = val.FindMember(name.data());
    val_out = (it != val.MemberEnd() && it->value.IsArray()) ? &it->value : nullptr;
}

void TextLibConfig::read_string(std::string_view name, jValue const& val, std::string& m_string)
{
    auto const& it = val.FindMember(name.data());
    m_string = (it != val.MemberEnd() && it->value.IsString()) ? it->value.GetString() : m_string;
}

void TextLibConfig::read_bool(std::string_view name, jValue const& val, bool& m_bool)
{
    auto const& it = val.FindMember(name.data());
    m_bool = (it != val.MemberEnd() && it->value.IsBool()) ? it->value.GetBool() : m_bool;
}

void TextLibConfig::read_int(std::string_view name, jValue const& val, int& m_int)
{
    auto const& it = val.FindMember(name.data());
    m_int = (it != val.MemberEnd() && it->value.IsInt() && it->value.GetInt() >= 0) ?
        it->value.GetInt() : m_int;
}