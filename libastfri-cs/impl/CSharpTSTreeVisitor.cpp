
#include "CSharpTSTreeVisitor.hpp"

#include <algorithm>
#include <iostream>

#include "utils.hpp"

namespace astfri::csharp
{
CSharpTSTreeVisitor::ExprHandler CSharpTSTreeVisitor::get_expr_handler(TSNode const& node)
{
    auto const it = this->expr_handlers_.find(ts_node_type(node));
    if (it != this->expr_handlers_.end())
    {
        return it->second;
    }
    return [](CSharpTSTreeVisitor*, TSNode const*) -> Expr*
    {
        return ExprFactory::get_instance().mk_unknown();
    };
}

Expr* CSharpTSTreeVisitor::handle_int_lit(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    std::string int_str = extract_node_text(*node, self->source_code_);
    std::erase(int_str, '_');

    size_t const string_len = int_str.length();
    std::string const prefix = int_str.substr(0, std::min<size_t>(2, string_len));
    std::string const suffix = int_str.substr(
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

Expr* CSharpTSTreeVisitor::handle_float_lit(
    CSharpTSTreeVisitor* self,
    TSNode const* node
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
    default: throw std::logic_error("Suffix \"" + std::to_string(suffix) + "\" Not Implemented");
    };
}

Expr* CSharpTSTreeVisitor::handle_bool_lit(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    const std::string bool_str = extract_node_text(*node, self->source_code_);
    return ExprFactory::get_instance().mk_bool_literal(bool_str == "true");
}

Expr* CSharpTSTreeVisitor::handle_char_lit(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    TSNode const content_node = ts_node_child(*node, 1);
    std::string const character_str = extract_node_text(content_node, self->source_code_);
    if (character_str.length() > 1)
    {
        //todo handle 16-bit unicode characters
        throw std::logic_error("Unicode characters not implemented");
    }
    return ExprFactory::get_instance().mk_char_literal(character_str[0]);
}

Expr* CSharpTSTreeVisitor::handle_str_lit(
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

Expr* CSharpTSTreeVisitor::handle_verbatim_str_lit(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    std::string node_contet = extract_node_text(*node, self->source_code_);
    node_contet.pop_back();
    node_contet.erase(node_contet.begin(), node_contet.begin() + 2);

    std::cout << node_contet << std::endl;
    return ExprFactory::get_instance().mk_string_literal(node_contet);
}

Expr* CSharpTSTreeVisitor::handle_interpolated_str_lit(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] TSNode const* node
)
{
    throw std::logic_error("Interpolated string literal not implemented");
}

Expr* CSharpTSTreeVisitor::handle_param_var_ref_expr(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    std::string const var_name = extract_node_text(*node, self->source_code_);
    return ExprFactory::get_instance().mk_param_var_ref(var_name);
}

Expr* CSharpTSTreeVisitor::handle_local_var_ref_expr(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    std::string const var_name = extract_node_text(*node, self->source_code_);
    return ExprFactory::get_instance().mk_local_var_ref(var_name);
}

Expr* CSharpTSTreeVisitor::handle_prefix_unary_op_expr(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    TSNode const right_side_node = ts_node_child(*node, 1);
    TSNode const op_node = ts_node_child(*node, 0);
    std::string op = extract_node_text(op_node, self->source_code_);
    std::erase_if(op, isspace);

    auto const it = self->prefix_unary_operations.find(op);
    if (it == self->prefix_unary_operations.end())
    {
        throw std::runtime_error("Operation \"" + op + "\" is not implemented");
    }

    UnaryOpType const op_type = it->second;
    ExprHandler const handler = self->get_expr_handler(right_side_node);
    Expr* right_side = handler(self, &right_side_node);
    return ExprFactory::get_instance().mk_unary_op(op_type, right_side);
}

Expr* CSharpTSTreeVisitor::handle_postfix_unary_op_expr(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    TSNode const left_side_node = ts_node_child(*node, 0);
    TSNode const op_node = ts_node_child(*node, 1);
    std::string const op = extract_node_text(op_node, self->source_code_);

    ExprHandler const handler = self->get_expr_handler(left_side_node);
    Expr* left_side = handler(self, &left_side_node);

    UnaryOpType op_type;
    if (op == "++")
    {
        op_type = UnaryOpType::PostIncrement;
    }
    else if (op == "--")
    {
        op_type = UnaryOpType::PostDecrement;
    }
    else if (op == "!")
    {
        return left_side;
    }
    else
    {
        throw std::runtime_error("Operation \"" + op + "\" is not implemented");
    }

    return ExprFactory::get_instance().mk_unary_op(op_type, left_side);
}

Expr* CSharpTSTreeVisitor::handle_binary_op_expr(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    TSNode const left = ts_node_child(*node, 0);
    TSNode const op_node = ts_node_child(*node, 1);
    TSNode const right = ts_node_child(*node, 2);
    std::string const op = extract_node_text(op_node, self->source_code_);

    auto const it = self->bin_operations.find(op);
    if (it == self->bin_operations.end())
    {
        throw std::runtime_error("Operation \"" + op + "\" is not implemented");
    }
    BinOpType const op_type = it->second;
    ExprHandler const left_handler = self->get_expr_handler(left);
    ExprHandler const right_handler = self->get_expr_handler(right);

    return ExprFactory::get_instance().mk_bin_on(
        left_handler(self, &left),
        op_type,
        right_handler(self, &right)
    );
}

Expr* CSharpTSTreeVisitor::handle_ternary_expr(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] TSNode const* node
)
{
    throw std::logic_error("Handling of Ternary expression is not implemented");
}

CSharpTSTreeVisitor::StmtHandler CSharpTSTreeVisitor::get_stmt_handler(TSNode const* node)
{
    auto const it = this->stmt_handlers_.find(ts_node_type(*node));
    if (it != this->stmt_handlers_.end())
    {
        return it->second;
    }
    return [](CSharpTSTreeVisitor*, TSNode const*) -> Stmt*
    {
        return StmtFactory::get_instance().mk_uknown();
    };
}

} // namespace astfri::csharp