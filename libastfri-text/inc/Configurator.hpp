#ifndef LIBASTFRI_TEXT_CONFIGURATOR
#define LIBASTFRI_TEXT_CONFIGURATOR

#include <memory>
#include <sstream>

class Configurator
{
    std::unique_ptr<std::stringstream> configFilePath_;
    std::unique_ptr<std::stringstream> defaultOutputFilePath_;

private:
    std::unique_ptr<std::stringstream> outputFileName_;
    std::unique_ptr<std::stringstream> outputFilePath_;
    std::unique_ptr<std::stringstream> outputFileFormat_;
    std::unique_ptr<std::stringstream> defaultStyle_;
    std::unique_ptr<std::stringstream> unknownWord_;
    std::unique_ptr<std::stringstream> unknownWordStyle_;
    std::unique_ptr<std::stringstream> invalidWord_;
    std::unique_ptr<std::stringstream> invalidWordStyle_;
    std::unique_ptr<std::stringstream> view_;
    std::unique_ptr<std::stringstream> publicWord_;
    std::unique_ptr<std::stringstream> privateWord_;
    std::unique_ptr<std::stringstream> protectedWord_;
    std::unique_ptr<std::stringstream> internalWord_;
    std::unique_ptr<std::stringstream> accAtribWord_;
    std::unique_ptr<std::stringstream> accConstrWord_;
    std::unique_ptr<std::stringstream> accDestrWord_;
    std::unique_ptr<std::stringstream> accMethWord_;
    std::unique_ptr<std::stringstream> accStyle_;
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
    std::unique_ptr<std::stringstream> classNameStyle_;
    std::unique_ptr<std::stringstream> interfaceNameStyle_;
    std::unique_ptr<std::stringstream> methodNameStyle_;
    std::unique_ptr<std::stringstream> functionNameStyle_;
    std::unique_ptr<std::stringstream> globalVarNameStyle_;
    std::unique_ptr<std::stringstream> memberVarNameStyle_;
    std::unique_ptr<std::stringstream> localVarNameStyle_;
    std::unique_ptr<std::stringstream> paramVarNameStyle_;
    std::unique_ptr<std::stringstream> assignOpWord_;
    std::unique_ptr<std::stringstream> moduloOpWord_;
    std::unique_ptr<std::stringstream> addressOpWord_;
    std::unique_ptr<std::stringstream> derefOpWord_;
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
    void set_input_path(std::string const& path);
    virtual void load_new_config_file();
    virtual void set_defaults();

public:
    std::stringstream const* get_output_file_name()
    {
        return outputFileName_.get();
    };

    std::stringstream const* get_output_file_path()
    {
        return outputFilePath_.get();
    };

    std::stringstream const* get_output_file_format()
    {
        return outputFileFormat_.get();
    };

    std::stringstream const* get_default_style()
    {
        return defaultStyle_.get();
    };

    std::stringstream const* get_unknown_word()
    {
        return unknownWord_.get();
    };

    std::stringstream const* get_unknown_word_style()
    {
        return unknownWordStyle_.get();
    };

    std::stringstream const* get_invalid_word()
    {
        return invalidWord_.get();
    };

    std::stringstream const* get_invalid_word_style()
    {
        return invalidWordStyle_.get();
    };

    std::stringstream const* get_view()
    {
        return view_.get();
    };

    std::stringstream const* get_public_word()
    {
        return publicWord_.get();
    };

    std::stringstream const* get_private_word()
    {
        return privateWord_.get();
    };

    std::stringstream const* get_protected_word()
    {
        return protectedWord_.get();
    };

    std::stringstream const* get_internal_word()
    {
        return internalWord_.get();
    };

    std::stringstream const* get_acc_atrib_word()
    {
        return accAtribWord_.get();
    };

    std::stringstream const* get_acc_constr_word()
    {
        return accConstrWord_.get();
    };

    std::stringstream const* get_acc_destr_word()
    {
        return accDestrWord_.get();
    };

    std::stringstream const* get_acc_meth_word()
    {
        return accMethWord_.get();
    };

    std::stringstream const* get_acc_style()
    {
        return accStyle_.get();
    };

    std::stringstream const* get_dynamic_word()
    {
        return dynamicWord_.get();
    };

    std::stringstream const* get_int_word()
    {
        return intWord_.get();
    };

    std::stringstream const* get_float_word()
    {
        return floatWord_.get();
    };

    std::stringstream const* get_char_word()
    {
        return charWord_.get();
    };

    std::stringstream const* get_bool_word()
    {
        return boolWord_.get();
    };

    std::stringstream const* get_void_word()
    {
        return voidWord_.get();
    };

    std::stringstream const* get_type_style()
    {
        return typeStyle_.get();
    };

    std::stringstream const* get_dynamic_type_style()
    {
        return dynamicTypeStyle_.get();
    };

    std::stringstream const* get_int_type_style()
    {
        return intTypeStyle_.get();
    };

    std::stringstream const* get_float_type_style()
    {
        return floatTypeStyle_.get();
    };

    std::stringstream const* get_char_type_style()
    {
        return charTypeStyle_.get();
    };

    std::stringstream const* get_bool_type_style()
    {
        return boolTypeStyle_.get();
    };

    std::stringstream const* get_void_type_style()
    {
        return voidTypeStyle_.get();
    };

    std::stringstream const* get_user_type_style()
    {
        return userTypeStyle_.get();
    };

    std::stringstream const* get_def_ref_name_style()
    {
        return defRefNameStyle_.get();
    };

    std::stringstream const* get_gener_param_name_style()
    {
        return generParamNameStyle_.get();
    };

    std::stringstream const* get_class_name_style()
    {
        return classNameStyle_.get();
    };

    std::stringstream const* get_interface_name_style()
    {
        return interfaceNameStyle_.get();
    };

    std::stringstream const* get_method_name_style()
    {
        return methodNameStyle_.get();
    };

    std::stringstream const* get_function_name_style()
    {
        return functionNameStyle_.get();
    };

    std::stringstream const* get_global_var_name_style()
    {
        return globalVarNameStyle_.get();
    };

    std::stringstream const* get_member_var_name_style()
    {
        return memberVarNameStyle_.get();
    };

    std::stringstream const* get_local_var_name_style()
    {
        return localVarNameStyle_.get();
    };

    std::stringstream const* get_param_var_name_style()
    {
        return paramVarNameStyle_.get();
    };

    std::stringstream const* get_assign_op_word()
    {
        return assignOpWord_.get();
    };

    std::stringstream const* get_modulo_op_word()
    {
        return moduloOpWord_.get();
    };

    std::stringstream const* get_address_op_word()
    {
        return addressOpWord_.get();
    };

    std::stringstream const* get_deref_op_word()
    {
        return derefOpWord_.get();
    };

    std::stringstream const* get_operator_style()
    {
        return operatorStyle_.get();
    };

    std::stringstream const* get_separator_style()
    {
        return separatorStyle_.get();
    };

    std::stringstream const* get_true_val()
    {
        return trueVal_.get();
    };

    std::stringstream const* get_false_val()
    {
        return falseVal_.get();
    };

    std::stringstream const* get_null_val()
    {
        return nullVal_.get();
    };

    std::stringstream const* get_val_style()
    {
        return valStyle_.get();
    };

    std::stringstream const* get_int_val_style()
    {
        return intValStyle_.get();
    };

    std::stringstream const* get_float_val_style()
    {
        return floatValStyle_.get();
    };

    std::stringstream const* get_char_val_style()
    {
        return charValStyle_.get();
    };

    std::stringstream const* get_string_val_style()
    {
        return stringValStyle_.get();
    };

    std::stringstream const* get_bool_val_style()
    {
        return boolValStyle_.get();
    };

    std::stringstream const* get_null_val_style()
    {
        return nullValStyle_.get();
    };
};

#endif