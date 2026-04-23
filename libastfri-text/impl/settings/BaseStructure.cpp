#include <libastfri-text/inc/settings/BaseStructure.hpp>

#include <rapidjson/istreamwrapper.h>

#include <fstream>

using namespace astfri::text;

void BaseStructure::change_to_default()
{
    tabulatorLength = 4;
    //
    namespaceBlockBracketNewLine = false;
    useNamespaceTabulator        = false;
    //
    objectBlockBracketNewLine   = false;
    functionBlockBracketNewLine = false;
    //
    loopBlockBracketNewLine = false;
    dowhileConditionNewLine = false;
    //
    conditionBlockBracketNewLine = false;
    elseifConditionNewLine       = false;
    elseConditionNewLine         = false;
    switchBlockBracketNewLine    = false;
    //
    trycatchBlockBracketNewLine = false;
    catchConditionNewLine       = false;
}

void BaseStructure::change_to_java_like()
{
    change_to_default();
}

void BaseStructure::change_to_cxx_like()
{
    tabulatorLength = 4;
    //
    namespaceBlockBracketNewLine = true;
    useNamespaceTabulator        = true;
    //
    objectBlockBracketNewLine   = true;
    functionBlockBracketNewLine = true;
    //
    loopBlockBracketNewLine = true;
    dowhileConditionNewLine = true;
    //
    conditionBlockBracketNewLine = true;
    elseifConditionNewLine       = true;
    elseConditionNewLine         = true;
    switchBlockBracketNewLine    = true;
    //
    trycatchBlockBracketNewLine = true;
    catchConditionNewLine       = true;
}

bool BaseStructure::try_create_json(std::string_view jsonPath, rapidjson::Document& doc)
{
    std::ifstream jsonFile(jsonPath.data());
    if (!jsonFile)
    {
        return false;
    }
    rapidjson::IStreamWrapper wrapper(jsonFile);
    doc.ParseStream(wrapper);
    return !doc.HasParseError();
}

void BaseStructure::load_from_file(std::string_view jsonPath)
{
    rapidjson::Document doc;
    if (try_create_json(jsonPath, doc))
    {
        load_from_json(doc);
    }
}

void BaseStructure::load_from_json(rapidjson::Document const& doc)
{
    jValue const* tmp;
    if (is_object("BASE_STRUCTURE", doc, tmp))
    {
        read_int("tab_length", *tmp, tabulatorLength);
        //
        read_bool("scope_bracket_new_line", *tmp, namespaceBlockBracketNewLine);
        read_bool("use_tab_for_scope", *tmp, useNamespaceTabulator);
        //
        read_bool("object_bracket_new_line", *tmp, objectBlockBracketNewLine);
        read_bool("function_bracket_new_line", *tmp, functionBlockBracketNewLine);
        //
        read_bool("loop_bracket_new_line", *tmp, loopBlockBracketNewLine);
        read_bool("do-while_condition_new_line", *tmp, dowhileConditionNewLine);
        //
        read_bool("condition_bracket_new_line", *tmp, conditionBlockBracketNewLine);
        read_bool("else-if_condition_new_line", *tmp, elseifConditionNewLine);
        read_bool("else_condition_new_line", *tmp, elseConditionNewLine);
        read_bool("switch_bracket_new_line", *tmp, switchBlockBracketNewLine);
        //
        read_bool("try-catch_bracket_new_line", *tmp, trycatchBlockBracketNewLine);
        read_bool("catch_condition_new_line", *tmp, catchConditionNewLine);
    }
}

bool BaseStructure::is_object(std::string_view name, jValue const& val, jValue const*& val_out)
{
    auto const& it = val.FindMember(name.data());
    if (it == val.MemberEnd()) {
        return false;
    }
    val_out = &it->value;
    return val_out->IsObject() && !val_out->ObjectEmpty();
}

void BaseStructure::read_array(std::string_view name, jValue const& val, jValue const*& val_out)
{
    auto const& it = val.FindMember(name.data());
    val_out = (it != val.MemberEnd() && it->value.IsArray()) ? &it->value : nullptr;
}

void BaseStructure::read_string(std::string_view name, jValue const& val, std::string& m_string)
{
    auto const& it = val.FindMember(name.data());
    m_string = (it != val.MemberEnd() && it->value.IsString()) ? it->value.GetString() : m_string;
}

void BaseStructure::read_bool(std::string_view name, jValue const& val, bool& m_bool)
{
    auto const& it = val.FindMember(name.data());
    m_bool = (it != val.MemberEnd() && it->value.IsBool()) ? it->value.GetBool() : m_bool;
}

void BaseStructure::read_int(std::string_view name, jValue const& val, int& m_int)
{
    auto const& it = val.FindMember(name.data());
    m_int = (it != val.MemberEnd() && it->value.IsInt() && it->value.GetInt() >= 0) ?
        it->value.GetInt() : m_int;
}