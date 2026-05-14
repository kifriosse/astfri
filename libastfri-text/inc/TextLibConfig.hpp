#ifndef LIBASTFRI_TEXT_TEXT_LIB_CONFIG
#define LIBASTFRI_TEXT_TEXT_LIB_CONFIG

#include <astfri/impl/Concepts.hpp>
#include <rapidjson/document.h>

#include <vector>

namespace astfri::text
{
    struct TextLibConfig
    {
        // 1) CODE_STRUCTURE
        int tabulatorLength               = 4; ///< length of indentation
        bool namespaceBlockBracketNewLine = false; ///< should open namespace body bracket on new line?
        bool useNamespaceTabulator        = false; ///< should use extra indentation inside namespace block?
        bool objectBlockBracketNewLine    = false; ///< should open object body bracket on new line?
        bool functionBlockBracketNewLine  = false; ///< should open func/method body bracket on new line?
        bool loopBlockBracketNewLine      = false; ///< should open loop body bracket on new line?
        bool dowhileConditionNewLine      = false; ///< should write "do-while" condition word on new line?
        bool conditionBlockBracketNewLine = false; ///< should open condition body bracket on new line?
        bool elseifConditionNewLine       = false; ///< should write "else-if" condition on new line?
        bool elseConditionNewLine         = false; ///< should write "else" condition on new line?
        bool switchBlockBracketNewLine    = false; ///< should open switch body bracket on new line?
        bool trycatchBlockBracketNewLine  = false; ///< should open "try-catch" body bracket on new line?
        bool catchConditionNewLine        = false; ///< should write "try-catch" condition on new line?
        // 2) PSEUDOCODE_STRUCTURE
        int textMarginLeft      = 3;
        int rowNumMarginLeft    = 1;
        bool shBrColors         = true;
        bool shRowNum           = true;
        bool shDotAfterRowNum   = true;
        bool shRowNumOnEmptyRow = true;
        bool shGlobVarDeclar    = true;
        bool shTemplateDeclar   = true;
        bool shClassDeclar      = true;
        bool shClassDefin       = true;
        bool shClassDefinInl    = false;
        bool shInterfDeclar     = true;
        bool shInterfDefin      = true;
        bool shMembVarDeclar    = true;
        bool shCoDeMeDeclar     = true;
        bool shCoDeMeDefin      = true;
        bool shCoDeMeOwner      = true;
        bool shCoDeMeTemplate   = true;
        bool shFuncDeclar       = true;
        bool shFuncDefin        = true;
        // 3) PSEUDOCODE_TEXT
        // 3.1 GENERAL
        std::string unknownTypeWord    = "UNKNOWN TYPE";
        std::string unknownExprWord    = "UNKNOWN EXPRESSION";
        std::string unknownStmtWord    = "UNKNOWN STATEMENT";
        std::string defaultTextStyle   = "font-family:Consolas;font-size:16px";
        std::string unknownPhraseStyle = "";
        std::string rowNumStyle        = "";
        std::vector<std::string> brColors = {"red", "green", "blue"};
        // 3.2 SYMBOLS
        // 3.2.1 OPERATORS
        std::string pointerWord = "↑";
        std::string assignWord  = "=";
        std::string moduloWord  = "%";
        std::string addressWord = "&";
        std::string derefWord   = "*";
        std::string opWordStyle = "";
        // 3.2.2 SEPARATORS
        std::string semicolonWord = ";";
        std::string sepWordStyle  = "";
        // 3.3 VALUES
        std::string trueWord          = "true";
        std::string falseWord         = "false";
        std::string nullWord          = "nullptr";
        std::string valueStyle        = "";
        std::string numericValueStyle = "";
        std::string stringValueStyle  = "";
        // 3.4 REFERENCE_NAMES
        std::string defaultRefNameStyle = "";
        std::string templateNameStyle   = "";
        std::string classNameStyle      = "";
        std::string interfaceNameStyle  = "";
        std::string methodNameStyle     = "";
        std::string functionNameStyle   = "";
        std::string defaultVarNameStyle = "";
        std::string globalVarNameStyle  = "";
        std::string memberVarNameStyle  = "";
        std::string localVarNameStyle   = "";
        std::string paramVarNameStyle   = "";
        // 3.5 SYSTEM_EXPRESSIONS
        // 3.5.1 ACCESS_MODIFIERS
        std::string publicWord       = "public";
        std::string protectedWord    = "protected";
        std::string privateWord      = "private";
        std::string internalWord     = "package-private";
        std::string attributesWord   = "attributes";
        std::string constructorsWord = "constructors";
        std::string destructorsWord  = "destructors";
        std::string methodsWord      = "methods";
        std::string accessModifStyle = "";
        // 3.5.2 DATA_TYPES
        std::string dynamicTypeWord      = "auto";
        std::string intTypeWord          = "int";
        std::string floatTypeWord        = "float";
        std::string charTypeWord         = "char";
        std::string boolTypeWord         = "bool";
        std::string voidTypeWord         = "void";
        std::string typeWordStyle        = "";
        std::string numericTypeWordStyle = "";
        std::string stringTypeWordStyle  = "";
        std::string systemTypeWordStyle  = "";
        // 3.5.3 OBJECTS
        std::string scopeWord       = "namespace";
        std::string templateWord    = "template";
        std::string classWord       = "class";
        std::string interfaceWord   = "interface";
        std::string implementWord   = "implements";
        std::string extendWord      = "extends";
        std::string virtualWord     = "virtual";
        std::string abstractWord    = "abstract";
        std::string staticWord      = "static";
        std::string overrideWord    = "override";
        std::string thisWord        = "this";
        std::string objectWordStyle = "";
        // 3.5.4 CONDITIONS
        std::string ifWord             = "if";
        std::string elseifWord         = "else if";
        std::string elseWord           = "else";
        std::string switchWord         = "switch";
        std::string caseWord           = "case";
        std::string defaultWord        = "default";
        std::string conditionWordStyle = "";
        // 3.5.5 LOOPS
        std::string doWord        = "do";
        std::string whileWord     = "while";
        std::string forWord       = "for";
        std::string foreachWord   = "for";
        std::string loopWordStyle = "";
        // 3.5.6 OTHER
        std::string returnWord     = "return";
        std::string continueWord   = "continue";
        std::string breakWord      = "break";
        std::string tryWord        = "try";
        std::string catchWord      = "catch";
        std::string finallyWord    = "finally";
        std::string throwWord      = "throw";
        std::string newWord        = "new";
        std::string deleteWord     = "delete";
        std::string otherExprStyle = "";
        // 3.6 SUPPORT_EXPRESSIONS
        std::string constructorWord  = "constructor";
        std::string destructorWord   = "destructor";
        std::string methodWord       = "method";
        std::string functionWord     = "function";
        std::string lambdaWord       = "λ";
        std::string callWord         = "call";
        std::string defineWord       = "define";
        std::string returnsWord      = "returns ->";
        std::string repeatWord       = "repeat";
        std::string supportExprStyle = "";
        //
        static TextLibConfig createDefault();
        static TextLibConfig createFromArgs(int argc, char* argv[]);
        static TextLibConfig createFromJson(rapidjson::Value const& node);
        static TextLibConfig createFromJson(std::filesystem::path const& path);
        //
        void change_to_default();
        void change_to_java_like();
        void change_to_cxx_like();
        //
        bool try_create_json(std::filesystem::path const& path, rapidjson::Document& doc);
        void load_from_file(std::filesystem::path const& path);
        void load_from_json(rapidjson::Value const& json);
    private:
        using jValue = rapidjson::Value;
        //
        void process_code_structure(jValue const& structure); // 1)
        void process_pseudocode_structure(jValue const& structure); // 2)
        void process_pseudocode_text(jValue const& text); // 3)
        //
        void process_general_text(jValue const& text); // 3.1
        void process_symbols(jValue const& symbols); // 3.2
        void process_values(jValue const& values); // 3.3
        void process_reference_names(jValue const& names); // 3.4
        void process_system_expressions(jValue const& expr); // 3.5
        void process_access_modifiers(jValue const& modifiers); // 3.5.1
        void process_data_types(jValue const& types); // 3.5.2
        void process_objects(jValue const& objects); // 3.5.3
        void process_conditions(jValue const& conditions); // 3.5.4
        void process_loops(jValue const& loops); // 3.5.5
        void process_other(jValue const& other); // 3.5.6
        void process_support_expressions(jValue const& expr); // 3.6
        //
        bool is_object(std::string_view name, jValue const& val, jValue const*& val_out);
        void read_array(std::string_view name, jValue const& val, jValue const*& val_out);
        void read_string(std::string_view name, jValue const& val, std::string& m_string);
        void read_bool(std::string_view name, jValue const& val, bool& m_bool);
        void read_int(std::string_view name, jValue const& val, int& m_int);
    };
    static_assert(IsConfigClass<TextLibConfig, rapidjson::Value>, "TextLibConfig is not valid!");
}

#endif