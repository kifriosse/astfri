#ifndef LIBASTFRI_TEXT_TEXT_CONFIG
#define LIBASTFRI_TEXT_TEXT_CONFIG

#include <libastfri-text/inc/settings/GeneralConfig.hpp>

#include <vector>

namespace astfri::text
{
    struct TextConfigurator : GeneralConfigurator
    {
        // TEXT_FORMAT
        std::string defaultTextStyle = "font-family:Consolas;font-size:18px";
        std::string rowNumStyle      = "";
        std::vector<std::string> bracketColors;
        int tabulatorLen        = 4;
        int textMarginLeftLen   = 3;
        int rowNumMarginLeftLen = 1;
        bool useBracketColors      = false;
        bool shRowNum              = true;
        bool shDotAfterRowNum      = true;
        bool shRowNumOnEmptyRow    = true;
        bool resetRowNumOnEmptyRow = false;
        bool newLineForCurlBracket = true;
        // CODE_STRUCTURE
        bool shOtherExpressions = true;
        bool shGlobalVars       = true;
        bool shGenericParams    = true;
        bool shClassDeclar      = true;
        bool shClassDefin       = true;
        bool shClassInline      = false;
        bool shInterfDeclar     = true;
        bool shInterfDefin      = true;
        bool shMemberVars       = true;
        bool shCoDeMeDeclar     = true;
        bool shCoDeMeDefin      = true;
        bool shCoDeMeOwner      = true;
        bool shCoDeMeTemplate   = true;
        bool shFuncDeclar       = true;
        bool shFuncDefin        = true;
        // SYSTEM_EXPRESSIONS
        std::string scopeWord     = "namespace";
        std::string classWord     = "class";
        std::string interfaceWord = "interface";
        std::string implementWord = "implements";
        std::string extendWord    = "extends";
        std::string thisWord      = "this";
        std::string returnWord    = "return";
        std::string continueWord  = "continue";
        std::string breakWord     = "break";
        std::string throwWord     = "throw";
        std::string ifWord        = "if";
        std::string elseWord      = "else";
        std::string doWord        = "do";
        std::string whileWord     = "while";
        std::string forWord       = "for";
        std::string repeatWord    = "repeat";
        std::string switchWord    = "switch";
        std::string caseWord      = "case";
        std::string defaultWord   = "default";
        std::string newWord       = "new";
        std::string deleteWord    = "delete";
        std::string pointerWord   = "↑";
        std::string overrideWord  = "override";
        std::string virtualWord   = "is virtual";
        std::string abstractWord  = "is abstract";
        std::string templateWord  = "template";
        // STYLE
        std::string systExprStyle      = "";
        std::string scopeWordStyle     = "";
        std::string classWordStyle     = "";
        std::string interfaceWordStyle = "";
        std::string implementWordStyle = "";
        std::string extendWordStyle    = "";
        std::string thisWordStyle      = "";
        std::string returnWordStyle    = "";
        std::string continueWordStyle  = "";
        std::string breakWordStyle     = "";
        std::string throwWordStyle     = "";
        std::string ifWordStyle        = "";
        std::string elseWordStyle      = "";
        std::string doWordStyle        = "";
        std::string whileWordStyle     = "";
        std::string forWordStyle       = "";
        std::string repeatWordStyle    = "";
        std::string switchWordStyle    = "";
        std::string caseWordStyle      = "";
        std::string defaultWordStyle   = "";
        std::string newWordStyle       = "";
        std::string deleteWordStyle    = "";
        std::string pointerWordStyle   = "";
        std::string overrideWordStyle  = "";
        std::string virtualWordStyle   = "";
        std::string abstractWordStyle  = "";
        std::string templateWordStyle  = "";
        // OTHER_EXPRESSIONS
        std::string constructorWord = "constructor";
        std::string destructorWord  = "destructor";
        std::string methodWord      = "method";
        std::string functionWord    = "function";
        std::string lambdaWord      = "λ";
        std::string callWord        = "call";
        std::string defineWord      = "define";
        std::string returnsWord     = "returns";
        // STYLE
        std::string otherExprStyle       = "";
        std::string constructorWordStyle = "";
        std::string destructorWordStyle  = "";
        std::string methodWordStyle      = "";
        std::string functionWordStyle    = "";
        std::string lambdaWordStyle      = "";
        std::string callWordStyle        = "";
        std::string defineWordStyle      = "";
        std::string returnsWordStyle     = "";
        //
        //
        void change_to_default();
        //
        void load_from_file(std::string_view jsonPath);
        void load_from_json(jValue const& json);
    private:
        void process_text_format(jValue const& format);
        void process_code_structure(jValue const& structure);
        void process_system_expressions(jValue const& expr);
        void process_other_expressions(jValue const& expr);
    };
}

#endif