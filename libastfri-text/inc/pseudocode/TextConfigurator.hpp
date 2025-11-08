#ifndef LIBASTFRI_TEXT_TEXT_CONFIGURATOR
#define LIBASTFRI_TEXT_TEXT_CONFIGURATOR

#include <libastfri-text/inc/pseudocode/GeneralConfigurator.hpp>

#include <vector>

namespace astfri::text
{
    class TextConfigurator : public GeneralConfigurator
    {
        // TEXT_FORMAT
        uptr<ss> defaultTextStyle_;
        uptr<ss> rowNumStyle_;
        uptr<std::vector<ss>> bracketColors_;
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
        uptr<ss> scopeWord_;
        uptr<ss> classWord_;
        uptr<ss> interfaceWord_;
        uptr<ss> implementWord_;
        uptr<ss> extendWord_;
        uptr<ss> thisWord_;
        uptr<ss> returnWord_;
        uptr<ss> continueWord_;
        uptr<ss> breakWord_;
        uptr<ss> throwWord_;
        uptr<ss> ifWord_;
        uptr<ss> elseWord_;
        uptr<ss> doWord_;
        uptr<ss> whileWord_;
        uptr<ss> forWord_;
        uptr<ss> repeatWord_;
        uptr<ss> switchWord_;
        uptr<ss> caseWord_;
        uptr<ss> defaultWord_;
        uptr<ss> newWord_;
        uptr<ss> deleteWord_;
        uptr<ss> pointerWord_;
        uptr<ss> overrideWord_;
        uptr<ss> virtualWord_;
        uptr<ss> abstractWord_;
        uptr<ss> templateWord_;
        // STYLE
        uptr<ss> systExprStyle_;
        uptr<ss> scopeWordStyle_;
        uptr<ss> classWordStyle_;
        uptr<ss> interfaceWordStyle_;
        uptr<ss> implementWordStyle_;
        uptr<ss> extendWordStyle_;
        uptr<ss> thisWordStyle_;
        uptr<ss> returnWordStyle_;
        uptr<ss> continueWordStyle_;
        uptr<ss> breakWordStyle_;
        uptr<ss> throwWordStyle_;
        uptr<ss> ifWordStyle_;
        uptr<ss> elseWordStyle_;
        uptr<ss> doWordStyle_;
        uptr<ss> whileWordStyle_;
        uptr<ss> forWordStyle_;
        uptr<ss> repeatWordStyle_;
        uptr<ss> switchWordStyle_;
        uptr<ss> caseWordStyle_;
        uptr<ss> defaultWordStyle_;
        uptr<ss> newWordStyle_;
        uptr<ss> deleteWordStyle_;
        uptr<ss> pointerWordStyle_;
        uptr<ss> overrideWordStyle_;
        uptr<ss> virtualWordStyle_;
        uptr<ss> abstractWordStyle_;
        uptr<ss> templateWordStyle_;
        // OTHER_EXPRESSIONS
        uptr<ss> constructorWord_;
        uptr<ss> destructorWord_;
        uptr<ss> methodWord_;
        uptr<ss> functionWord_;
        uptr<ss> lambdaWord_;
        uptr<ss> callWord_;
        uptr<ss> defineWord_;
        uptr<ss> returnsWord_;
        // STYLE
        uptr<ss> otherExprStyle_;
        uptr<ss> constructorWordStyle_;
        uptr<ss> destructorWordStyle_;
        uptr<ss> methodWordStyle_;
        uptr<ss> functionWordStyle_;
        uptr<ss> lambdaWordStyle_;
        uptr<ss> callWordStyle_;
        uptr<ss> defineWordStyle_;
        uptr<ss> returnsWordStyle_;
        // -----
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
        ss const* default_text_style()
        {
            return defaultTextStyle_.get();
        }

        ss const* row_num_style()
        {
            return rowNumStyle_.get();
        }

        std::vector<ss> const* bracket_colors()
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
        ss const* scope_word()
        {
            return scopeWord_.get();
        }

        ss const* class_word()
        {
            return classWord_.get();
        }

        ss const* interface_word()
        {
            return interfaceWord_.get();
        }

        ss const* implement_word()
        {
            return implementWord_.get();
        }

        ss const* extend_word()
        {
            return extendWord_.get();
        }

        ss const* this_word()
        {
            return thisWord_.get();
        }

        ss const* return_word()
        {
            return returnWord_.get();
        }

        ss const* continue_word()
        {
            return continueWord_.get();
        }

        ss const* break_word()
        {
            return breakWord_.get();
        }

        ss const* throw_word()
        {
            return throwWord_.get();
        }

        ss const* if_word()
        {
            return ifWord_.get();
        }

        ss const* else_word()
        {
            return elseWord_.get();
        }

        ss const* do_word()
        {
            return doWord_.get();
        }

        ss const* while_word()
        {
            return whileWord_.get();
        }

        ss const* for_word()
        {
            return forWord_.get();
        }

        ss const* repeat_word()
        {
            return repeatWord_.get();
        }

        ss const* switch_word()
        {
            return switchWord_.get();
        }

        ss const* case_word()
        {
            return caseWord_.get();
        }

        ss const* default_word()
        {
            return defaultWord_.get();
        }

        ss const* new_word()
        {
            return newWord_.get();
        }

        ss const* delete_word()
        {
            return deleteWord_.get();
        }

        ss const* pointer_word()
        {
            return pointerWord_.get();
        }

        ss const* override_word()
        {
            return overrideWord_.get();
        }

        ss const* virtual_word()
        {
            return virtualWord_.get();
        }

        ss const* abstract_word()
        {
            return abstractWord_.get();
        }

        ss const* template_word()
        {
            return templateWord_.get();
        }

        // STYLE
        ss const* system_expr_style()
        {
            return systExprStyle_.get();
        }

        ss const* scope_word_style()
        {
            return scopeWordStyle_.get();
        }

        ss const* class_word_style()
        {
            return classWordStyle_.get();
        }

        ss const* interface_word_style()
        {
            return interfaceWordStyle_.get();
        }

        ss const* implement_word_style()
        {
            return implementWordStyle_.get();
        }

        ss const* extend_word_style()
        {
            return extendWordStyle_.get();
        }

        ss const* this_word_style()
        {
            return thisWordStyle_.get();
        }

        ss const* return_word_style()
        {
            return returnWordStyle_.get();
        }

        ss const* continue_word_style()
        {
            return continueWordStyle_.get();
        }

        ss const* break_word_style()
        {
            return breakWordStyle_.get();
        }

        ss const* throw_word_style()
        {
            return throwWordStyle_.get();
        }

        ss const* if_word_style()
        {
            return ifWordStyle_.get();
        }

        ss const* else_word_style()
        {
            return elseWordStyle_.get();
        }

        ss const* do_word_style()
        {
            return doWordStyle_.get();
        }

        ss const* while_word_style()
        {
            return whileWordStyle_.get();
        }

        ss const* for_word_style()
        {
            return forWordStyle_.get();
        }

        ss const* repeat_word_style()
        {
            return repeatWordStyle_.get();
        }

        ss const* switch_word_style()
        {
            return switchWordStyle_.get();
        }

        ss const* case_word_style()
        {
            return caseWordStyle_.get();
        }

        ss const* default_word_style()
        {
            return defaultWordStyle_.get();
        }

        ss const* new_word_style()
        {
            return newWordStyle_.get();
        }

        ss const* delete_word_style()
        {
            return deleteWordStyle_.get();
        }

        ss const* pointer_word_style()
        {
            return pointerWordStyle_.get();
        }

        ss const* override_word_style()
        {
            return overrideWordStyle_.get();
        }

        ss const* virtual_word_style()
        {
            return virtualWordStyle_.get();
        }

        ss const* abstract_word_style()
        {
            return abstractWordStyle_.get();
        }

        ss const* template_word_style()
        {
            return templateWordStyle_.get();
        }

        // OTHER_EXPRESSIONS
        ss const* constructor_word()
        {
            return constructorWord_.get();
        }

        ss const* destructor_word()
        {
            return destructorWord_.get();
        }

        ss const* method_word()
        {
            return methodWord_.get();
        }

        ss const* function_word()
        {
            return functionWord_.get();
        }

        ss const* lambda_word()
        {
            return lambdaWord_.get();
        }

        ss const* call_word()
        {
            return callWord_.get();
        }

        ss const* define_word()
        {
            return defineWord_.get();
        }

        ss const* returns_word()
        {
            return returnsWord_.get();
        }

        ss const* other_expression_style()
        {
            return otherExprStyle_.get();
        }

        ss const* constructor_word_style()
        {
            return constructorWordStyle_.get();
        }

        ss const* destructor_word_style()
        {
            return destructorWordStyle_.get();
        }

        ss const* method_word_style()
        {
            return methodWordStyle_.get();
        }

        ss const* function_word_style()
        {
            return functionWordStyle_.get();
        }

        ss const* lambda_word_style()
        {
            return lambdaWordStyle_.get();
        }

        ss const* call_word_style()
        {
            return callWordStyle_.get();
        }

        ss const* define_word_style()
        {
            return defineWordStyle_.get();
        }

        ss const* returns_word_style()
        {
            return returnsWordStyle_.get();
        }
    };
}

#endif