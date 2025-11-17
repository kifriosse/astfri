#include <libastfri-text/inc/pseudocode/TextConfigurator.hpp>

#include <rapidjson/istreamwrapper.h>

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace astfri::text;

TextConfigurator& TextConfigurator::get_instance()
{
    static TextConfigurator configurator;
    return configurator;
}

TextConfigurator::TextConfigurator() :
    GeneralConfigurator()
{
    isDefaultState_ = true;
    doOnlyUpdate_ = false;
    set_defaults();
}

//
// -----
//

void TextConfigurator::update_configuration()
{
    doOnlyUpdate_ = true;
    reload_configuration();
    doOnlyUpdate_ = false;
}

void TextConfigurator::reload_configuration()
{
    std::string input;
    std::cout << " > Do you want to use configuration file? [\"y\"/...]: ";
    std::getline(std::cin, input);
    if (input == "y")
    {
        std::cout << " > Path to .json file (without suffix): ";
        std::getline(std::cin, input);
        input.append(".json");
        std::error_code ec;
        if (std::filesystem::is_regular_file(input, ec) && !ec)
        {
            std::ifstream jsonfile(input);
            if (!jsonfile)
            {
                std::cout << " > File cannot be opened.\n";
                std::cout << " > No change in configuration.\n";
                return;
            }
            rj::IStreamWrapper wrap(jsonfile);
            rj::Document doc;
            doc.ParseStream(wrap);
            if (doc.HasParseError())
            {
                std::cout << " > Error when parsing configuration file.\n";
                std::cout << " > No change in configuration.\n";
                return;
            }
            if (!doOnlyUpdate_)
            {
                set_defaults();
                GeneralConfigurator::set_defaults();
            }
            isDefaultState_ = false;
            rj::Value const* docPtr = &doc;
            process_document(docPtr);
            GeneralConfigurator::process_document(docPtr);
            std::cout << " > Configuration was changed.\n";
        }
        else
        {
            std::cout << " > File not found.\n";
            std::cout << " > No change in configuration.\n";
        }
    }
    else
    {
        if (!isDefaultState_)
        {
            set_defaults();
            GeneralConfigurator::set_defaults();
            isDefaultState_ = true;
            std::cout << " > Configuration set to default state.\n";
        }
        else
        {
            std::cout << " > No change in configuration.\n";
        }
    }
}

void TextConfigurator::set_defaults()
{
    // TEXT_FORMAT
    defaultTextStyle_ = std::make_unique<std::stringstream>(
        "font-family:Consolas;font-size:18px");
    rowNumStyle_ = std::make_unique<std::stringstream>();
    bracketColors_ = std::make_unique<std::vector<std::stringstream>>();
    tabulatorLen_ = 4;
    textMarginLeftLen_ = 3;
    rowNumMarginLeftLen_ = 1;
    useBracketColors_ = false;
    shRowNum_ = true;
    shDotAfterRowNum_ = true;
    shRowNumOnEmptyRow_ = true;
    resetRowNumOnEmptyRow_ = false;
    newLineForCurlBracket_ = true;
    // CODE_STRUCTURE
    shOtherExpressions_ = true;
    shGlobalVars_ = true;
    shGenericParams_ = true;
    shClassDeclar_ = true;
    shClassDefin_ = true;
    shClassInline_ = false;
    shInterfDeclar_ = true;
    shInterfDefin_ = true;
    shMemberVars_ = true;
    shCoDeMeDeclar_ = true;
    shCoDeMeDefin_ = true;
    shCoDeMeOwner_ = true;
    shCoDeMeTemplate_ = true;
    shFuncDeclar_ = true;
    shFuncDefin_ = true;
    // SYSTEM_EXPRESSIONS
    scopeWord_     = std::make_unique<std::stringstream>("namespace");
    classWord_     = std::make_unique<std::stringstream>("class");
    interfaceWord_ = std::make_unique<std::stringstream>("interface");
    implementWord_ = std::make_unique<std::stringstream>("implements");
    extendWord_    = std::make_unique<std::stringstream>("extends");
    thisWord_      = std::make_unique<std::stringstream>("this");
    returnWord_    = std::make_unique<std::stringstream>("return");
    continueWord_  = std::make_unique<std::stringstream>("continue");
    breakWord_     = std::make_unique<std::stringstream>("break");
    throwWord_     = std::make_unique<std::stringstream>("throw");
    ifWord_        = std::make_unique<std::stringstream>("if");
    elseWord_      = std::make_unique<std::stringstream>("else");
    doWord_        = std::make_unique<std::stringstream>("do");
    whileWord_     = std::make_unique<std::stringstream>("while");
    forWord_       = std::make_unique<std::stringstream>("for");
    repeatWord_    = std::make_unique<std::stringstream>("repeat");
    switchWord_    = std::make_unique<std::stringstream>("switch");
    caseWord_      = std::make_unique<std::stringstream>("case");
    defaultWord_   = std::make_unique<std::stringstream>("default");
    newWord_       = std::make_unique<std::stringstream>("new");
    deleteWord_    = std::make_unique<std::stringstream>("delete");
    pointerWord_   = std::make_unique<std::stringstream>("↑");
    overrideWord_  = std::make_unique<std::stringstream>("override");
    virtualWord_   = std::make_unique<std::stringstream>("is virtual");
    abstractWord_  = std::make_unique<std::stringstream>("is abstract");
    templateWord_  = std::make_unique<std::stringstream>("template");
    // STYLE
    systExprStyle_      = std::make_unique<std::stringstream>();
    scopeWordStyle_     = std::make_unique<std::stringstream>();
    classWordStyle_     = std::make_unique<std::stringstream>();
    interfaceWordStyle_ = std::make_unique<std::stringstream>();
    implementWordStyle_ = std::make_unique<std::stringstream>();
    extendWordStyle_    = std::make_unique<std::stringstream>();
    thisWordStyle_      = std::make_unique<std::stringstream>();
    returnWordStyle_    = std::make_unique<std::stringstream>();
    continueWordStyle_  = std::make_unique<std::stringstream>();
    breakWordStyle_     = std::make_unique<std::stringstream>();
    throwWordStyle_     = std::make_unique<std::stringstream>();
    ifWordStyle_        = std::make_unique<std::stringstream>();
    elseWordStyle_      = std::make_unique<std::stringstream>();
    doWordStyle_        = std::make_unique<std::stringstream>();
    whileWordStyle_     = std::make_unique<std::stringstream>();
    forWordStyle_       = std::make_unique<std::stringstream>();
    repeatWordStyle_    = std::make_unique<std::stringstream>();
    switchWordStyle_    = std::make_unique<std::stringstream>();
    caseWordStyle_      = std::make_unique<std::stringstream>();
    defaultWordStyle_   = std::make_unique<std::stringstream>();
    newWordStyle_       = std::make_unique<std::stringstream>();
    deleteWordStyle_    = std::make_unique<std::stringstream>();
    pointerWordStyle_   = std::make_unique<std::stringstream>();
    overrideWordStyle_  = std::make_unique<std::stringstream>();
    virtualWordStyle_   = std::make_unique<std::stringstream>();
    abstractWordStyle_  = std::make_unique<std::stringstream>();
    templateWordStyle_  = std::make_unique<std::stringstream>();
    // OTHER_EXPRESSIONS
    constructorWord_ = std::make_unique<std::stringstream>("constructor");
    destructorWord_  = std::make_unique<std::stringstream>("destructor");
    methodWord_      = std::make_unique<std::stringstream>("method");
    functionWord_    = std::make_unique<std::stringstream>("function");
    lambdaWord_      = std::make_unique<std::stringstream>("λ");
    callWord_        = std::make_unique<std::stringstream>("call");
    defineWord_      = std::make_unique<std::stringstream>("define");
    returnsWord_     = std::make_unique<std::stringstream>("returns");
    // STYLE
    otherExprStyle_       = std::make_unique<std::stringstream>();
    constructorWordStyle_ = std::make_unique<std::stringstream>();
    destructorWordStyle_  = std::make_unique<std::stringstream>();
    methodWordStyle_      = std::make_unique<std::stringstream>();
    functionWordStyle_    = std::make_unique<std::stringstream>();
    lambdaWordStyle_      = std::make_unique<std::stringstream>();
    callWordStyle_        = std::make_unique<std::stringstream>();
    defineWordStyle_      = std::make_unique<std::stringstream>();
    returnsWordStyle_     = std::make_unique<std::stringstream>();
}

void TextConfigurator::process_document(rj::Value const*& doc)
{
    rj::Value const* textConf;
    // TEXT_CONFIGURATOR
    if (is_object("TEXT_CONFIGURATOR", doc, textConf))
    {
        rj::Value const* tmp;
        // TEXT_FORMAT
        if (is_object("TEXT_FORMAT", textConf, tmp))
        {
            process_text_format(tmp);
        }
        // CODE_STRUCTURE
        if (is_object("CODE_STRUCTURE", textConf, tmp))
        {
            process_code_structure(tmp);
        }
        // SYSTEM_EXPRESSIONS
        if (is_object("SYSTEM_EXPRESSIONS", textConf, tmp))
        {
            process_system_expressions(tmp);
        }
        // OTHER_EXPRESSIONS
        if (is_object("OTHER_EXPRESSIONS", textConf, tmp))
        {
            process_other_expressions(tmp);
        }
    }
}

void TextConfigurator::process_text_format(rj::Value const*& format)
{
    std::string stringVal;
    if (is_string("default_text_style", format, stringVal, false))
    {
        defaultTextStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("row_number_style", format, stringVal, false))
    {
        rowNumStyle_ = std::make_unique<std::stringstream>(stringVal);
    }
    rj::Value const* array;
    if (is_array("bracket_colors", format, array))
    {
        bracketColors_->clear();
        for (size_t i = 0; i < array->Size(); ++i)
        {
            if (array[i].IsString() && array[i].GetStringLength() > 0)
            {
                bracketColors_->push_back(std::stringstream(array[i].GetString()));
                if (bracketColors_->size() == 4)
                {
                    break;
                }
            }
        }
    }
    if (is_int("tabulator_length", format, tabulatorLen_))
    { }
    if (is_int("text_margin_left_length", format, textMarginLeftLen_))
    { }
    if (is_int("row_number_margin_left_length", format, rowNumMarginLeftLen_))
    { }
    if (is_bool("use_bracket_colors", format, useBracketColors_))
    { }
    if (is_bool("show_row_number", format, shRowNum_))
    { }
    if (is_bool("show_dot_after_row_number", format, shDotAfterRowNum_))
    { }
    if (is_bool("show_row_num_on_empty_row", format, shRowNumOnEmptyRow_))
    { }
    if (is_bool("reset_row_number_on_empty_row", format, resetRowNumOnEmptyRow_))
    { }
    if (is_bool("new_line_for_curl_bracket", format, newLineForCurlBracket_))
    { }
}

void TextConfigurator::process_code_structure(rj::Value const*& structure)
{
    if (is_bool("show_other_expressions", structure, shOtherExpressions_))
    { }
    if (is_bool("show_global_vars", structure, shGlobalVars_))
    { }
    if (is_bool("show_generic_params", structure, shGenericParams_))
    { }
    if (is_bool("show_class_declaration", structure, shClassDeclar_))
    { }
    if (is_bool("show_class_definition", structure, shClassDefin_))
    { }
    if (is_bool("show_class_inline", structure, shClassInline_))
    { }
    if (is_bool("show_interface_declaration", structure, shInterfDeclar_))
    { }
    if (is_bool("show_interface_definition", structure, shInterfDefin_))
    { }
    if (is_bool("show_member_vars", structure, shMemberVars_))
    { }
    if (is_bool("show_member_operations_declaration", structure, shCoDeMeDeclar_))
    { }
    if (is_bool("show_member_operations_definition", structure, shCoDeMeDefin_))
    { }
    if (is_bool("show_member_operations_owner", structure, shCoDeMeOwner_))
    { }
    if (is_bool("show_member_operations_template", structure, shCoDeMeTemplate_))
    { }
    if (is_bool("show_function_declaration", structure, shFuncDeclar_))
    { }
    if (is_bool("show_function_definition", structure, shFuncDefin_))
    { }
}

void TextConfigurator::process_system_expressions(rj::Value const*& expr)
{
    std::string stringVal;
    if (is_string("scope_word", expr, stringVal, true))
    {
        scopeWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("class_word", expr, stringVal, true))
    {
        classWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("interface_word", expr, stringVal, true))
    {
        interfaceWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("implement_word", expr, stringVal, true))
    {
        implementWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("extend_word", expr, stringVal, true))
    {
        extendWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("this_word", expr, stringVal, true))
    {
        thisWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("return_word", expr, stringVal, true))
    {
        returnWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("continue_word", expr, stringVal, true))
    {
        continueWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("break_word", expr, stringVal, true))
    {
        breakWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("throw_word", expr, stringVal, true))
    {
        throwWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("if_word", expr, stringVal, true))
    {
        ifWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("else_word", expr, stringVal, true))
    {
        elseWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("do_word", expr, stringVal, true))
    {
        doWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("while_word", expr, stringVal, true))
    {
        whileWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("for_word", expr, stringVal, true))
    {
        forWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("repeat_word", expr, stringVal, true))
    {
        repeatWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("switch_word", expr, stringVal, true))
    {
        switchWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("case_word", expr, stringVal, true))
    {
        caseWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("default_word", expr, stringVal, true))
    {
        defaultWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("new_word", expr, stringVal, true))
    {
        newWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("delete_word", expr, stringVal, true))
    {
        deleteWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("pointer_word", expr, stringVal, false))
    {
        pointerWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("override_word", expr, stringVal, false))
    {
        overrideWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("virtual_word", expr, stringVal, true))
    {
        virtualWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("abstract_word", expr, stringVal, true))
    {
        abstractWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("template_word", expr, stringVal, true))
    {
        templateWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    rj::Value const* style;
    if (is_object("STYLE", expr, style))
    {
        if (is_string("default_style", style, stringVal, false))
        {
            systExprStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("scope_word_style", style, stringVal, false))
        {
            scopeWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("class_word_style", style, stringVal, false))
        {
            classWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("interface_word_style", style, stringVal, false))
        {
            interfaceWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("implement_word_style", style, stringVal, false))
        {
            implementWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("extend_word_style", style, stringVal, false))
        {
            extendWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("this_word_style", style, stringVal, false))
        {
            thisWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("return_word_style", style, stringVal, false))
        {
            returnWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("continue_word_style", style, stringVal, false))
        {
            continueWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("break_word_style", style, stringVal, false))
        {
            breakWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("throw_word_style", style, stringVal, false))
        {
            throwWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("if_word_style", style, stringVal, false))
        {
            ifWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("else_word_style", style, stringVal, false))
        {
            elseWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("do_word_style", style, stringVal, false))
        {
            doWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("while_word_style", style, stringVal, false))
        {
            whileWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("for_word_style", style, stringVal, false))
        {
            forWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("repeat_word_style", style, stringVal, false))
        {
            repeatWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("switch_word_style", style, stringVal, false))
        {
            switchWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("case_word_style", style, stringVal, false))
        {
            caseWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("default_word_style", style, stringVal, false))
        {
            defaultWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("new_word_style", style, stringVal, false))
        {
            newWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("delete_word_style", style, stringVal, false))
        {
            deleteWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("pointer_word_style", style, stringVal, false))
        {
            pointerWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("override_word_style", style, stringVal, false))
        {
            overrideWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("virtual_word_style", style, stringVal, false))
        {
            virtualWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("abstract_word_style", style, stringVal, false))
        {
            abstractWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("template_word_style", style, stringVal, false))
        {
            templateWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
    }
}

void TextConfigurator::process_other_expressions(rj::Value const*& expr)
{
    std::string stringVal;
    if (is_string("constructor_word", expr, stringVal, true))
    {
        constructorWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("destructor_word", expr, stringVal, true))
    {
        destructorWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("method_word", expr, stringVal, true))
    {
        methodWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("function_word", expr, stringVal, true))
    {
        functionWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("lambda_word", expr, stringVal, false))
    {
        lambdaWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("call_word", expr, stringVal, true))
    {
        callWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("define_word", expr, stringVal, true))
    {
        defineWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    if (is_string("returns_word", expr, stringVal, true))
    {
        returnsWord_ = std::make_unique<std::stringstream>(stringVal);
    }
    rj::Value const* style;
    if (is_object("STYLE", expr, style))
    {
        if (is_string("default_style", style, stringVal, false))
        {
            otherExprStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("constructor_word_style", style, stringVal, false))
        {
            constructorWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("destructor_word_style", style, stringVal, false))
        {
            destructorWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("method_word_style", style, stringVal, false))
        {
            methodWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("function_word_style", style, stringVal, false))
        {
            functionWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("lambda_word_style", style, stringVal, false))
        {
            lambdaWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("call_word_style", style, stringVal, false))
        {
            callWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("define_word_style", style, stringVal, false))
        {
            defineWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
        if (is_string("returns_word_style", style, stringVal, false))
        {
            returnsWordStyle_ = std::make_unique<std::stringstream>(stringVal);
        }
    }
}