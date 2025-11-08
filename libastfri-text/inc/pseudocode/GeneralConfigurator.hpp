#ifndef LIBASTFRI_TEXT_GENERAL_CONFIGURATOR
#define LIBASTFRI_TEXT_GENERAL_CONFIGURATOR

#include <libastfri-text/inc/OutputFileManager.hpp>

#include <rapidjson/document.h>

#include <sstream>

namespace rj = rapidjson;

using STRING = char const*;
using ss = std::stringstream;

template<typename T>
using uptr = std::unique_ptr<T>;

namespace astfri::text
{
    class GeneralConfigurator
    {
        // FILE_SETTINGS
        bool overwriteFile_;
        uptr<ss> outputFileName_;
        uptr<ss> outputFilePath_;
        uptr<ss> outputFileFormat_;
        // UNKNOWN_PHRASES
        uptr<ss> unknownTypeWord_;
        uptr<ss> unknownExprWord_;
        uptr<ss> unknownStmtWord_;
        uptr<ss> invalidTypeWord_;
        uptr<ss> invalidExprWord_;
        uptr<ss> invalidStmtWord_;
        // STYLE
        uptr<ss> defaultPhraseStyle_;
        uptr<ss> unknownTypeWordStyle_;
        uptr<ss> unknownExprWordStyle_;
        uptr<ss> unknownStmtWordStyle_;
        uptr<ss> invalidTypeWordStyle_;
        uptr<ss> invalidExprWordStyle_;
        uptr<ss> invalidStmtWordStyle_;
        // ACCESS_MODIFIERS
        bool useInnerView_;
        uptr<ss> publicWord_;
        uptr<ss> protectedWord_;
        uptr<ss> privateWord_;
        uptr<ss> internalWord_;
        uptr<ss> attributesWord_;
        uptr<ss> constructorsWord_;
        uptr<ss> destructorsWord_;
        uptr<ss> methodsWord_;
        uptr<ss> accessModifierStyle_;
        // DATA_TYPES
        uptr<ss> dynamicTypeWord_;
        uptr<ss> intTypeWord_;
        uptr<ss> floatTypeWord_;
        uptr<ss> charTypeWord_;
        uptr<ss> boolTypeWord_;
        uptr<ss> voidTypeWord_;
        // STYLE
        uptr<ss> defaultTypeWordStyle_;
        uptr<ss> dynamicTypeWordStyle_;
        uptr<ss> intTypeWordStyle_;
        uptr<ss> floatTypeWordStyle_;
        uptr<ss> charTypeWordStyle_;
        uptr<ss> boolTypeWordStyle_;
        uptr<ss> voidTypeWordStyle_;
        uptr<ss> userTypeStyle_;
        // REFERENCE_NAMES
        uptr<ss> defaultRefNameStyle_;
        uptr<ss> genericParamNameStyle_;
        uptr<ss> classNameStyle_;
        uptr<ss> interfaceNameStyle_;
        uptr<ss> methodNameStyle_;
        uptr<ss> functionNameStyle_;
        uptr<ss> defaultVarNameStyle_;
        uptr<ss> globalVarNameStyle_;
        uptr<ss> memberVarNameStyle_;
        uptr<ss> localVarNameStyle_;
        uptr<ss> paramVarNameStyle_;
        // OPERATORS
        uptr<ss> assignOpWord_;
        uptr<ss> moduloOpWord_;
        uptr<ss> addressOpWord_;
        uptr<ss> derefOpWord_;
        uptr<ss> operatorStyle_;
        // SEPARATORS
        uptr<ss> separatorStyle_;
        // VALUES
        uptr<ss> trueValWord_;
        uptr<ss> falseValWord_;
        uptr<ss> nullValWord_;
        // STYLE
        uptr<ss> defaultValStyle_;
        uptr<ss> intValStyle_;
        uptr<ss> floatValStyle_;
        uptr<ss> charValStyle_;
        uptr<ss> stringValStyle_;
        uptr<ss> boolValStyle_;
        uptr<ss> nullValStyle_;
        // -----
        OutputFileManager* fmanager_;
    protected:
        explicit GeneralConfigurator();
        virtual ~GeneralConfigurator() = default;
    protected:
        virtual void set_defaults();
        virtual void process_document(rj::Value const*& doc);
        // -----
        bool is_string(STRING name, rj::Value const*& rjval, std::string& getString, bool checkStrLen);
        bool is_object(STRING name, rj::Value const*& rjval, rj::Value const*& getObject);
        bool is_array(STRING name, rj::Value const*& rjval, rj::Value const*& getArray);
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

        ss const* output_file_name()
        {
            return outputFileName_.get();
        }

        ss const* output_file_path()
        {
            return outputFilePath_.get();
        }

        ss const* output_file_format()
        {
            return outputFileFormat_.get();
        }

        // UNKNOWN_PHRASES
        ss const* unknown_type_word()
        {
            return unknownTypeWord_.get();
        }

        ss const* unknown_expr_word()
        {
            return unknownExprWord_.get();
        }

        ss const* unknown_stmt_word()
        {
            return unknownStmtWord_.get();
        }

        ss const* invalid_type_word()
        {
            return invalidTypeWord_.get();
        }

        ss const* invalid_expr_word()
        {
            return invalidExprWord_.get();
        }

        ss const* invalid_stmt_word()
        {
            return invalidStmtWord_.get();
        }

        // STYLE
        ss const* default_NA_phrase_style()
        {
            return defaultPhraseStyle_.get();
        }

        ss const* unknown_type_word_style()
        {
            return unknownTypeWordStyle_.get();
        }

        ss const* unknown_expr_word_style()
        {
            return unknownExprWordStyle_.get();
        }

        ss const* unknown_stmt_word_style()
        {
            return unknownStmtWordStyle_.get();
        }

        ss const* invalid_type_word_style()
        {
            return invalidTypeWordStyle_.get();
        }

        ss const* invalid_expr_word_style()
        {
            return invalidExprWordStyle_.get();
        }

        ss const* invalid_stmt_word_style()
        {
            return invalidStmtWordStyle_.get();
        }

        // ACCESS_MODIFIERS
        bool const& use_inner_view()
        {
            return useInnerView_;
        }

        ss const* public_word()
        {
            return publicWord_.get();
        }

        ss const* protected_word()
        {
            return protectedWord_.get();
        }

        ss const* private_word()
        {
            return privateWord_.get();
        }

        ss const* internal_word()
        {
            return internalWord_.get();
        }

        ss const* attributes_word()
        {
            return attributesWord_.get();
        }

        ss const* constructors_word()
        {
            return constructorsWord_.get();
        }

        ss const* destructors_word()
        {
            return destructorsWord_.get();
        }

        ss const* methods_word()
        {
            return methodsWord_.get();
        }

        ss const* access_mod_style()
        {
            return accessModifierStyle_.get();
        }

        // DATA_TYPES
        ss const* dynamic_type_word()
        {
            return dynamicTypeWord_.get();
        }

        ss const* int_type_word()
        {
            return intTypeWord_.get();
        }

        ss const* float_type_word()
        {
            return floatTypeWord_.get();
        }

        ss const* char_type_word()
        {
            return charTypeWord_.get();
        }

        ss const* bool_type_word()
        {
            return boolTypeWord_.get();
        }

        ss const* void_type_word()
        {
            return voidTypeWord_.get();
        }

        // STYLE
        ss const* default_type_word_style()
        {
            return defaultTypeWordStyle_.get();
        }

        ss const* dynamic_type_word_style()
        {
            return dynamicTypeWordStyle_.get();
        }

        ss const* int_type_word_style()
        {
            return intTypeWordStyle_.get();
        }

        ss const* float_type_word_style()
        {
            return floatTypeWordStyle_.get();
        }

        ss const* char_type_word_style()
        {
            return charTypeWordStyle_.get();
        }

        ss const* bool_type_word_style()
        {
            return boolTypeWordStyle_.get();
        }

        ss const* void_type_word_style()
        {
            return voidTypeWordStyle_.get();
        }

        ss const* user_type_style()
        {
            return userTypeStyle_.get();
        }

        // REFERENCE_NAMES
        ss const* default_ref_name_style()
        {
            return defaultRefNameStyle_.get();
        }

        ss const* gen_param_name_style()
        {
            return genericParamNameStyle_.get();
        }

        ss const* class_name_style()
        {
            return classNameStyle_.get();
        }

        ss const* interface_name_style()
        {
            return interfaceNameStyle_.get();
        }

        ss const* method_name_style()
        {
            return methodNameStyle_.get();
        }

        ss const* function_name_style()
        {
            return functionNameStyle_.get();
        }

        ss const* default_var_name_style()
        {
            return defaultVarNameStyle_.get();
        }

        ss const* global_var_name_style()
        {
            return globalVarNameStyle_.get();
        }

        ss const* member_var_name_style()
        {
            return memberVarNameStyle_.get();
        }

        ss const* local_var_name_style()
        {
            return localVarNameStyle_.get();
        }

        ss const* param_var_name_style()
        {
            return paramVarNameStyle_.get();
        }

        // OPERATORS
        ss const* assign_op_word()
        {
            return assignOpWord_.get();
        }

        ss const* modulo_op_word()
        {
            return moduloOpWord_.get();
        }

        ss const* address_op_word()
        {
            return addressOpWord_.get();
        }

        ss const* deref_op_word()
        {
            return derefOpWord_.get();
        }

        ss const* operator_style()
        {
            return operatorStyle_.get();
        }

        // SEPARATORS
        ss const* separator_style()
        {
            return separatorStyle_.get();
        }

        // VALUES
        ss const* true_val_word()
        {
            return trueValWord_.get();
        }

        ss const* false_val_word()
        {
            return falseValWord_.get();
        }

        ss const* null_val_word()
        {
            return nullValWord_.get();
        }

        // STYLE
        ss const* default_val_style()
        {
            return defaultValStyle_.get();
        }

        ss const* int_val_style()
        {
            return intValStyle_.get();
        }

        ss const* float_val_style()
        {
            return floatValStyle_.get();
        }

        ss const* char_val_style()
        {
            return charValStyle_.get();
        }

        ss const* string_val_style()
        {
            return stringValStyle_.get();
        }

        ss const* bool_val_style()
        {
            return boolValStyle_.get();
        }

        ss const* null_val_style()
        {
            return nullValStyle_.get();
        }
    };
}

#endif