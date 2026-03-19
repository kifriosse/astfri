#ifndef LIBASTFRI_TEXT_GENERAL_CONFIGURATOR
#define LIBASTFRI_TEXT_GENERAL_CONFIGURATOR

#include <libastfri-text/inc/OutputFileManager.hpp>

#include <rapidjson/document.h>

#include <sstream>

namespace rj = rapidjson;
using STRING = const char*;

namespace astfri::text {
class GeneralConfigurator {
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
    virtual void process_document(const rj::Value*& doc);
    // -----
    bool is_string(STRING name, const rj::Value*& rjval, std::string& getString, bool checkStrLen);
    bool is_object(STRING name, const rj::Value*& rjval, const rj::Value*& getObject);
    bool is_array(STRING name, const rj::Value*& rjval, const rj::Value*& getArray);
    bool is_bool(STRING name, const rj::Value*& rjval, bool& getBool);
    bool is_int(STRING name, const rj::Value*& rjval, int& getInt);

private:
    void process_file_settings(const rj::Value*& settings);
    void process_unknown_phrases(const rj::Value*& phrases);
    void process_access_modifiers(const rj::Value*& modifiers);
    void process_data_types(const rj::Value*& types);
    void process_reference_names(const rj::Value*& names);
    void process_ops_and_seps(const rj::Value*& signs);
    void process_values(const rj::Value*& values);

public:
    // FILE_SETTINGS
    const bool& overwrite_file() {
        return overwriteFile_;
    }

    const std::stringstream* output_file_name() {
        return outputFileName_.get();
    }

    const std::stringstream* output_file_path() {
        return outputFilePath_.get();
    }

    const std::stringstream* output_file_format() {
        return outputFileFormat_.get();
    }

    // UNKNOWN_PHRASES
    const std::stringstream* unknown_type_word() {
        return unknownTypeWord_.get();
    }

    const std::stringstream* unknown_expr_word() {
        return unknownExprWord_.get();
    }

    const std::stringstream* unknown_stmt_word() {
        return unknownStmtWord_.get();
    }

    const std::stringstream* invalid_type_word() {
        return invalidTypeWord_.get();
    }

    const std::stringstream* invalid_expr_word() {
        return invalidExprWord_.get();
    }

    const std::stringstream* invalid_stmt_word() {
        return invalidStmtWord_.get();
    }

    // STYLE
    const std::stringstream* default_NA_phrase_style() {
        return defaultPhraseStyle_.get();
    }

    const std::stringstream* unknown_type_word_style() {
        return unknownTypeWordStyle_.get();
    }

    const std::stringstream* unknown_expr_word_style() {
        return unknownExprWordStyle_.get();
    }

    const std::stringstream* unknown_stmt_word_style() {
        return unknownStmtWordStyle_.get();
    }

    const std::stringstream* invalid_type_word_style() {
        return invalidTypeWordStyle_.get();
    }

    const std::stringstream* invalid_expr_word_style() {
        return invalidExprWordStyle_.get();
    }

    const std::stringstream* invalid_stmt_word_style() {
        return invalidStmtWordStyle_.get();
    }

    // ACCESS_MODIFIERS
    const bool& use_inner_view() {
        return useInnerView_;
    }

    const std::stringstream* public_word() {
        return publicWord_.get();
    }

    const std::stringstream* protected_word() {
        return protectedWord_.get();
    }

    const std::stringstream* private_word() {
        return privateWord_.get();
    }

    const std::stringstream* internal_word() {
        return internalWord_.get();
    }

    const std::stringstream* attributes_word() {
        return attributesWord_.get();
    }

    const std::stringstream* constructors_word() {
        return constructorsWord_.get();
    }

    const std::stringstream* destructors_word() {
        return destructorsWord_.get();
    }

    const std::stringstream* methods_word() {
        return methodsWord_.get();
    }

    const std::stringstream* access_mod_style() {
        return accessModifierStyle_.get();
    }

    // DATA_TYPES
    const std::stringstream* dynamic_type_word() {
        return dynamicTypeWord_.get();
    }

    const std::stringstream* int_type_word() {
        return intTypeWord_.get();
    }

    const std::stringstream* float_type_word() {
        return floatTypeWord_.get();
    }

    const std::stringstream* char_type_word() {
        return charTypeWord_.get();
    }

    const std::stringstream* bool_type_word() {
        return boolTypeWord_.get();
    }

    const std::stringstream* void_type_word() {
        return voidTypeWord_.get();
    }

    // STYLE
    const std::stringstream* default_type_word_style() {
        return defaultTypeWordStyle_.get();
    }

    const std::stringstream* dynamic_type_word_style() {
        return dynamicTypeWordStyle_.get();
    }

    const std::stringstream* int_type_word_style() {
        return intTypeWordStyle_.get();
    }

    const std::stringstream* float_type_word_style() {
        return floatTypeWordStyle_.get();
    }

    const std::stringstream* char_type_word_style() {
        return charTypeWordStyle_.get();
    }

    const std::stringstream* bool_type_word_style() {
        return boolTypeWordStyle_.get();
    }

    const std::stringstream* void_type_word_style() {
        return voidTypeWordStyle_.get();
    }

    const std::stringstream* user_type_style() {
        return userTypeStyle_.get();
    }

    // REFERENCE_NAMES
    const std::stringstream* default_ref_name_style() {
        return defaultRefNameStyle_.get();
    }

    const std::stringstream* gen_param_name_style() {
        return genericParamNameStyle_.get();
    }

    const std::stringstream* class_name_style() {
        return classNameStyle_.get();
    }

    const std::stringstream* interface_name_style() {
        return interfaceNameStyle_.get();
    }

    const std::stringstream* method_name_style() {
        return methodNameStyle_.get();
    }

    const std::stringstream* function_name_style() {
        return functionNameStyle_.get();
    }

    const std::stringstream* default_var_name_style() {
        return defaultVarNameStyle_.get();
    }

    const std::stringstream* global_var_name_style() {
        return globalVarNameStyle_.get();
    }

    const std::stringstream* member_var_name_style() {
        return memberVarNameStyle_.get();
    }

    const std::stringstream* local_var_name_style() {
        return localVarNameStyle_.get();
    }

    const std::stringstream* param_var_name_style() {
        return paramVarNameStyle_.get();
    }

    // OPERATORS
    const std::stringstream* assign_op_word() {
        return assignOpWord_.get();
    }

    const std::stringstream* modulo_op_word() {
        return moduloOpWord_.get();
    }

    const std::stringstream* address_op_word() {
        return addressOpWord_.get();
    }

    const std::stringstream* deref_op_word() {
        return derefOpWord_.get();
    }

    const std::stringstream* operator_style() {
        return operatorStyle_.get();
    }

    // SEPARATORS
    const std::stringstream* separator_style() {
        return separatorStyle_.get();
    }

    // VALUES
    const std::stringstream* true_val_word() {
        return trueValWord_.get();
    }

    const std::stringstream* false_val_word() {
        return falseValWord_.get();
    }

    const std::stringstream* null_val_word() {
        return nullValWord_.get();
    }

    // STYLE
    const std::stringstream* default_val_style() {
        return defaultValStyle_.get();
    }

    const std::stringstream* int_val_style() {
        return intValStyle_.get();
    }

    const std::stringstream* float_val_style() {
        return floatValStyle_.get();
    }

    const std::stringstream* char_val_style() {
        return charValStyle_.get();
    }

    const std::stringstream* string_val_style() {
        return stringValStyle_.get();
    }

    const std::stringstream* bool_val_style() {
        return boolValStyle_.get();
    }

    const std::stringstream* null_val_style() {
        return nullValStyle_.get();
    }
};
} // namespace astfri::text

#endif