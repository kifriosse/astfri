#include <astfri/Astfri.hpp>

#include <libastfri-cs/impl/CSFwd.hpp>
#include <libastfri-cs/impl/data/AccessType.hpp>
#include <libastfri-cs/impl/data/Identifiers.hpp>
#include <libastfri-cs/impl/regs/Maps.hpp>
#include <libastfri-cs/impl/util/Common.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>
#include <libastfri-cs/impl/util/Utils.hpp>
#include <libastfri-cs/impl/visitors/src_code/SrcCodeTransformer.hpp>

#include <tree_sitter/api.h>

#include <charconv>
#include <stdexcept>
#include <string>

namespace astfri::csharp {

Expr* SrcCodeTransformer::visit_int_lit(SrcCodeTransformer* self, const TSNode& node) {
    std::string rawStr = util::extract_text(node, self->src_str());
    std::erase(rawStr, '_');

    const size_t len = rawStr.length();
    const std::string_view prefix(rawStr.data(), std::min<size_t>(2, len));

    std::string_view sv = rawStr;
    int base            = 10;
    if (prefix == "0x")
        base = 16;
    else if (prefix == "0b")
        base = 2;

    if (base != 10)
        sv.remove_prefix(2);

    const util::IntSuffix suffixType
        = util::get_suffix_type(sv.substr(len >= 2 ? len - 2 : 0, std::min<size_t>(2, len)));

    switch (suffixType) {
    case util::IntSuffix::U:
    case util::IntSuffix::L: {
        sv.remove_suffix(1);
        break;
    }
    case util::IntSuffix::UL: {
        sv.remove_suffix(2);
        break;
    }
    default:
        break;
    }

    int val         = 0;
    auto [ptr, err] = std::from_chars(sv.data(), sv.data() + rawStr.size(), val, base);
    if (err != std::error_code{})
        return exprFact_.mk_unknown();

    if (suffixType == util::IntSuffix::None || suffixType == util::IntSuffix::U) {
        // todo add handeling of unsigned integers
        return exprFact_.mk_int_literal(val);
    }

    // todo handeling of long and unsigned long
    // throw std::logic_error("This integer type is not implemented");
    return exprFact_.mk_unknown();
}

Expr* SrcCodeTransformer::visit_float_lit(SrcCodeTransformer* self, const TSNode& node) {
    std::string rawStr = util::extract_text(node, self->src_str());
    const char suffix  = static_cast<char>(std::tolower(rawStr[rawStr.length() - 1]));

    std::erase(rawStr, '_');
    const bool doubleSuffix = suffix == 'd';
    if (! std::isalpha(suffix) || doubleSuffix) {
        if (doubleSuffix)
            rawStr.pop_back();

        // todo handle double
        // throw std::logic_error(
        //     "Handling of double floating point numbers is not implemented"
        // );
        return exprFact_.mk_unknown();
    }

    rawStr.pop_back();
    switch (suffix) {
    case 'f':
        return exprFact_.mk_float_literal(std::stof(rawStr));
    case 'm':
        // decimal - 128-bit precision integer - uses base 10, not base 2
        // todo handle decimal
        // throw std::logic_error("Handling of Decimal literal not
        // implemented");
        return exprFact_.mk_unknown();
    default:
        // throw std::logic_error(
        //     "Suffix \"" + std::to_string(suffix) + "\" Not Implemented"
        // );
        return exprFact_.mk_unknown();
    }
}

Expr* SrcCodeTransformer::visit_bool_lit(SrcCodeTransformer* self, const TSNode& node) {
    const std::string boolStr = util::extract_text(node, self->src_str());
    return exprFact_.mk_bool_literal(boolStr == "true");
}

Expr* SrcCodeTransformer::visit_char_lit(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nChar        = ts_node_named_child(node, 0);
    const std::string charStr = util::extract_text(nChar, self->src_str());
    if (charStr.length() > 1) {
        // todo handle 16-bit unicode characters
        // throw std::logic_error("Unicode characters not implemented");
        return exprFact_.mk_unknown();
    }
    return exprFact_.mk_char_literal(charStr[0]);
}

Expr* SrcCodeTransformer::visit_str_lit(SrcCodeTransformer* self, const TSNode& node) {
    std::string str{};
    if (ts_node_child_count(node) >= 3) {
        const TSNode nStr = ts_node_named_child(node, 0);
        str               = util::extract_text(nStr, self->src_str());
    }
    return exprFact_.mk_string_literal(str);
}

Expr* SrcCodeTransformer::visit_null_lit(
    [[maybe_unused]] SrcCodeTransformer* self,
    [[maybe_unused]] const TSNode& node
) {
    return exprFact_.mk_null_literal();
}

Expr* SrcCodeTransformer::visit_this_expr(
    [[maybe_unused]] SrcCodeTransformer* self,
    [[maybe_unused]] const TSNode& node
) {
    return exprFact_.mk_this();
}

Expr* SrcCodeTransformer::visit_base_expr(
    [[maybe_unused]] SrcCodeTransformer* self,
    [[maybe_unused]] const TSNode& node
) {
    return exprFact_.mk_base();
}

Expr* SrcCodeTransformer::visit_verbatim_str_lit(SrcCodeTransformer* self, const TSNode& node) {
    std::string nContent = util::extract_text(node, self->src_str());
    nContent.pop_back();
    nContent.erase(nContent.begin(), nContent.begin() + 2);
    nContent = util::escape_string(nContent, true);

    return exprFact_.mk_string_literal(nContent);
}

Expr* SrcCodeTransformer::visit_raw_str_lit(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nContent = ts_node_named_child(node, 1);
    std::string content   = util::extract_text(nContent, self->src_str());
    content               = util::escape_string(content, false);
    return exprFact_.mk_string_literal(content);
}

Expr* SrcCodeTransformer::visit_interpolated_str_lit(
    [[maybe_unused]] SrcCodeTransformer* self,
    [[maybe_unused]] const TSNode& node
) {
    // throw std::logic_error("Interpolated string literal not implemented");
    return exprFact_.mk_unknown();
}

Expr* SrcCodeTransformer::visit_identifier(SrcCodeTransformer* self, const TSNode& node) {
    std::string name = util::extract_text(node, self->src_str());
    Stmt* defStmt    = self->semContext_.find_var(name, access::None{});
    if (! defStmt)
        return exprFact_.mk_unknown();

    if (is_a<MemberVarDefStmt>(defStmt)) // todo static handling
        return exprFact_.mk_member_var_ref(exprFact_.mk_this(), std::move(name));
    if (is_a<ParamVarDefStmt>(defStmt))
        return exprFact_.mk_param_var_ref(std::move(name));
    if (is_a<LocalVarDefStmt>(defStmt))
        return exprFact_.mk_local_var_ref(std::move(name));

    return exprFact_.mk_unknown();
}

Expr* SrcCodeTransformer::visit_memb_access(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nLeft     = util::child_by_field_name(node, "expression");
    const TSNode nRight    = util::child_by_field_name(node, "name");
    const ExprMapper mLeft = MapManager::get_expr_mapper(nLeft);
    std::string name       = util::extract_text(nRight, self->src_str());
    Expr* left             = mLeft(self, nLeft);

    access::Qualifier qualif;
    if (is_a<BaseExpr>(left))
        qualif = access::Base{};
    else if (is_a<ThisExpr>(left))
        qualif = access::Instance{};
    // else if (is_a<ClassRefExpr>(left))
    //     qualif = access::Static{};

    // todo for future use
    if ([[maybe_unused]] const auto varDef = self->semContext_.find_var(name, qualif)) {
        return exprFact_.mk_member_var_ref(left, std::move(name));
    }
    // todo generic member access
    return exprFact_.mk_unknown();
}

Expr* SrcCodeTransformer::visit_invoc(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nFunc         = util::child_by_field_name(node, "function");
    const TSNode nArgList      = util::child_by_field_name(node, "arguments");
    std::vector<Expr*> argList = self->visit_arg_list(nArgList);
    // if it doesn't have a left side - it is not a member access node in tree sitter
    if (ts_node_symbol(nFunc) == MapManager::get_symbol(NodeType::Identifier)) {
        std::string name = util::extract_text(nFunc, self->src_str());
        InvocationId id{
            .name       = name,
            .paramCount = argList.size(),
        };
        const InvocationType invocType
            = self->semContext_.find_invoc_type(std::move(id), access::None{});

        // name = util::extract_text(n_func, self->src_str());
        switch (invocType) {
        case InvocationType::LocalFunc:
            return exprFact_.mk_function_call(std::move(name), std::move(argList));
        case InvocationType::Delegate: {
            const ExprMapper mLeft = MapManager::get_expr_mapper(nFunc);
            Expr* left             = mLeft(self, nFunc);
            return exprFact_.mk_lambda_call(left, std::move(argList));
        }
        case InvocationType::Method:
            return exprFact_
                .mk_method_call(exprFact_.mk_this(), std::move(name), std::move(argList));
        case InvocationType::StaticMethod: {
            const auto current = self->semContext_.current_type();
            // todo add static method call handling
            return exprFact_.mk_method_call(
                exprFact_.mk_class_ref(current->type->name),
                std::move(name),
                std::move(argList)
            );
        }
        default:
            return exprFact_.mk_unknown();
        }
    }
    // accessing a member of some variable - also includes `this`
    if (ts_node_symbol(nFunc) == MapManager::get_symbol(NodeType::MemberAccess)) {
        const TSNode nName     = util::child_by_field_name(nFunc, "name");
        const TSNode nLeft     = util::child_by_field_name(nFunc, "expression");
        const ExprMapper mLeft = MapManager::get_expr_mapper(nLeft);
        Expr* left             = mLeft(self, nLeft);
        std::string name       = util::extract_text(nName, self->src_str());
        // it's a member of an instance - method or delegate type attribute
        access::Qualifier qualif;
        if (is_a<BaseExpr>(left))
            qualif = access::Base{};
        else if (is_a<ThisExpr>(left))
            qualif = access::Instance{};
        // else if (is_a<ClassRefExpr>(left))
        //     qualif = access::Static{};
        // todo accessing of static members left side is a Usertype Reference
        else
            return exprFact_.mk_unknown();

        InvocationId id{
            .name       = name,
            .paramCount = argList.size(),
        };

        switch (self->semContext_.find_invoc_type(std::move(id), qualif)) {
        case InvocationType::Delegate:
            return exprFact_.mk_lambda_call(
                exprFact_.mk_member_var_ref(left, std::move(name)),
                std::move(argList)
            );
        case InvocationType::Method:
            return exprFact_.mk_method_call(left, std::move(name), std::move(argList));
        default:
            // todo placeholder
            return exprFact_.mk_unknown();
        }
    }
    // left side is an anonymous lambda
    TSNode nLambda;
    TSNode nDelegate;
    if (util::is_anonymous_lambda(nFunc, &nLambda, &nDelegate)) {
        // todo add lambda parsing
        return exprFact_.mk_lambda_call(exprFact_.mk_unknown(), std::move(argList));
    }

    return exprFact_.mk_unknown();
}

Expr* SrcCodeTransformer::visit_prefix_unary_opr(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nOp     = ts_node_child(node, 0);
    const TSNode nRight  = ts_node_child(node, 1);
    // std::erase_if(op, isspace);

    const auto res = MapManager::get_prefix_unary_op(nOp);
    if (! res)
        // throw std::runtime_error("Operation \"" + op + "\" is not
        // implemented");
        return exprFact_.mk_unknown();

    const UnaryOpType opType = *res;
    const ExprMapper mRight  = MapManager::get_expr_mapper(nRight);
    Expr* right              = mRight(self, nRight);
    return exprFact_.mk_unary_op(opType, right);
}

Expr* SrcCodeTransformer::visit_postfix_unary_opr(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nLeft     = ts_node_child(node, 0);
    const TSNode nOp       = ts_node_child(node, 1);
    const std::string op   = util::extract_text(nOp, self->src_str());
    const ExprMapper mLeft = MapManager::get_expr_mapper(nLeft);
    Expr* left             = mLeft(self, nLeft);

    UnaryOpType opType;
    if (op == "++")
        opType = UnaryOpType::PostIncrement;
    else if (op == "--")
        opType = UnaryOpType::PostDecrement;
    else if (op == "!")
        return left;
    else
        return exprFact_.mk_unknown();

    return exprFact_.mk_unary_op(opType, left);
}

Expr* SrcCodeTransformer::visit_ref_expr(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nExpr = ts_node_named_child(node, 0);
    Expr* expr         = MapManager::get_expr_mapper(nExpr)(self, nExpr);
    return exprFact_.mk_unary_op(UnaryOpType::AddressOf, expr);
}

Expr* SrcCodeTransformer::visit_binary_opr(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nLeft      = ts_node_named_child(node, 0);
    const TSNode nOp        = ts_node_child(node, 1);
    const TSNode nRight     = ts_node_named_child(node, 1);
    const ExprMapper mLeft  = MapManager::get_expr_mapper(nLeft);
    const ExprMapper mRight = MapManager::get_expr_mapper(nRight);

    const auto opOpt        = MapManager::get_bin_op(nOp);
    if (! opOpt) {
        const std::string op    = util::extract_text(nOp, self->src_str());
        // `a ?? b` same as `a != null ? a : b`
        Expr* left = mLeft(self, nLeft);
        BinOpExpr* cond
            = exprFact_.mk_bin_on(left, BinOpType::NotEqual, exprFact_.mk_null_literal());
        IfExpr* ternary = exprFact_.mk_if(cond, left, mRight(self, nRight));
        if (op == "\?\?") {
            return ternary;
        }
        if (op == "\?\?=") {
            // `a ??= b` is the same as `a = a ?? b` which is the same as `a = a
            // != null ? a : b`
            return exprFact_.mk_bin_on(left, BinOpType::Assign, ternary);
        }

        // throw std::runtime_error("Operation \"" + op + "\" is not
        // implemented");
        return exprFact_.mk_unknown();
    }

    return exprFact_.mk_bin_on(mLeft(self, nLeft), *opOpt, mRight(self, nRight));
}

Expr* SrcCodeTransformer::visit_ternary_expr(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nCode      = ts_node_named_child(node, 0);
    const TSNode nTrue      = ts_node_named_child(node, 1);
    const TSNode nFalse     = ts_node_named_child(node, 2);

    const ExprMapper mCond  = MapManager::get_expr_mapper(nCode);
    const ExprMapper mtrue  = MapManager::get_expr_mapper(nTrue);
    const ExprMapper mfalse = MapManager::get_expr_mapper(nFalse);

    return exprFact_.mk_if(mCond(self, nCode), mtrue(self, nTrue), mfalse(self, nFalse));
}

Expr* SrcCodeTransformer::visit_parenthesized_expr(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nExpr     = ts_node_named_child(node, 0);
    const ExprMapper mExpr = MapManager::get_expr_mapper(nExpr);
    return exprFact_.mk_bracket(mExpr(self, nExpr));
}

Expr* SrcCodeTransformer::visit_const_pattern(SrcCodeTransformer* self, const TSNode& node) {
    const TSNode nInside = ts_node_child(node, 0);
    return MapManager::get_expr_mapper(nInside)(self, nInside);
}

} // namespace astfri::csharp