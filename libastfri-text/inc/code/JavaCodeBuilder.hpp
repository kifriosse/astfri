#ifndef LIBASTFRI_TEXT_JAVA_CODE_BUILDER
#define LIBASTFRI_TEXT_JAVA_CODE_BUILDER

#include <libastfri/impl/Scope.hpp>

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
        // GENERAL_TEXT
        void write_opening_curl_bracket() override;
        void write_opening_else_word() override;
        // EXPORT
        void create_java_file(std::string const& name, Scope const& scope);
        void create_java_file(std::string const& name);
    };
}

#endif