#include <libastfri-cs/impl/CSAliases.hpp>
#include <libastfri-cs/impl/data/AccessType.hpp>
#include <libastfri-cs/impl/data/Identifiers.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/util/Common.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/util/Utils.hpp>
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
    std::string intStr = util::extract_text(node, self->src_str());
    std::erase(intStr, '_');

    const size_t len         = intStr.length();
    const std::string prefix = intStr.substr(0, std::min<size_t>(2, len));
    const std::string suffix
        = intStr.substr(len >= 2 ? len - 2 : 0, std::min<size_t>(2, len));

    int base = 10;
    if (prefix == "0x")
        base = 16;
    else if (prefix == "0b")
        base = 2;

    const util::IntSuffix suffixType = util::get_suffix_type(suffix);

    switch (suffixType)
    {
    case util::IntSuffix::U:
    case util::IntSuffix::L:
    {
        intStr.pop_back();
        break;
    }
    case util::IntSuffix::UL:
    {
        intStr.erase(intStr.end() - 2, intStr.end());
        break;
    }
    default:
        break;
    }

    if (suffixType == util::IntSuffix::None || suffixType == util::IntSuffix::U)
    {
        // todo add handeling of unsigned integers
        return exprFact_.mk_int_literal(std::stoi(intStr, nullptr, base));
    }

    // todo handeling of long and unsigned long
    throw std::logic_error("This integer type is not implemented");
}

Expr* SrcCodeVisitor::visit_float_lit(SrcCodeVisitor* self, const TSNode& node)
{
    std::string floatStr = util::extract_text(node, self->src_str());
    const char suffix
        = static_cast<char>(std::tolower(floatStr[floatStr.length() - 1]));

    std::erase(floatStr, '_');
    const bool doubleSuffix = suffix == 'd';
    if (! std::isalpha(suffix) || doubleSuffix)
    {
        if (doubleSuffix)
            floatStr.pop_back();

        // todo handle double
        throw std::logic_error(
            "Handling of double floating point numbers is not implemented"
        );
    }

    floatStr.pop_back();
    switch (suffix)
    {
    case 'f':
        return exprFact_.mk_float_literal(std::stof(floatStr));
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
    const std::string boolStr = util::extract_text(node, self->src_str());
    return exprFact_.mk_bool_literal(boolStr == "true");
}

Expr* SrcCodeVisitor::visit_char_lit(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode nChar        = ts_node_named_child(node, 0);
    const std::string charStr = util::extract_text(nChar, self->src_str());
    if (charStr.length() > 1)
    {
        // todo handle 16-bit unicode characters
        throw std::logic_error("Unicode characters not implemented");
    }
    return exprFact_.mk_char_literal(charStr[0]);
}

Expr* SrcCodeVisitor::visit_str_lit(SrcCodeVisitor* self, const TSNode& node)
{
    std::string str{};
    if (ts_node_child_count(node) >= 3)
    {
        const TSNode nStr = ts_node_named_child(node, 0);
        str               = util::extract_text(nStr, self->src_str());
    }
    return exprFact_.mk_string_literal(str);
}

Expr* SrcCodeVisitor::visit_null_lit(
    [[maybe_unused]] SrcCodeVisitor* self,
    [[maybe_unused]] const TSNode& node
)
{
    return exprFact_.mk_null_literal();
}

Expr* SrcCodeVisitor::visit_this_expr(
    [[maybe_unused]] SrcCodeVisitor* self,
    [[maybe_unused]] const TSNode& node
)
{
    return exprFact_.mk_this();
}

Expr* SrcCodeVisitor::visit_verbatim_str_lit(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    std::string nContent = util::extract_text(node, self->src_str());
    nContent.pop_back();
    nContent.erase(nContent.begin(), nContent.begin() + 2);
    nContent = util::escape_string(nContent, true);

    return exprFact_.mk_string_literal(nContent);
}

Expr* SrcCodeVisitor::visit_raw_str_lit(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode nContent = ts_node_named_child(node, 1);
    std::string content   = util::extract_text(nContent, self->src_str());
    content               = util::escape_string(content, false);
    return exprFact_.mk_string_literal(content);
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
    Stmt* defStmt    = self->semanticContext_.find_var(name, access::None{});
    if (! defStmt)
        return exprFact_.mk_unknown();

    if (is_a<MemberVarDefStmt>(defStmt)) // todo static handling
        return exprFact_.mk_member_var_ref(
            exprFact_.mk_this(),
            std::move(name)
        );
    if (is_a<ParamVarDefStmt>(defStmt))
        return exprFact_.mk_param_var_ref(std::move(name));
    if (is_a<LocalVarDefStmt>(defStmt))
        return exprFact_.mk_local_var_ref(std::move(name));

    return exprFact_.mk_unknown();
}

Expr* SrcCodeVisitor::visit_memb_access_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode nLeft      = util::child_by_field_name(node, "expression");
    const TSNode nRight     = util::child_by_field_name(node, "name");
    const ExprHandler hLeft = RegManager::get_expr_handler(nLeft);
    std::string name        = util::extract_text(nRight, self->src_str());
    Expr* left              = hLeft(self, nLeft);
    if (is_a<ThisExpr>(left))
    {
        // UserTypeDefStmt* owner = self->semantic_context_.current_type()->def;
        if ([[maybe_unused]] const auto varDef = // todo for future use
            self->semanticContext_.find_var(name, access::Instance{}))
        {
            return exprFact_.mk_member_var_ref(left, std::move(name));
        }
    }
    else if ([[maybe_unused]] auto classRef = as_a<ClassRefExpr>(left))
    {
        // todo static member access handling
    }
    // todo generic member access
    // return exprFact_.mk_member_var_ref(left, std::move(name));
    return exprFact_.mk_unknown();
}

Expr* SrcCodeVisitor::visit_invoc_expr(SrcCodeVisitor* self, const TSNode& node)
{
    static const TSSymbol sIndentifier
        = util::symbol_for_name("identifier", true);
    static const TSSymbol sMembAccess
        = util::symbol_for_name("member_access_expression", true);
    // std::cout << "Invocation Expression: " << std::endl;
    // print_child_nodes_types(node, self->get_src_code());
    const TSNode nFunc         = util::child_by_field_name(node, "function");
    const TSNode nArgList      = util::child_by_field_name(node, "arguments");
    std::vector<Expr*> argList = self->visit_arg_list(nArgList);
    // if it doesn't have a left side - it not a member access node in tree
    // sitter
    if (ts_node_symbol(nFunc) == sIndentifier)
    {
        // todo add handling of local functions
        std::string name = util::extract_text(nFunc, self->src_str());
        InvocationId id{
            .name       = name,
            .paramCount = argList.size(),
        };
        const CallType invocType = self->semanticContext_.find_invoc_type(
            std::move(id),
            access::None{}
        );

        // name = util::extract_text(n_func, self->src_str());
        switch (invocType)
        {
        case CallType::LocalFunc:
            return exprFact_.mk_function_call(
                std::move(name),
                std::move(argList)
            );
        case CallType::Delegate:
        {
            const ExprHandler hLeft = RegManager::get_expr_handler(nFunc);
            Expr* left              = hLeft(self, nFunc);
            return exprFact_.mk_lambda_call(left, std::move(argList));
        }
        case CallType::Method:
            return exprFact_.mk_method_call(
                exprFact_.mk_this(),
                std::move(name),
                std::move(argList)
            );
        case CallType::StaticMethod:
        {
            const auto* current = self->semanticContext_.current_type();
            // todo add static method call handling
            return exprFact_.mk_method_call(
                exprFact_.mk_class_ref(current->name_),
                std::move(name),
                std::move(argList)
            );
        }
        default:
            return exprFact_.mk_unknown();
        }
    }
    // accessing a member of some variable - also includes `this`
    if (ts_node_symbol(nFunc) == sMembAccess)
    {
        const TSNode nName = util::child_by_field_name(nFunc, "name");
        const TSNode nLeft = util::child_by_field_name(nFunc, "expression");
        const ExprHandler hLeft = RegManager::get_expr_handler(nLeft);
        Expr* left              = hLeft(self, nLeft);
        std::string name        = util::extract_text(nName, self->src_str());
        // it's a member of an instance - method or delegate type attribute
        if (is_a<ThisExpr>(left))
        {
            InvocationId id{
                .name       = name,
                .paramCount = argList.size(),
            };

            const CallType invocType = self->semanticContext_.find_invoc_type(
                std::move(id),
                access::Instance{}
            );

            switch (invocType)
            {
            case CallType::Delegate:
                return exprFact_.mk_lambda_call(
                    exprFact_.mk_member_var_ref(left, std::move(name)),
                    std::move(argList)
                );
            case CallType::Method:
                return exprFact_
                    .mk_method_call(left, std::move(name), std::move(argList));
            default:
                // todo placeholder
                return exprFact_.mk_unknown();
            }
        }
        // todo accessing of base members

        // todo accessing of static members left side is a Usertype Reference
        return exprFact_.mk_unknown();
    }
    // left side is a anonymous lambda
    TSNode nLambda;
    TSNode nDelegate;
    if (util::is_anonymous_lambda(nFunc, &nLambda, &nDelegate))
    {
        // todo add anonymous lambda call
        return exprFact_.mk_lambda_call(
            exprFact_.mk_unknown(),
            std::move(argList)
        );
    }

    return exprFact_.mk_unknown();
}

Expr* SrcCodeVisitor::visit_prefix_unary_op_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode nOp     = ts_node_child(node, 0);
    const TSNode nRight  = ts_node_child(node, 1);
    const std::string op = util::extract_text(nOp, self->src_str());
    // std::erase_if(op, isspace);

    const auto res = RegManager::get_prefix_unary_op(op);
    if (! res)
        throw std::runtime_error("Operation \"" + op + "\" is not implemented");

    const UnaryOpType opType = *res;
    const ExprHandler hRight = RegManager::get_expr_handler(nRight);
    Expr* right              = hRight(self, nRight);
    return exprFact_.mk_unary_op(opType, right);
}

Expr* SrcCodeVisitor::visit_postfix_unary_op_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode nLeft      = ts_node_child(node, 0);
    const TSNode nOp        = ts_node_child(node, 1);
    const std::string op    = util::extract_text(nOp, self->src_str());
    const ExprHandler hLeft = RegManager::get_expr_handler(nLeft);
    Expr* left              = hLeft(self, nLeft);

    UnaryOpType opType;
    if (op == "++")
        opType = UnaryOpType::PostIncrement;
    else if (op == "--")
        opType = UnaryOpType::PostDecrement;
    else if (op == "!")
        return left;
    else
        throw std::runtime_error(
            "Unary prefix operation \"" + op + "\" is not implemented"
        );

    return exprFact_.mk_unary_op(opType, left);
}

Expr* SrcCodeVisitor::visit_ref_expr(SrcCodeVisitor* self, const TSNode& node)
{
    const TSNode nExpr = ts_node_named_child(node, 0);
    Expr* expr         = RegManager::get_expr_handler(nExpr)(self, nExpr);
    return exprFact_.mk_unary_op(UnaryOpType::AddressOf, expr);
}

Expr* SrcCodeVisitor::visit_binary_op_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode nLeft       = ts_node_named_child(node, 0);
    const TSNode nOp         = ts_node_child(node, 1);
    const TSNode nRight      = ts_node_named_child(node, 1);
    const ExprHandler hLeft  = RegManager::get_expr_handler(nLeft);
    const ExprHandler hRight = RegManager::get_expr_handler(nRight);
    const std::string op     = util::extract_text(nOp, self->src_str());

    const auto opOpt         = RegManager::get_bin_op(op);
    if (! opOpt)
    {
        // `a ?? b` same as `a != null ? a : b`
        Expr* left      = hLeft(self, nLeft);
        BinOpExpr* cond = exprFact_.mk_bin_on(
            left,
            BinOpType::NotEqual,
            exprFact_.mk_null_literal()
        );
        IfExpr* ternary = exprFact_.mk_if(cond, left, hRight(self, nRight));
        if (op == "\?\?")
        {
            return ternary;
        }
        if (op == "\?\?=")
        {
            // `a ??= b` is the same as `a = a ?? b` which is the same as `a = a
            // != null ? a : b`
            return exprFact_.mk_bin_on(left, BinOpType::Assign, ternary);
        }

        throw std::runtime_error("Operation \"" + op + "\" is not implemented");
    }

    return exprFact_
        .mk_bin_on(hLeft(self, nLeft), *opOpt, hRight(self, nRight));
}

Expr* SrcCodeVisitor::visit_ternary_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode nCode       = ts_node_named_child(node, 0);
    const TSNode nTrue       = ts_node_named_child(node, 1);
    const TSNode nFalse      = ts_node_named_child(node, 2);
    const ExprHandler hCond  = RegManager::get_expr_handler(nCode);
    const ExprHandler htrue  = RegManager::get_expr_handler(nTrue);
    const ExprHandler hfalse = RegManager::get_expr_handler(nFalse);

    return exprFact_
        .mk_if(hCond(self, nCode), htrue(self, nTrue), hfalse(self, nFalse));
}

Expr* SrcCodeVisitor::visit_parenthesized_expr(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode nExpr      = ts_node_named_child(node, 0);
    const ExprHandler hExpr = RegManager::get_expr_handler(nExpr);
    return exprFact_.mk_bracket(hExpr(self, nExpr));
}

Expr* SrcCodeVisitor::visit_const_pattern(
    SrcCodeVisitor* self,
    const TSNode& node
)
{
    const TSNode nInside = ts_node_child(node, 0);
    return RegManager::get_expr_handler(nInside)(self, nInside);
}

} // namespace astfri::csharp