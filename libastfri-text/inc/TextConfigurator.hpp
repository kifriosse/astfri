#ifndef LIBASTFRI_TEXT_INC_TEXT_CONFIG
#define LIBASTFRI_TEXT_INC_TEXT_CONFIG

#include <libastfri-text/inc/Configurator.hpp>

struct TextConfigurator : Configurator {
private:
    std::unique_ptr<std::stringstream> outputFileFormat_;
    bool showRowNumber_;
    bool showOpeningBracketOnNewLine_;
    bool showClassDecl_;
    bool showClassBody_;
    bool showAttribute_;
    bool showMethodBody_;
    bool showMethodDecl_;
    bool showMethodDefin_;
    bool showMethodDefinInline_;
    bool showFunctionDecl_;
    bool showFunctionBody_;
    int lengthOfTabWord_;
    int lengthOfLeftMargin_;
    int sizeOfRowNumber_;
    std::unique_ptr<std::stringstream> classWord_;
    std::unique_ptr<std::stringstream> thisWord_;
    std::unique_ptr<std::stringstream> returnWord_;
    std::unique_ptr<std::stringstream> throwWord_;
    std::unique_ptr<std::stringstream> ifWord_;
    std::unique_ptr<std::stringstream> elseWord_;
    std::unique_ptr<std::stringstream> forWord_;
    std::unique_ptr<std::stringstream> whileWord_;
    std::unique_ptr<std::stringstream> doWord_;
    std::unique_ptr<std::stringstream> switchWord_;
    std::unique_ptr<std::stringstream> caseWord_;
public:
    TextConfigurator();
    void load_new_config_file() override;
private:
    void set_defaults() override;
public:
    const std::stringstream* get_output_file_format() { return outputFileFormat_.get(); };
    const bool& sh_row_number()         { return showRowNumber_; };
    const bool& sh_open_br_new_line()   { return showOpeningBracketOnNewLine_; };
    const bool& sh_class_decl()         { return showClassDecl_; };
    const bool& sh_class_body()         { return showClassBody_; };
    const bool& sh_attribute()          { return showAttribute_; };
    const bool& sh_method_body()        { return showMethodBody_; };
    const bool& sh_method_decl()        { return showMethodDecl_; };
    const bool& sh_method_defin()       { return showMethodDefin_; };
    const bool& sh_method_def_inline()  { return showMethodDefinInline_; };
    const bool& sh_function_decl()      { return showFunctionDecl_; };
    const bool& sh_function_body()      { return showFunctionBody_; };
    const int& get_len_tab_word()       { return lengthOfTabWord_; };
    const int& get_len_left_margin()    { return lengthOfLeftMargin_; };
    const int& get_row_num_size()       { return sizeOfRowNumber_; };
    const std::stringstream* get_class_word()   { return classWord_.get(); };
    const std::stringstream* get_this_word()    { return thisWord_.get(); };
    const std::stringstream* get_return_word()  { return returnWord_.get(); };
    const std::stringstream* get_throw_word()   { return throwWord_.get(); };
    const std::stringstream* get_if_word()      { return ifWord_.get(); };
    const std::stringstream* get_else_word()    { return elseWord_.get(); };
    const std::stringstream* get_for_word()     { return forWord_.get(); };
    const std::stringstream* get_while_word()   { return whileWord_.get(); };
    const std::stringstream* get_do_word()      { return doWord_.get(); };
    const std::stringstream* get_switch_word()  { return switchWord_.get(); };
    const std::stringstream* get_case_word()    { return caseWord_.get(); };
};

#endif