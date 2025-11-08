#ifndef LIBASTFRI_TEXT_JAVA_CODE_BUILDER
#define LIBASTFRI_TEXT_JAVA_CODE_BUILDER

#include <libastfri-text/inc/code/AbstractCodeBuilder.hpp>

namespace astfri::text
{
    class JavaCodeBuilder : public AbstractCodeBuilder
    {
    public:
        static JavaCodeBuilder& get_instance();
        JavaCodeBuilder(JavaCodeBuilder const&) = delete;
        JavaCodeBuilder(JavaCodeBuilder&&)      = delete;
        JavaCodeBuilder& operator=(JavaCodeBuilder const&) = delete;
        JavaCodeBuilder& operator=(JavaCodeBuilder&&)      = delete;
    private:
        explicit JavaCodeBuilder() = default;
        ~JavaCodeBuilder() = default;
    public:
        // BRACKETS, SEPARATORS, OPERATORS
        void write_opening_curl_bracket() override;
    };
}

#endif