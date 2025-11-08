
#include "CSharpTSTreeVisitor.hpp"

#include <algorithm>
#include <iostream>

#include "utils.hpp"

namespace astfri::csharp
{
Expr* CSharpTSTreeVisitor::handler_expr(TSNode const* node)
{
    auto it = this->expr_handlers_.find(ts_node_type(*node));
    if (it != this->expr_handlers_.end())
    {
        return it->second(this, node);
    }
    return ExprFactory::get_instance().mk_unknown();
}

Expr* CSharpTSTreeVisitor::handle_int_literal(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    std::string int_str = extract_node_text(*node, self->source_code_);
    std::erase(int_str, '_');

    size_t string_len = int_str.length();
    const std::string prefix = int_str.substr(0, std::min<size_t>(2, string_len));
    const std::string suffix = int_str.substr(
        string_len >= 2 ? string_len - 2 : 0,
        std::min<size_t>(2, string_len)
    );

    int base = 10;
    if (prefix == "0x")
    {
        base = 16;
    }
    else if (prefix == "0b")
    {
        base = 2;
    }

    const IntSuffix suffix_type = get_suffix_type(suffix);

    switch (suffix_type)
    {
        case IntSuffix::U:
        case IntSuffix::L:
        {
            int_str.pop_back();
            break;
        }
        case IntSuffix::UL:
        {
            int_str.erase(int_str.end() - 2, int_str.end());
            break;
        }
        default: {}
    }

    if (suffix_type == IntSuffix::None || suffix_type == IntSuffix::U)
    {
        //todo add handeling of unsigned integers
        return ExprFactory::get_instance().mk_int_literal(std::stoi(int_str, nullptr, base));
    }

    //todo handeling of long and unsigned long
    throw std::logic_error("This integer type is not implemented");
}

Expr* CSharpTSTreeVisitor::handle_float_literal(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] TSNode const* node
)
{
    std::string float_str = extract_node_text(*node, self->source_code_);
    const char suffix = static_cast<char>(std::tolower(float_str[float_str.length() - 1]));

    float_str.pop_back();
    std::erase(float_str, '_');

    if (!std::isalpha(suffix) || suffix == 'd')
    {
        //todo handle double
        throw std::logic_error("Handling of double floating point numbers is not implemented");
    }

    switch (suffix)
    {
    case 'f':
        return ExprFactory::get_instance().mk_float_literal(std::stof(float_str));
    case 'm':
        // decimal - 128-bit precision integer - used base 10, not base 2
        //todo handle decimal
        throw std::logic_error("Handling of Decimal literal not implemented");
    default: throw std::logic_error("Not Implemented");
    };
}

Expr* CSharpTSTreeVisitor::handle_bool_literal(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    const size_t from = ts_node_start_byte(*node);
    const size_t to = ts_node_end_byte(*node);
    const std::string bool_str = self->source_code_.substr(from, to - from);

    return ExprFactory::get_instance().mk_bool_literal(bool_str == "true");
}

Expr* CSharpTSTreeVisitor::handle_char_literal(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    TSNode content_node = ts_node_child(*node, 1);
    std::string character_str = extract_node_text(content_node, self->source_code_);
    if (character_str.length() > 1)
    {
        throw std::logic_error("Unicode characters not implemented");
    }
    return ExprFactory::get_instance().mk_char_literal(character_str[0]);
}

Expr* CSharpTSTreeVisitor::handle_string_literal(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    if (ts_node_child_count(*node) < 3)
        return ExprFactory::get_instance().mk_string_literal("");

    //todo handle escape sequences

    const TSNode str_content = ts_node_child(*node, 1);
    const std::string content = extract_node_text(str_content, self->source_code_);
    return ExprFactory::get_instance().mk_string_literal(content);
}

Expr* CSharpTSTreeVisitor::handle_null_literal(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] TSNode const* node
)
{
    return ExprFactory::get_instance().mk_null_literal();
}

Expr* CSharpTSTreeVisitor::handle_this_expr(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] TSNode const* node
)
{
    return ExprFactory::get_instance().mk_this();
}

Expr* CSharpTSTreeVisitor::handle_verbatin_string_literal(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] TSNode const* node
)
{
    throw std::logic_error("Verbatim string literal not implemented");
}

Expr* CSharpTSTreeVisitor::handle_interpolated_string_literal(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] TSNode const* node
)
{
    throw std::logic_error("Verbatim string literal not implemented");
}

std::string CSharpTSTreeVisitor::extract_node_text(
    const TSNode& node,
    const std::string& source_code
)
{
    const size_t from = ts_node_start_byte(node);
    const size_t to = ts_node_end_byte(node);
    return source_code.substr(from, to - from);
}

} // namespace astfri::csharp