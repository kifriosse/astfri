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
        // BRACKETS, SEPARATORS, OPERATORS
        virtual void write_opening_curl_bracket() = 0;
        // UNKNOWN_PHRASES
        void write_unknown_type();
        void write_unknown_expr();
        void write_unknown_stmt();
        void write_invalid_type();
        void write_invalid_expr();
        void write_invalid_stmt();
    };
}

#endif