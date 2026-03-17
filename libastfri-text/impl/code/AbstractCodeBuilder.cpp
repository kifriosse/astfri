#include <libastfri-text/inc/code/AbstractCodeBuilder.hpp>

using namespace astfri::text;

//
// GENERAL_TEXT
//

void AbstractCodeBuilder::append_text(const std::string& text) {
    if (isEmptyLine_) {
        for (int i = 0; i < indentationLevel_ * 4; ++i) {
            append_space();
        }
        isEmptyLine_ = false;
    }
    *buildedText_ << text;
}

void AbstractCodeBuilder::append_new_line() {
    *buildedText_ << "\n";
    isEmptyLine_ = true;
}

void AbstractCodeBuilder::append_space() {
    *buildedText_ << " ";
}

void AbstractCodeBuilder::write_separator(const std::string& sep) {
    append_text(sep);
}

void AbstractCodeBuilder::write_right_bracket(const std::string& br) {
    append_text(br);
}

void AbstractCodeBuilder::write_left_bracket(const std::string& br) {
    append_text(br);
}

//
// UNKNOWN_PHRASES
//

void AbstractCodeBuilder::write_unknown_type() {
    append_text("UNKNOWN_TYPE");
}

void AbstractCodeBuilder::write_unknown_expr() {
    append_text("UNKNOWN_EXPRESSION");
}

void AbstractCodeBuilder::write_unknown_stmt() {
    append_text("UNKNOWN_STATEMENT");
}

void AbstractCodeBuilder::write_invalid_type() {
    append_text("INVALID_TYPE");
}

void AbstractCodeBuilder::write_invalid_expr() {
    append_text("INVALID_EXPRESSION");
}

void AbstractCodeBuilder::write_invalid_stmt() {
    append_text("INVALID_STATEMENT");
}

//
// VALUES
//

void AbstractCodeBuilder::write_int_val(const int& val) {
    append_text(std::to_string(val));
}

void AbstractCodeBuilder::write_float_val(const float& val) {
    append_text(std::to_string(val) + "f");
}

void AbstractCodeBuilder::write_char_val(const char& val) {
    append_text("'" + std::string(1, val) + "'");
}

void AbstractCodeBuilder::write_string_val(const std::string& val) {
    append_text("\"" + val + "\"");
}

void AbstractCodeBuilder::write_bool_val(const bool& val) {
    if (val) {
        append_text("true");
    }
    else {
        append_text("false");
    }
}