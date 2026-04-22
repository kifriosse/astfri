#include <libastfri-text/inc/settings/BaseStructure.hpp>

#include <rapidjson/istreamwrapper.h>

#include <filesystem>
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

void BaseStructure::load_from_file(std::string_view jsonPath)
{
    std::error_code ec;
    if (!std::filesystem::is_regular_file(jsonPath, ec) || ec)
    {
        return;
    }
    std::ifstream jsonFile(jsonPath.data());
    if (!jsonFile)
    {
        return;
    }
    rapidjson::IStreamWrapper wrapper(jsonFile);
    rapidjson::Document doc;
    doc.ParseStream(wrapper);
    if (doc.HasParseError())
    {
        return;
    }
    //
    jValue const* baseStructure;
    if (is_object("BASE_STRUCTURE", doc, baseStructure))
    {
        load_from_json(*baseStructure);
    }
}

void BaseStructure::load_from_json(jValue const& json)
{
    read_int("tab_length", json, tabulatorLength);
    //
    read_bool("scope_bracket_new_line", json, namespaceBlockBracketNewLine);
    read_bool("use_tab_for_scope", json, useNamespaceTabulator);
    //
    read_bool("object_bracket_new_line", json, objectBlockBracketNewLine);
    read_bool("function_bracket_new_line", json, functionBlockBracketNewLine);
    //
    read_bool("loop_bracket_new_line", json, loopBlockBracketNewLine);
    read_bool("do-while_condition_new_line", json, dowhileConditionNewLine);
    //
    read_bool("condition_bracket_new_line", json, conditionBlockBracketNewLine);
    read_bool("else-if_condition_new_line", json, elseifConditionNewLine);
    read_bool("else_condition_new_line", json, elseConditionNewLine);
    read_bool("switch_bracket_new_line", json, switchBlockBracketNewLine);
    //
    read_bool("try-catch_bracket_new_line", json, trycatchBlockBracketNewLine);
    read_bool("catch_condition_new_line", json, catchConditionNewLine);
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