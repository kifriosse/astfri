#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/utils.hpp>
#include <libastfri-cs/impl/visitor/CSharpTSTreeVisitor.hpp>

namespace astfri::csharp
{

Expr* CSharpTSTreeVisitor::handle_int_lit(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    std::string int_str = extract_node_text(*node, self->source_code_);
    std::erase(int_str, '_');

    const size_t string_len = int_str.length();
    const std::string prefix
        = int_str.substr(0, std::min<size_t>(2, string_len));
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
    default:
        break;
    }

    if (suffix_type == IntSuffix::None || suffix_type == IntSuffix::U)
    {
        // todo add handeling of unsigned integers
        return ExprFactory::get_instance().mk_int_literal(
            std::stoi(int_str, nullptr, base)
        );
    }

    // todo handeling of long and unsigned long
    throw std::logic_error("This integer type is not implemented");
}

Expr* CSharpTSTreeVisitor::handle_float_lit(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    std::string float_str = extract_node_text(*node, self->source_code_);
    const char suffix
        = static_cast<char>(std::tolower(float_str[float_str.length() - 1]));

    float_str.pop_back();
    std::erase(float_str, '_');

    if (! std::isalpha(suffix) || suffix == 'd')
    {
        // todo handle double
        throw std::logic_error(
            "Handling of double floating point numbers is not implemented"
        );
    }

    switch (suffix)
    {
    case 'f':
        return ExprFactory::get_instance().mk_float_literal(
            std::stof(float_str)
        );
    case 'm':
        // decimal - 128-bit precision integer - uses base 10, not base 2
        // todo handle decimal
        throw std::logic_error("Handling of Decimal literal not implemented");
    default:
        throw std::logic_error(
            "Suffix \"" + std::to_string(suffix) + "\" Not Implemented"
        );
    };
}

Expr* CSharpTSTreeVisitor::handle_bool_lit(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const std::string bool_str = extract_node_text(*node, self->source_code_);
    return ExprFactory::get_instance().mk_bool_literal(bool_str == "true");
}

Expr* CSharpTSTreeVisitor::handle_char_lit(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode content_node = ts_node_child(*node, 1);
    const std::string character_str
        = extract_node_text(content_node, self->source_code_);
    if (character_str.length() > 1)
    {
        // todo handle 16-bit unicode characters
        throw std::logic_error("Unicode characters not implemented");
    }
    return ExprFactory::get_instance().mk_char_literal(character_str[0]);
}

Expr* CSharpTSTreeVisitor::handle_str_lit(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    if (ts_node_child_count(*node) < 3)
        return ExprFactory::get_instance().mk_string_literal("");

    const TSNode str_content = ts_node_child(*node, 1);
    const std::string content
        = extract_node_text(str_content, self->source_code_);
    return ExprFactory::get_instance().mk_string_literal(content);
}

Expr* CSharpTSTreeVisitor::handle_null_literal(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] const TSNode* node
)
{
    return ExprFactory::get_instance().mk_null_literal();
}

Expr* CSharpTSTreeVisitor::handle_this_expr(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] const TSNode* node
)
{
    return ExprFactory::get_instance().mk_this();
}

Expr* CSharpTSTreeVisitor::handle_verbatim_str_lit(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    std::string node_contet = extract_node_text(*node, self->source_code_);
    node_contet.pop_back();
    node_contet.erase(node_contet.begin(), node_contet.begin() + 2);
    node_contet = escape_string(node_contet, true);

    return ExprFactory::get_instance().mk_string_literal(node_contet);
}

Expr* CSharpTSTreeVisitor::handle_interpolated_str_lit(
    [[maybe_unused]] CSharpTSTreeVisitor* self,
    [[maybe_unused]] const TSNode* node
)
{
    throw std::logic_error("Interpolated string literal not implemented");
}

Expr* CSharpTSTreeVisitor::handle_param_var_ref_expr(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    // todo this might be not correctly done
    const std::string var_name = extract_node_text(*node, self->source_code_);
    return ExprFactory::get_instance().mk_param_var_ref(var_name);
}

Expr* CSharpTSTreeVisitor::handle_local_var_ref_expr(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const std::string var_name = extract_node_text(*node, self->source_code_);
    return ExprFactory::get_instance().mk_local_var_ref(var_name);
}

Expr* CSharpTSTreeVisitor::handle_prefix_unary_op_expr(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode right_side_node = ts_node_child(*node, 1);
    const TSNode op_node         = ts_node_child(*node, 0);
    std::string op = extract_node_text(op_node, self->source_code_);
    std::erase_if(op, isspace);

    const auto res = NodeRegistry::get_prefix_unary_op(op);
    if (! res.has_value())
    {
        throw std::runtime_error("Operation \"" + op + "\" is not implemented");
    }

    const UnaryOpType op_type = *res;
    const ExprHandler handler = NodeRegistry::get_expr_handler(right_side_node);
    Expr* right_side          = handler(self, &right_side_node);
    return ExprFactory::get_instance().mk_unary_op(op_type, right_side);
}

Expr* CSharpTSTreeVisitor::handle_postfix_unary_op_expr(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode left_side_node = ts_node_child(*node, 0);
    const TSNode op_node        = ts_node_child(*node, 1);
    const std::string op      = extract_node_text(op_node, self->source_code_);

    const ExprHandler handler = NodeRegistry::get_expr_handler(left_side_node);
    Expr* left_side           = handler(self, &left_side_node);

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
    const TSNode* node
)
{
    ExprFactory& expr_factory       = ExprFactory::get_instance();

    const TSNode left               = ts_node_child(*node, 0);
    const TSNode op_node            = ts_node_child(*node, 1);
    const TSNode right              = ts_node_child(*node, 2);
    const ExprHandler left_handler  = NodeRegistry::get_expr_handler(left);
    const ExprHandler right_handler = NodeRegistry::get_expr_handler(right);
    const std::string op = extract_node_text(op_node, self->source_code_);

    const auto res       = NodeRegistry::get_bin_op(op);
    if (! res.has_value())
    {
        // `a ?? b` same as `a != null ? a : b`
        Expr* left_expr      = left_handler(self, &left);
        BinOpExpr* condition = expr_factory.mk_bin_on(
            left_expr,
            BinOpType::NotEqual,
            expr_factory.mk_null_literal()
        );
        IfExpr* ternary = expr_factory.mk_if(
            condition,
            left_expr,
            right_handler(self, &right)
        );
        if (op == "\?\?")
        {
            return ternary;
        }
        if (op == "\?\?=")
        {
            // `a ??= b` is the same as `a = a ?? b` which is the same as `a = a
            // != null ? a : b`
            return expr_factory
                .mk_bin_on(left_expr, BinOpType::Assign, ternary);
        }

        throw std::runtime_error("Operation \"" + op + "\" is not implemented");
    }

    const BinOpType op_type = *res;

    return expr_factory.mk_bin_on(
        left_handler(self, &left),
        op_type,
        right_handler(self, &right)
    );
}

Expr* CSharpTSTreeVisitor::handle_ternary_expr(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode cond_node         = ts_node_child(*node, 0);
    const TSNode if_true           = ts_node_child(*node, 2);
    const TSNode if_false          = ts_node_child(*node, 4);
    const ExprHandler cond_handler = NodeRegistry::get_expr_handler(cond_node);
    const ExprHandler if_true_handler = NodeRegistry::get_expr_handler(if_true);
    const ExprHandler if_false_handler
        = NodeRegistry::get_expr_handler(if_false);

    return ExprFactory::get_instance().mk_if(
        cond_handler(self, &cond_node),
        if_true_handler(self, &if_true),
        if_false_handler(self, &if_false)
    );
}

Expr* CSharpTSTreeVisitor::handle_parenthesized_expr(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    const TSNode expr_node    = ts_node_child(*node, 1);
    const ExprHandler handler = NodeRegistry::get_expr_handler(expr_node);
    return expr_factory_.mk_bracket(handler(self, &expr_node));
}

} // namespace astfri::csharp