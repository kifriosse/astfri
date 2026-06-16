#ifndef LIBASTFRI_TEXT_TEXT_LIB_CONFIG
#define LIBASTFRI_TEXT_TEXT_LIB_CONFIG

#include <astfri/impl/Concepts.hpp>

#include <rapidjson/document.h>

#include <vector>


namespace astfri::text {


struct Config {
    // 1) CODE_STRUCTURE
    int tabulatorLength;                ///< length of indentation
    bool namespaceBlockBracketNewLine;  ///< should open namespace body bracket on new line?
    bool useNamespaceTabulator;         ///< should use extra indentation inside namespace block?
    bool objectBlockBracketNewLine;     ///< should open object body bracket on new line?
    bool functionBlockBracketNewLine;   ///< should open func/method body bracket on new line?
    bool loopBlockBracketNewLine;       ///< should open loop body bracket on new line?
    bool dowhileConditionNewLine;       ///< should write "do-while" condition word on new line?
    bool conditionBlockBracketNewLine;  ///< should open condition body bracket on new line?
    bool elseifConditionNewLine;        ///< should write "else-if" condition on new line?
    bool elseConditionNewLine;          ///< should write "else" condition on new line?
    bool switchBlockBracketNewLine;     ///< should open switch body bracket on new line?
    bool trycatchBlockBracketNewLine;   ///< should open "try-catch" body bracket on new line?
    bool catchConditionNewLine;         ///< should write "try-catch" condition on new line?
    // 2) PSEUDOCODE_STRUCTURE
    int textMarginLeft;
    int rowNumMarginLeft;
    bool shBrColors;
    bool shRowNum;
    bool shDotAfterRowNum;
    bool shRowNumOnEmptyRow;
    bool shGlobVarDeclar;
    bool shTemplateDeclar;
    bool shClassDeclar;
    bool shClassDefin;
    bool shClassDefinInl;
    bool shInterfDeclar;
    bool shInterfDefin;
    bool shMembVarDeclar;
    bool shCoDeMeDeclar;
    bool shCoDeMeDefin;
    bool shCoDeMeOwner;
    bool shCoDeMeTemplate;
    bool shFuncDeclar;
    bool shFuncDefin;
    // 3) PSEUDOCODE_TEXT
    // 3.1 GENERAL
    std::string unknownTypeWord;
    std::string unknownExprWord;
    std::string unknownStmtWord;
    std::string defaultTextStyle;
    std::string unknownPhraseStyle;
    std::string rowNumStyle;
    // 3.2 SYMBOLS
    // 3.2.1 OPERATORS
    std::string pointerWord;
    std::string assignWord;
    std::string moduloWord;
    std::string addressWord;
    std::string derefWord;
    std::string opWordStyle;
    std::vector<std::string> brColors;
    // 3.2.2 SEPARATORS
    std::string semicolonWord;
    std::string sepWordStyle;
    // 3.3 VALUES
    std::string trueWord;
    std::string falseWord;
    std::string nullWord;
    std::string valueStyle;
    std::string numericValueStyle;
    std::string stringValueStyle;
    // 3.4 REFERENCE_NAMES
    std::string defaultRefNameStyle;
    std::string templateNameStyle;
    std::string classNameStyle;
    std::string interfaceNameStyle;
    std::string methodNameStyle;
    std::string functionNameStyle;
    std::string defaultVarNameStyle;
    std::string globalVarNameStyle;
    std::string memberVarNameStyle;
    std::string localVarNameStyle;
    std::string paramVarNameStyle;
    // 3.5 SYSTEM_EXPRESSIONS
    // 3.5.1 ACCESS_MODIFIERS
    std::string publicWord;
    std::string protectedWord;
    std::string privateWord;
    std::string internalWord;
    std::string attributesWord;
    std::string constructorsWord;
    std::string destructorsWord;
    std::string methodsWord;
    std::string accessModifStyle;
    // 3.5.2 DATA_TYPES
    std::string dynamicTypeWord;
    std::string intTypeWord;
    std::string floatTypeWord;
    std::string charTypeWord;
    std::string boolTypeWord;
    std::string voidTypeWord;
    std::string typeWordStyle;
    std::string numericTypeWordStyle;
    std::string stringTypeWordStyle;
    std::string systemTypeWordStyle;
    // 3.5.3 OBJECTS
    std::string scopeWord;
    std::string templateWord;
    std::string classWord;
    std::string interfaceWord;
    std::string implementWord;
    std::string extendWord;
    std::string virtualWord;
    std::string abstractWord;
    std::string staticWord;
    std::string overrideWord;
    std::string thisWord;
    std::string objectWordStyle;
    // 3.5.4 CONDITIONS
    std::string ifWord;
    std::string elseifWord;
    std::string elseWord;
    std::string switchWord;
    std::string caseWord;
    std::string defaultWord;
    std::string conditionWordStyle;
    // 3.5.5 LOOPS
    std::string doWord;
    std::string whileWord;
    std::string forWord;
    std::string foreachWord;
    std::string loopWordStyle;
    // 3.5.6 OTHER
    std::string returnWord;
    std::string continueWord;
    std::string breakWord;
    std::string tryWord;
    std::string catchWord;
    std::string finallyWord;
    std::string throwWord;
    std::string newWord;
    std::string deleteWord;
    std::string otherExprStyle;
    // 3.6 SUPPORT_EXPRESSIONS
    std::string constructorWord;
    std::string destructorWord;
    std::string methodWord;
    std::string functionWord;
    std::string lambdaWord;
    std::string callWord;
    std::string defineWord;
    std::string returnsWord;
    std::string repeatWord;
    std::string supportExprStyle;
    // 4) OUTPUT_SETTINGS
    std::string fileName;
    std::string filePath;
    std::string fileFormat;

public:
    static Config createDefault();
    static Config createFromArgs(int argc, char* argv[]);
    static Config createFromJson(const rapidjson::Value &node);
    static Config createFromJson(const std::filesystem::path &path);

public:
    void write_json(rapidjson::Value &out, rapidjson::Document::AllocatorType &alloc) const;
    void write_json_file(const std::filesystem::path &path) const;

    void change_to_java_like();
    void change_to_cxx_like();

private:
    void read_code_structure(const rapidjson::Value &structure); // 1)
    void read_pseudocode_structure(const rapidjson::Value &structure); // 2)
    void read_pseudocode_text(const rapidjson::Value &text); // 3)
    void read_output_settings(const rapidjson::Value &settings); // 4)
    //
    void read_general_text(const rapidjson::Value &text); // 3.1
    void read_symbols(const rapidjson::Value &symbols); // 3.2
    void read_values(const rapidjson::Value &values); // 3.3
    void read_reference_names(const rapidjson::Value &names); // 3.4
    void read_system_expressions(const rapidjson::Value &expr); // 3.5
    void read_access_modifiers(const rapidjson::Value &modifiers); // 3.5.1
    void read_data_types(const rapidjson::Value &types); // 3.5.2
    void read_objects(const rapidjson::Value &objects); // 3.5.3
    void read_conditions(const rapidjson::Value &conditions); // 3.5.4
    void read_loops(const rapidjson::Value &loops); // 3.5.5
    void read_other(const rapidjson::Value &other); // 3.5.6
    void read_support_expressions(const rapidjson::Value &expr); // 3.6
    //
    void write_code_structure(rapidjson::Value &structure, rapidjson::Document::AllocatorType &alloc) const; // 1)
    //
    void write_pseudocode_structure(rapidjson::Value &structure, rapidjson::Document::AllocatorType &alloc) const; // 2)
    //
    void write_pseudocode_text(rapidjson::Value &text, rapidjson::Document::AllocatorType &alloc) const; // 3)
    ////
    void write_general_text(rapidjson::Value &text, rapidjson::Document::AllocatorType &alloc) const; // 3.1
    void write_symbols(rapidjson::Value &symbols, rapidjson::Document::AllocatorType &alloc) const; // 3.2
    void write_values(rapidjson::Value &values, rapidjson::Document::AllocatorType &alloc) const; // 3.3
    void write_reference_names(rapidjson::Value &names, rapidjson::Document::AllocatorType &alloc) const; // 3.4
    void write_system_expressions(rapidjson::Value &expr, rapidjson::Document::AllocatorType &alloc) const; // 3.5
    //////
    void write_access_modifiers(rapidjson::Value &modifiers, rapidjson::Document::AllocatorType& alloc) const; // 3.5.1
    void write_data_types(rapidjson::Value &types, rapidjson::Document::AllocatorType& alloc) const; // 3.5.2
    void write_objects(rapidjson::Value &objects, rapidjson::Document::AllocatorType& alloc) const; // 3.5.3
    void write_conditions(rapidjson::Value &conditions, rapidjson::Document::AllocatorType& alloc) const; // 3.5.4
    void write_loops(rapidjson::Value &loops, rapidjson::Document::AllocatorType& alloc) const; // 3.5.5
    void write_other(rapidjson::Value &other, rapidjson::Document::AllocatorType& alloc) const; // 3.5.6
    ////
    void write_support_expressions(rapidjson::Value &expr, rapidjson::Document::AllocatorType &alloc) const; // 3.6
    //
    void write_output_settings(rapidjson::Value &settings, rapidjson::Document::AllocatorType &alloc) const; // 4)
};

static_assert(IsConfigClass<Config, rapidjson::Value>);


} // namespace astfri::text

#endif