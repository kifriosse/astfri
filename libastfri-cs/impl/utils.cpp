#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/utils.hpp>
#include <libastfri-cs/impl/visitor/SourceCodeVisitor.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <cmath>
#include <filesystem>
#include <iostream>
#include <unordered_map>

namespace astfri::csharp::util
{
std::vector<TSNode> find_nodes(
    const TSNode& root,
    const TSLanguage* lang,
    const std::string& query_str
)
{
    std::vector<TSNode> results;
    TSQueryError err;
    uint32_t offset;
    TSQuery* query = ts_query_new(
        lang,
        query_str.c_str(),
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
    const std::string& query_str
)
{
    TSQueryError err;
    uint32_t offset;
    TSQuery* query = ts_query_new(
        lang,
        query_str.c_str(),
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

IntSuffix get_suffix_type(const std::string& suffix)
{
    if (suffix.empty())
        return IntSuffix::None;

    const char first  = static_cast<char>(std::tolower(suffix[0]));
    const char second = static_cast<char>(
        std::tolower(suffix.length() == 2 ? suffix[1] : '\0')
    );

    // considering that we can't get stuff like UU or LL or L1 or U1
    if ((first == 'u' && second == 'l') || (first == 'l' && second == 'u'))
    {
        return IntSuffix::UL;
    }
    if (first == 'l' || first == 'u')
    {
        return first == 'l' ? IntSuffix::L : IntSuffix::U;
    }
    if (second == 'l' || second == 'u')
    {
        return second == 'l' ? IntSuffix::L : IntSuffix::U;
    }
    return IntSuffix::None;
}

bool almost_equal(const double a, const double b, const double epsilon)
{
    return std::fabs(a - b) < epsilon;
}

std::string extract_node_text(
    const TSNode& node,
    const std::string& source_code
)
{
    if (ts_node_is_null(node))
        throw std::runtime_error("Node is null");

    const size_t from = ts_node_start_byte(node);
    const size_t to   = ts_node_end_byte(node);
    return source_code.substr(from, to - from);
}

void split_namespace(
    std::stack<std::string>& scope_str,
    const std::string& namespace_name
)
{
    const auto r_begin = std::make_reverse_iterator(namespace_name.end());
    const auto r_end   = std::make_reverse_iterator(namespace_name.begin());
    auto it            = r_begin;
    auto slice_end     = namespace_name.end();

    while (it != r_end)
    {
        if (*it == '.')
        {
            auto slice_start = it.base();
            scope_str.emplace(slice_start, slice_end);
            slice_end = slice_start - 1;
        }
        ++it;
    }
}

bool is_interface_name(const std::string& name)
{
    return name.size() >= 2 && name[0] == 'I' && std::isupper(name[1]);
}

std::string remove_comments(
    const std::string& source_code,
    const TSNode& root,
    const TSLanguage* lang,
    const std::filesystem::path& path
)
{
    std::string new_source;

    static std::string query = R"(
        (comment) @comment
        (ERROR) @error
    )";

    TSQueryError query_error;
    uint32_t offset;
    TSQuery* ts_query = ts_query_new(
        lang,
        query.c_str(),
        query.length(),
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
        if (ts_node_type(node) == std::string("ERROR"))
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

    ts_query_cursor_delete(cursor);
    ts_query_delete(ts_query);
    return new_source;
}

std::string escape_string(
    const std::string_view str_view,
    const bool is_verbatim
)
{
    std::string escaped_str;
    for (size_t i = 0; i < str_view.length(); ++i)
    {
        switch (const char c = str_view[i])
        {
        case '\n':
            escaped_str += "\\n";
            break;
        case '\t':
            escaped_str += "\\t";
            break;
        case '\r':
            escaped_str += "\\r";
            break;
        case '\"':
        {
            if (! is_verbatim)
            {
                escaped_str += "\\\"";
                break;
            }

            const size_t next = i + 1;
            if (next < str_view.length() && str_view.at(next) == '\"')
            {
                escaped_str += "\\\"";
                i += 1;
            }
            break;
        }
        case '\'':
            escaped_str += "\\\'";
            break;
        case '\\':
            escaped_str += "\\\\";
            break;
        case '\0':
            escaped_str += "\\0";
            break;
        case '\a':
            escaped_str += "\\a";
            break;
        case '\b':
            escaped_str += "\\b";
            break;
        case '\f':
            escaped_str += "\\f";
            break;
        case '\v':
            escaped_str += "\\v";
            break;
        default:
            escaped_str += c;
            break;
        }
    }
    return escaped_str;
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

void print_child_nodes_types(const TSNode& node, const std::string& source)
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

Scope create_scope(
    const TSNode& node,
    const TSLanguage* lang,
    const std::string& source
)
{
    enum NodeType
    {
        Class,
        Interface,
        Namespace,
        Root,
    };

    static std::unordered_map<std::string, NodeType> node_type_map = {
        {"class_declaration",     Class    },
        {"interface_declaration", Interface},
        {"namespace_declaration", Namespace},
        {"compilation_unit",      Root     },
    };

    std::stack<std::string> scope_str;
    Scope scope           = {};
    TSNode current        = node;
    TSNode parent         = ts_node_parent(current);

    bool found_name_space = false;
    while (! ts_node_is_null(parent))
    {
        const std::string parent_type = ts_node_type(parent);
        const auto res                = node_type_map.find(parent_type);
        current                       = parent;
        parent                        = ts_node_parent(current);

        if (res == node_type_map.end())
            continue;

        switch (res->second)
        {
        case Class:
        case Interface:
        {
            const TSNode name_node
                = ts_node_child_by_field_name(current, "name", 4);
            const std::string name = extract_node_text(name_node, source);
            scope_str.push(name);
            break;
        }
        case Root:
        {
            if (found_name_space)
                break;

            std::string file_namespace_query
                = "(file_scoped_namespace_declaration) @namespace";
            const TSNode namespace_node
                = find_first_node(current, lang, file_namespace_query);
            if (ts_node_is_null(namespace_node))
                break;

            const TSNode name_node
                = ts_node_child_by_field_name(namespace_node, "name", 4);
            const std::string name = extract_node_text(name_node, source);
            split_namespace(scope_str, name);
            break;
        }
        case Namespace:
        {
            found_name_space = true;
            const TSNode name_node
                = ts_node_child_by_field_name(current, "name", 4);
            const std::string name = extract_node_text(name_node, source);
            split_namespace(scope_str, name);
            break;
        }
        }
    }

    while (! scope_str.empty())
    {
        scope.names_.push_back(scope_str.top());
        scope_str.pop();
    }
    return scope;
}

Type* make_type(const TSNode& node, const std::string& source_code)
{
    TypeFactory& type_factory      = TypeFactory::get_instance();
    std::string type_name          = extract_node_text(node, source_code);
    const char last_char           = type_name[type_name.length() - 1];
    const bool is_indirection_type = last_char == '*' || last_char == '&';
    if (is_indirection_type)
    {
        type_name.pop_back();
    }

    std::erase_if(type_name, isspace);
    const auto res = RegManager::get_type(type_name);
    // todo implement scope
    Type* type = res.has_value() ? *res : type_factory.mk_class(type_name, {});

    return is_indirection_type ? type_factory.mk_indirect(type) : type;
}

} // namespace astfri::csharp::util