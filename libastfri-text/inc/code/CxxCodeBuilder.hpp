#ifndef LIBASTFRI_TEXT_CXX_CODE_BUILDER
#define LIBASTFRI_TEXT_CXX_CODE_BUILDER

#include <libastfri-text/inc/code/AbstractCodeBuilder.hpp>

namespace astfri::text {
class CxxCodeBuilder : public AbstractCodeBuilder {
public:
    static CxxCodeBuilder& get_instatnce();
    CxxCodeBuilder(const CxxCodeBuilder&)            = delete;
    CxxCodeBuilder(CxxCodeBuilder&&)                 = delete;
    CxxCodeBuilder& operator=(const CxxCodeBuilder&) = delete;
    CxxCodeBuilder& operator=(CxxCodeBuilder&&)      = delete;

private:
    explicit CxxCodeBuilder() = default;
    ~CxxCodeBuilder()         = default;

public:
    // GENERAL_TEXT
    void write_opening_curl_bracket() override;
    void write_opening_else_word() override;
};
} // namespace astfri::text

#endif