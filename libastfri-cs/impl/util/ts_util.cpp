#include <libastfri-cs/impl/util/ts_util.hpp>

#include <tree_sitter/api.h>

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace astfri::csharp::regs
{

const std::string Queries::top_level_stmt_query =
    R"(
    (namespace_declaration
        body: (declaration_list
        [
            (class_declaration)     @class
            (interface_declaration) @interface
            (struct_declaration)    @struct
            (enum_declaration)      @enum
            (delegate_declaration)  @delegate
            (record_declaration)    @record
        ])
    )
    (compilation_unit
    [
        (class_declaration)     @class
        (interface_declaration) @interface
        (struct_declaration)    @struct
        (enum_declaration)      @enum
        (delegate_declaration)  @delegate
        (record_declaration)    @record
    ])
    )";

const std::string Queries::decl_query =
    R"(
    (variable_declaration (variable_declarator) @var_decl)
    )";

const std::string Queries::var_modif_query =
    R"(
    [
        (field_declaration           (modifier) @modifier)
        (local_declaration_statement (modifier) @modifier)
        (parameter                   (modifier) @modifier)
    ]
    )";

const std::string Queries::method_modif_query =
    R"(
        (method_declaration (modifier) @modifier)
    )";

const std::string Queries::file_namespace_query
    = "(file_scoped_namespace_declaration) @namespace";

const std::string Queries::comment_error_query =
    R"(
        (comment) @comment
        (ERROR) @error
    )";

} // namespace astfri::csharp::regs

namespace astfri::csharp::util
{

TSNode child_by_field_name(const TSNode& node, const std::string_view name)
{
    return ts_node_child_by_field_name(node, name.data(), name.length());
}

std::string extract_node_text(
    const TSNode& node,
    const std::string_view src_code
)
{
    if (ts_node_is_null(node))
        throw std::runtime_error("Node is null");

    const size_t from = ts_node_start_byte(node);
    const size_t to   = ts_node_end_byte(node);
    return {src_code.data() + from, to - from};
}

std::vector<TSNode> find_nodes(
    const TSNode& root,
    const TSLanguage* lang,
    const std::string_view query_str
)
{
    std::vector<TSNode> results;
    TSQueryError err;
    uint32_t offset;
    TSQuery* query = ts_query_new(
        lang,
        query_str.data(),
        query_str.length(),
        &offset,
        &err
    );

    if (! query)
    {
        throw std::runtime_error(
            "Error while creating query at offset " + std::to_string(offset)
            + " Error code: " + std::to_string(err)
        );
    }

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
    ts_query_delete(query);

    return results;
}

TSNode find_first_node(
    const TSNode& root,
    const TSLanguage* lang,
    const std::string_view query_str
)
{
    TSQueryError err;
    uint32_t offset;
    TSQuery* query = ts_query_new(
        lang,
        query_str.data(),
        query_str.length(),
        &offset,
        &err
    );

    if (! query)
    {
        throw std::runtime_error(
            "Error while creating query at offset " + std::to_string(offset)
            + " Error code: " + std::to_string(err)
        );
    }

    TSQueryCursor* cursor = ts_query_cursor_new();
    ts_query_cursor_exec(cursor, query, root);

    TSQueryMatch match;
    TSNode result = {};
    while (ts_query_cursor_next_match(cursor, &match))
    {
        if (match.capture_count >= 1)
        {
            result = match.captures[0].node;
            break;
        }
    }

    ts_query_cursor_delete(cursor);
    ts_query_delete(query);

    return result;
}

void print_child_nodes_types(const TSNode& node)
{
    for (size_t i = 0; i < ts_node_child_count(node); ++i)
    {
        const TSNode child = ts_node_child(node, i);
        std::string type   = ts_node_type(child);
        std::cout << "Child " << i << " type: " << type << '\n';
    }
}

void print_child_nodes_types(const TSNode& node, const std::string_view source)
{
    for (size_t i = 0; i < ts_node_child_count(node); ++i)
    {
        const TSNode child = ts_node_child(node, i);
        std::string type   = ts_node_type(child);
        std::string text   = extract_node_text(child, source);
        std::cout << "Child " << i << " type: \'" << type << "\' text: \""
                  << text << "\"" << '\n';
    }
}

std::string remove_comments(
    const TSNode& root,
    const std::string_view source_code,
    const std::filesystem::path& path,
    const TSLanguage* lang
)
{
    std::string new_source;

    TSQueryError query_error;
    uint32_t offset;
    TSQuery* ts_query = ts_query_new(
        lang,
        regs::Queries::comment_error_query.c_str(),
        regs::Queries::comment_error_query.length(),
        &offset,
        &query_error
    );
    if (! ts_query)
    {
        throw std::runtime_error(
            "Error while creating query at offset " + std::to_string(offset)
            + " Error code: " + std::to_string(query_error)
        );
    }
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
    ts_query_delete(ts_query);

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

    return new_source;
}

bool has_variadic_param(const TSNode& node, TSNode* type_node)
{
    const TSNode found_type = child_by_field_name(node, "type");
    if (type_node && ! ts_node_is_null(found_type))
        *type_node = found_type;

    const TSNode name_node = child_by_field_name(node, "name");
    return ! ts_node_is_null(found_type) && ! ts_node_is_null(name_node);
}

} // namespace astfri::csharp::util