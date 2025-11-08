#ifndef LIBASTFRI_TEXT_ABSTRACT_BUILDER
#define LIBASTFRI_TEXT_ABSTRACT_BUILDER

#include <sstream>
#include <memory>

namespace astfri::text
{
    class AbstractBuilder
    {
    protected:
        std::unique_ptr<std::stringstream> buildedText_;
        short indentationLevel_;
        bool isEmptyLine_;
    public:
        explicit AbstractBuilder();
        virtual ~AbstractBuilder() = default;
    public:
        // SET_UP
        void increase_indentation();
        void decrease_indentation();
        virtual void reset_builder();
        // GENERAL_TEXT
        virtual void append_text(std::string const& text) = 0;
        virtual void append_new_line() = 0;
        virtual void append_space() = 0;
    };
}

#endif