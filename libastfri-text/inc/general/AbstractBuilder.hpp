#ifndef LIBASTFRI_TEXT_ABSTRACT_BUILDER
#define LIBASTFRI_TEXT_ABSTRACT_BUILDER

#include <cstdint>
#include <string>

namespace astfri::text
{
    class AbstractBuilder
    {
    protected:
        std::string m_buildedText = "";
        uint8_t m_indentationLevel = 0;
        bool m_isEmptyLine = true;
    public:
        virtual ~AbstractBuilder() = default;
        //
        virtual void reset_builder();
        void increase_indentation();
        void decrease_indentation();
        // GENERAL_TEXT
        virtual void write_space()                            = 0;
        virtual void write_new_line()                         = 0;
        virtual void write_text(std::string_view text)        = 0;
        virtual void write_oparator(std::string_view op)      = 0;
        virtual void write_separator(std::string_view sep)    = 0;
        virtual void write_left_bracket(std::string_view br)  = 0;
        virtual void write_right_bracket(std::string_view br) = 0;
        virtual void write_opening_curl_bracket()             = 0;
        virtual void write_opening_else_word()                = 0;
        virtual void write_opening_catch_word()               = 0;
        // UNKNOWN_PHRASES
        virtual void write_unknown_type() = 0;
        virtual void write_unknown_expr() = 0;
        virtual void write_unknown_stmt() = 0;
        // VALUES
        virtual void write_int_val(const int& val)          = 0;
        virtual void write_float_val(const float& val)      = 0;
        virtual void write_char_val(const char& val)        = 0;
        virtual void write_string_val(std::string_view val) = 0;
        virtual void write_bool_val(const bool& val)        = 0;
    };
}

#endif