#include <libastfri-text/inc/pseudocode/AbstractTextBuilder.hpp>

using namespace astfri::text;

AbstractTextBuilder::AbstractTextBuilder() :
    configurator_(&TextConfigurator::get_instance()),
    rowCount_(1)
{
}

//
// SET_UP
//

void AbstractTextBuilder::reset_builder()
{
    rowCount_ = 1;
    AbstractBuilder::reset_builder();
}

//
// GENERAL_TEXT
//

void AbstractTextBuilder::append_text(std::string const& text)
{
    if (isEmptyLine_) {
        for (int i = 0; i < configurator_->text_margin_left_len(); ++i) {
            append_space();
        }
        for (int i = 0; i < indentationLevel_ * configurator_->tabulator_len(); ++i) {
            append_space();
        }
        isEmptyLine_ = false;
    }
    *buildedText_ << text;
}