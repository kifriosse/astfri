#ifndef LIBASTFRI_TEXT_GENERAL_CONFIG
#define LIBASTFRI_TEXT_GENERAL_CONFIG

#include <libastfri-text/inc/settings/BaseStructure.hpp>
#include <libastfri-text/inc/settings/OutputFileManager.hpp>

namespace astfri::text
{
    struct GeneralConfigurator : BaseStructure
    {
        OutputFileManager& fmanager = OutputFileManager::get_instance();
        // FILE_SETTINGS
        bool overwriteFile           = false;
        std::string outputFileName   = "output";
        std::string outputFilePath   = fmanager.default_folder_path();
        std::string outputFileFormat = "txt";
        // UNKNOWN_PHRASES
        std::string unknownTypeWord = "UNKNOWN TYPE";
        std::string unknownExprWord = "UNKNOWN EXPRESSION";
        std::string unknownStmtWord = "UNKNOWN STATEMENT";
        // STYLE
        std::string defaultPhraseStyle   = "";
        std::string unknownTypeWordStyle = "";
        std::string unknownExprWordStyle = "";
        std::string unknownStmtWordStyle = "";
        // ACCESS_MODIFIERS
        bool useInnerView               = true;
        std::string publicWord          = "public";
        std::string protectedWord       = "protected";
        std::string privateWord         = "private";
        std::string internalWord        = "internal";
        std::string attributesWord      = "attributes";
        std::string constructorsWord    = "constructors";
        std::string destructorsWord     = "destructors";
        std::string methodsWord         = "methods";
        std::string accessModifierStyle = "";
        // DATA_TYPES
        std::string dynamicTypeWord = "auto";
        std::string intTypeWord     = "int";
        std::string floatTypeWord   = "float";
        std::string charTypeWord    = "char";
        std::string boolTypeWord    = "bool";
        std::string voidTypeWord    = "void";
        // STYLE
        std::string defaultTypeWordStyle = "";
        std::string dynamicTypeWordStyle = "";
        std::string intTypeWordStyle     = "";
        std::string floatTypeWordStyle   = "";
        std::string charTypeWordStyle    = "";
        std::string boolTypeWordStyle    = "";
        std::string voidTypeWordStyle    = "";
        std::string userTypeStyle        = "";
        // REFERENCE_NAMES
        std::string defaultRefNameStyle   = "";
        std::string genericParamNameStyle = "";
        std::string classNameStyle        = "";
        std::string interfaceNameStyle    = "";
        std::string methodNameStyle       = "";
        std::string functionNameStyle     = "";
        std::string defaultVarNameStyle   = "";
        std::string globalVarNameStyle    = "";
        std::string memberVarNameStyle    = "";
        std::string localVarNameStyle     = "";
        std::string paramVarNameStyle     = "";
        // OPERATORS
        std::string assignOpWord  = "=";
        std::string moduloOpWord  = "%";
        std::string addressOpWord = "&";
        std::string derefOpWord   = "*";
        std::string operatorStyle = "";
        // SEPARATORS
        std::string separatorStyle = "";
        // VALUES
        std::string trueValWord  = "true";
        std::string falseValWord = "false";
        std::string nullValWord  = "NULL";
        // STYLE
        std::string defaultValStyle = "";
        std::string intValStyle     = "";
        std::string floatValStyle   = "";
        std::string charValStyle    = "";
        std::string stringValStyle  = "";
        std::string boolValStyle    = "";
        std::string nullValStyle    = "";
        //
        //
        virtual ~GeneralConfigurator() = default;
        //
        void change_to_default();
        //
        void load_from_json(jValue const& json);
        //
    private:
        void process_file_settings(jValue const& settings);
        void process_unknown_phrases(jValue const& phrases);
        void process_access_modifiers(jValue const& modifiers);
        void process_data_types(jValue const& types);
        void process_reference_names(jValue const& names);
        void process_ops_and_seps(jValue const& signs);
        void process_values(jValue const& values);
    };
}

#endif