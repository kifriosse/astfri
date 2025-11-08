#ifndef LIBASTFRI_TEXT_CXX_CODE_BUILDER
#define LIBASTFRI_TEXT_CXX_CODE_BUILDER

#include <libastfri-text/inc/AbstractTextBuilder.hpp>

namespace astfri::text
{
    class CxxCodeBuilder : public AbstractTextBuilder
    {
    public:
        explicit CxxCodeBuilder() = default;
        ~CxxCodeBuilder() = default;
    public:
        // BRACKETS, SEPARATORS, OPERATORS
        void write_opening_curl_bracket() override;
        void write_address_operator() override;
        void write_deref_operator() override;
        // DATA_TYPES
        void write_dynamic_type_word() override;
        void write_float_type_word() override;
        void write_bool_type_word() override;
        // VALUES
        void write_null_val() override;
        // SYSTEM_EXPRESSIONS
        void write_scope_word() override;
        void write_delete_word() override;
        void write_pointer_word() override;
        void write_virtual_word() override;
        void write_template_word() override;
    };
}

#endif