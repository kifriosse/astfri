#include <libastfri-cs/impl/NodeRegistry.hpp>
#include <libastfri-cs/impl/utils.hpp>
#include <libastfri-cs/impl/visitor/CSharpTSTreeVisitor.hpp>

#include <queue>

namespace astfri::csharp
{

Scope CSharpTSTreeVisitor::create_scope(const TSNode* node) const
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
            const std::string name
                = extract_node_text(name_node, this->source_code_);
            scope_str.push(name);
            break;
        }
        case Root:
        {
            if (found_name_space)
                break;

            std::string file_namespace_query
                = "(file_scoped_namespace_declaration) @namespace";
            const TSNode namespace_node = find_first_node(
                current,
                this->language_,
                file_namespace_query
            );
            if (ts_node_is_null(namespace_node))
                break;

            const TSNode name_node
                = ts_node_child_by_field_name(namespace_node, "name", 4);
            const std::string name
                = extract_node_text(name_node, this->source_code_);
            split_namespace(scope_str, name);
            break;
        }
        case Namespace:
        {
            found_name_space = true;
            const TSNode name_node
                = ts_node_child_by_field_name(current, "name", 4);
            const std::string name
                = extract_node_text(name_node, this->source_code_);
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

Stmt* CSharpTSTreeVisitor::make_while_loop(
    const TSNode* node,
    const bool is_do_while
)
{
    const TSNode cond_node = ts_node_child_by_field_name(*node, "condition", 9);
    const TSNode body_node = ts_node_child_by_field_name(*node, "body", 4);
    const ExprHandler cond_handler = NodeRegistry::get_expr_handler(cond_node);
    const StmtHandler body_handler = NodeRegistry::get_stmt_handler(body_node);
    Expr* condition                = cond_handler(this, &cond_node);
    Stmt* body                     = body_handler(this, &body_node);

    if (is_do_while)
        return stmt_factory_.mk_do_while(condition, body);

    return stmt_factory_.mk_while(condition, body);
}

/**
 * Turns an expression list into a chained comma operator expression
 * @param start_node node from which to start
 * @param end_node node at which to end (exclusive). If nullptr, ends when there
 * aren't other siblings
 * @return returns the chained comma operator expression. If there is only one
 * expression, that expression is returned. If start and end are the same,
 * nullptr is returned.
 */
Expr* CSharpTSTreeVisitor::expr_list_to_comma_op(
    const TSNode& start_node,
    const TSNode* end_node
)
{
    std::queue<Expr*> exprs;
    TSNode next = start_node;
    while (! ts_node_is_null(next))
    {
        TSNode current_node = next;
        next                = ts_node_next_sibling(current_node);
        if (end_node && ts_node_eq(current_node, *end_node))
            break;

        if (NodeRegistry::is_structural_node(ts_node_type(current_node)))
            continue;

        const ExprHandler expr_handler
            = NodeRegistry::get_expr_handler(current_node);
        exprs.push(expr_handler(this, &current_node));
    }

    if (exprs.empty())
        return nullptr;

    Expr* init_expr = exprs.front();
    exprs.pop();

    while (! exprs.empty())
    {
        Expr* right = exprs.front();
        exprs.pop();
        init_expr = expr_factory_.mk_bin_on(init_expr, BinOpType::Comma, right);
    }
    return init_expr;
}

std::vector<ParamVarDefStmt*> CSharpTSTreeVisitor::handle_param_list(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    std::vector<ParamVarDefStmt*> parameters;
    if (! ts_tree_cursor_goto_first_child(&cursor))
        throw std::logic_error("Invalid Node");

    const StmtHandler handler = NodeRegistry::get_stmt_handler("parameter");
    do
    {
        TSNode current = ts_tree_cursor_current_node(&cursor);
        if (NodeRegistry::is_structural_node(ts_node_type(current)))
            continue;

        parameters.emplace_back(as_a<ParamVarDefStmt>(handler(self, &current)));
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    ts_tree_cursor_delete(&cursor);
    return parameters;
}

std::vector<Expr*> CSharpTSTreeVisitor::handle_argument_list(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    TSTreeCursor cursor = ts_tree_cursor_new(*node);
    TSNode current      = ts_tree_cursor_current_node(&cursor);
    if (! ts_tree_cursor_goto_first_child(&cursor))
        throw std::logic_error("Invalid node");

    std::vector<Expr*> exprs;
    do
    {
        current = ts_tree_cursor_current_node(&cursor);
        if (NodeRegistry::is_structural_node(ts_node_type(current)))
            continue;

        ts_tree_cursor_goto_first_child(&cursor);

        current                  = ts_tree_cursor_current_node(&cursor);
        ExprHandler expr_handler = NodeRegistry::get_expr_handler(current);
        exprs.emplace_back(expr_handler(self, &current));

        ts_tree_cursor_goto_parent(&cursor);
    } while (ts_tree_cursor_goto_next_sibling(&cursor));

    return exprs;
}

Stmt* CSharpTSTreeVisitor::handle_for_init_var_def(
    CSharpTSTreeVisitor* self,
    const TSNode* node
)
{
    static const std::string decl_query =
        R"(
        (variable_declaration
            (variable_declarator) @var_decl)
        )";
    std::vector<VarDefStmt*> var_defs;
    const TSNode type_node = ts_node_child_by_field_name(*node, "type", 4);
    Type* type             = make_type(self, type_node);
    std::vector<TSNode> decltr_nodes
        = find_nodes(*node, self->language_, decl_query);
    for (const auto declarator_node : decltr_nodes)
    {
        TSNode var_name_node = ts_node_child(declarator_node, 0);
        TSNode right_node    = ts_node_child(declarator_node, 2);
        ExprHandler right_side_handler
            = NodeRegistry::get_expr_handler(right_node);
        LocalVarDefStmt* var_def = stmt_factory_.mk_local_var_def(
            extract_node_text(var_name_node, self->source_code_),
            type,
            right_side_handler(self, &right_node)
        );
        var_defs.push_back(var_def);
        self->semantic_context_.add_local_var(var_def);
    }

    if (var_defs.size() > 1)
    {
        return stmt_factory_.mk_def(var_defs);
    }
    return var_defs.front();
}
} // namespace astfri::csharp
