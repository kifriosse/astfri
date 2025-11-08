#ifndef LIBASTFRI_TEXT_JAVA_CODE_BUILDER
#define LIBASTFRI_TEXT_JAVA_CODE_BUILDER

#include <libastfri-text/inc/AbstractTextBuilder.hpp>

namespace astfri::text
{
    class JavaCodeBuilder : public AbstractTextBuilder
    {
    public:
        explicit JavaCodeBuilder() = default;
        ~JavaCodeBuilder() = default;
    public:
        // BRACKETS, SEPARATORS, OPERATORS
        void write_opening_curl_bracket() override;
        // DATA_TYPES
        void write_dynamic_type_word() override;
        void write_float_type_word() override;
        void write_bool_type_word() override;
        // VALUES
        void write_null_val() override;
        // SYSTEM_EXPRESSIONS
        void write_scope_word() override;
        void write_interface_word() override;
        void write_implement_word() override;
        void write_extend_word() override;
        void write_abstract_word() override;
    };
}

#endif