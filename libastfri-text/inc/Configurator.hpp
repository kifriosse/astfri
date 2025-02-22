#ifndef LIBASTFRI_TEXT_INC_CONFIGURATOR
#define LIBASTFRI_TEXT_INC_CONFIGURATOR

#include <sstream>
#include <memory>

struct Configurator {
private:
    std::unique_ptr<std::stringstream> configFilePath_;
    std::unique_ptr<std::stringstream> outputFileName_;
    std::unique_ptr<std::stringstream> defFolderPath_;
    std::unique_ptr<std::stringstream> outFolderPath_;
private:
    std::unique_ptr<std::stringstream> defaultOutputFilePath_;
    std::unique_ptr<std::stringstream> outputFilePath_;
    std::unique_ptr<std::stringstream> view_;
    std::unique_ptr<std::stringstream> intWord_;
    std::unique_ptr<std::stringstream> floatWord_;
    std::unique_ptr<std::stringstream> charWord_;
    std::unique_ptr<std::stringstream> boolWord_;
    std::unique_ptr<std::stringstream> voidWord_;
    std::unique_ptr<std::stringstream> assignWord_;
    std::unique_ptr<std::stringstream> publicWord_;
    std::unique_ptr<std::stringstream> privateWord_;
    std::unique_ptr<std::stringstream> protectedWord_;
    std::unique_ptr<std::stringstream> font_;
    std::unique_ptr<std::stringstream> classWordColor_;
    std::unique_ptr<std::stringstream> methodNameColor_;
    std::unique_ptr<std::stringstream> classNameColor_;
    std::unique_ptr<std::stringstream> dataTypeColor_;
    std::unique_ptr<std::stringstream> accessModColor_;
protected:
    Configurator();
    void set_input_path(std::string path);
    void reset_def_path();
    void reset_out_path();
    virtual void load_new_config_file();
    virtual void set_defaults();
public:
    const std::stringstream* get_default_output_path()  { return defaultOutputFilePath_.get(); };
    const std::stringstream* get_output_file_path()     { return outputFilePath_.get(); };
    const std::stringstream* get_view()                 { return view_.get(); };
    const std::stringstream* get_int_word()             { return intWord_.get(); };
    const std::stringstream* get_float_word()           { return floatWord_.get(); };
    const std::stringstream* get_char_word()            { return charWord_.get(); };
    const std::stringstream* get_bool_word()            { return boolWord_.get(); };
    const std::stringstream* get_void_word()            { return voidWord_.get(); };
    const std::stringstream* get_assign_word()          { return assignWord_.get(); };
    const std::stringstream* get_public_word()          { return publicWord_.get(); };
    const std::stringstream* get_private_word()         { return privateWord_.get(); };
    const std::stringstream* get_protected_word()       { return protectedWord_.get(); };
    const std::stringstream* get_font()                 { return font_.get(); };
    const std::stringstream* get_class_word_color()     { return classWordColor_.get(); };
    const std::stringstream* get_method_name_color()    { return methodNameColor_.get(); };
    const std::stringstream* get_class_name_color()     { return classNameColor_.get(); };
    const std::stringstream* get_data_type_color()      { return dataTypeColor_.get(); };
    const std::stringstream* get_access_mod_color()     { return accessModColor_.get(); };
};

#endif