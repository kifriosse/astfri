#pragma once
#include <sstream>

struct Configurator {
private:
    const std::stringstream* outputFileName_;//bez pripony
    const std::stringstream* outputType_;//text-uml
    const std::stringstream* view_;//vnutorny-vonkajsi
    const std::stringstream* intWord_;
    const std::stringstream* floatWord_;
    const std::stringstream* charWord_;
    const std::stringstream* boolWord_;
    const std::stringstream* voidWord_;
    const std::stringstream* assignWord_;
    const std::stringstream* privateWord_;
    const std::stringstream* publicWord_;
    //-------------------------------------------
    const std::stringstream* outputFormat_;//txt-pdf...
    const std::stringstream* tabWord_;
    bool showClassBody_;
    bool showMethodBody_;
    bool showMethodDefinition_;
    bool showFunctionBody_;
    bool openBracketOnNewLine_;//true=new_line false=same_line
    bool showRowNumber_;
    //-------------------------------------------
    const std::stringstream* classWord_;
    const std::stringstream* ifWord_;
    const std::stringstream* elseWord_;
    const std::stringstream* forWord_;
    const std::stringstream* whileWord_;
    const std::stringstream* doWord_;
    const std::stringstream* returnWord_;
    const std::stringstream* throwWord_;
    const std::stringstream* switchWord_;
    const std::stringstream* caseWord_;
    const std::stringstream* thisWord_;
    //-------------------------------------------
    const std::stringstream* classWordColor_;
    const std::stringstream* classNameColor_;
public:
    Configurator();
    ~Configurator();
    const std::stringstream* get_output_file_name() const { return outputFileName_; };
    const std::stringstream* get_output_type() const { return outputType_; };
    const std::stringstream* get_view() const { return view_; };
    const std::stringstream* get_int_word() const { return intWord_; };
    const std::stringstream* get_float_word() const { return floatWord_; };
    const std::stringstream* get_char_word() const { return charWord_; };
    const std::stringstream* get_bool_word() const { return boolWord_; };
    const std::stringstream* get_void_word() const { return voidWord_; };
    const std::stringstream* get_assign_word() const { return assignWord_; };
    const std::stringstream* get_private_word() const { return privateWord_; };
    const std::stringstream* get_public_word() const { return publicWord_; };
    const std::stringstream* get_output_format() const { return outputFormat_; };
    bool show_class_body() const { return showClassBody_; };
    bool show_method_body() const { return showMethodBody_; };
    bool show_method_definition() const { return showMethodDefinition_; };
    bool show_function_body() const { return showFunctionBody_; };
    bool open_bracket_new_line() const { return openBracketOnNewLine_; };
    bool show_row_number() const { return showRowNumber_; };
    const std::stringstream* get_tab_word() const { return tabWord_; };
    const std::stringstream* get_class_word() const { return classWord_; };
    const std::stringstream* get_if_word() const { return ifWord_; };
    const std::stringstream* get_else_word() const { return elseWord_; };
    const std::stringstream* get_for_word() const { return forWord_; };
    const std::stringstream* get_while_word() const { return whileWord_; };
    const std::stringstream* get_do_word() const { return doWord_; };
    const std::stringstream* get_return_word() const { return returnWord_; };
    const std::stringstream* get_throw_word() const { return throwWord_; };
    const std::stringstream* get_switch_word() const { return switchWord_; };
    const std::stringstream* get_case_word() const { return caseWord_; };
    const std::stringstream* get_this_word() const { return thisWord_; };
    const std::stringstream* get_class_word_color() const { return classWordColor_; };
    const std::stringstream* get_class_name_color() const { return classNameColor_; };
private:
    void set_defaults();
};