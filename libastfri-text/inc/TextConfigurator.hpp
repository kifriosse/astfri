#ifndef LIBASTFRI_TEXT_TEXT_CONFIGURATOR
#define LIBASTFRI_TEXT_TEXT_CONFIGURATOR

#include <libastfri-text/inc/Configurator.hpp>
#include <vector>

class TextConfigurator : public Configurator {
    bool useCppBr_;
    bool useBrCol_;
    bool shRow_;
    bool shOtherExpr_;
    bool shClassDec_;
    bool shClassDef_;
    bool shInterfDec_;
    bool shInterfDef_;
    bool shGenerPar_;
    bool shAttrib_;
    bool shMethDec_;
    bool shMethDef_;
    bool shMethOwner_;
    bool shMethTempl_;
    bool shFuncDec_;
    bool shFuncDef_;
    bool shGlobPar_;
    int tabLen_;
    int margLen_;
    std::unique_ptr<std::vector<std::stringstream>> roundBrCol_;
    std::unique_ptr<std::vector<std::stringstream>> curlBrCol_;
    std::unique_ptr<std::stringstream> rowStyle_;
    std::unique_ptr<std::stringstream> classWord_;
    std::unique_ptr<std::stringstream> interfaceWord_;
    std::unique_ptr<std::stringstream> implementWord_;
    std::unique_ptr<std::stringstream> extendWord_;
    std::unique_ptr<std::stringstream> thisWord_;
    std::unique_ptr<std::stringstream> returnWord_;
    std::unique_ptr<std::stringstream> continueWord_;
    std::unique_ptr<std::stringstream> breakWord_;
    std::unique_ptr<std::stringstream> throwWord_;
    std::unique_ptr<std::stringstream> ifWord_;
    std::unique_ptr<std::stringstream> elseWord_;
    std::unique_ptr<std::stringstream> doWord_;
    std::unique_ptr<std::stringstream> whileWord_;
    std::unique_ptr<std::stringstream> forWord_;
    std::unique_ptr<std::stringstream> repeatWord_;
    std::unique_ptr<std::stringstream> switchWord_;
    std::unique_ptr<std::stringstream> caseWord_;
    std::unique_ptr<std::stringstream> defaultWord_;
    std::unique_ptr<std::stringstream> newWord_;
    std::unique_ptr<std::stringstream> deleteWord_;
    std::unique_ptr<std::stringstream> pointerWord_;
    std::unique_ptr<std::stringstream> virtualWord_;
    std::unique_ptr<std::stringstream> abstractWord_;
    std::unique_ptr<std::stringstream> templateWord_;
    std::unique_ptr<std::stringstream> systExprStyle_;
    std::unique_ptr<std::stringstream> classWordStyle_;
    std::unique_ptr<std::stringstream> interfaceWordStyle_;
    std::unique_ptr<std::stringstream> implementWordStyle_;
    std::unique_ptr<std::stringstream> extendWordStyle_;
    std::unique_ptr<std::stringstream> thisWordStyle_;
    std::unique_ptr<std::stringstream> returnWordStyle_;
    std::unique_ptr<std::stringstream> continueWordStyle_;
    std::unique_ptr<std::stringstream> breakWordStyle_;
    std::unique_ptr<std::stringstream> throwWordStyle_;
    std::unique_ptr<std::stringstream> ifWordStyle_;
    std::unique_ptr<std::stringstream> elseWordStyle_;
    std::unique_ptr<std::stringstream> doWordStyle_;
    std::unique_ptr<std::stringstream> whileWordStyle_;
    std::unique_ptr<std::stringstream> forWordStyle_;
    std::unique_ptr<std::stringstream> repeatWordStyle_;
    std::unique_ptr<std::stringstream> switchWordStyle_;
    std::unique_ptr<std::stringstream> caseWordStyle_;
    std::unique_ptr<std::stringstream> defaultWordStyle_;
    std::unique_ptr<std::stringstream> newWordStyle_;
    std::unique_ptr<std::stringstream> deleteWordStyle_;
    std::unique_ptr<std::stringstream> pointerWordStyle_;
    std::unique_ptr<std::stringstream> virtualWordStyle_;
    std::unique_ptr<std::stringstream> abstractWordStyle_;
    std::unique_ptr<std::stringstream> templateWordStyle_;
    std::unique_ptr<std::stringstream> constrWord_;
    std::unique_ptr<std::stringstream> destrWord_;
    std::unique_ptr<std::stringstream> methodWord_;
    std::unique_ptr<std::stringstream> functionWord_;
    std::unique_ptr<std::stringstream> lambdaWord_;
    std::unique_ptr<std::stringstream> callWord_;
    std::unique_ptr<std::stringstream> defineWord_;
    std::unique_ptr<std::stringstream> returnsWord_;
    std::unique_ptr<std::stringstream> otherExprStyle_;
    std::unique_ptr<std::stringstream> constrStyle_;
    std::unique_ptr<std::stringstream> destrStyle_;
    std::unique_ptr<std::stringstream> methodWordStyle_;
    std::unique_ptr<std::stringstream> functionWordStyle_;
    std::unique_ptr<std::stringstream> lambdaWordStyle_;
    std::unique_ptr<std::stringstream> callWordStyle_;
    std::unique_ptr<std::stringstream> defineWordStyle_;
    std::unique_ptr<std::stringstream> returnsWordStyle_;
public:
    TextConfigurator();
    void reset();
private:
    void load_new_config_file() override;
    void set_defaults() override;
public:
    const bool& use_cpp_br()    { return useCppBr_; };
    const bool& use_br_col()    { return useBrCol_; };
    const bool& sh_row()        { return shRow_; };
    const bool& sh_other_expr() { return shOtherExpr_; };
    const bool& sh_class_dec()  { return shClassDec_; };
    const bool& sh_class_def()  { return shClassDef_; };
    const bool& sh_interf_dec() { return shInterfDec_; };
    const bool& sh_interf_def() { return shInterfDef_; };
    const bool& sh_gener_par()  { return shGenerPar_; };
    const bool& sh_attrib()     { return shAttrib_; };
    const bool& sh_meth_dec()   { return shMethDec_; };
    const bool& sh_meth_def()   { return shMethDef_; };
    const bool& sh_meth_owner() { return shMethOwner_; };
    const bool& sh_meth_templ() { return shMethTempl_; };
    const bool& sh_func_dec()   { return shFuncDec_; };
    const bool& sh_func_def()   { return shFuncDef_; };
    const bool& sh_glob_par()   { return shGlobPar_; };
    const int& get_tab_len()    { return tabLen_; };
    const int& get_marg_len()   { return margLen_; };
    const std::vector<std::stringstream>* get_round_br_col() { return roundBrCol_.get(); };
    const std::vector<std::stringstream>* get_curl_br_col()  { return curlBrCol_.get(); };
    const std::stringstream* get_row_style()      { return rowStyle_.get(); };
    const std::stringstream* get_class_word()     { return classWord_.get(); };
    const std::stringstream* get_interface_word() { return interfaceWord_.get(); };
    const std::stringstream* get_implement_word() { return implementWord_.get(); };
    const std::stringstream* get_extend_word()    { return extendWord_.get(); };
    const std::stringstream* get_this_word()      { return thisWord_.get(); };
    const std::stringstream* get_return_word()    { return returnWord_.get(); };
    const std::stringstream* get_continue_word()  { return continueWord_.get(); };
    const std::stringstream* get_break_word()     { return breakWord_.get(); };
    const std::stringstream* get_throw_word()     { return throwWord_.get(); };
    const std::stringstream* get_if_word()        { return ifWord_.get(); };
    const std::stringstream* get_else_word()      { return elseWord_.get(); };
    const std::stringstream* get_do_word()        { return doWord_.get(); };
    const std::stringstream* get_while_word()     { return whileWord_.get(); };
    const std::stringstream* get_for_word()       { return forWord_.get(); };
    const std::stringstream* get_repeat_word()    { return repeatWord_.get(); };
    const std::stringstream* get_switch_word()    { return switchWord_.get(); };
    const std::stringstream* get_case_word()      { return caseWord_.get(); };
    const std::stringstream* get_default_word()   { return defaultWord_.get(); };
    const std::stringstream* get_new_word()       { return newWord_.get(); };
    const std::stringstream* get_delete_word()    { return deleteWord_.get(); };
    const std::stringstream* get_pointer_word()   { return pointerWord_.get(); };
    const std::stringstream* get_virtual_word()   { return virtualWord_.get(); };
    const std::stringstream* get_abstract_word()  { return abstractWord_.get(); };
    const std::stringstream* get_template_word()  { return templateWord_.get(); };
    const std::stringstream* get_sys_ex_style()         { return systExprStyle_.get(); };
    const std::stringstream* get_class_word_style()     { return classWordStyle_.get(); };
    const std::stringstream* get_interface_word_style() { return interfaceWordStyle_.get(); };
    const std::stringstream* get_implement_word_style() { return implementWordStyle_.get(); };
    const std::stringstream* get_extend_word_style()    { return extendWordStyle_.get(); };
    const std::stringstream* get_this_word_style()      { return thisWordStyle_.get(); };
    const std::stringstream* get_return_word_style()    { return returnWordStyle_.get(); };
    const std::stringstream* get_continue_word_style()  { return continueWordStyle_.get(); };
    const std::stringstream* get_break_word_style()     { return breakWordStyle_.get(); };
    const std::stringstream* get_throw_word_style()     { return throwWordStyle_.get(); };
    const std::stringstream* get_if_word_style()        { return ifWordStyle_.get(); };
    const std::stringstream* get_else_word_style()      { return elseWordStyle_.get(); };
    const std::stringstream* get_do_word_style()        { return doWordStyle_.get(); };
    const std::stringstream* get_while_word_style()     { return whileWordStyle_.get(); };
    const std::stringstream* get_for_word_style()       { return forWordStyle_.get(); };
    const std::stringstream* get_repeat_word_style()    { return repeatWordStyle_.get(); };
    const std::stringstream* get_switch_word_style()    { return switchWordStyle_.get(); };
    const std::stringstream* get_case_word_style()      { return caseWordStyle_.get(); };
    const std::stringstream* get_default_word_style()   { return defaultWordStyle_.get(); };
    const std::stringstream* get_new_word_style()       { return newWordStyle_.get(); };
    const std::stringstream* get_delete_word_style()    { return deleteWordStyle_.get(); };
    const std::stringstream* get_pointer_word_style()   { return pointerWordStyle_.get(); };
    const std::stringstream* get_virtual_word_style()   { return virtualWordStyle_.get(); };
    const std::stringstream* get_abstract_word_style()  { return abstractWordStyle_.get(); };
    const std::stringstream* get_template_word_style()  { return templateWordStyle_.get(); };
    const std::stringstream* get_constr_word()          { return constrWord_.get(); };
    const std::stringstream* get_destr_word()           { return destrWord_.get(); };
    const std::stringstream* get_method_word()          { return methodWord_.get(); };
    const std::stringstream* get_function_word()        { return functionWord_.get(); };
    const std::stringstream* get_lambda_word()          { return lambdaWord_.get(); };
    const std::stringstream* get_call_word()            { return callWord_.get(); };
    const std::stringstream* get_define_word()          { return defineWord_.get(); };
    const std::stringstream* get_returns_word()         { return returnsWord_.get(); };
    const std::stringstream* get_other_expr_style()     { return otherExprStyle_.get(); };
    const std::stringstream* get_constr_style()         { return constrStyle_.get(); };
    const std::stringstream* get_destr_style()          { return destrStyle_.get(); };
    const std::stringstream* get_method_word_style()    { return methodWordStyle_.get(); };
    const std::stringstream* get_function_word_style()  { return functionWordStyle_.get(); };
    const std::stringstream* get_lambda_word_style()    { return lambdaWordStyle_.get(); };
    const std::stringstream* get_call_word_style()      { return callWordStyle_.get(); };
    const std::stringstream* get_define_word_style()    { return defineWordStyle_.get(); };
    const std::stringstream* get_returns_word_style()   { return returnsWordStyle_.get(); };
};

#endif