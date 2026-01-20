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
    if (! ts_node_is_null(node))
    {
        const size_t from = ts_node_start_byte(node);
        const size_t to   = ts_node_end_byte(node);
        return {src_code.data() + from, to - from};
    }
    return {};
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
    static constexpr auto query_type  = QueryType::CommentError;

    static const auto& query_reg      = QueryReg::get();

    std::string new_src;
    size_t next_start = 0;
    bool has_err      = false;
    auto process      = [&](const TSQueryMatch& match)
    {
        static const Query* const query   = query_reg.get_query(query_type);
        static const CaptureId comment_id = query->id("comment");
        static const CaptureId error_id   = query->id("error");

        for (uint32_t i = 0; i < match.capture_count; ++i)
        {
            auto& [node, index] = match.captures[i];
            if (index == error_id)
            {
                if (! has_err)
                {
                    std::cerr << "Source code contains syntax errors:\n\n";
                    has_err = true;
                }
                const auto& [row, column] = ts_node_start_point(node);
                std::cerr << "Warning: Syntax error at line " << row + 1
                          << ", column " << column + 1 << "\n";
            }
            else if (index == comment_id)
            {
                const size_t start = ts_node_start_byte(node);
                const size_t n     = start - next_start;
                new_src += source_code.substr(next_start, n);
                next_start = ts_node_end_byte(node);
            }
        }
    };
    for_each_match(root, query_type, process);
    new_src += source_code.substr(next_start);

    if (has_err)
    {
        throw std::runtime_error(
            "Source code in file \"" + path.string()
            + "\" contains syntax errors."
        );
    }

    return new_src;
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