#include "utils.hpp"

#include <cstring>
#include <iostream>
#include <regex>
#include <stdexcept>

namespace astfri::csharp
{
std::vector<TSNode> find_nodes(TSNode root, const TSLanguage *lang, const char *query_str)
{
    std::vector<TSNode> results;
    TSQueryError err;
    uint32_t offset;
    TSQuery *query =ts_query_new(lang, query_str, std::strlen(query_str), &offset, &err);
    if (!query)
    {
        throw std::runtime_error("Error while creating query at offset " + std::to_string(offset)
            + " Error code: " + std::to_string(err));
    }
    TSQueryCursor *cursor = ts_query_cursor_new();
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

IntSuffix get_suffix_type(std::string const& suffix)
{
    if (suffix.empty()) return IntSuffix::None;

    const char first = static_cast<char>(std::tolower(suffix[0]));
    const char second = static_cast<char>(std::tolower(suffix.length() == 2 ? suffix[1] : '\0'));

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

std::string extract_node_text(
    const TSNode& node,
    const std::string& source_code
)
{
    const size_t from = ts_node_start_byte(node);
    const size_t to = ts_node_end_byte(node);
    return source_code.substr(from, to - from);
}

} // namespace astfri::csharp