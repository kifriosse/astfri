#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/AccessType.hpp>
#include <libastfri-cs/impl/data/Identifiers.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/util/common.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri-cs/impl/util/utils.hpp>
#include <libastfri-cs/impl/visitor/SrcCodeVisitor.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <cstring>
#include <stdexcept>
#include <string>

namespace astfri::csharp
{

Expr* SrcCodeVisitor::visit_int_lit(SrcCodeVisitor* self, const TSNode& node)
{
    std::string int_str = util::extract_text(node, self->src_str());
    std::erase(int_str, '_');

    const size_t len         = int_str.length();
    const std::string prefix = int_str.substr(0, std::min<size_t>(2, len));
    const std::string suffix
        = int_str.substr(len >= 2 ? len - 2 : 0, std::min<size_t>(2, len));

    int base = 10;
    if (prefix == "0x")
        base = 16;
    else if (prefix == "0b")
        base = 2;

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
        return expr_f_.mk_int_literal(std::stoi(int_str, nullptr, base));
    }

    // todo handeling of long and unsigned long
    throw std::logic_error("This integer type is not implemented");
}

Expr* SrcCodeVisitor::visit_float_lit(SrcCodeVisitor* self, const TSNode& node)
{
    std::string float_str = util::extract_text(node, self->src_str());
    const char suffix
        = static_cast<char>(std::tolower(float_str[float_str.length() - 1]));

    std::erase(float_str, '_');
    const bool double_suffix = suffix == 'd';
    if (! std::isalpha(suffix) || double_suffix)
    {
        if (double_suffix)
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
        return expr_f_.mk_float_literal(std::stof(float_str));
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

Expr* SrcCodeVisitor::visit_bool_lit(SrcCodeVisitor* self, const TSNode& node)
{
    const std::string bool_str = util::extract_text(node, self->src_str());
    return expr_f_.mk_bool_literal(bool_str == "true");
}

Expr* SrcCodeVisitor::visit_char_lit(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode n_char        = ts_node_named_child(node, 0);
    const std::string char_str = util::extract_text(n_char, self->src_str());
    if (char_str.length() > 1)
    {
        // todo handle 16-bit unicode characters
        throw std::logic_error("Unicode characters not implemented");
    }
    return expr_f_.mk_char_literal(char_str[0]);
}

Expr* SrcCodeVisitor::visit_str_lit(SrcCodeVisitor* self, const TSNode& node)
{
    std::string str{};
    if (ts_node_child_count(node) >= 3)
    {
        const TSNode n_str = ts_node_named_child(node, 0);
        str                = util::extract_text(n_str, self->src_str());
    }
    return expr_f_.mk_string_literal(std::move(str));
}

Expr* SrcCodeVisitor::visit_null_lit(
    [[maybe_unused]] SrcCodeVisitor* self,
    [[maybe_unused]] const TSNode& node
)
{
    return expr_f_.mk_null_literal();
}

Expr* SrcCodeVisitor::visit_this_expr(
    [[maybe_unused]] SrcCodeVisitor* self,
    [[maybe_unused]] const TSNode& node
)
{
    return expr_f_.mk_this();
}

Expr* SrcCodeVisitor::visit_verbatim_str_lit(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    std::string node_contet = util::extract_text(node, self->src_str());
    node_contet.pop_back();
    node_contet.erase(node_contet.begin(), node_contet.begin() + 2);
    node_contet = util::escape_string(node_contet, true);

    return expr_f_.mk_string_literal(std::move(node_contet));
}

Expr* SrcCodeVisitor::visit_raw_str_lit(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode n_content = ts_node_named_child(node, 1);
    std::string content    = util::extract_text(n_content, self->src_str());
    content                = util::escape_string(content, false);
    return expr_f_.mk_string_literal(std::move(content));
}

Expr* SrcCodeVisitor::visit_interpolated_str_lit(
    [[maybe_unused]] SrcCodeVisitor* self,
    [[maybe_unused]] const TSNode& node
)
{
    throw std::logic_error("Interpolated string literal not implemented");
}

Expr* SrcCodeVisitor::visit_identifier(SrcCodeVisitor* self, const TSNode& node)
{
    std::string name = util::extract_text(node, self->src_str());
    Stmt* def_stmt   = self->semantic_context_.find_var(name, access::None{});
    if (! def_stmt)
        return expr_f_.mk_unknown();

    if (is_a<MemberVarDefStmt>(def_stmt)) // todo static handling
        return expr_f_.mk_member_var_ref(expr_f_.mk_this(), std::move(name));
    if (is_a<ParamVarDefStmt>(def_stmt))
        return expr_f_.mk_param_var_ref(std::move(name));
    if (is_a<LocalVarDefStmt>(def_stmt))
        return expr_f_.mk_local_var_ref(std::move(name));

    return expr_f_.mk_unknown();
}

Expr* SrcCodeVisitor::visit_memb_access_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode n_left      = util::child_by_field_name(node, "expression");
    const TSNode n_right     = util::child_by_field_name(node, "name");
    const ExprHandler h_left = RegManager::get_expr_handler(n_left);
    std::string name         = util::extract_text(n_right, self->src_str());
    Expr* left               = h_left(self, n_left);
    if (is_a<ThisExpr>(left))
    {
        // UserTypeDefStmt* owner = self->semantic_context_.current_type()->def;
        if ([[maybe_unused]] const auto var_def = // todo for future use
            self->semantic_context_.find_var(name, access::Instance{}))
        {
            return expr_f_.mk_member_var_ref(left, std::move(name));
        }
    }
    else if ([[maybe_unused]] auto class_ref = as_a<ClassRefExpr>(left))
    {
        // todo static member access handling
    }
    return expr_f_.mk_member_var_ref(left, std::move(name));
}

Expr* SrcCodeVisitor::visit_invoc_expr(SrcCodeVisitor* self, const TSNode& node)
{
    static const TSSymbol identifier_symb
        = util::symbol_for_name(self->lang_, "identifier", true);
    static const TSSymbol memb_access_symb
        = util::symbol_for_name(self->lang_, "member_access_expression", true);
    // std::cout << "Invocation Expression: " << std::endl;
    // print_child_nodes_types(node, self->get_src_code());
    const TSNode n_func         = util::child_by_field_name(node, "function");
    const TSNode n_arg_list     = util::child_by_field_name(node, "arguments");
    std::vector<Expr*> arg_list = self->visit_arg_list(n_arg_list);
    // if it doesn't have a left side - it not a member access node in tree
    // sitter
    if (ts_node_symbol(n_func) == identifier_symb)
    {
        // todo add handling of local functions
        std::string name = util::extract_text(n_func, self->src_str());
        FuncId func_id{
            .name        = std::move(name),
            .param_count = arg_list.size(),
        };
        const CallType invoc_type
            = self->semantic_context_.find_invoc_type(func_id, access::None{});

        // name = util::extract_text(n_func, self->src_str());
        switch (invoc_type)
        {
        case CallType::LocalFunc:
            return expr_f_.mk_function_call(
                std::move(func_id.name),
                std::move(arg_list)
            );
        case CallType::Delegate:
        {
            const ExprHandler h_def = RegManager::get_expr_handler(n_func);
            Expr* left              = h_def(self, n_func);
            return expr_f_.mk_lambda_call(left, std::move(arg_list));
        }
        case CallType::Method:
            return expr_f_.mk_method_call(
                expr_f_.mk_this(),
                std::move(func_id.name),
                std::move(arg_list)
            );
        case CallType::StaticMethod:
        {
            const auto* current = self->semantic_context_.current_type();
            // todo add static method call handling
            return expr_f_.mk_method_call(
                expr_f_.mk_class_ref(current->name_),
                std::move(func_id.name),
                std::move(arg_list)
            );
        }
        default:
            return expr_f_.mk_unknown();
        }
    }
    // accessing a member of some variable - also includes `this`
    if (ts_node_symbol(n_func) == memb_access_symb)
    {
        const TSNode n_name = util::child_by_field_name(n_func, "name");
        const TSNode n_left = util::child_by_field_name(n_func, "expression");
        const ExprHandler h_left = RegManager::get_expr_handler(n_left);
        Expr* left               = h_left(self, n_left);
        std::string name         = util::extract_text(n_name, self->src_str());
        // it's a member of an instance - method or delegate type attribute
        if (is_a<ThisExpr>(left))
        {
            FuncId func_id{
                .name        = std::move(name),
                .param_count = arg_list.size(),
            };

            const CallType invoc_type = self->semantic_context_.find_invoc_type(
                func_id,
                access::Instance{}
            );

            switch (invoc_type)
            {
            case CallType::Delegate:
                return expr_f_.mk_lambda_call(
                    expr_f_.mk_member_var_ref(left, std::move(func_id.name)),
                    std::move(arg_list)
                );
            case CallType::Method:
                return expr_f_.mk_method_call(
                    left,
                    std::move(func_id.name),
                    std::move(arg_list)
                );
            default:
                // todo placeholder
                return expr_f_.mk_method_call(
                    left,
                    std::move(func_id.name),
                    std::move(arg_list)
                );
            }
        }
        // todo accessing of base members

        // todo accessing of static members left side is a Usertype Reference
        return expr_f_
            .mk_method_call(left, std::move(name), std::move(arg_list));
    }
    // todo add anonymous lambda call
    // left side is a anonymous lambda
    // if (is_anonymou_lambda(function_node))
    // {
    //
    // }

    return expr_f_.mk_unknown();
}

Expr* SrcCodeVisitor::visit_prefix_unary_op_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode n_op    = ts_node_child(node, 0);
    const TSNode n_right = ts_node_child(node, 1);
    const std::string op = util::extract_text(n_op, self->src_str());
    // std::erase_if(op, isspace);

    const auto res = RegManager::get_prefix_unary_op(op);
    if (! res)
        throw std::runtime_error("Operation \"" + op + "\" is not implemented");

    const UnaryOpType op_type = *res;
    const ExprHandler h_right = RegManager::get_expr_handler(n_right);
    Expr* right               = h_right(self, n_right);
    return expr_f_.mk_unary_op(op_type, right);
}

Expr* SrcCodeVisitor::visit_postfix_unary_op_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode n_left      = ts_node_child(node, 0);
    const TSNode n_op        = ts_node_child(node, 1);
    const std::string op     = util::extract_text(n_op, self->src_str());
    const ExprHandler h_left = RegManager::get_expr_handler(n_left);
    Expr* left_side          = h_left(self, n_left);

    UnaryOpType op_type;
    if (op == "++")
        op_type = UnaryOpType::PostIncrement;
    else if (op == "--")
        op_type = UnaryOpType::PostDecrement;
    else if (op == "!")
        return left_side;
    else
        throw std::runtime_error(
            "Unary prefix operation \"" + op + "\" is not implemented"
        );

    return expr_f_.mk_unary_op(op_type, left_side);
}

Expr* SrcCodeVisitor::visit_ref_expr(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode n_expr = ts_node_named_child(node, 0);
    Expr* expr          = RegManager::get_expr_handler(n_expr)(self, n_expr);
    return expr_f_.mk_unary_op(UnaryOpType::AddressOf, expr);
}

Expr* SrcCodeVisitor::visit_binary_op_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode n_left       = ts_node_named_child(node, 0);
    const TSNode n_op         = ts_node_child(node, 1);
    const TSNode n_right      = ts_node_named_child(node, 1);
    const ExprHandler h_left  = RegManager::get_expr_handler(n_left);
    const ExprHandler h_right = RegManager::get_expr_handler(n_right);
    const std::string op      = util::extract_text(n_op, self->src_str());

    const auto op_opt         = RegManager::get_bin_op(op);
    if (! op_opt)
    {
        // `a ?? b` same as `a != null ? a : b`
        Expr* left      = h_left(self, n_left);
        BinOpExpr* cond = expr_f_.mk_bin_on(
            left,
            BinOpType::NotEqual,
            expr_f_.mk_null_literal()
        );
        IfExpr* ternary = expr_f_.mk_if(cond, left, h_right(self, n_right));
        if (op == "\?\?")
        {
            return ternary;
        }
        if (op == "\?\?=")
        {
            // `a ??= b` is the same as `a = a ?? b` which is the same as `a = a
            // != null ? a : b`
            return expr_f_.mk_bin_on(left, BinOpType::Assign, ternary);
        }

        throw std::runtime_error("Operation \"" + op + "\" is not implemented");
    }

    return expr_f_
        .mk_bin_on(h_left(self, n_left), *op_opt, h_right(self, n_right));
}

Expr* SrcCodeVisitor::visit_ternary_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode n_cond       = ts_node_named_child(node, 0);
    const TSNode n_true       = ts_node_named_child(node, 1);
    const TSNode n_false      = ts_node_named_child(node, 2);
    const ExprHandler h_cond  = RegManager::get_expr_handler(n_cond);
    const ExprHandler h_true  = RegManager::get_expr_handler(n_true);
    const ExprHandler h_false = RegManager::get_expr_handler(n_false);

    return expr_f_.mk_if(
        h_cond(self, n_cond),
        h_true(self, n_true),
        h_false(self, n_false)
    );
}

Expr* SrcCodeVisitor::visit_parenthesized_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode n_expr      = ts_node_named_child(node, 0);
    const ExprHandler h_expr = RegManager::get_expr_handler(n_expr);
    return expr_f_.mk_bracket(h_expr(self, n_expr));
}

Expr* SrcCodeVisitor::visit_const_pattern(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode n_inside = ts_node_child(node, 0);
    return RegManager::get_expr_handler(n_inside)(self, n_inside);
}

} // namespace astfri::csharp