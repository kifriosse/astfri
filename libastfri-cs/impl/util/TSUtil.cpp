#include <libastfri-cs/impl/regs/QueryRegistry.hpp>
#include <libastfri-cs/impl/util/TSUtil.hpp>

#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-c-sharp.h>

#include <filesystem>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>

namespace astfri::csharp::util
{

TSNode child_by_field_name(const TSNode& node, const std::string_view name)
{
    return ts_node_child_by_field_name(node, name.data(), name.length());
}

TSSymbol symbol_for_name(const std::string_view name, const bool named)
{
    static const TSLanguage* const lang = tree_sitter_c_sharp();
    return ts_language_symbol_for_name(lang, name.data(), name.length(), named);
}

std::string extract_text(const TSNode& node, const std::string_view src)
{
    if (! ts_node_is_null(node))
    {
        const size_t from = ts_node_start_byte(node);
        const size_t to   = ts_node_end_byte(node);
        return {src.data() + from, to - from};
    }
    return {};
}

void print_child_nodes_types(const TSNode& node, const bool named)
{
    size_t i     = 0;
    auto process = [&i](const TSNode& child) -> void
    { std::cout << "Child " << ++i << " type: \'" << ts_node_type(child); };
    for_each_child_node(node, process, named);
}

void print_child_nodes_types(
    const TSNode& node,
    const std::string_view src,
    const bool named
)
{
    size_t i     = 0;
    auto process = [&src, &i](const TSNode& child) -> void
    {
        const std::string type = ts_node_type(child);
        const std::string text = extract_text(child, src);
        std::cout << "Child " << ++i << " type: \'" << type << "\' text: \""
                  << text << "\"" << '\n';
    };
    for_each_child_node(node, process, named);
}

std::string remove_comments(
    const TSNode& root,
    const std::string_view src,
    const std::filesystem::path& path
)
{
    using namespace regs;
    static constexpr auto qType      = QueryType::CommentError;
    static const auto& queryReg      = QueryReg::get();
    static const Query* const query  = queryReg.get_query(qType);
    static const CaptureId commentId = query->id("comment");
    static const CaptureId errorId   = query->id("error");

    std::string newSrc;
    size_t nextStart = 0;
    bool hasErr      = false;
    auto process     = [&](const TSQueryMatch& match) -> void
    {
        for (CaptureId i = 0; i < match.capture_count; ++i)
        {
            auto& [node, index] = match.captures[i];
            if (index == errorId)
            {
                if (! hasErr)
                {
                    std::cerr << "Source code contains syntax errors:\n\n";
                    hasErr = true;
                }
                const auto& [row, column] = ts_node_start_point(node);
                std::cerr << "Warning: Syntax error at line " << row + 1
                          << ", column " << column + 1 << "\n";
            }
            else if (index == commentId)
            {
                const size_t start = ts_node_start_byte(node);
                const size_t n     = start - nextStart;
                newSrc += src.substr(nextStart, n);
                nextStart = ts_node_end_byte(node);
            }
        }
    };
    for_each_match(root, qType, process);
    newSrc += src.substr(nextStart);

    if (hasErr)
    {
        throw std::runtime_error(
            "Source code in file \"" + path.string()
            + "\" contains syntax errors."
        );
    }

    return newSrc;
}

bool has_variadic_param(const TSNode& node, TSNode* nType)
{
    const TSNode l_nType = child_by_field_name(node, "type");
    if (nType && ! ts_node_is_null(l_nType))
        *nType = l_nType;

    const TSNode nName = child_by_field_name(node, "name");
    return ! ts_node_is_null(l_nType) && ! ts_node_is_null(nName);
}

bool is_anonymous_lambda(const TSNode& node, TSNode* lambda, TSNode* delegate)
{
    static const TSSymbol sCastExpr = symbol_for_name("cast_expr", true);
    static const TSSymbol sLambda = symbol_for_name("lambda_expression", true);

    const TSNode nCast            = unwrap_parantheses(node);
    if (ts_node_symbol(nCast) != sCastExpr)
        return false;

    const TSNode nValue  = child_by_field_name(nCast, "value");
    const TSNode nLambda = unwrap_parantheses(nValue);

    if (ts_node_symbol(nLambda) != sLambda)
        return false;

    if (lambda)
        *lambda = nLambda;
    if (delegate)
        *delegate = child_by_field_name(nCast, "type");
    return true;
}

TSNode unwrap_parantheses(const TSNode& node)
{
    static const TSSymbol sBracketExpr
        = symbol_for_name("parenthesized_expression", true);

    TSNode current = node;
    while (! ts_node_is_null(current)
           && ts_node_symbol(current) == sBracketExpr)
    {
        current = ts_node_named_child(current, 0);
    }
    return current;
}

} // namespace astfri::csharp::util