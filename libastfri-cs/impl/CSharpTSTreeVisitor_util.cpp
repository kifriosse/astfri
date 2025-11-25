#include <libastfri-cs/impl/CSharpTSTreeVisitor.hpp>
#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/utils.hpp>

namespace astfri::csharp
{

Scope CSharpTSTreeVisitor::create_scope(TSNode const* node) const
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
    TSNode current        = *node;
    TSNode parent         = ts_node_parent(current);

    bool found_name_space = false;
    while (! ts_node_is_null(parent))
    {
        std::string const parent_type = ts_node_type(parent);
        auto const res                = node_type_map.find(parent_type);
        current                       = parent;
        parent                        = ts_node_parent(current);

        if (res == node_type_map.end())
            continue;

        switch (res->second)
        {
        case Class:
        case Interface:
        {
            TSNode const name_node = ts_node_child_by_field_name(current, "name", 4);
            std::string const name = extract_node_text(name_node, this->source_code_);
            scope_str.push(name);
            break;
        }
        case Root:
        {
            if (found_name_space)
                break;

            std::string file_namespace_query = "(file_scoped_namespace_declaration) @namespace";
            TSNode const namespace_node
                = find_first_node(current, this->language_, file_namespace_query);
            if (ts_node_is_null(namespace_node))
                break;

            TSNode const name_node = ts_node_child_by_field_name(namespace_node, "name", 4);
            std::string const name = extract_node_text(name_node, this->source_code_);
            split_namespace(scope_str, name);
            break;
        }
        case Namespace:
        {
            found_name_space       = true;
            TSNode const name_node = ts_node_child_by_field_name(current, "name", 4);
            std::string const name = extract_node_text(name_node, this->source_code_);
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

std::vector<ParamVarDefStmt*> CSharpTSTreeVisitor::handle_param_list(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    std::vector<ParamVarDefStmt*> parameters;
    if (! ts_tree_cursor_goto_first_child(&cursor))
        throw std::logic_error("Invalid Node");

    StmtHandler const handler = NodeRegistry::get_stmt_handler("parameter");
    do
    {
        TSNode current = ts_tree_cursor_current_node(&cursor);
        parameters.emplace_back(as_a<ParamVarDefStmt>(handler(self, &current)));
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    return parameters;
}

std::vector<Expr*> CSharpTSTreeVisitor::handle_argument_list(
    CSharpTSTreeVisitor* self,
    TSNode const* node
)
{
    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    if (! ts_tree_cursor_goto_first_child(&cursor))
        throw std::logic_error("Invalid node");

    std::vector<Expr*> exprs;
    do
    {
        ts_tree_cursor_goto_first_child(&cursor);

        TSNode current           = ts_tree_cursor_current_node(&cursor);
        ExprHandler expr_handler = NodeRegistry::get_expr_handler(current);
        exprs.emplace_back(expr_handler(self, &current));

        ts_tree_cursor_goto_parent(&cursor);
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    return exprs;
}
} // namespace astfri::csharp
