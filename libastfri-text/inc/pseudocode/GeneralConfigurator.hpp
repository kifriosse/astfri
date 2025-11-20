#ifndef LIBASTFRI_TEXT_GENERAL_CONFIGURATOR
#define LIBASTFRI_TEXT_GENERAL_CONFIGURATOR

#include <libastfri-text/inc/OutputFileManager.hpp>

#include <rapidjson/document.h>

#include <sstream>

namespace rj = rapidjson;
using STRING = char const*;

namespace astfri::text
{
    class GeneralConfigurator
    {
        // FILE_SETTINGS
        bool overwriteFile_;
        std::unique_ptr<std::stringstream> outputFileName_;
        std::unique_ptr<std::stringstream> outputFilePath_;
        std::unique_ptr<std::stringstream> outputFileFormat_;
        // UNKNOWN_PHRASES
        std::unique_ptr<std::stringstream> unknownTypeWord_;
        std::unique_ptr<std::stringstream> unknownExprWord_;
        std::unique_ptr<std::stringstream> unknownStmtWord_;
        std::unique_ptr<std::stringstream> invalidTypeWord_;
        std::unique_ptr<std::stringstream> invalidExprWord_;
        std::unique_ptr<std::stringstream> invalidStmtWord_;
        // STYLE
        std::unique_ptr<std::stringstream> defaultPhraseStyle_;
        std::unique_ptr<std::stringstream> unknownTypeWordStyle_;
        std::unique_ptr<std::stringstream> unknownExprWordStyle_;
        std::unique_ptr<std::stringstream> unknownStmtWordStyle_;
        std::unique_ptr<std::stringstream> invalidTypeWordStyle_;
        std::unique_ptr<std::stringstream> invalidExprWordStyle_;
        std::unique_ptr<std::stringstream> invalidStmtWordStyle_;
        // ACCESS_MODIFIERS
        bool useInnerView_;
        std::unique_ptr<std::stringstream> publicWord_;
        std::unique_ptr<std::stringstream> protectedWord_;
        std::unique_ptr<std::stringstream> privateWord_;
        std::unique_ptr<std::stringstream> internalWord_;
        std::unique_ptr<std::stringstream> attributesWord_;
        std::unique_ptr<std::stringstream> constructorsWord_;
        std::unique_ptr<std::stringstream> destructorsWord_;
        std::unique_ptr<std::stringstream> methodsWord_;
        std::unique_ptr<std::stringstream> accessModifierStyle_;
        // DATA_TYPES
        std::unique_ptr<std::stringstream> dynamicTypeWord_;
        std::unique_ptr<std::stringstream> intTypeWord_;
        std::unique_ptr<std::stringstream> floatTypeWord_;
        std::unique_ptr<std::stringstream> charTypeWord_;
        std::unique_ptr<std::stringstream> boolTypeWord_;
        std::unique_ptr<std::stringstream> voidTypeWord_;
        // STYLE
        std::unique_ptr<std::stringstream> defaultTypeWordStyle_;
        std::unique_ptr<std::stringstream> dynamicTypeWordStyle_;
        std::unique_ptr<std::stringstream> intTypeWordStyle_;
        std::unique_ptr<std::stringstream> floatTypeWordStyle_;
        std::unique_ptr<std::stringstream> charTypeWordStyle_;
        std::unique_ptr<std::stringstream> boolTypeWordStyle_;
        std::unique_ptr<std::stringstream> voidTypeWordStyle_;
        std::unique_ptr<std::stringstream> userTypeStyle_;
        // REFERENCE_NAMES
        std::unique_ptr<std::stringstream> defaultRefNameStyle_;
        std::unique_ptr<std::stringstream> genericParamNameStyle_;
        std::unique_ptr<std::stringstream> classNameStyle_;
        std::unique_ptr<std::stringstream> interfaceNameStyle_;
        std::unique_ptr<std::stringstream> methodNameStyle_;
        std::unique_ptr<std::stringstream> functionNameStyle_;
        std::unique_ptr<std::stringstream> defaultVarNameStyle_;
        std::unique_ptr<std::stringstream> globalVarNameStyle_;
        std::unique_ptr<std::stringstream> memberVarNameStyle_;
        std::unique_ptr<std::stringstream> localVarNameStyle_;
        std::unique_ptr<std::stringstream> paramVarNameStyle_;
        // OPERATORS
        std::unique_ptr<std::stringstream> assignOpWord_;
        std::unique_ptr<std::stringstream> moduloOpWord_;
        std::unique_ptr<std::stringstream> addressOpWord_;
        std::unique_ptr<std::stringstream> derefOpWord_;
        std::unique_ptr<std::stringstream> operatorStyle_;
        // SEPARATORS
        std::unique_ptr<std::stringstream> separatorStyle_;
        // VALUES
        std::unique_ptr<std::stringstream> trueValWord_;
        std::unique_ptr<std::stringstream> falseValWord_;
        std::unique_ptr<std::stringstream> nullValWord_;
        // STYLE
        std::unique_ptr<std::stringstream> defaultValStyle_;
        std::unique_ptr<std::stringstream> intValStyle_;
        std::unique_ptr<std::stringstream> floatValStyle_;
        std::unique_ptr<std::stringstream> charValStyle_;
        std::unique_ptr<std::stringstream> stringValStyle_;
        std::unique_ptr<std::stringstream> boolValStyle_;
        std::unique_ptr<std::stringstream> nullValStyle_;
    private:
        OutputFileManager* fmanager_;
    protected:
        explicit GeneralConfigurator();
        virtual ~GeneralConfigurator() = default;
    protected:
        virtual void set_defaults();
        virtual void process_document(rj::Value const*& doc);
        // -----
        bool is_string(STRING name, rj::Value const*& rjval,
                                    std::string& getString, bool checkStrLen);
        bool is_object(STRING name, rj::Value const*& rjval,
                                    rj::Value const*& getObject);
        bool is_array(STRING name, rj::Value const*& rjval,
                                    rj::Value const*& getArray);
        bool is_bool(STRING name, rj::Value const*& rjval, bool& getBool);
        bool is_int(STRING name, rj::Value const*& rjval, int& getInt);
    private:
        void process_file_settings(rj::Value const*& settings);
        void process_unknown_phrases(rj::Value const*& phrases);
        void process_access_modifiers(rj::Value const*& modifiers);
        void process_data_types(rj::Value const*& types);
        void process_reference_names(rj::Value const*& names);
        void process_ops_and_seps(rj::Value const*& signs);
        void process_values(rj::Value const*& values);
    public:
        // FILE_SETTINGS
        bool const& overwrite_file()
        {
            return overwriteFile_;
        }

        std::stringstream const* output_file_name()
        {
            return outputFileName_.get();
        }

        std::stringstream const* output_file_path()
        {
            return outputFilePath_.get();
        }

        std::stringstream const* output_file_format()
        {
            return outputFileFormat_.get();
        }

        // UNKNOWN_PHRASES
        std::stringstream const* unknown_type_word()
        {
            return unknownTypeWord_.get();
        }

        std::stringstream const* unknown_expr_word()
        {
            return unknownExprWord_.get();
        }

        std::stringstream const* unknown_stmt_word()
        {
            return unknownStmtWord_.get();
        }

        std::stringstream const* invalid_type_word()
        {
            return invalidTypeWord_.get();
        }

        std::stringstream const* invalid_expr_word()
        {
            return invalidExprWord_.get();
        }

        std::stringstream const* invalid_stmt_word()
        {
            return invalidStmtWord_.get();
        }

        // STYLE
        std::stringstream const* default_NA_phrase_style()
        {
            return defaultPhraseStyle_.get();
        }

        std::stringstream const* unknown_type_word_style()
        {
            return unknownTypeWordStyle_.get();
        }

        std::stringstream const* unknown_expr_word_style()
        {
            return unknownExprWordStyle_.get();
        }

        std::stringstream const* unknown_stmt_word_style()
        {
            return unknownStmtWordStyle_.get();
        }

        std::stringstream const* invalid_type_word_style()
        {
            return invalidTypeWordStyle_.get();
        }

        std::stringstream const* invalid_expr_word_style()
        {
            return invalidExprWordStyle_.get();
        }

        std::stringstream const* invalid_stmt_word_style()
        {
            return invalidStmtWordStyle_.get();
        }

        // ACCESS_MODIFIERS
        bool const& use_inner_view()
        {
            return useInnerView_;
        }

        std::stringstream const* public_word()
        {
            return publicWord_.get();
        }

        std::stringstream const* protected_word()
        {
            return protectedWord_.get();
        }

        std::stringstream const* private_word()
        {
            return privateWord_.get();
        }

        std::stringstream const* internal_word()
        {
            return internalWord_.get();
        }

        std::stringstream const* attributes_word()
        {
            return attributesWord_.get();
        }

        std::stringstream const* constructors_word()
        {
            return constructorsWord_.get();
        }

        std::stringstream const* destructors_word()
        {
            return destructorsWord_.get();
        }

        std::stringstream const* methods_word()
        {
            return methodsWord_.get();
        }

        std::stringstream const* access_mod_style()
        {
            return accessModifierStyle_.get();
        }

        // DATA_TYPES
        std::stringstream const* dynamic_type_word()
        {
            return dynamicTypeWord_.get();
        }

        std::stringstream const* int_type_word()
        {
            return intTypeWord_.get();
        }

        std::stringstream const* float_type_word()
        {
            return floatTypeWord_.get();
        }

        std::stringstream const* char_type_word()
        {
            return charTypeWord_.get();
        }

        std::stringstream const* bool_type_word()
        {
            return boolTypeWord_.get();
        }

        std::stringstream const* void_type_word()
        {
            return voidTypeWord_.get();
        }

        // STYLE
        std::stringstream const* default_type_word_style()
        {
            return defaultTypeWordStyle_.get();
        }

        std::stringstream const* dynamic_type_word_style()
        {
            return dynamicTypeWordStyle_.get();
        }

        std::stringstream const* int_type_word_style()
        {
            return intTypeWordStyle_.get();
        }

        std::stringstream const* float_type_word_style()
        {
            return floatTypeWordStyle_.get();
        }

        std::stringstream const* char_type_word_style()
        {
            return charTypeWordStyle_.get();
        }

        std::stringstream const* bool_type_word_style()
        {
            return boolTypeWordStyle_.get();
        }

        std::stringstream const* void_type_word_style()
        {
            return voidTypeWordStyle_.get();
        }

        std::stringstream const* user_type_style()
        {
            return userTypeStyle_.get();
        }

        // REFERENCE_NAMES
        std::stringstream const* default_ref_name_style()
        {
            return defaultRefNameStyle_.get();
        }

        std::stringstream const* gen_param_name_style()
        {
            return genericParamNameStyle_.get();
        }

        std::stringstream const* class_name_style()
        {
            return classNameStyle_.get();
        }

        std::stringstream const* interface_name_style()
        {
            return interfaceNameStyle_.get();
        }

        std::stringstream const* method_name_style()
        {
            return methodNameStyle_.get();
        }

        std::stringstream const* function_name_style()
        {
            return functionNameStyle_.get();
        }

        std::stringstream const* default_var_name_style()
        {
            return defaultVarNameStyle_.get();
        }

        std::stringstream const* global_var_name_style()
        {
            return globalVarNameStyle_.get();
        }

        std::stringstream const* member_var_name_style()
        {
            return memberVarNameStyle_.get();
        }

        std::stringstream const* local_var_name_style()
        {
            return localVarNameStyle_.get();
        }

        std::stringstream const* param_var_name_style()
        {
            return paramVarNameStyle_.get();
        }

        // OPERATORS
        std::stringstream const* assign_op_word()
        {
            return assignOpWord_.get();
        }

        std::stringstream const* modulo_op_word()
        {
            return moduloOpWord_.get();
        }

        std::stringstream const* address_op_word()
        {
            return addressOpWord_.get();
        }

        std::stringstream const* deref_op_word()
        {
            return derefOpWord_.get();
        }

        std::stringstream const* operator_style()
        {
            return operatorStyle_.get();
        }

        // SEPARATORS
        std::stringstream const* separator_style()
        {
            return separatorStyle_.get();
        }

        // VALUES
        std::stringstream const* true_val_word()
        {
            return trueValWord_.get();
        }

        std::stringstream const* false_val_word()
        {
            return falseValWord_.get();
        }

        std::stringstream const* null_val_word()
        {
            return nullValWord_.get();
        }

        // STYLE
        std::stringstream const* default_val_style()
        {
            return defaultValStyle_.get();
        }

        std::stringstream const* int_val_style()
        {
            return intValStyle_.get();
        }

        std::stringstream const* float_val_style()
        {
            return floatValStyle_.get();
        }

        std::stringstream const* char_val_style()
        {
            return charValStyle_.get();
        }

        std::stringstream const* string_val_style()
        {
            return stringValStyle_.get();
        }

        std::stringstream const* bool_val_style()
        {
            return boolValStyle_.get();
        }

        std::stringstream const* null_val_style()
        {
            return nullValStyle_.get();
        }
    };
}

#endif