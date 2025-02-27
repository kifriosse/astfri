#ifndef LIBASTFRI_TEXT_INC_TEXT_CONFIG
#define LIBASTFRI_TEXT_INC_TEXT_CONFIG

#include <libastfri-text/inc/Configurator.hpp>

struct TextConfigurator : Configurator {
private:
    bool showRowNumber_;
    bool showOpeningBracketOnNewLine_;
    bool showOtherExprs_;
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
    std::unique_ptr<std::stringstream> rowNumberStyle_;
    std::unique_ptr<std::stringstream> classWord_;
    std::unique_ptr<std::stringstream> thisWord_;
    std::unique_ptr<std::stringstream> returnWord_;
    std::unique_ptr<std::stringstream> throwWord_;
    std::unique_ptr<std::stringstream> ifWord_;
    std::unique_ptr<std::stringstream> elseWord_;
    std::unique_ptr<std::stringstream> doWord_;
    std::unique_ptr<std::stringstream> whileWord_;
    std::unique_ptr<std::stringstream> forWord_;
    std::unique_ptr<std::stringstream> repeatWord_;
    std::unique_ptr<std::stringstream> switchWord_;
    std::unique_ptr<std::stringstream> caseWord_;
    std::unique_ptr<std::stringstream> systExprStyle_;
    std::unique_ptr<std::stringstream> classWordStyle_;
    std::unique_ptr<std::stringstream> thisWordStyle_;
    std::unique_ptr<std::stringstream> returnWordStyle_;
    std::unique_ptr<std::stringstream> throwWordStyle_;
    std::unique_ptr<std::stringstream> ifWordStyle_;
    std::unique_ptr<std::stringstream> elseWordStyle_;
    std::unique_ptr<std::stringstream> doWordStyle_;
    std::unique_ptr<std::stringstream> whileWordStyle_;
    std::unique_ptr<std::stringstream> forWordStyle_;
    std::unique_ptr<std::stringstream> repeatWordStyle_;
    std::unique_ptr<std::stringstream> switchWordStyle_;
    std::unique_ptr<std::stringstream> caseWordStyle_;
    std::unique_ptr<std::stringstream> methodWord_;
    std::unique_ptr<std::stringstream> functionWord_;
    std::unique_ptr<std::stringstream> lambdaWord_;
    std::unique_ptr<std::stringstream> callWord_;
    std::unique_ptr<std::stringstream> defineWord_;
    std::unique_ptr<std::stringstream> returnsWord_;
    std::unique_ptr<std::stringstream> otherExprStyle_;
    std::unique_ptr<std::stringstream> methodWordStyle_;
    std::unique_ptr<std::stringstream> functionWordStyle_;
    std::unique_ptr<std::stringstream> lambdaWordStyle_;
    std::unique_ptr<std::stringstream> callWordStyle_;
    std::unique_ptr<std::stringstream> defineWordStyle_;
    std::unique_ptr<std::stringstream> returnsWordStyle_;
public:
    TextConfigurator();
    void load_new_config_file() override;
private:
    void set_defaults() override;
public:
    const bool& sh_row_number()         { return showRowNumber_; };
    const bool& sh_open_br_new_line()   { return showOpeningBracketOnNewLine_; };
    const bool& sh_other_expressions()  { return showOtherExprs_; };
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
    const std::stringstream* get_row_num_style()  { return rowNumberStyle_.get(); };
    const std::stringstream* get_class_word()     { return classWord_.get(); };
    const std::stringstream* get_this_word()      { return thisWord_.get(); };
    const std::stringstream* get_return_word()    { return returnWord_.get(); };
    const std::stringstream* get_throw_word()     { return throwWord_.get(); };
    const std::stringstream* get_if_word()        { return ifWord_.get(); };
    const std::stringstream* get_else_word()      { return elseWord_.get(); };
    const std::stringstream* get_do_word()        { return doWord_.get(); };
    const std::stringstream* get_while_word()     { return whileWord_.get(); };
    const std::stringstream* get_for_word()       { return forWord_.get(); };
    const std::stringstream* get_repeat_word()    { return repeatWord_.get(); };
    const std::stringstream* get_switch_word()    { return switchWord_.get(); };
    const std::stringstream* get_case_word()      { return caseWord_.get(); };
    const std::stringstream* get_sys_ex_style()         { return systExprStyle_.get(); };
    const std::stringstream* get_class_word_style()     { return classWordStyle_.get(); };
    const std::stringstream* get_this_word_style()      { return thisWordStyle_.get(); };
    const std::stringstream* get_return_word_style()    { return returnWordStyle_.get(); };
    const std::stringstream* get_throw_word_style()     { return throwWordStyle_.get(); };
    const std::stringstream* get_if_word_style()        { return ifWordStyle_.get(); };
    const std::stringstream* get_else_word_style()      { return elseWordStyle_.get(); };
    const std::stringstream* get_do_word_style()        { return doWordStyle_.get(); };
    const std::stringstream* get_while_word_style()     { return whileWordStyle_.get(); };
    const std::stringstream* get_for_word_style()       { return forWordStyle_.get(); };
    const std::stringstream* get_repeat_word_style()    { return repeatWordStyle_.get(); };
    const std::stringstream* get_switch_word_style()    { return switchWordStyle_.get(); };
    const std::stringstream* get_case_word_style()      { return caseWordStyle_.get(); };
    const std::stringstream* get_method_word()          { return methodWord_.get(); };
    const std::stringstream* get_function_word()        { return functionWord_.get(); };
    const std::stringstream* get_lambda_word()          { return lambdaWord_.get(); };
    const std::stringstream* get_call_word()            { return callWord_.get(); };
    const std::stringstream* get_define_word()          { return defineWord_.get(); };
    const std::stringstream* get_returns_word()         { return returnsWord_.get(); };
    const std::stringstream* get_other_expr_style()     { return otherExprStyle_.get(); };
    const std::stringstream* get_method_word_style()    { return methodWordStyle_.get(); };
    const std::stringstream* get_function_word_style()  { return functionWordStyle_.get(); };
    const std::stringstream* get_lambda_word_style()    { return lambdaWordStyle_.get(); };
    const std::stringstream* get_call_word_style()      { return callWordStyle_.get(); };
    const std::stringstream* get_define_word_style()    { return defineWordStyle_.get(); };
    const std::stringstream* get_returns_word_style()   { return returnsWord_.get(); };
};

#endif