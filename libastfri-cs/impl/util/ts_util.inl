#ifndef CSHARP_TS_UTIL_INL
#define CSHARP_TS_UTIL_INL

namespace astfri::csharp::util
{

template<typename Func>
requires std::same_as<std::invoke_result_t<Func, TSNode>, bool>
void for_each_child_node(const TSNode node, Func process, const bool only_named)
{
    TSTreeCursor cursor = ts_tree_cursor_new(node);
    if (ts_tree_cursor_goto_first_child(&cursor))
    {
        do
        {
            TSNode n_current = ts_tree_cursor_current_node(&cursor);
            if (only_named && ! ts_node_is_named(n_current))
                continue;

            if (! process(n_current))
                break;

        } while (ts_tree_cursor_goto_next_sibling(&cursor));
    }
    ts_tree_cursor_delete(&cursor);
}

template<typename Func>
requires std::same_as<std::invoke_result_t<Func, TSNode>, void>
void for_each_child_node(const TSNode node, Func process, const bool only_named)
{
    TSTreeCursor cursor = ts_tree_cursor_new(node);
    if (ts_tree_cursor_goto_first_child(&cursor))
    {
        do
        {
            TSNode n_current = ts_tree_cursor_current_node(&cursor);
            if (only_named && ! ts_node_is_named(n_current))
                continue;

            process(n_current);

        } while (ts_tree_cursor_goto_next_sibling(&cursor));
    }
    ts_tree_cursor_delete(&cursor);
}

template<std::invocable<TSNode> Func>
void process_param_list(TSNode node, Func collector)
{
    TSNode n_type{};
    const bool is_variadic = has_variadic_param(node, &n_type);

    auto process           = [&](TSNode current) -> bool
    {
        if (is_variadic && ts_node_eq(current, n_type))
        {
            collector(node);
            return false;
        }

        collector(current);
        return true;
    };
    util::for_each_child_node(node, process);
}

template<std::invocable<const TSQueryMatch&> Func>
void for_each_match(const TSNode root, const regs::QueryType type, Func process)
{
    static auto& query_reg = regs::QueryReg::get();

    if (const regs::Query* query = query_reg.get_query(type))
    {
        TSQueryCursor* cursor = ts_query_cursor_new();
        ts_query_cursor_exec(cursor, query->get(), root);

        TSQueryMatch match;
        while (ts_query_cursor_next_match(cursor, &match))
        {
            process(match);
        }

        ts_query_cursor_delete(cursor);
    }
}

} // namespace astfri::csharp::util

#endif // CSHARP_TS_UTIL_INL