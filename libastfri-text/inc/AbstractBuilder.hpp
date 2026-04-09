#ifndef LIBASTFRI_TEXT_ABSTRACT_BUILDER
#define LIBASTFRI_TEXT_ABSTRACT_BUILDER

#include <memory>
#include <sstream>

namespace astfri::text {
class AbstractBuilder {
protected:
    std::unique_ptr<std::stringstream> buildedText_;
    short indentationLevel_;
    bool isEmptyLine_;

public:
    explicit AbstractBuilder();
    virtual ~AbstractBuilder() = default;

public:
    std::stringstream* get_builded_text(); // TODO tmp
    // SET_UP
    void increase_indentation();
    void decrease_indentation();
    virtual void reset_builder();
    // GENERAL_TEXT
    virtual void append_text(const std::string& text)       = 0;
    virtual void append_new_line()                          = 0;
    virtual void append_space()                             = 0;
    virtual void write_separator(const std::string& sep)    = 0;
    virtual void write_right_bracket(const std::string& br) = 0;
    virtual void write_left_bracket(const std::string& br)  = 0;
    virtual void write_opening_curl_bracket()               = 0;
    virtual void write_opening_else_word()                  = 0;
    // UNKNOWN_PHRASES
    virtual void write_unknown_type() = 0;
    virtual void write_unknown_expr() = 0;
    virtual void write_unknown_stmt() = 0;
    virtual void write_invalid_type() = 0;
    virtual void write_invalid_expr() = 0;
    virtual void write_invalid_stmt() = 0;
    // VALUES
    virtual void write_int_val(const int& val)            = 0;
    virtual void write_float_val(const float& val)        = 0;
    virtual void write_char_val(const char& val)          = 0;
    virtual void write_string_val(const std::string& val) = 0;
    virtual void write_bool_val(const bool& val)          = 0;
};
} // namespace astfri::text

#endif