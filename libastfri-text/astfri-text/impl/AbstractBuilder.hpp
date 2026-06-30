#ifndef LIBASTFRI_TEXT_ABSTRACT_BUILDER
#define LIBASTFRI_TEXT_ABSTRACT_BUILDER

#include <string>

namespace astfri::text
{
    using NewLineOrCurrent = bool;
    using UsingIndentation = bool;
    //
    constexpr bool NEW_LINE     = true;
    constexpr bool CURRENT_LINE = false;

    class AbstractBuilder
    {
    protected:
        std::string m_buildedText = "";
        int m_indentationLevel    = 0;
        bool m_isEmptyLine        = true;
    public:
        virtual ~AbstractBuilder() = default;
        //
        virtual void reset_builder()            = 0;
        virtual std::string& get_builded_text() = 0;
        //
        void increase_indentation();
        void decrease_indentation();
        //
        void write_opening_curl_bracket(NewLineOrCurrent option, UsingIndentation decision);
        void write_closing_curl_bracket(UsingIndentation decision);
        void write_comma_space();
        // 3.1 GENERAL
        virtual void write_text(std::string_view text) = 0;
        virtual void write_new_line()                  = 0;
        virtual void write_space()                     = 0;
        // 3.2.1 OPERATORS
        virtual void write_left_bracket(std::string_view br)  = 0;
        virtual void write_right_bracket(std::string_view br) = 0;
        // 3.2.2 SEPARATORS
        virtual void write_separator(std::string_view sep) = 0;
    protected:
        void write_new_line_or_space(NewLineOrCurrent option);
    };
}

#endif