#ifndef LIBASTFRI_TEXT_TEXT_BUILDER
#define LIBASTFRI_TEXT_TEXT_BUILDER

#include <string>

namespace astfri::text
{
    class TextBuilder
    {
        std::string m_buildedText = "";
        int m_indentationLevel    = 0;
    protected:
        bool m_isEmptyLine = true;
    public:
        virtual ~TextBuilder() = default;
        //
        std::string& get_builded_text();
        void reset_text_builder();
        void increase_indentation();
        void decrease_indentation();
        void write_opening_curl_bracket(bool const& onNewLine, bool incIndent = true);
        void write_closing_curl_bracket(bool decIndent = true);
        void write_comma_space();
        //
        virtual void write_text(std::string_view text)        = 0;
        virtual void write_separator(std::string_view sep)    = 0;
        virtual void write_left_bracket(std::string_view br)  = 0;
        virtual void write_right_bracket(std::string_view br) = 0;
        //
        virtual void write_unknown_type() = 0;
        virtual void write_unknown_expr() = 0;
        virtual void write_unknown_stmt() = 0;
        virtual void write_new_line()     = 0;
        virtual void write_space()        = 0;
    };
}

#endif