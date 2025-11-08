#include <libastfri-text/inc/code/AbstractCodeBuilder.hpp>

using namespace astfri::text;

//
// GENERAL_TEXT
//

void AbstractCodeBuilder::append_text(std::string const& text)
{
    if (isEmptyLine_) {
        for (int i = 0; i < indentationLevel_ * 4; ++i) {
            append_space();
        }
        isEmptyLine_ = false;
    }
    *buildedText_ << text;
}

void AbstractCodeBuilder::append_new_line()
{
    *buildedText_ << "\n";
    isEmptyLine_ = true;
}

void AbstractCodeBuilder::append_space()
{
    *buildedText_ << " ";
}

//
// UNKNOWN_PHRASES
//

void AbstractCodeBuilder::write_unknown_type()
{
    append_text("UNKNOWN_TYPE");
}

void AbstractCodeBuilder::write_unknown_expr()
{
    append_text("UNKNOWN_EXPRESSION");
}

void AbstractCodeBuilder::write_unknown_stmt()
{
    append_text("UNKNOWN_STATEMENT");
}

void AbstractCodeBuilder::write_invalid_type()
{
    append_text("INVALID_TYPE");
}

void AbstractCodeBuilder::write_invalid_expr()
{
    append_text("INVALID_EXPRESSION");
}

void AbstractCodeBuilder::write_invalid_stmt()
{
    append_text("INVALID_STATEMENT");
}