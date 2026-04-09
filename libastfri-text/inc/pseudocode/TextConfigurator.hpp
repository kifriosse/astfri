#ifndef LIBASTFRI_TEXT_TEXT_CONFIGURATOR
#define LIBASTFRI_TEXT_TEXT_CONFIGURATOR

#include <libastfri-text/inc/pseudocode/GeneralConfigurator.hpp>

#include <vector>

namespace astfri::text {
class TextConfigurator : public GeneralConfigurator {
    // TEXT_FORMAT
    std::unique_ptr<std::stringstream> defaultTextStyle_;
    std::unique_ptr<std::stringstream> rowNumStyle_;
    std::unique_ptr<std::vector<std::stringstream>> bracketColors_;
    int tabulatorLen_;
    int textMarginLeftLen_;
    int rowNumMarginLeftLen_;
    bool useBracketColors_;
    bool shRowNum_;
    bool shDotAfterRowNum_;
    bool shRowNumOnEmptyRow_;
    bool resetRowNumOnEmptyRow_;
    bool newLineForCurlBracket_;
    // CODE_STRUCTURE
    bool shOtherExpressions_;
    bool shGlobalVars_;
    bool shGenericParams_;
    bool shClassDeclar_;
    bool shClassDefin_;
    bool shClassInline_;
    bool shInterfDeclar_;
    bool shInterfDefin_;
    bool shMemberVars_;
    bool shCoDeMeDeclar_;
    bool shCoDeMeDefin_;
    bool shCoDeMeOwner_;
    bool shCoDeMeTemplate_;
    bool shFuncDeclar_;
    bool shFuncDefin_;
    // SYSTEM_EXPRESSIONS
    std::unique_ptr<std::stringstream> scopeWord_;
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
    std::unique_ptr<std::stringstream> overrideWord_;
    std::unique_ptr<std::stringstream> virtualWord_;
    std::unique_ptr<std::stringstream> abstractWord_;
    std::unique_ptr<std::stringstream> templateWord_;
    // STYLE
    std::unique_ptr<std::stringstream> systExprStyle_;
    std::unique_ptr<std::stringstream> scopeWordStyle_;
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
    std::unique_ptr<std::stringstream> overrideWordStyle_;
    std::unique_ptr<std::stringstream> virtualWordStyle_;
    std::unique_ptr<std::stringstream> abstractWordStyle_;
    std::unique_ptr<std::stringstream> templateWordStyle_;
    // OTHER_EXPRESSIONS
    std::unique_ptr<std::stringstream> constructorWord_;
    std::unique_ptr<std::stringstream> destructorWord_;
    std::unique_ptr<std::stringstream> methodWord_;
    std::unique_ptr<std::stringstream> functionWord_;
    std::unique_ptr<std::stringstream> lambdaWord_;
    std::unique_ptr<std::stringstream> callWord_;
    std::unique_ptr<std::stringstream> defineWord_;
    std::unique_ptr<std::stringstream> returnsWord_;
    // STYLE
    std::unique_ptr<std::stringstream> otherExprStyle_;
    std::unique_ptr<std::stringstream> constructorWordStyle_;
    std::unique_ptr<std::stringstream> destructorWordStyle_;
    std::unique_ptr<std::stringstream> methodWordStyle_;
    std::unique_ptr<std::stringstream> functionWordStyle_;
    std::unique_ptr<std::stringstream> lambdaWordStyle_;
    std::unique_ptr<std::stringstream> callWordStyle_;
    std::unique_ptr<std::stringstream> defineWordStyle_;
    std::unique_ptr<std::stringstream> returnsWordStyle_;

private:
    bool isDefaultState_;
    bool doOnlyUpdate_;

public:
    static TextConfigurator& get_instance();
    TextConfigurator(const TextConfigurator&)            = delete;
    TextConfigurator(TextConfigurator&&)                 = delete;
    TextConfigurator& operator=(const TextConfigurator&) = delete;
    TextConfigurator& operator=(TextConfigurator&&)      = delete;

private:
    explicit TextConfigurator();
    ~TextConfigurator() = default;

public:
    void update_configuration();
    void reload_configuration();

private:
    void set_defaults() override;
    void process_document(const rj::Value*& doc) override;
    // -----
    void process_text_format(const rj::Value*& format);
    void process_code_structure(const rj::Value*& structure);
    void process_system_expressions(const rj::Value*& expr);
    void process_other_expressions(const rj::Value*& expr);

public:
    // TEXT_FORMAT
    const std::stringstream* default_text_style() {
        return defaultTextStyle_.get();
    }

    const std::stringstream* row_num_style() {
        return rowNumStyle_.get();
    }

    const std::vector<std::stringstream>* bracket_colors() {
        return bracketColors_.get();
    }

    const int& tabulator_len() {
        return tabulatorLen_;
    }

    const int& text_margin_left_len() {
        return textMarginLeftLen_;
    }

    const int& row_num_margin_left_len() {
        return rowNumMarginLeftLen_;
    }

    const bool& use_bracket_colors() {
        return useBracketColors_;
    }

    const bool& sh_row_num() {
        return shRowNum_;
    }

    const bool& sh_dot_after_row_num() {
        return shDotAfterRowNum_;
    }

    const bool& sh_row_num_empty_row() {
        return shRowNumOnEmptyRow_;
    }

    const bool& reset_row_num_empty_row() {
        return resetRowNumOnEmptyRow_;
    }

    const bool& new_line_curl_bracket() {
        return newLineForCurlBracket_;
    }

    // CODE_STRUCTURE
    const bool& sh_other_expr() {
        return shOtherExpressions_;
    }

    const bool& sh_global_vars() {
        return shGlobalVars_;
    }

    const bool& sh_generic_param() {
        return shGenericParams_;
    }

    const bool& sh_class_declar() {
        return shClassDeclar_;
    }

    const bool& sh_class_defin() {
        return shClassDefin_;
    }

    const bool& sh_class_inline() {
        return shClassInline_;
    }

    const bool& sh_interf_declar() {
        return shInterfDeclar_;
    }

    const bool& sh_interf_defin() {
        return shInterfDefin_;
    }

    const bool& sh_member_vars() {
        return shMemberVars_;
    }

    const bool& sh_con_des_meth_declar() {
        return shCoDeMeDeclar_;
    }

    const bool& sh_con_des_meth_defin() {
        return shCoDeMeDefin_;
    }

    const bool& sh_con_des_meth_owner() {
        return shCoDeMeOwner_;
    }

    const bool& sh_con_des_meth_template() {
        return shCoDeMeTemplate_;
    }

    const bool& sh_func_declar() {
        return shFuncDeclar_;
    }

    const bool& sh_func_defin() {
        return shFuncDefin_;
    }

    // SYSTEM_EXPRESSIONS
    const std::stringstream* scope_word() {
        return scopeWord_.get();
    }

    const std::stringstream* class_word() {
        return classWord_.get();
    }

    const std::stringstream* interface_word() {
        return interfaceWord_.get();
    }

    const std::stringstream* implement_word() {
        return implementWord_.get();
    }

    const std::stringstream* extend_word() {
        return extendWord_.get();
    }

    const std::stringstream* this_word() {
        return thisWord_.get();
    }

    const std::stringstream* return_word() {
        return returnWord_.get();
    }

    const std::stringstream* continue_word() {
        return continueWord_.get();
    }

    const std::stringstream* break_word() {
        return breakWord_.get();
    }

    const std::stringstream* throw_word() {
        return throwWord_.get();
    }

    const std::stringstream* if_word() {
        return ifWord_.get();
    }

    const std::stringstream* else_word() {
        return elseWord_.get();
    }

    const std::stringstream* do_word() {
        return doWord_.get();
    }

    const std::stringstream* while_word() {
        return whileWord_.get();
    }

    const std::stringstream* for_word() {
        return forWord_.get();
    }

    const std::stringstream* repeat_word() {
        return repeatWord_.get();
    }

    const std::stringstream* switch_word() {
        return switchWord_.get();
    }

    const std::stringstream* case_word() {
        return caseWord_.get();
    }

    const std::stringstream* default_word() {
        return defaultWord_.get();
    }

    const std::stringstream* new_word() {
        return newWord_.get();
    }

    const std::stringstream* delete_word() {
        return deleteWord_.get();
    }

    const std::stringstream* pointer_word() {
        return pointerWord_.get();
    }

    const std::stringstream* override_word() {
        return overrideWord_.get();
    }

    const std::stringstream* virtual_word() {
        return virtualWord_.get();
    }

    const std::stringstream* abstract_word() {
        return abstractWord_.get();
    }

    const std::stringstream* template_word() {
        return templateWord_.get();
    }

    // STYLE
    const std::stringstream* system_expr_style() {
        return systExprStyle_.get();
    }

    const std::stringstream* scope_word_style() {
        return scopeWordStyle_.get();
    }

    const std::stringstream* class_word_style() {
        return classWordStyle_.get();
    }

    const std::stringstream* interface_word_style() {
        return interfaceWordStyle_.get();
    }

    const std::stringstream* implement_word_style() {
        return implementWordStyle_.get();
    }

    const std::stringstream* extend_word_style() {
        return extendWordStyle_.get();
    }

    const std::stringstream* this_word_style() {
        return thisWordStyle_.get();
    }

    const std::stringstream* return_word_style() {
        return returnWordStyle_.get();
    }

    const std::stringstream* continue_word_style() {
        return continueWordStyle_.get();
    }

    const std::stringstream* break_word_style() {
        return breakWordStyle_.get();
    }

    const std::stringstream* throw_word_style() {
        return throwWordStyle_.get();
    }

    const std::stringstream* if_word_style() {
        return ifWordStyle_.get();
    }

    const std::stringstream* else_word_style() {
        return elseWordStyle_.get();
    }

    const std::stringstream* do_word_style() {
        return doWordStyle_.get();
    }

    const std::stringstream* while_word_style() {
        return whileWordStyle_.get();
    }

    const std::stringstream* for_word_style() {
        return forWordStyle_.get();
    }

    const std::stringstream* repeat_word_style() {
        return repeatWordStyle_.get();
    }

    const std::stringstream* switch_word_style() {
        return switchWordStyle_.get();
    }

    const std::stringstream* case_word_style() {
        return caseWordStyle_.get();
    }

    const std::stringstream* default_word_style() {
        return defaultWordStyle_.get();
    }

    const std::stringstream* new_word_style() {
        return newWordStyle_.get();
    }

    const std::stringstream* delete_word_style() {
        return deleteWordStyle_.get();
    }

    const std::stringstream* pointer_word_style() {
        return pointerWordStyle_.get();
    }

    const std::stringstream* override_word_style() {
        return overrideWordStyle_.get();
    }

    const std::stringstream* virtual_word_style() {
        return virtualWordStyle_.get();
    }

    const std::stringstream* abstract_word_style() {
        return abstractWordStyle_.get();
    }

    const std::stringstream* template_word_style() {
        return templateWordStyle_.get();
    }

    // OTHER_EXPRESSIONS
    const std::stringstream* constructor_word() {
        return constructorWord_.get();
    }

    const std::stringstream* destructor_word() {
        return destructorWord_.get();
    }

    const std::stringstream* method_word() {
        return methodWord_.get();
    }

    const std::stringstream* function_word() {
        return functionWord_.get();
    }

    const std::stringstream* lambda_word() {
        return lambdaWord_.get();
    }

    const std::stringstream* call_word() {
        return callWord_.get();
    }

    const std::stringstream* define_word() {
        return defineWord_.get();
    }

    const std::stringstream* returns_word() {
        return returnsWord_.get();
    }

    const std::stringstream* other_expression_style() {
        return otherExprStyle_.get();
    }

    const std::stringstream* constructor_word_style() {
        return constructorWordStyle_.get();
    }

    const std::stringstream* destructor_word_style() {
        return destructorWordStyle_.get();
    }

    const std::stringstream* method_word_style() {
        return methodWordStyle_.get();
    }

    const std::stringstream* function_word_style() {
        return functionWordStyle_.get();
    }

    const std::stringstream* lambda_word_style() {
        return lambdaWordStyle_.get();
    }

    const std::stringstream* call_word_style() {
        return callWordStyle_.get();
    }

    const std::stringstream* define_word_style() {
        return defineWordStyle_.get();
    }

    const std::stringstream* returns_word_style() {
        return returnsWordStyle_.get();
    }
};
} // namespace astfri::text

#endif