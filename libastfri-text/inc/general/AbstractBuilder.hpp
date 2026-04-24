#ifndef LIBASTFRI_TEXT_ABSTRACT_BUILDER
#define LIBASTFRI_TEXT_ABSTRACT_BUILDER

#include <string>

namespace astfri::text
{
    class AbstractBuilder
    {
    protected:
        std::string m_buildedText = "";
        int m_indentationLevel = 0;
        bool m_isEmptyLine = true;
    public:
        virtual ~AbstractBuilder() = default;
        //
        std::string& get_text();
        void reset_builder();
        void increase_indentation();
        void decrease_indentation();
        // GENERAL_TEXT
        void write_opening_curl_bracket(bool const& onNewLine, bool incIndent = true);
        void write_closing_curl_bracket(bool decIndent = true);
        virtual void write_space()                     = 0;
        virtual void write_new_line()                  = 0;
        virtual void write_text(std::string_view text) = 0;
        // SEPARATORS
        virtual void write_left_bracket(std::string_view br)  = 0;
        virtual void write_right_bracket(std::string_view br) = 0;
        virtual void write_separator(std::string_view sep)    = 0;
        // UNKNOWN_PHRASES
        virtual void write_unknown_type() = 0;
        virtual void write_unknown_expr() = 0;
        virtual void write_unknown_stmt() = 0;
    };
}

#endif