#include <libastfri-text/inc/AbstractBuilder.hpp>

using namespace astfri::text;

AbstractBuilder::AbstractBuilder() :
    buildedText_(std::make_unique<std::stringstream>()),
    indentationLevel_(0),
    isEmptyLine_(true)
{
}

//
// SET_UP
//

void AbstractBuilder::increase_indentation()
{
    ++indentationLevel_;
}

void AbstractBuilder::decrease_indentation()
{
    --indentationLevel_;
}

void AbstractBuilder::reset_builder()
{
    buildedText_ = std::make_unique<std::stringstream>();
    indentationLevel_ = 0;
    isEmptyLine_ = true;
}