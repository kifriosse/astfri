#ifndef LIBASTFRI_TEXT_TEXT_CONFIGURATOR
#define LIBASTFRI_TEXT_TEXT_CONFIGURATOR

#include <libastfri-text/inc/Configurator.hpp>

#include <vector>

class TextConfigurator : public Configurator
{
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
    bool const& use_cpp_br()
    {
        return useCppBr_;
    };

    bool const& use_br_col()
    {
        return useBrCol_;
    };

    bool const& sh_row()
    {
        return shRow_;
    };

    bool const& sh_other_expr()
    {
        return shOtherExpr_;
    };

    bool const& sh_class_dec()
    {
        return shClassDec_;
    };

    bool const& sh_class_def()
    {
        return shClassDef_;
    };

    bool const& sh_interf_dec()
    {
        return shInterfDec_;
    };

    bool const& sh_interf_def()
    {
        return shInterfDef_;
    };

    bool const& sh_gener_par()
    {
        return shGenerPar_;
    };

    bool const& sh_attrib()
    {
        return shAttrib_;
    };

    bool const& sh_meth_dec()
    {
        return shMethDec_;
    };

    bool const& sh_meth_def()
    {
        return shMethDef_;
    };

    bool const& sh_meth_owner()
    {
        return shMethOwner_;
    };

    bool const& sh_meth_templ()
    {
        return shMethTempl_;
    };

    bool const& sh_func_dec()
    {
        return shFuncDec_;
    };

    bool const& sh_func_def()
    {
        return shFuncDef_;
    };

    bool const& sh_glob_par()
    {
        return shGlobPar_;
    };

    int const& get_tab_len()
    {
        return tabLen_;
    };

    int const& get_marg_len()
    {
        return margLen_;
    };

    std::vector<std::stringstream> const* get_round_br_col()
    {
        return roundBrCol_.get();
    };

    std::vector<std::stringstream> const* get_curl_br_col()
    {
        return curlBrCol_.get();
    };

    std::stringstream const* get_row_style()
    {
        return rowStyle_.get();
    };

    std::stringstream const* get_class_word()
    {
        return classWord_.get();
    };

    std::stringstream const* get_interface_word()
    {
        return interfaceWord_.get();
    };

    std::stringstream const* get_implement_word()
    {
        return implementWord_.get();
    };

    std::stringstream const* get_extend_word()
    {
        return extendWord_.get();
    };

    std::stringstream const* get_this_word()
    {
        return thisWord_.get();
    };

    std::stringstream const* get_return_word()
    {
        return returnWord_.get();
    };

    std::stringstream const* get_continue_word()
    {
        return continueWord_.get();
    };

    std::stringstream const* get_break_word()
    {
        return breakWord_.get();
    };

    std::stringstream const* get_throw_word()
    {
        return throwWord_.get();
    };

    std::stringstream const* get_if_word()
    {
        return ifWord_.get();
    };

    std::stringstream const* get_else_word()
    {
        return elseWord_.get();
    };

    std::stringstream const* get_do_word()
    {
        return doWord_.get();
    };

    std::stringstream const* get_while_word()
    {
        return whileWord_.get();
    };

    std::stringstream const* get_for_word()
    {
        return forWord_.get();
    };

    std::stringstream const* get_repeat_word()
    {
        return repeatWord_.get();
    };

    std::stringstream const* get_switch_word()
    {
        return switchWord_.get();
    };

    std::stringstream const* get_case_word()
    {
        return caseWord_.get();
    };

    std::stringstream const* get_default_word()
    {
        return defaultWord_.get();
    };

    std::stringstream const* get_new_word()
    {
        return newWord_.get();
    };

    std::stringstream const* get_delete_word()
    {
        return deleteWord_.get();
    };

    std::stringstream const* get_pointer_word()
    {
        return pointerWord_.get();
    };

    std::stringstream const* get_virtual_word()
    {
        return virtualWord_.get();
    };

    std::stringstream const* get_abstract_word()
    {
        return abstractWord_.get();
    };

    std::stringstream const* get_template_word()
    {
        return templateWord_.get();
    };

    std::stringstream const* get_sys_ex_style()
    {
        return systExprStyle_.get();
    };

    std::stringstream const* get_class_word_style()
    {
        return classWordStyle_.get();
    };

    std::stringstream const* get_interface_word_style()
    {
        return interfaceWordStyle_.get();
    };

    std::stringstream const* get_implement_word_style()
    {
        return implementWordStyle_.get();
    };

    std::stringstream const* get_extend_word_style()
    {
        return extendWordStyle_.get();
    };

    std::stringstream const* get_this_word_style()
    {
        return thisWordStyle_.get();
    };

    std::stringstream const* get_return_word_style()
    {
        return returnWordStyle_.get();
    };

    std::stringstream const* get_continue_word_style()
    {
        return continueWordStyle_.get();
    };

    std::stringstream const* get_break_word_style()
    {
        return breakWordStyle_.get();
    };

    std::stringstream const* get_throw_word_style()
    {
        return throwWordStyle_.get();
    };

    std::stringstream const* get_if_word_style()
    {
        return ifWordStyle_.get();
    };

    std::stringstream const* get_else_word_style()
    {
        return elseWordStyle_.get();
    };

    std::stringstream const* get_do_word_style()
    {
        return doWordStyle_.get();
    };

    std::stringstream const* get_while_word_style()
    {
        return whileWordStyle_.get();
    };

    std::stringstream const* get_for_word_style()
    {
        return forWordStyle_.get();
    };

    std::stringstream const* get_repeat_word_style()
    {
        return repeatWordStyle_.get();
    };

    std::stringstream const* get_switch_word_style()
    {
        return switchWordStyle_.get();
    };

    std::stringstream const* get_case_word_style()
    {
        return caseWordStyle_.get();
    };

    std::stringstream const* get_default_word_style()
    {
        return defaultWordStyle_.get();
    };

    std::stringstream const* get_new_word_style()
    {
        return newWordStyle_.get();
    };

    std::stringstream const* get_delete_word_style()
    {
        return deleteWordStyle_.get();
    };

    std::stringstream const* get_pointer_word_style()
    {
        return pointerWordStyle_.get();
    };

    std::stringstream const* get_virtual_word_style()
    {
        return virtualWordStyle_.get();
    };

    std::stringstream const* get_abstract_word_style()
    {
        return abstractWordStyle_.get();
    };

    std::stringstream const* get_template_word_style()
    {
        return templateWordStyle_.get();
    };

    std::stringstream const* get_constr_word()
    {
        return constrWord_.get();
    };

    std::stringstream const* get_destr_word()
    {
        return destrWord_.get();
    };

    std::stringstream const* get_method_word()
    {
        return methodWord_.get();
    };

    std::stringstream const* get_function_word()
    {
        return functionWord_.get();
    };

    std::stringstream const* get_lambda_word()
    {
        return lambdaWord_.get();
    };

    std::stringstream const* get_call_word()
    {
        return callWord_.get();
    };

    std::stringstream const* get_define_word()
    {
        return defineWord_.get();
    };

    std::stringstream const* get_returns_word()
    {
        return returnsWord_.get();
    };

    std::stringstream const* get_other_expr_style()
    {
        return otherExprStyle_.get();
    };

    std::stringstream const* get_constr_style()
    {
        return constrStyle_.get();
    };

    std::stringstream const* get_destr_style()
    {
        return destrStyle_.get();
    };

    std::stringstream const* get_method_word_style()
    {
        return methodWordStyle_.get();
    };

    std::stringstream const* get_function_word_style()
    {
        return functionWordStyle_.get();
    };

    std::stringstream const* get_lambda_word_style()
    {
        return lambdaWordStyle_.get();
    };

    std::stringstream const* get_call_word_style()
    {
        return callWordStyle_.get();
    };

    std::stringstream const* get_define_word_style()
    {
        return defineWordStyle_.get();
    };

    std::stringstream const* get_returns_word_style()
    {
        return returnsWordStyle_.get();
    };
};

#endif