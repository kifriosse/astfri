#include <libastfri-text/inc/settings/TextConfig.hpp>

using namespace astfri::text;

void TextConfigurator::change_to_default()
{
    // TEXT_FORMAT
    defaultTextStyle  = "font-family:Consolas;font-size:18px";
    rowNumStyle       = "";
    bracketColors     = {};
    tabulatorLen      = 4;
    textMarginLeftLen = 3;
    rowNumMarginLeftLen   = 1;
    useBracketColors      = false;
    shRowNum              = true;
    shDotAfterRowNum      = true;
    shRowNumOnEmptyRow    = true;
    resetRowNumOnEmptyRow = false;
    newLineForCurlBracket = true;
    // CODE_STRUCTURE
    shOtherExpressions = true;
    shGlobalVars       = true;
    shGenericParams    = true;
    shClassDeclar      = true;
    shClassDefin       = true;
    shClassInline      = false;
    shInterfDeclar     = true;
    shInterfDefin      = true;
    shMemberVars       = true;
    shCoDeMeDeclar     = true;
    shCoDeMeDefin      = true;
    shCoDeMeOwner      = true;
    shCoDeMeTemplate   = true;
    shFuncDeclar       = true;
    shFuncDefin        = true;
    // SYSTEM_EXPRESSIONS
    scopeWord     = "namespace";
    classWord     = "class";
    interfaceWord = "interface";
    implementWord = "implements";
    extendWord    = "extends";
    thisWord      = "this";
    returnWord    = "return";
    continueWord  = "continue";
    breakWord     = "break";
    throwWord     = "throw";
    ifWord        = "if";
    elseWord      = "else";
    doWord        = "do";
    whileWord     = "while";
    forWord       = "for";
    repeatWord    = "repeat";
    switchWord    = "switch";
    caseWord      = "case";
    defaultWord   = "default";
    newWord       = "new";
    deleteWord    = "delete";
    pointerWord   = "↑";
    overrideWord  = "override";
    virtualWord   = "is virtual";
    abstractWord  = "is abstract";
    templateWord  = "template";
    // STYLE
    systExprStyle      = "";
    scopeWordStyle     = "";
    classWordStyle     = "";
    interfaceWordStyle = "";
    implementWordStyle = "";
    extendWordStyle    = "";
    thisWordStyle      = "";
    returnWordStyle    = "";
    continueWordStyle  = "";
    breakWordStyle     = "";
    throwWordStyle     = "";
    ifWordStyle        = "";
    elseWordStyle      = "";
    doWordStyle        = "";
    whileWordStyle     = "";
    forWordStyle       = "";
    repeatWordStyle    = "";
    switchWordStyle    = "";
    caseWordStyle      = "";
    defaultWordStyle   = "";
    newWordStyle       = "";
    deleteWordStyle    = "";
    pointerWordStyle   = "";
    overrideWordStyle  = "";
    virtualWordStyle   = "";
    abstractWordStyle  = "";
    templateWordStyle  = "";
    // OTHER_EXPRESSIONS
    constructorWord = "constructor";
    destructorWord  = "destructor";
    methodWord      = "method";
    functionWord    = "function";
    lambdaWord      = "λ";
    callWord        = "call";
    defineWord      = "define";
    returnsWord     = "returns";
    // STYLE
    otherExprStyle       = "";
    constructorWordStyle = "";
    destructorWordStyle  = "";
    methodWordStyle      = "";
    functionWordStyle    = "";
    lambdaWordStyle      = "";
    callWordStyle        = "";
    defineWordStyle      = "";
    returnsWordStyle     = "";
}

void TextConfigurator::load_from_file(std::string_view jsonPath)
{
    rapidjson::Document doc;
    if (try_create_json(jsonPath, doc))
    {
        load_from_json(doc);
    }
}

void TextConfigurator::load_from_json(jValue const& json)
{
    jValue const* textConf;
    // TEXT_CONFIGURATOR
    if (is_object("TEXT_CONFIGURATOR", json, textConf))
    {
        jValue const* tmp;
        // TEXT_FORMAT
        if (is_object("TEXT_FORMAT", *textConf, tmp))
        {
            process_text_format(*tmp);
        }
        // CODE_STRUCTURE
        if (is_object("CODE_STRUCTURE", *textConf, tmp))
        {
            process_code_structure(*tmp);
        }
        // SYSTEM_EXPRESSIONS
        if (is_object("SYSTEM_EXPRESSIONS", *textConf, tmp))
        {
            process_system_expressions(*tmp);
        }
        // OTHER_EXPRESSIONS
        if (is_object("OTHER_EXPRESSIONS", *textConf, tmp))
        {
            process_other_expressions(*tmp);
        }
    }
}

void TextConfigurator::process_text_format(jValue const& format)
{
    read_string("default_text_style", format, defaultTextStyle);
    read_string("row_number_style", format, rowNumStyle);
    //
    jValue const* array;
    read_array("bracket_colors", format, array);
    if (array)
    {
        bracketColors.clear();
        for (size_t i = 0; i < array->Size(); ++i)
        {
            if (array[i].IsString() && array[i].GetStringLength() > 0)
            {
                bracketColors.push_back(std::string(array[i].GetString()));
                if (bracketColors.size() == 4)
                {
                    break;
                }
            }
        }
    }
    //
    read_int("tabulator_length", format, tabulatorLen);
    read_int("text_margin_left_length", format, textMarginLeftLen);
    read_int("row_number_margin_left_length", format, rowNumMarginLeftLen);
    read_bool("use_bracket_colors", format, useBracketColors);
    read_bool("show_row_number", format, shRowNum);
    read_bool("show_dot_after_row_number", format, shDotAfterRowNum);
    read_bool("show_row_num_on_empty_row", format, shRowNumOnEmptyRow);
    read_bool("reset_row_number_on_empty_row", format, resetRowNumOnEmptyRow);
    read_bool("new_line_for_curl_bracket", format, newLineForCurlBracket);
}

void TextConfigurator::process_code_structure(jValue const& structure)
{
    read_bool("show_other_expressions", structure, shOtherExpressions);
    read_bool("show_global_vars", structure, shGlobalVars);
    read_bool("show_generic_params", structure, shGenericParams);
    read_bool("show_class_declaration", structure, shClassDeclar);
    read_bool("show_class_definition", structure, shClassDefin);
    read_bool("show_class_inline", structure, shClassInline);
    read_bool("show_interface_declaration", structure, shInterfDeclar);
    read_bool("show_interface_definition", structure, shInterfDefin);
    read_bool("show_member_vars", structure, shMemberVars);
    read_bool("show_member_operations_declaration", structure, shCoDeMeDeclar);
    read_bool("show_member_operations_definition", structure, shCoDeMeDefin);
    read_bool("show_member_operations_owner", structure, shCoDeMeOwner);
    read_bool("show_member_operations_template", structure, shCoDeMeTemplate);
    read_bool("show_function_declaration", structure, shFuncDeclar);
    read_bool("show_function_definition", structure, shFuncDefin);
}

void TextConfigurator::process_system_expressions(jValue const& expr)
{
    read_string("scope_word", expr, scopeWord);
    read_string("class_word", expr, classWord);
    read_string("interface_word", expr, interfaceWord);
    read_string("implement_word", expr, implementWord);
    read_string("extend_word", expr, extendWord);
    read_string("this_word", expr, thisWord);
    read_string("return_word", expr, returnWord);
    read_string("continue_word", expr, continueWord);
    read_string("break_word", expr, breakWord);
    read_string("throw_word", expr, throwWord);
    read_string("if_word", expr, ifWord);
    read_string("else_word", expr, elseWord);
    read_string("do_word", expr, doWord);
    read_string("while_word", expr, whileWord);
    read_string("for_word", expr, forWord);
    read_string("repeat_word", expr, repeatWord);
    read_string("switch_word", expr, switchWord);
    read_string("case_word", expr, caseWord);
    read_string("default_word", expr, defaultWord);
    read_string("new_word", expr, newWord);
    read_string("delete_word", expr, deleteWord);
    read_string("pointer_word", expr, pointerWord);
    read_string("override_word", expr, overrideWord);
    read_string("virtual_word", expr, virtualWord);
    read_string("abstract_word", expr, abstractWord);
    read_string("template_word", expr, templateWord);
    jValue const* style;
    if (is_object("STYLE", expr, style))
    {
        read_string("default_style", *style, systExprStyle);
        read_string("scope_word_style", *style, scopeWordStyle);
        read_string("class_word_style", *style, classWordStyle);
        read_string("interface_word_style", *style, interfaceWordStyle);
        read_string("implement_word_style", *style, implementWordStyle);
        read_string("extend_word_style", *style, extendWordStyle);
        read_string("this_word_style", *style, thisWordStyle);
        read_string("return_word_style", *style, returnWordStyle);
        read_string("continue_word_style", *style, continueWordStyle);
        read_string("break_word_style", *style, breakWordStyle);
        read_string("throw_word_style", *style, throwWordStyle);
        read_string("if_word_style", *style, ifWordStyle);
        read_string("else_word_style", *style, elseWordStyle);
        read_string("do_word_style", *style, doWordStyle);
        read_string("while_word_style", *style, whileWordStyle);
        read_string("for_word_style", *style, forWordStyle);
        read_string("repeat_word_style", *style, repeatWordStyle);
        read_string("switch_word_style", *style, switchWordStyle);
        read_string("case_word_style", *style, caseWordStyle);
        read_string("default_word_style", *style, defaultWordStyle);
        read_string("new_word_style", *style, newWordStyle);
        read_string("delete_word_style", *style, deleteWordStyle);
        read_string("pointer_word_style", *style, pointerWordStyle);
        read_string("override_word_style", *style, overrideWordStyle);
        read_string("virtual_word_style", *style, virtualWordStyle);
        read_string("abstract_word_style", *style, abstractWordStyle);
        read_string("template_word_style", *style, templateWordStyle);
    }
}

void TextConfigurator::process_other_expressions(jValue const& expr)
{
    read_string("constructor_word", expr, constructorWord);
    read_string("destructor_word", expr, destructorWord);
    read_string("method_word", expr, methodWord);
    read_string("function_word", expr, functionWord);
    read_string("lambda_word", expr, lambdaWord);
    read_string("call_word", expr, callWord);
    read_string("define_word", expr, defineWord);
    read_string("returns_word", expr, returnsWord);
    jValue const* style;
    if (is_object("STYLE", expr, style))
    {
        read_string("default_style", *style, otherExprStyle);
        read_string("constructor_word_style", *style, constructorWordStyle);
        read_string("destructor_word_style", *style, destructorWordStyle);
        read_string("method_word_style", *style, methodWordStyle);
        read_string("function_word_style", *style, functionWordStyle);
        read_string("lambda_word_style", *style, lambdaWordStyle);
        read_string("call_word_style", *style, callWordStyle);
        read_string("define_word_style", *style, defineWordStyle);
        read_string("returns_word_style", *style, returnsWordStyle);
    }
}