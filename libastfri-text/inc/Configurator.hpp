#ifndef LIBASTFRI_TEXT_INC_CONFIGURATOR
#define LIBASTFRI_TEXT_INC_CONFIGURATOR

#include <sstream>
#include <memory>

struct Configurator {
private:
    std::unique_ptr<std::stringstream> configFilePath_;
    std::unique_ptr<std::stringstream> defFolderPath_;
    std::unique_ptr<std::stringstream> outFolderPath_;
    std::unique_ptr<std::stringstream> defaultOutputFilePath_;
private:
    std::unique_ptr<std::stringstream> outputFileName_;
    std::unique_ptr<std::stringstream> outputFilePath_;
    std::unique_ptr<std::stringstream> outputFileFormat_;
    std::unique_ptr<std::stringstream> defaultStyle_;
    std::unique_ptr<std::stringstream> unknownWord_;
    std::unique_ptr<std::stringstream> unknownWordStyle_;
    std::unique_ptr<std::stringstream> view_;
    std::unique_ptr<std::stringstream> publicWord_;
    std::unique_ptr<std::stringstream> privateWord_;
    std::unique_ptr<std::stringstream> protectedWord_;
    std::unique_ptr<std::stringstream> accessModStyle_;
    std::unique_ptr<std::stringstream> dynamicWord_;
    std::unique_ptr<std::stringstream> intWord_;
    std::unique_ptr<std::stringstream> floatWord_;
    std::unique_ptr<std::stringstream> charWord_;
    std::unique_ptr<std::stringstream> boolWord_;
    std::unique_ptr<std::stringstream> voidWord_;
    std::unique_ptr<std::stringstream> typeStyle_;
    std::unique_ptr<std::stringstream> dynamicTypeStyle_;
    std::unique_ptr<std::stringstream> intTypeStyle_;
    std::unique_ptr<std::stringstream> floatTypeStyle_;
    std::unique_ptr<std::stringstream> charTypeStyle_;
    std::unique_ptr<std::stringstream> boolTypeStyle_;
    std::unique_ptr<std::stringstream> voidTypeStyle_;
    std::unique_ptr<std::stringstream> userTypeStyle_;
    std::unique_ptr<std::stringstream> defRefNameStyle_;
    std::unique_ptr<std::stringstream> generParamNameStyle_;
    std::unique_ptr<std::stringstream> generParamConstrStyle_;
    std::unique_ptr<std::stringstream> classNameStyle_;
    std::unique_ptr<std::stringstream> methodNameStyle_;
    std::unique_ptr<std::stringstream> functionNameStyle_;
    std::unique_ptr<std::stringstream> globalVarNameStyle_;
    std::unique_ptr<std::stringstream> memberVarNameStyle_;
    std::unique_ptr<std::stringstream> localVarNameStyle_;
    std::unique_ptr<std::stringstream> paramVarNameStyle_;
    std::unique_ptr<std::stringstream> assignOpWord_;
    std::unique_ptr<std::stringstream> operatorStyle_;
    std::unique_ptr<std::stringstream> separatorStyle_;
    std::unique_ptr<std::stringstream> trueVal_;
    std::unique_ptr<std::stringstream> falseVal_;
    std::unique_ptr<std::stringstream> nullVal_;
    std::unique_ptr<std::stringstream> valStyle_;
    std::unique_ptr<std::stringstream> intValStyle_;
    std::unique_ptr<std::stringstream> floatValStyle_;
    std::unique_ptr<std::stringstream> charValStyle_;
    std::unique_ptr<std::stringstream> stringValStyle_;
    std::unique_ptr<std::stringstream> boolValStyle_;
    std::unique_ptr<std::stringstream> nullValStyle_;
protected:
    Configurator();
    void set_input_path(std::string path);
    void reset_def_path();
    void reset_out_path();
    virtual void load_new_config_file();
    virtual void set_defaults();
public:
    const std::stringstream* get_output_file_name()     { return outputFileName_.get(); };
    const std::stringstream* get_default_output_path()  { return defaultOutputFilePath_.get(); };
    const std::stringstream* get_output_file_path()     { return outputFilePath_.get(); };
    const std::stringstream* get_output_file_format()   { return outputFileFormat_.get(); };
    const std::stringstream* get_default_style()        { return defaultStyle_.get(); };
    const std::stringstream* get_unknown_word()         { return unknownWord_.get(); };
    const std::stringstream* get_unknown_word_style()   { return unknownWordStyle_.get(); };
    const std::stringstream* get_view()                 { return view_.get(); };
    const std::stringstream* get_public_word()          { return publicWord_.get(); };
    const std::stringstream* get_private_word()         { return privateWord_.get(); };
    const std::stringstream* get_protected_word()       { return protectedWord_.get(); };
    const std::stringstream* get_access_mod_style()     { return accessModStyle_.get(); };
    const std::stringstream* get_dynamic_word()  { return dynamicWord_.get(); };
    const std::stringstream* get_int_word()      { return intWord_.get(); };
    const std::stringstream* get_float_word()    { return floatWord_.get(); };
    const std::stringstream* get_char_word()     { return charWord_.get(); };
    const std::stringstream* get_bool_word()     { return boolWord_.get(); };
    const std::stringstream* get_void_word()     { return voidWord_.get(); };
    const std::stringstream* get_type_style()    { return typeStyle_.get(); };
    const std::stringstream* get_dynamic_type_style()  { return dynamicTypeStyle_.get(); };
    const std::stringstream* get_int_type_style()      { return intTypeStyle_.get(); };
    const std::stringstream* get_float_type_style()    { return floatTypeStyle_.get(); };
    const std::stringstream* get_char_type_style()     { return charTypeStyle_.get(); };
    const std::stringstream* get_bool_type_style()     { return boolTypeStyle_.get(); };
    const std::stringstream* get_void_type_style()     { return voidTypeStyle_.get(); };
    const std::stringstream* get_user_type_style()     { return userTypeStyle_.get(); };
    const std::stringstream* get_def_ref_name_style()  { return defRefNameStyle_.get(); };
    const std::stringstream* get_gener_param_name_style()    { return generParamNameStyle_.get(); };
    const std::stringstream* get_gener_param_constr_style()  { return generParamConstrStyle_.get(); };
    const std::stringstream* get_class_name_style()          { return classNameStyle_.get(); };
    const std::stringstream* get_method_name_style()         { return methodNameStyle_.get(); };
    const std::stringstream* get_function_name_style()       { return functionNameStyle_.get(); };
    const std::stringstream* get_global_var_name_style()     { return globalVarNameStyle_.get(); };
    const std::stringstream* get_member_var_name_style()     { return memberVarNameStyle_.get(); };
    const std::stringstream* get_local_var_name_style()      { return localVarNameStyle_.get(); };
    const std::stringstream* get_param_var_name_style()      { return paramVarNameStyle_.get(); };
    const std::stringstream* get_assign_op_word()    { return assignOpWord_.get(); };
    const std::stringstream* get_operator_style()    { return operatorStyle_.get(); };
    const std::stringstream* get_separator_style()   { return separatorStyle_.get(); };
    const std::stringstream* get_true_val()          { return trueVal_.get(); };
    const std::stringstream* get_false_val()         { return falseVal_.get(); };
    const std::stringstream* get_null_val()          { return nullVal_.get(); };
    const std::stringstream* get_val_style()         { return valStyle_.get(); };
    const std::stringstream* get_int_val_style()     { return intValStyle_.get(); };
    const std::stringstream* get_float_val_style()   { return floatValStyle_.get(); };
    const std::stringstream* get_char_val_style()    { return charValStyle_.get(); };
    const std::stringstream* get_string_val_style()  { return stringValStyle_.get(); };
    const std::stringstream* get_bool_val_style()    { return boolValStyle_.get(); };
    const std::stringstream* get_null_val_style()    { return nullValStyle_.get(); };
};

#endif