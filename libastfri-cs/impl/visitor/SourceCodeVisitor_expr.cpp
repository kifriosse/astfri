#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/utils.hpp>
#include <libastfri-cs/impl/visitor/SourceCodeVisitor.hpp>

#include <cstring>
#include <iostream>

namespace astfri::csharp
{

Expr* SourceCodeVisitor::handle_int_lit(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    std::string int_str = util::extract_node_text(*node, self->get_src_code());
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

    const util::IntSuffix suffix_type = util::get_suffix_type(suffix);

    switch (suffix_type)
    {
    case util::IntSuffix::U:
    case util::IntSuffix::L:
    {
        int_str.pop_back();
        break;
    }
    case util::IntSuffix::UL:
    {
        int_str.erase(int_str.end() - 2, int_str.end());
        break;
    }
    default:
        break;
    }

    if (suffix_type == util::IntSuffix::None
        || suffix_type == util::IntSuffix::U)
    {
        // todo add handeling of unsigned integers
        return expr_factory_.mk_int_literal(std::stoi(int_str, nullptr, base));
    }

    // todo handeling of long and unsigned long
    throw std::logic_error("This integer type is not implemented");
}

Expr* SourceCodeVisitor::handle_float_lit(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    std::string float_str
        = util::extract_node_text(*node, self->get_src_code());
    const char suffix
        = static_cast<char>(std::tolower(float_str[float_str.length() - 1]));

    std::erase(float_str, '_');
    const bool has_double_suffix = suffix == 'd';
    if (! std::isalpha(suffix) || has_double_suffix)
    {
        if (has_double_suffix)
            float_str.pop_back();

        // todo handle double
        throw std::logic_error(
            "Handling of double floating point numbers is not implemented"
        );
    }

    float_str.pop_back();
    switch (suffix)
    {
    case 'f':
        return expr_factory_.mk_float_literal(std::stof(float_str));
    case 'm':
        // decimal - 128-bit precision integer - uses base 10, not base 2
        // todo handle decimal
        throw std::logic_error("Handling of Decimal literal not implemented");
    default:
        throw std::logic_error(
            "Suffix \"" + std::to_string(suffix) + "\" Not Implemented"
        );
    }
}

Expr* SourceCodeVisitor::handle_bool_lit(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    const std::string bool_str
        = util::extract_node_text(*node, self->get_src_code());
    return expr_factory_.mk_bool_literal(bool_str == "true");
}

Expr* SourceCodeVisitor::handle_char_lit(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode content_node = ts_node_child(*node, 1);
    const std::string character_str
        = util::extract_node_text(content_node, self->get_src_code());
    if (character_str.length() > 1)
    {
        // todo handle 16-bit unicode characters
        throw std::logic_error("Unicode characters not implemented");
    }
    return expr_factory_.mk_char_literal(character_str[0]);
}

Expr* SourceCodeVisitor::handle_str_lit(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    if (ts_node_child_count(*node) < 3)
        return expr_factory_.mk_string_literal("");

    const TSNode str_content = ts_node_child(*node, 1);
    const std::string content
        = util::extract_node_text(str_content, self->get_src_code());
    return expr_factory_.mk_string_literal(content);
}

Expr* SourceCodeVisitor::handle_null_literal(
    [[maybe_unused]] SourceCodeVisitor* self,
    [[maybe_unused]] const TSNode* node
)
{
    return expr_factory_.mk_null_literal();
}

Expr* SourceCodeVisitor::handle_this_expr(
    [[maybe_unused]] SourceCodeVisitor* self,
    [[maybe_unused]] const TSNode* node
)
{
    return expr_factory_.mk_this();
}

Expr* SourceCodeVisitor::handle_verbatim_str_lit(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    std::string node_contet
        = util::extract_node_text(*node, self->get_src_code());
    node_contet.pop_back();
    node_contet.erase(node_contet.begin(), node_contet.begin() + 2);
    node_contet = util::escape_string(node_contet, true);

    return expr_factory_.mk_string_literal(node_contet);
}

Expr* SourceCodeVisitor::handle_raw_str_lit(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode content_node = ts_node_child(*node, 1);
    std::string content
        = util::extract_node_text(content_node, self->get_src_code());
    content = util::escape_string(content, false);
    return expr_factory_.mk_string_literal(content);
}

Expr* SourceCodeVisitor::handle_interpolated_str_lit(
    [[maybe_unused]] SourceCodeVisitor* self,
    [[maybe_unused]] const TSNode* node
)
{
    throw std::logic_error("Interpolated string literal not implemented");
}

Expr* SourceCodeVisitor::handle_identifier(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    const std::string identifier
        = util::extract_node_text(*node, self->get_src_code());
    Stmt* def_stmt
        = self->semantic_context_.find_var(identifier, AccessType::None);
    if (! def_stmt)
        return expr_factory_.mk_unknown();

    if (is_a<MemberVarDefStmt>(def_stmt))
        return expr_factory_.mk_member_var_ref(
            expr_factory_.mk_this(),
            identifier
        );
    if (is_a<ParamVarDefStmt>(def_stmt))
        return expr_factory_.mk_param_var_ref(identifier);
    if (is_a<LocalVarDefStmt>(def_stmt))
        return expr_factory_.mk_local_var_ref(identifier);

    return expr_factory_.mk_unknown();
}

Expr* SourceCodeVisitor::handle_memb_access_expr(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode left_side_node
        = ts_node_child_by_field_name(*node, "expression", 10);
    const TSNode right_node = ts_node_child_by_field_name(*node, "name", 4);
    const ExprHandler left_handler
        = RegManager::get_expr_handler(left_side_node);
    const std::string name
        = util::extract_node_text(right_node, self->get_src_code());
    Expr* left_side = left_handler(self, &left_side_node);
    if (is_a<ThisExpr>(left_side))
    {
        UserTypeDefStmt* owner = self->semantic_context_.current_type();
        if (const auto member_var_data = // todo for future use
            self->semantic_context_.get_memb_var_data(name, owner))
        {
            return expr_factory_.mk_member_var_ref(left_side, name);
        }
    }
    return expr_factory_.mk_member_var_ref(left_side, name);
}

Expr* SourceCodeVisitor::handle_invocation_expr(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    static const TSSymbol identifier_node_symb = ts_language_symbol_for_name(
        self->get_lang(),
        "identifier",
        std::strlen("identifier"),
        true
    );
    static const TSSymbol memb_access_node_symb = ts_language_symbol_for_name(
        self->get_lang(),
        "member_access_expression",
        std::strlen("member_access_expression"),
        true
    );
    // std::cout << "Invocation Expression: " << std::endl;
    // print_child_nodes_types(*node, self->get_src_code());
    const TSNode function_node
        = ts_node_child_by_field_name(*node, "function", 8);
    const TSNode arg_list_node
        = ts_node_child_by_field_name(*node, "arguments", strlen("arguments"));
    const std::vector<Expr*> arg_list
        = handle_argument_list(self, &arg_list_node);
    // if it doesn't have a left side - it not a member access node in tree
    // sitter
    if (ts_node_symbol(function_node) == identifier_node_symb)
    {
        // todo add handling of local functions
        const std::string name
            = util::extract_node_text(function_node, self->get_src_code());
        ThisExpr* this_expr = expr_factory_.mk_this();
        if (self->semantic_context_.find_var(name, AccessType::None))
        {
            const ExprHandler var_def_handler
                = RegManager::get_expr_handler(function_node);
            Expr* left = var_def_handler(self, &function_node);
            return expr_factory_.mk_lambda_call(left, arg_list);
        }
        // todo add static invocation handling
        return expr_factory_.mk_method_call(this_expr, name, arg_list);
    }
    // accessing a member of some variable - also includes `this`
    if (ts_node_symbol(function_node) == memb_access_node_symb)
    {
        // todo fix function call, method call, and lambda call distinction
        const TSNode name_node
            = ts_node_child_by_field_name(function_node, "name", 4);
        const TSNode left_node
            = ts_node_child_by_field_name(function_node, "expression", 10);
        const ExprHandler left_handler
            = RegManager::get_expr_handler(left_node);
        Expr* left_side = left_handler(self, &left_node);
        const std::string name
            = util::extract_node_text(name_node, self->get_src_code());
        // it's a member of an instance - method or delegate type attribute
        if (is_a<ThisExpr>(left_side))
        {
            UserTypeDefStmt* owner = self->semantic_context_.current_type();
            // todo for future use
            if (const auto* var_def_data
                = self->semantic_context_.get_memb_var_data(name, owner))
            {
                return expr_factory_.mk_lambda_call(
                    expr_factory_.mk_member_var_ref(left_side, name),
                    arg_list
                );
            }
            // todo when references to methods are going to be added into method
            // calls
            return expr_factory_.mk_method_call(left_side, name, arg_list);
        }

        // todo accessing of static members left side is a Usertype Reference
        return expr_factory_.mk_method_call(left_side, name, arg_list);
    }
    // todo add anonymous lambda call
    // left side is a anonymous lambda
    // if (is_anonymou_lambda(function_node))
    // {
    //
    // }

    return expr_factory_.mk_unknown();
}

Expr* SourceCodeVisitor::handle_prefix_unary_op_expr(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode right_side_node = ts_node_child(*node, 1);
    const TSNode op_node         = ts_node_child(*node, 0);
    std::string op = util::extract_node_text(op_node, self->get_src_code());
    std::erase_if(op, isspace);

    const auto res = RegManager::get_prefix_unary_op(op);
    if (! res.has_value())
    {
        throw std::runtime_error("Operation \"" + op + "\" is not implemented");
    }

    const UnaryOpType op_type = *res;
    const ExprHandler handler = RegManager::get_expr_handler(right_side_node);
    Expr* right_side          = handler(self, &right_side_node);
    return expr_factory_.mk_unary_op(op_type, right_side);
}

Expr* SourceCodeVisitor::handle_postfix_unary_op_expr(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode left_side_node = ts_node_child(*node, 0);
    const TSNode op_node        = ts_node_child(*node, 1);
    const std::string op
        = util::extract_node_text(op_node, self->get_src_code());

    const ExprHandler handler = RegManager::get_expr_handler(left_side_node);
    Expr* left_side           = handler(self, &left_side_node);

    UnaryOpType op_type;
    if (op == "++")
        op_type = UnaryOpType::PostIncrement;
    else if (op == "--")
        op_type = UnaryOpType::PostDecrement;
    else if (op == "!")
        return left_side;
    else
        throw std::runtime_error("Operation \"" + op + "\" is not implemented");

    return expr_factory_.mk_unary_op(op_type, left_side);
}

Expr* SourceCodeVisitor::handle_binary_op_expr(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    ExprFactory& expr_factory       = ExprFactory::get_instance();

    const TSNode left               = ts_node_child(*node, 0);
    const TSNode op_node            = ts_node_child(*node, 1);
    const TSNode right              = ts_node_child(*node, 2);
    const ExprHandler left_handler  = RegManager::get_expr_handler(left);
    const ExprHandler right_handler = RegManager::get_expr_handler(right);
    const std::string op
        = util::extract_node_text(op_node, self->get_src_code());

    const auto res = RegManager::get_bin_op(op);
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

Expr* SourceCodeVisitor::handle_ternary_expr(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode cond_node            = ts_node_child(*node, 0);
    const TSNode if_true              = ts_node_child(*node, 2);
    const TSNode if_false             = ts_node_child(*node, 4);
    const ExprHandler cond_handler    = RegManager::get_expr_handler(cond_node);
    const ExprHandler if_true_handler = RegManager::get_expr_handler(if_true);
    const ExprHandler if_false_handler = RegManager::get_expr_handler(if_false);

    return expr_factory_.mk_if(
        cond_handler(self, &cond_node),
        if_true_handler(self, &if_true),
        if_false_handler(self, &if_false)
    );
}

Expr* SourceCodeVisitor::handle_parenthesized_expr(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode expr_node    = ts_node_child(*node, 1);
    const ExprHandler handler = RegManager::get_expr_handler(expr_node);
    return expr_factory_.mk_bracket(handler(self, &expr_node));
}

Expr* SourceCodeVisitor::handle_const_pattern(
    SourceCodeVisitor* self,
    const TSNode* node
)
{
    const TSNode inside_node = ts_node_child(*node, 0);
    return RegManager::get_expr_handler(inside_node)(self, &inside_node);
}

} // namespace astfri::csharp