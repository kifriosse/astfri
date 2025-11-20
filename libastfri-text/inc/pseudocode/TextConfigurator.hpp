#ifndef LIBASTFRI_TEXT_TEXT_CONFIGURATOR
#define LIBASTFRI_TEXT_TEXT_CONFIGURATOR

#include <libastfri-text/inc/pseudocode/GeneralConfigurator.hpp>

#include <vector>

namespace astfri::text
{
    class TextConfigurator : public GeneralConfigurator
    {
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
        TextConfigurator(TextConfigurator const&) = delete;
        TextConfigurator(TextConfigurator&&)      = delete;
        TextConfigurator& operator=(TextConfigurator const&) = delete;
        TextConfigurator& operator=(TextConfigurator&&)      = delete;
    private:
        explicit TextConfigurator();
        ~TextConfigurator() = default;
    public:
        void update_configuration();
        void reload_configuration();
    private:
        void set_defaults() override;
        void process_document(rj::Value const*& doc) override;
        // -----
        void process_text_format(rj::Value const*& format);
        void process_code_structure(rj::Value const*& structure);
        void process_system_expressions(rj::Value const*& expr);
        void process_other_expressions(rj::Value const*& expr);
    public:
        // TEXT_FORMAT
        std::stringstream const* default_text_style()
        {
            return defaultTextStyle_.get();
        }

        std::stringstream const* row_num_style()
        {
            return rowNumStyle_.get();
        }

        std::vector<std::stringstream> const* bracket_colors()
        {
            return bracketColors_.get();
        }

        int const& tabulator_len()
        {
            return tabulatorLen_;
        }

        int const& text_margin_left_len()
        {
            return textMarginLeftLen_;
        }

        int const& row_num_margin_left_len()
        {
            return rowNumMarginLeftLen_;
        }

        bool const& use_bracket_colors()
        {
            return useBracketColors_;
        }

        bool const& sh_row_num()
        {
            return shRowNum_;
        }

        bool const& sh_dot_after_row_num()
        {
            return shDotAfterRowNum_;
        }

        bool const& sh_row_num_empty_row()
        {
            return shRowNumOnEmptyRow_;
        }

        bool const& reset_row_num_empty_row()
        {
            return resetRowNumOnEmptyRow_;
        }

        bool const& new_line_curl_bracket()
        {
            return newLineForCurlBracket_;
        }

        // CODE_STRUCTURE
        bool const& sh_other_expr()
        {
            return shOtherExpressions_;
        }

        bool const& sh_global_vars()
        {
            return shGlobalVars_;
        }

        bool const& sh_generic_param()
        {
            return shGenericParams_;
        }

        bool const& sh_class_declar()
        {
            return shClassDeclar_;
        }

        bool const& sh_class_defin()
        {
            return shClassDefin_;
        }

        bool const& sh_class_inline()
        {
            return shClassInline_;
        }

        bool const& sh_interf_declar()
        {
            return shInterfDeclar_;
        }

        bool const& sh_interf_defin()
        {
            return shInterfDefin_;
        }

        bool const& sh_member_vars()
        {
            return shMemberVars_;
        }

        bool const& sh_con_des_meth_declar()
        {
            return shCoDeMeDeclar_;
        }

        bool const& sh_con_des_meth_defin()
        {
            return shCoDeMeDefin_;
        }

        bool const& sh_con_des_meth_owner()
        {
            return shCoDeMeOwner_;
        }

        bool const& sh_con_des_meth_template()
        {
            return shCoDeMeTemplate_;
        }

        bool const& sh_func_declar()
        {
            return shFuncDeclar_;
        }

        bool const& sh_func_defin()
        {
            return shFuncDefin_;
        }

        // SYSTEM_EXPRESSIONS
        std::stringstream const* scope_word()
        {
            return scopeWord_.get();
        }

        std::stringstream const* class_word()
        {
            return classWord_.get();
        }

        std::stringstream const* interface_word()
        {
            return interfaceWord_.get();
        }

        std::stringstream const* implement_word()
        {
            return implementWord_.get();
        }

        std::stringstream const* extend_word()
        {
            return extendWord_.get();
        }

        std::stringstream const* this_word()
        {
            return thisWord_.get();
        }

        std::stringstream const* return_word()
        {
            return returnWord_.get();
        }

        std::stringstream const* continue_word()
        {
            return continueWord_.get();
        }

        std::stringstream const* break_word()
        {
            return breakWord_.get();
        }

        std::stringstream const* throw_word()
        {
            return throwWord_.get();
        }

        std::stringstream const* if_word()
        {
            return ifWord_.get();
        }

        std::stringstream const* else_word()
        {
            return elseWord_.get();
        }

        std::stringstream const* do_word()
        {
            return doWord_.get();
        }

        std::stringstream const* while_word()
        {
            return whileWord_.get();
        }

        std::stringstream const* for_word()
        {
            return forWord_.get();
        }

        std::stringstream const* repeat_word()
        {
            return repeatWord_.get();
        }

        std::stringstream const* switch_word()
        {
            return switchWord_.get();
        }

        std::stringstream const* case_word()
        {
            return caseWord_.get();
        }

        std::stringstream const* default_word()
        {
            return defaultWord_.get();
        }

        std::stringstream const* new_word()
        {
            return newWord_.get();
        }

        std::stringstream const* delete_word()
        {
            return deleteWord_.get();
        }

        std::stringstream const* pointer_word()
        {
            return pointerWord_.get();
        }

        std::stringstream const* override_word()
        {
            return overrideWord_.get();
        }

        std::stringstream const* virtual_word()
        {
            return virtualWord_.get();
        }

        std::stringstream const* abstract_word()
        {
            return abstractWord_.get();
        }

        std::stringstream const* template_word()
        {
            return templateWord_.get();
        }

        // STYLE
        std::stringstream const* system_expr_style()
        {
            return systExprStyle_.get();
        }

        std::stringstream const* scope_word_style()
        {
            return scopeWordStyle_.get();
        }

        std::stringstream const* class_word_style()
        {
            return classWordStyle_.get();
        }

        std::stringstream const* interface_word_style()
        {
            return interfaceWordStyle_.get();
        }

        std::stringstream const* implement_word_style()
        {
            return implementWordStyle_.get();
        }

        std::stringstream const* extend_word_style()
        {
            return extendWordStyle_.get();
        }

        std::stringstream const* this_word_style()
        {
            return thisWordStyle_.get();
        }

        std::stringstream const* return_word_style()
        {
            return returnWordStyle_.get();
        }

        std::stringstream const* continue_word_style()
        {
            return continueWordStyle_.get();
        }

        std::stringstream const* break_word_style()
        {
            return breakWordStyle_.get();
        }

        std::stringstream const* throw_word_style()
        {
            return throwWordStyle_.get();
        }

        std::stringstream const* if_word_style()
        {
            return ifWordStyle_.get();
        }

        std::stringstream const* else_word_style()
        {
            return elseWordStyle_.get();
        }

        std::stringstream const* do_word_style()
        {
            return doWordStyle_.get();
        }

        std::stringstream const* while_word_style()
        {
            return whileWordStyle_.get();
        }

        std::stringstream const* for_word_style()
        {
            return forWordStyle_.get();
        }

        std::stringstream const* repeat_word_style()
        {
            return repeatWordStyle_.get();
        }

        std::stringstream const* switch_word_style()
        {
            return switchWordStyle_.get();
        }

        std::stringstream const* case_word_style()
        {
            return caseWordStyle_.get();
        }

        std::stringstream const* default_word_style()
        {
            return defaultWordStyle_.get();
        }

        std::stringstream const* new_word_style()
        {
            return newWordStyle_.get();
        }

        std::stringstream const* delete_word_style()
        {
            return deleteWordStyle_.get();
        }

        std::stringstream const* pointer_word_style()
        {
            return pointerWordStyle_.get();
        }

        std::stringstream const* override_word_style()
        {
            return overrideWordStyle_.get();
        }

        std::stringstream const* virtual_word_style()
        {
            return virtualWordStyle_.get();
        }

        std::stringstream const* abstract_word_style()
        {
            return abstractWordStyle_.get();
        }

        std::stringstream const* template_word_style()
        {
            return templateWordStyle_.get();
        }

        // OTHER_EXPRESSIONS
        std::stringstream const* constructor_word()
        {
            return constructorWord_.get();
        }

        std::stringstream const* destructor_word()
        {
            return destructorWord_.get();
        }

        std::stringstream const* method_word()
        {
            return methodWord_.get();
        }

        std::stringstream const* function_word()
        {
            return functionWord_.get();
        }

        std::stringstream const* lambda_word()
        {
            return lambdaWord_.get();
        }

        std::stringstream const* call_word()
        {
            return callWord_.get();
        }

        std::stringstream const* define_word()
        {
            return defineWord_.get();
        }

        std::stringstream const* returns_word()
        {
            return returnsWord_.get();
        }

        std::stringstream const* other_expression_style()
        {
            return otherExprStyle_.get();
        }

        std::stringstream const* constructor_word_style()
        {
            return constructorWordStyle_.get();
        }

        std::stringstream const* destructor_word_style()
        {
            return destructorWordStyle_.get();
        }

        std::stringstream const* method_word_style()
        {
            return methodWordStyle_.get();
        }

        std::stringstream const* function_word_style()
        {
            return functionWordStyle_.get();
        }

        std::stringstream const* lambda_word_style()
        {
            return lambdaWordStyle_.get();
        }

        std::stringstream const* call_word_style()
        {
            return callWordStyle_.get();
        }

        std::stringstream const* define_word_style()
        {
            return defineWordStyle_.get();
        }

        std::stringstream const* returns_word_style()
        {
            return returnsWordStyle_.get();
        }
    };
}

#endif