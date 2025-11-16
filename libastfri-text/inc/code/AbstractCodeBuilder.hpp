#ifndef LIBASTFRI_TEXT_ABSTRACT_CODE_BUILDER
#define LIBASTFRI_TEXT_ABSTRACT_CODE_BUILDER

#include <libastfri-text/inc/AbstractBuilder.hpp>

namespace astfri::text
{
    class AbstractCodeBuilder : public AbstractBuilder
    {
    public:
        virtual ~AbstractCodeBuilder() = default;
    public:
        // GENERAL_TEXT
        void append_text(std::string const& text) override;
        void append_new_line() override;
        void append_space() override;
        void write_separator(std::string const& sep) override;
        void write_right_bracket(std::string const& br) override;
        void write_left_bracket(std::string const& br) override;
        void write_opening_curl_bracket() override = 0;
        void write_opening_else_word() override = 0;
        // UNKNOWN_PHRASES
        void write_unknown_type() override;
        void write_unknown_expr() override;
        void write_unknown_stmt() override;
        void write_invalid_type() override;
        void write_invalid_expr() override;
        void write_invalid_stmt() override;
        // VALUES
        void write_int_val(int const& val) override;
        void write_float_val(float const& val) override;
        void write_char_val(char const& val) override;
        void write_string_val(std::string const& val) override;
        void write_bool_val(bool const& val) override;
    };
}

#endif