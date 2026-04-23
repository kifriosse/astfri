#ifndef LIBASTFRI_TEXT_TEXT_CONFIG
#define LIBASTFRI_TEXT_TEXT_CONFIG

#include <libastfri-text/inc/settings/GeneralConfig.hpp>

#include <vector>

namespace astfri::text
{
    struct TextConfig : GeneralConfig
    {
        // PSEUDOCODE_STYLE
        std::string textStyle   = "font-family:Consolas;font-size:16px";
        std::string rowNumStyle = "";
        std::vector<std::string> brColors = {"red", "green", "blue"};
        int textMarginLeft   = 3;
        int rowNumMarginLeft = 1;
        bool shBrColors            = true;
        bool shRowNum              = true;
        bool shDotAfterRowNum      = true;
        bool shRowNumOnEmptyRow    = true;
        bool resetRowNumOnEmptyRow = false;
        // PSEUDOCODE_STRUCTURE
        bool shOtherExprs     = true;
        bool shGlobVarDeclar  = true;
        bool shTemplateDeclar = true;
        bool shClassDeclar    = true;
        bool shClassDefin     = true;
        bool shClassDefinInl  = false;
        bool shInterfDeclar   = true;
        bool shInterfDefin    = true;
        bool shMembVarDeclar  = true;
        bool shCoDeMeDeclar   = true;
        bool shCoDeMeDefin    = true;
        bool shCoDeMeOwner    = true;
        bool shCoDeMeTemplate = true;
        bool shFuncDeclar     = true;
        bool shFuncDefin      = true;
        // SYSTEM_EXPRESSIONS
        std::string scopeWord     = "namespace";
        std::string templateWord  = "template";
        std::string classWord     = "class";
        std::string interfaceWord = "interface";
        std::string implementWord = "implements";
        std::string extendWord    = "extends";
        std::string virtualWord   = "virtual";
        std::string abstractWord  = "abstract";
        std::string staticWord    = "static";
        std::string overrideWord  = "override";
        std::string thisWord      = "this";
        std::string newWord       = "new";
        std::string deleteWord    = "delete";
        std::string returnWord    = "return";
        std::string continueWord  = "continue";
        std::string breakWord     = "break";
        std::string ifWord        = "if";
        std::string elseifWord    = "else if";
        std::string elseWord      = "else";
        std::string switchWord    = "switch";
        std::string caseWord      = "case";
        std::string defaultWord   = "default";
        std::string doWord        = "do";
        std::string whileWord     = "while";
        std::string forWord       = "for";
        std::string foreachWord   = "for";
        std::string tryWord       = "try";
        std::string catchWord     = "catch";
        std::string throwWord     = "throw";
        std::string pointerWord   = "↑";
        // STYLE
        std::string systemExprStyle    = "";
        std::string scopeWordStyle     = "";
        std::string templateWordStyle  = "";
        std::string classWordStyle     = "";
        std::string interfaceWordStyle = "";
        std::string implementWordStyle = "";
        std::string extendWordStyle    = "";
        std::string virtualWordStyle   = "";
        std::string abstractWordStyle  = "";
        std::string staticWordStyle    = "";
        std::string overrideWordStyle  = "";
        std::string thisWordStyle      = "";
        std::string newWordStyle       = "";
        std::string deleteWordStyle    = "";
        std::string returnWordStyle    = "";
        std::string continueWordStyle  = "";
        std::string breakWordStyle     = "";
        std::string ifWordStyle        = "";
        std::string elseifWordStyle    = "";
        std::string elseWordStyle      = "";
        std::string switchWordStyle    = "";
        std::string caseWordStyle      = "";
        std::string defaultWordStyle   = "";
        std::string doWordStyle        = "";
        std::string whileWordStyle     = "";
        std::string forWordStyle       = "";
        std::string foreachWordStyle   = "";
        std::string tryWordStyle       = "";
        std::string catchWordStyle     = "";
        std::string throwWordStyle     = "";
        std::string pointerWordStyle   = "";
        // OTHER_EXPRESSIONS
        std::string constructorWord = "constructor";
        std::string destructorWord  = "destructor";
        std::string methodWord      = "method";
        std::string functionWord    = "function";
        std::string lambdaWord      = "λ";
        std::string callWord        = "call";
        std::string defineWord      = "define";
        std::string returnsWord     = "returns ->";
        std::string repeatWord      = "repeat";
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
        std::string repeatWordStyle      = "";
        //
        //
        void change_to_default();
        //
        void load_from_file(std::string_view jsonPath);
        void load_from_json(rapidjson::Document const& doc);
    private:
        void process_pseudocode_style(jValue const& style);
        void process_pseudocode_structure(jValue const& structure);
        void process_system_expressions(jValue const& expr);
        void process_other_expressions(jValue const& expr);
    };
}

#endif