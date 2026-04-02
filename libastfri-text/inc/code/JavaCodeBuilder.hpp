#ifndef LIBASTFRI_TEXT_JAVA_CODE_BUILDER
#define LIBASTFRI_TEXT_JAVA_CODE_BUILDER

#include <libastfri-text/inc/code/AbstractCodeBuilder.hpp>
#include <astfri/impl/Scope.hpp>

namespace astfri::text {
class JavaCodeBuilder : public AbstractCodeBuilder {
public:
    static JavaCodeBuilder& get_instance();
    JavaCodeBuilder(const JavaCodeBuilder&)            = delete;
    JavaCodeBuilder(JavaCodeBuilder&&)                 = delete;
    JavaCodeBuilder& operator=(const JavaCodeBuilder&) = delete;
    JavaCodeBuilder& operator=(JavaCodeBuilder&&)      = delete;

private:
    explicit JavaCodeBuilder() = default;
    ~JavaCodeBuilder()         = default;

public:
    // GENERAL_TEXT
    void write_opening_curl_bracket() override;
    void write_opening_else_word() override;
    // EXPORT
    void create_java_file(const std::string& name, const Scope& scope);
    void create_java_file(const std::string& name);
};
} // namespace astfri::text

#endif