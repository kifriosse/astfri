#ifndef LIBASTFRI_TEXT_BASE_STRUCTURE
#define LIBASTFRI_TEXT_BASE_STRUCTURE

#include <rapidjson/document.h>

namespace astfri::text
{
    struct BaseStructure
    {
        using jValue = rapidjson::Value;
        //
        int tabulatorLength = 4; ///< length of indentation
        //
        bool namespaceBlockBracketNewLine = false; ///< should open namespace body bracket on new line?
        bool useNamespaceTabulator        = false; ///< should use extra indentation inside namespace block?
        //
        bool objectBlockBracketNewLine   = false; ///< should open object body bracket on new line?
        bool functionBlockBracketNewLine = false; ///< should open func/method body bracket on new line?
        //
        bool loopBlockBracketNewLine = false; ///< should open loop body bracket on new line?
        bool dowhileConditionNewLine = false; ///< should write "do-while" condition word on new line?
        //
        bool conditionBlockBracketNewLine = false; ///< should open condition body bracket on new line?
        bool elseifConditionNewLine       = false; ///< should write "else-if" condition on new line?
        bool elseConditionNewLine         = false; ///< should write "else" condition on new line?
        bool switchBlockBracketNewLine    = false; ///< shoud open switch body bracket on new line?
        //
        bool trycatchBlockBracketNewLine = false; ///< should open "try-catch" body bracket on new line?
        bool catchConditionNewLine       = false; ///< should write "try-catch" condition on new line?
        //
        //
        virtual ~BaseStructure() = default;
        //
        void change_to_default();
        void change_to_java_like();
        void change_to_cxx_like();
        //
        bool try_create_json(std::string_view jsonPath, rapidjson::Document& doc);
        void load_from_file(std::string_view jsonPath);
        void load_from_json(jValue const& json);
        //
        bool is_object(std::string_view name, jValue const& val, jValue const*& val_out);
        void read_array(std::string_view name, jValue const& val, jValue const*& val_out);
        void read_string(std::string_view name, jValue const& val, std::string& m_string);
        void read_bool(std::string_view name, jValue const& val, bool& m_bool);
        void read_int(std::string_view name, jValue const& val, int& m_int);
    };
}

#endif