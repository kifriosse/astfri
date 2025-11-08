#ifndef LIBASTFRI_TEXT_CXX_CODE_BUILDER
#define LIBASTFRI_TEXT_CXX_CODE_BUILDER

#include <libastfri-text/inc/code/AbstractCodeBuilder.hpp>

namespace astfri::text
{
    class CxxCodeBuilder : public AbstractCodeBuilder
    {
    public:
        static CxxCodeBuilder& get_instatnce();
        CxxCodeBuilder(CxxCodeBuilder const&) = delete;
        CxxCodeBuilder(CxxCodeBuilder&&)      = delete;
        CxxCodeBuilder& operator=(CxxCodeBuilder const&) = delete;
        CxxCodeBuilder& operator=(CxxCodeBuilder&&)      = delete;
    private:
        explicit CxxCodeBuilder() = default;
        ~CxxCodeBuilder() = default;
    public:
        // BRACKETS, SEPARATORS, OPERATORS
        void write_opening_curl_bracket() override;
    };
}

#endif