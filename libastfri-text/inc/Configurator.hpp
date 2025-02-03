#pragma once
#include <sstream>

struct Configurator {
private:
    const std::stringstream* outputFileName_;//bez pripony
    const std::stringstream* outputType_;//text-uml
    const std::stringstream* view_;//vnutorny-vonkajsi
    const std::stringstream* intWord_;
    const std::stringstream* charWord_;
    //-------------------------------------------
    const std::stringstream* outputFormat_;//txt-pdf...
    const std::stringstream* tabWord_;
    bool showClassBody_;
    bool showMethodBody_;
    bool showFunctionBody_;
    bool openBracketOnNewLine_;//true=new_line false=same_line
    //-------------------------------------------
    const std::stringstream* classWord_;
    const std::stringstream* ifWord_;
    const std::stringstream* elseWord_;
    const std::stringstream* forWord_;
    const std::stringstream* whileWord_;
public:
    Configurator(const std::string& configFileName);
    ~Configurator();
    const std::stringstream* get_output_file_name() const { return outputFileName_; };
    const std::stringstream* get_output_type() const { return outputType_; };
    const std::stringstream* get_view() const { return view_; };
    const std::stringstream* get_int_word() const { return intWord_; };
    const std::stringstream* get_char_word() const { return charWord_; };
    const std::stringstream* get_output_format() const { return outputFormat_; };
    bool show_class_body() const { return showClassBody_; };
    bool show_method_body() const { return showMethodBody_; };
    bool show_function_body() const { return showFunctionBody_; };
    bool open_bracket_new_line() const { return openBracketOnNewLine_; };
    const std::stringstream* get_tab_word() const { return tabWord_; };
    const std::stringstream* get_class_word() const { return classWord_; };
    const std::stringstream* get_if_word() const { return ifWord_; };
    const std::stringstream* get_else_word() const { return elseWord_; };
    const std::stringstream* get_for_word() const { return forWord_; };
    const std::stringstream* get_while_word() const { return whileWord_; };
private:
    void set_defaults();
};