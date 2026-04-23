#include <libastfri-text/inc/settings/TextConfig.hpp>

using namespace astfri::text;

void TextConfig::change_to_default()
{
    GeneralConfig::change_to_default();
    // PSEUDOCODE_STYLE
    textStyle   = "font-family:Consolas;font-size:16px";
    rowNumStyle = "";
    brColors    = {"red", "green", "blue"};
    textMarginLeft   = 3;
    rowNumMarginLeft = 1;
    shBrColors            = true;
    shRowNum              = true;
    shDotAfterRowNum      = true;
    shRowNumOnEmptyRow    = true;
    resetRowNumOnEmptyRow = false;
    // PSEUDOCODE_STRUCTURE
    shOtherExprs     = true;
    shGlobVarDeclar  = true;
    shTemplateDeclar = true;
    shClassDeclar    = true;
    shClassDefin     = true;
    shClassDefinInl  = false;
    shInterfDeclar   = true;
    shInterfDefin    = true;
    shMembVarDeclar  = true;
    shCoDeMeDeclar   = true;
    shCoDeMeDefin    = true;
    shCoDeMeOwner    = true;
    shCoDeMeTemplate = true;
    shFuncDeclar     = true;
    shFuncDefin      = true;
    // SYSTEM_EXPRESSIONS
    scopeWord     = "namespace";
    templateWord  = "template";
    classWord     = "class";
    interfaceWord = "interface";
    implementWord = "implements";
    extendWord    = "extends";
    virtualWord   = "virtual";
    abstractWord  = "abstract";
    staticWord    = "static";
    overrideWord  = "override";
    thisWord      = "this";
    newWord       = "new";
    deleteWord    = "delete";
    returnWord    = "return";
    continueWord  = "continue";
    breakWord     = "break";
    ifWord        = "if";
    elseifWord    = "else if";
    elseWord      = "else";
    switchWord    = "switch";
    caseWord      = "case";
    defaultWord   = "default";
    doWord        = "do";
    whileWord     = "while";
    forWord       = "for";
    foreachWord   = "for";
    tryWord       = "try";
    catchWord     = "catch";
    throwWord     = "throw";
    pointerWord   = "↑";
    // STYLE
    systemExprStyle    = "";
    scopeWordStyle     = "";
    templateWordStyle  = "";
    classWordStyle     = "";
    interfaceWordStyle = "";
    implementWordStyle = "";
    extendWordStyle    = "";
    virtualWordStyle   = "";
    abstractWordStyle  = "";
    staticWordStyle    = "";
    overrideWordStyle  = "";
    thisWordStyle      = "";
    newWordStyle       = "";
    deleteWordStyle    = "";
    returnWordStyle    = "";
    continueWordStyle  = "";
    breakWordStyle     = "";
    ifWordStyle        = "";
    elseifWordStyle    = "";
    elseWordStyle      = "";
    switchWordStyle    = "";
    caseWordStyle      = "";
    defaultWordStyle   = "";
    doWordStyle        = "";
    whileWordStyle     = "";
    forWordStyle       = "";
    foreachWordStyle   = "";
    tryWordStyle       = "";
    catchWordStyle     = "";
    throwWordStyle     = "";
    pointerWordStyle   = "";
    // OTHER_EXPRESSIONS
    constructorWord = "constructor";
    destructorWord  = "destructor";
    methodWord      = "method";
    functionWord    = "function";
    lambdaWord      = "λ";
    callWord        = "call";
    defineWord      = "define";
    returnsWord     = "returns ->";
    repeatWord      = "repeat";
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
    repeatWordStyle      = "";
}

void TextConfig::load_from_file(std::string_view jsonPath)
{
    rapidjson::Document doc;
    if (try_create_json(jsonPath, doc))
    {
        load_from_json(doc);
    }
}

void TextConfig::load_from_json(rapidjson::Document const& doc)
{
    GeneralConfig::load_from_json(doc);
    //
    jValue const* tmp;
    if (is_object("TEXT_CONFIG", doc, tmp))
    {
        process_pseudocode_style(*tmp);
        process_pseudocode_structure(*tmp);
        process_system_expressions(*tmp);
        process_other_expressions(*tmp);
    }
}

void TextConfig::process_pseudocode_style(jValue const& style)
{
    jValue const* tmp;
    if (is_object("PSEUDOCODE_STYLE", style, tmp))
    {
        read_string("text_style", *tmp, textStyle);
        read_string("row_number_style", *tmp, rowNumStyle);
        //
        jValue const* array = nullptr;
        read_array("bracket_colors", *tmp, array);
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
        read_int("text_margin_left", *tmp, textMarginLeft);
        read_int("row_number_margin_left", *tmp, rowNumMarginLeft);
        read_bool("show_bracket_colors", *tmp, shBrColors);
        read_bool("show_row_number", *tmp, shRowNum);
        read_bool("show_dot_after_row_number", *tmp, shDotAfterRowNum);
        read_bool("show_row_number_on_empty_row", *tmp, shRowNumOnEmptyRow);
        read_bool("reset_row_number_on_empty_row", *tmp, resetRowNumOnEmptyRow);
    }
}

void TextConfig::process_pseudocode_structure(jValue const& structure)
{
    jValue const* tmp;
    if (is_object("PSEUDOCODE_STRUCTURE", structure, tmp))
    {
        read_bool("show_other_expressions", *tmp, shOtherExprs);
        read_bool("show_global_var_declaration", *tmp, shGlobVarDeclar);
        read_bool("show_template_declaration", *tmp, shTemplateDeclar);
        read_bool("show_class_declaration", *tmp, shClassDeclar);
        read_bool("show_class_definition", *tmp, shClassDefin);
        read_bool("show_class_definition_inline", *tmp, shClassDefinInl);
        read_bool("show_interface_declaration", *tmp, shInterfDeclar);
        read_bool("show_interface_definition", *tmp, shInterfDefin);
        read_bool("show_member_var_declaration", *tmp, shMembVarDeclar);
        read_bool("show_member_operation_declaration", *tmp, shCoDeMeDeclar);
        read_bool("show_member_operation_definition", *tmp, shCoDeMeDefin);
        read_bool("show_member_operation_owner", *tmp, shCoDeMeOwner);
        read_bool("show_member_operation_template", *tmp, shCoDeMeTemplate);
        read_bool("show_function_declaration", *tmp, shFuncDeclar);
        read_bool("show_function_definition", *tmp, shFuncDefin);
    }
}

void TextConfig::process_system_expressions(jValue const& expr)
{
    jValue const* tmp;
    if (is_object("SYSTEM_EXPRESSIONS", expr, tmp))
    {
        read_string("scope_word", *tmp, scopeWord);
        read_string("template_word", *tmp, templateWord);
        read_string("class_word", *tmp, classWord);
        read_string("interface_word", *tmp, interfaceWord);
        read_string("implement_word", *tmp, implementWord);
        read_string("extend_word", *tmp, extendWord);
        read_string("virtual_word", *tmp, virtualWord);
        read_string("abstract_word", *tmp, abstractWord);
        read_string("static_word", *tmp, staticWord);
        read_string("override_word", *tmp, overrideWord);
        read_string("this_word", *tmp, thisWord);
        read_string("new_word", *tmp, newWord);
        read_string("delete_word", *tmp, deleteWord);
        read_string("return_word", *tmp, returnWord);
        read_string("continue_word", *tmp, continueWord);
        read_string("break_word", *tmp, breakWord);
        read_string("if_word", *tmp, ifWord);
        read_string("else_if_word", *tmp, elseifWord);
        read_string("else_word", *tmp, elseWord);
        read_string("switch_word", *tmp, switchWord);
        read_string("case_word", *tmp, caseWord);
        read_string("default_word", *tmp, defaultWord);
        read_string("do_word", *tmp, doWord);
        read_string("while_word", *tmp, whileWord);
        read_string("for_word", *tmp, forWord);
        read_string("for_each_word", *tmp, foreachWord);
        read_string("try_word", *tmp, tryWord);
        read_string("catch_word", *tmp, catchWord);
        read_string("throw_word", *tmp, throwWord);
        read_string("pointer_word", *tmp, pointerWord);
        //
        if (is_object("STYLE", *tmp, tmp))
        {
            read_string("default_style", *tmp, systemExprStyle);
            read_string("scope_word_style", *tmp, scopeWordStyle);
            read_string("template_word_style", *tmp, templateWordStyle);
            read_string("class_word_style", *tmp, classWordStyle);
            read_string("interface_word_style", *tmp, interfaceWordStyle);
            read_string("implement_word_style", *tmp, implementWordStyle);
            read_string("extend_word_style", *tmp, extendWordStyle);
            read_string("virtual_word_style", *tmp, virtualWordStyle);
            read_string("abstract_word_style", *tmp, abstractWordStyle);
            read_string("static_word_style", *tmp, staticWordStyle);
            read_string("override_word_style", *tmp, overrideWordStyle);
            read_string("this_word_style", *tmp, thisWordStyle);
            read_string("new_word_style", *tmp, newWordStyle);
            read_string("delete_word_style", *tmp, deleteWordStyle);
            read_string("return_word_style", *tmp, returnWordStyle);
            read_string("continue_word_style", *tmp, continueWordStyle);
            read_string("break_word_style", *tmp, breakWordStyle);
            read_string("if_word_style", *tmp, ifWordStyle);
            read_string("else_if_word_style", *tmp, elseifWordStyle);
            read_string("else_word_style", *tmp, elseWordStyle);
            read_string("switch_word_style", *tmp, switchWordStyle);
            read_string("case_word_style", *tmp, caseWordStyle);
            read_string("default_word_style", *tmp, defaultWordStyle);
            read_string("do_word_style", *tmp, doWordStyle);
            read_string("while_word_style", *tmp, whileWordStyle);
            read_string("for_word_style", *tmp, forWordStyle);
            read_string("for_each_word_style", *tmp, foreachWordStyle);
            read_string("try_word_style", *tmp, tryWordStyle);
            read_string("catch_word_style", *tmp, catchWordStyle);
            read_string("throw_word_style", *tmp, throwWordStyle);
            read_string("pointer_word_style", *tmp, pointerWordStyle);
        }
    }
}

void TextConfig::process_other_expressions(jValue const& expr)
{
    jValue const* tmp;
    if (is_object("OTHER_EXPRESSIONS", expr, tmp))
    {
        read_string("constructor_word", *tmp, constructorWord);
        read_string("destructor_word", *tmp, destructorWord);
        read_string("method_word", *tmp, methodWord);
        read_string("function_word", *tmp, functionWord);
        read_string("lambda_word", *tmp, lambdaWord);
        read_string("call_word", *tmp, callWord);
        read_string("define_word", *tmp, defineWord);
        read_string("returns_word", *tmp, returnsWord);
        read_string("repeat_word", *tmp, repeatWord);
        //
        if (is_object("STYLE", *tmp, tmp))
        {
            read_string("default_style", *tmp, otherExprStyle);
            read_string("constructor_word_style", *tmp, constructorWordStyle);
            read_string("destructor_word_style", *tmp, destructorWordStyle);
            read_string("method_word_style", *tmp, methodWordStyle);
            read_string("function_word_style", *tmp, functionWordStyle);
            read_string("lambda_word_style", *tmp, lambdaWordStyle);
            read_string("call_word_style", *tmp, callWordStyle);
            read_string("define_word_style", *tmp, defineWordStyle);
            read_string("returns_word_style", *tmp, returnsWordStyle);
            read_string("repeat_word_style", *tmp, repeatWordStyle);
        }
    }
}