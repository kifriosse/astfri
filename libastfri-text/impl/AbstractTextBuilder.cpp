#include <libastfri-text/inc/AbstractTextBuilder.hpp>

using namespace astfri::text;

AbstractTextBuilder::AbstractTextBuilder()
{
    reset_builder();
}

AbstractTextBuilder::~AbstractTextBuilder() = default;

//
// SET_UP
//

void AbstractTextBuilder::reset_builder()
{
    indentationLevel_ = 0;
    isEmptyLine_ = true;
    buildedText_ = std::make_unique<std::stringstream>();
}

void AbstractTextBuilder::increase_indentation()
{
    ++indentationLevel_;
}

void AbstractTextBuilder::decrease_indentation()
{
    --indentationLevel_;
}

//
// GENERAL_TEXT
//

void AbstractTextBuilder::append_text(std::string const& text)
{
    if (isEmptyLine_) {
        for (int i = 0; i < indentationLevel_ * 4; ++i) {
            append_space();
        }
        isEmptyLine_ = false;
    }
    *buildedText_ << text;
}

void AbstractTextBuilder::append_new_line()
{
    *buildedText_ << "\n";
    isEmptyLine_ = true;
}

void AbstractTextBuilder::append_space()
{
    *buildedText_ << " ";
}

//
// BRACKETS, SEPARATORS, OPERATORS
//

void AbstractTextBuilder::write_right_bracket(std::string const& br)
{
    append_text(br);
}

void AbstractTextBuilder::write_left_bracket(std::string const& br)
{
    append_text(br);
}

void AbstractTextBuilder::write_separator(std::string const& sep)
{
    append_text(sep);
}

void AbstractTextBuilder::write_operator(std::string const& op)
{
    append_text(op);
}

void AbstractTextBuilder::write_assign_operator()
{
    append_text("=");
}

void AbstractTextBuilder::write_modulo_operator()
{
    append_text("%");
}

//
// UNKNOWN_PHRASES
//

void AbstractTextBuilder::write_unknown_type()
{
    append_text("UNKNOWN_TYPE");
}

void AbstractTextBuilder::write_unknown_expr()
{
    append_text("UNKNOWN_EXPRESSION");
}

void AbstractTextBuilder::write_unknown_stmt()
{
    append_text("UNKNOWN_STATEMENT");
}

void AbstractTextBuilder::write_invalid_type()
{
    append_text("INVALID_TYPE");
}

void AbstractTextBuilder::write_invalid_expr()
{
    append_text("INVALID_EXPRESSION");
}

void AbstractTextBuilder::write_invalid_stmt()
{
    append_text("INVALID_STATEMENT");
}

//
// ACCESS_MODIFIERS
//

void AbstractTextBuilder::write_public_word()
{
    append_text("public");
}

void AbstractTextBuilder::write_protected_word()
{
    append_text("protected");
}

void AbstractTextBuilder::write_private_word()
{
    append_text("private");
}

//
// DATA_TYPES
//

void AbstractTextBuilder::write_int_type_word()
{
    append_text("int");
}

void AbstractTextBuilder::write_char_type_word()
{
    append_text("char");
}

void AbstractTextBuilder::write_void_type_word()
{
    append_text("void");
}

void AbstractTextBuilder::write_class_type(std::string const& name)
{
    append_text(name);
}

void AbstractTextBuilder::write_interface_type(std::string const& name)
{
    append_text(name);
}

//
// REFERENCE_NAMES
//

void AbstractTextBuilder::write_scope_name(std::string const& name)
{
    append_text(name);
}

void AbstractTextBuilder::write_gen_param_name(std::string const& name)
{
    append_text(name);
}

void AbstractTextBuilder::write_class_name(std::string const& name)
{
    append_text(name);
}

void AbstractTextBuilder::write_interface_name(std::string const& name)
{
    append_text(name);
}

void AbstractTextBuilder::write_method_name(std::string const& name)
{
    append_text(name);
}

void AbstractTextBuilder::write_function_name(std::string const& name)
{
    append_text(name);
}

void AbstractTextBuilder::write_global_var_name(std::string const& name)
{
    append_text(name);
}

void AbstractTextBuilder::write_member_var_name(std::string const& name)
{
    append_text(name);
}

void AbstractTextBuilder::write_local_var_name(std::string const& name)
{
    append_text(name);
}

void AbstractTextBuilder::write_param_var_name(std::string const& name)
{
    append_text(name);
}

//
// VALUES
//

void AbstractTextBuilder::write_int_val(int const& val)
{
    append_text(std::to_string(val));
}

void AbstractTextBuilder::write_float_val(float const& val)
{
    append_text(std::to_string(val) + "f");
}

void AbstractTextBuilder::write_char_val(char const& val)
{
    append_text("'" + std::string(1, val) + "'");
}

void AbstractTextBuilder::write_string_val(std::string const& val)
{
    append_text("\"" + val + "\"");
}

void AbstractTextBuilder::write_bool_val(bool const& val)
{
    if (val) {
        append_text("true");
    } else {
        append_text("false");
    }
}

//
// SYSTEM_EXPRESSIONS
//

void AbstractTextBuilder::write_class_word()
{
    append_text("class");
}

void AbstractTextBuilder::write_this_word()
{
    append_text("this");
}

void AbstractTextBuilder::write_return_word()
{
    append_text("return");
}

void AbstractTextBuilder::write_continue_word()
{
    append_text("continue");
}

void AbstractTextBuilder::write_break_word()
{
    append_text("break");
}

void AbstractTextBuilder::write_throw_word()
{
    append_text("throw");
}

void AbstractTextBuilder::write_if_word()
{
    append_text("if");
}

void AbstractTextBuilder::write_else_word()
{
    append_text("else");
}

void AbstractTextBuilder::write_do_word()
{
    append_text("do");
}

void AbstractTextBuilder::write_while_word()
{
    append_text("while");
}

void AbstractTextBuilder::write_for_word()
{
    append_text("for");
}

void AbstractTextBuilder::write_switch_word()
{
    append_text("switch");
}

void AbstractTextBuilder::write_case_word()
{
    append_text("case");
}

void AbstractTextBuilder::write_default_word()
{
    append_text("default");
}

void AbstractTextBuilder::write_new_word()
{
    append_text("new");
}

void AbstractTextBuilder::write_override_word()
{
    append_text("override");
}