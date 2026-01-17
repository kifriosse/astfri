#include <libastfri-cs/impl/regs/QueryRegistry.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>

#include <tree_sitter/api.h>

#include <filesystem>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

namespace astfri::csharp::util
{

TSNode child_by_field_name(const TSNode& node, const std::string_view name)
{
    return ts_node_child_by_field_name(node, name.data(), name.length());
}

TSSymbol symbol_for_name(
    const TSLanguage* lang,
    const std::string_view name,
    const bool named
)
{
    return ts_language_symbol_for_name(lang, name.data(), name.length(), named);
}

std::string extract_text(const TSNode& node, const std::string_view src_code)
{
    if (ts_node_is_null(node))
        throw std::runtime_error("Node is null");

    const size_t from = ts_node_start_byte(node);
    const size_t to   = ts_node_end_byte(node);
    return {src_code.data() + from, to - from};
}

std::vector<TSNode> find_nodes(const TSNode& root, regs::QueryType query_type)
{
    static auto& query_reg = regs::QueryReg::get();
    std::vector<TSNode> results;

    if (const TSQuery* query = query_reg.get_query(query_type))
    {
        TSQueryCursor* cursor = ts_query_cursor_new();
        ts_query_cursor_exec(cursor, query, root);

        TSQueryMatch match;
        while (ts_query_cursor_next_match(cursor, &match))
        {
            for (int i = 0; i < match.capture_count; ++i)
            {
                results.push_back(match.captures[i].node);
            }
        }

        ts_query_cursor_delete(cursor);
    }
    return results;
}

TSNode find_first_node(const TSNode& root, const regs::QueryType query_type)
{
    static auto& query_reg = regs::QueryReg::get();
    TSNode result{};
    if (const TSQuery* query = query_reg.get_query(query_type))
    {
        TSQueryCursor* cursor = ts_query_cursor_new();
        ts_query_cursor_exec(cursor, query, root);

        TSQueryMatch match;
        if (ts_query_cursor_next_match(cursor, &match))
            result = match.captures[0].node;

        ts_query_cursor_delete(cursor);
    }

    return result;
}

void print_child_nodes_types(const TSNode& node, const bool named)
{
    size_t i     = 0;
    auto process = [&i](TSNode child)
    { std::cout << "Child " << ++i << " type: \'" << ts_node_type(child); };
    for_each_child_node(node, process, named);
}

void print_child_nodes_types(
    const TSNode& node,
    const std::string_view source,
    const bool named
)
{
    size_t i     = 0;
    auto process = [&source, &i](TSNode child)
    {
        const std::string type = ts_node_type(child);
        const std::string text = extract_text(child, source);
        std::cout << "Child " << ++i << " type: \'" << type << "\' text: \""
                  << text << "\"" << '\n';
    };
    for_each_child_node(node, process, named);
}

std::string remove_comments(
    const TSNode& root,
    const std::string_view source_code,
    const std::filesystem::path& path
)
{
    using namespace regs;
    using enum QueryType;
    static auto& query_reg = QueryReg::get();

    std::string new_source;

    if (const TSQuery* ts_query = query_reg.get_query(CommentError))
    {
        TSQueryCursor* cursor = ts_query_cursor_new();
        ts_query_cursor_exec(cursor, ts_query, root);

        TSQueryMatch match;
        size_t next_start = 0;
        std::vector<TSNode> errors;
        uint32_t capture_index;

        while (ts_query_cursor_next_capture(cursor, &match, &capture_index))
        {
            const TSNode node = match.captures[0].node;
            if (ts_node_is_error(node))
            {
                errors.emplace_back(node);
                continue;
            }
            const size_t start = ts_node_start_byte(node);
            const size_t n     = start - next_start;
            new_source += source_code.substr(next_start, n);
            next_start = ts_node_end_byte(node);
        }
        new_source += source_code.substr(next_start);

        ts_query_cursor_delete(cursor);

        if (! errors.empty())
        {
            std::cerr << "Source code contains syntax errors:\n\n";
            for (const auto& error_node : errors)
            {
                const auto& [row, column] = ts_node_start_point(error_node);
                std::cerr << "Warning: Syntax error at line " << row + 1
                          << ", column " << column + 1 << "\n";
            }

            std::cerr << std::endl;

            throw std::runtime_error(
                "Source code in file \"" + path.string()
                + "\" contains syntax errors."
            );
        }
    }

    return new_source;
}

bool has_variadic_param(const TSNode& node, TSNode* type_node)
{
    const TSNode n_type = child_by_field_name(node, "type");
    if (type_node && ! ts_node_is_null(n_type))
        *type_node = n_type;

    const TSNode n_name = child_by_field_name(node, "name");
    return ! ts_node_is_null(n_type) && ! ts_node_is_null(n_name);
}

} // namespace astfri::csharp::util