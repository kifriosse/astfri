#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/Registries.hpp>
#include <libastfri-cs/impl/util/astfri_util.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri-cs/impl/util/utils.hpp>

namespace astfri::csharp::util
{

Scope create_scope(
    const TSNode& node,
    const TSLanguage* lang,
    const std::string_view source
)
{
    enum NodeType
    {
        Class,
        Interface,
        Namespace,
        Root,
    };

    // todo add other type like struct and record
    static std::unordered_map<std::string_view, NodeType> node_type_map = {
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
        const auto res = node_type_map.find(ts_node_type(parent));
        current        = parent;
        parent         = ts_node_parent(current);

        if (res == node_type_map.end())
            continue;

        switch (res->second)
        {
        case Class:
        case Interface:
        {
            const TSNode name_node = child_by_field_name(current, "name");
            const std::string name = extract_node_text(name_node, source);
            scope_str.push(name);
            break;
        }
        case Root:
        {
            if (found_name_space)
                break;

            const TSNode namespace_node = find_first_node(
                current,
                lang,
                regs::Queries::file_namespace_query
            );
            if (ts_node_is_null(namespace_node))
                break;

            const TSNode name_node
                = child_by_field_name(namespace_node, "name");
            const std::string name = extract_node_text(name_node, source);
            split_namespace(scope_str, name);
            break;
        }
        case Namespace:
        {
            found_name_space       = true;
            const TSNode name_node = child_by_field_name(current, "name");
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

Type* make_type(const TSNode& node, const std::string_view src_code)
{
    TypeFactory& type_factory      = TypeFactory::get_instance();
    std::string type_name          = extract_node_text(node, src_code);
    const char last_char           = type_name[type_name.length() - 1];
    const bool is_indirection_type = last_char == '*' || last_char == '&';
    // todo add `ref`
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

ParamVarDefStmt* make_param_def(
    const TSNode& node,
    const TSLanguage* lang,
    const std::string_view source_code
)
{
    StmtFactory& stmt_factory   = StmtFactory::get_instance();
    TypeFactory& type_factory   = TypeFactory::get_instance();
    const CSModifiers param_mod = CSModifiers::handle_modifiers(
        find_nodes(node, lang, regs::Queries::var_modif_query),
        source_code
    );

    const TSNode type_node       = child_by_field_name(node, "type");
    const TSNode param_name_node = child_by_field_name(node, "name");
    Type* param_type             = make_type(type_node, source_code);

    if (param_mod.has(CSModifier::Out) || param_mod.has(CSModifier::Ref))
    {
        param_type = type_factory.mk_indirect(param_type);
    }
    else if (param_mod.has(CSModifier::In)
             || (param_mod.has(CSModifier::Readonly)
                 && param_mod.has(CSModifier::Ref)))
    {
        // todo should be a constat reference for now, it will be just indirect
        param_type = type_factory.mk_indirect(param_type);
    }

    return stmt_factory.mk_param_var_def(
        extract_node_text(param_name_node, source_code),
        param_type,
        nullptr
    );
}

ParamSignature discover_params(const TSNode& node, std::string_view src_code)
{
    static StmtFactory& stmt_factory = StmtFactory::get_instance();
    std::vector<ParamVarDefStmt*> params;
    std::vector<ParamMetadata> params_data;

    auto collector = [&](const TSNode& current)
    {
        const TSNode param_name_node = child_by_field_name(current, "name");
        const TSNode type_node       = child_by_field_name(current, "type");
        std::string param_name = extract_node_text(param_name_node, src_code);
        Type* type             = make_type(type_node, src_code);
        ParamVarDefStmt* param_def = stmt_factory.mk_param_var_def(
            std::move(param_name),
            type,
            nullptr
        );
        params.push_back(param_def);
        params_data.emplace_back(
            param_def,
            current,
            ts_node_next_named_sibling(param_name_node)
        );
    };
    process_param_list(node, collector);
    return {std::move(params), std::move(params_data)};
}

void process_param_list(
    const TSNode& node,
    const std::function<void(const TSNode&)>& collector
)
{
    TSNode type_node{};
    const bool is_variadic = has_variadic_param(node, &type_node);
    TSTreeCursor cursor    = ts_tree_cursor_new(node);
    if (ts_tree_cursor_goto_first_child(&cursor))
    {
        do
        {
            TSNode current = ts_tree_cursor_current_node(&cursor);
            if (is_variadic && ts_node_eq(current, type_node))
            {
                collector(node);
                break;
            }

            if (! ts_node_is_named(current))
                continue;

            collector(current);
        } while (ts_tree_cursor_goto_next_sibling(&cursor));
    }

    ts_tree_cursor_delete(&cursor);
}

FunctionMetadata make_func_metadata(
    const TSNode& node,
    const std::string_view src_code
)
{
    StmtFactory& stmt_factory  = StmtFactory::get_instance();
    const TSNode name_node     = child_by_field_name(node, "name");
    const TSNode return_node   = child_by_field_name(node, "type");
    const TSNode params_node   = child_by_field_name(node, "parameters");
    std::string name           = extract_node_text(name_node, src_code);
    Type* ret_type             = make_type(return_node, src_code);
    auto [params, params_data] = discover_params(params_node, src_code);
    return FunctionMetadata{
        .params   = std::move(params_data),
        .func_def = stmt_factory.mk_function_def(
            std::move(name),
            std::move(params),
            ret_type,
            nullptr
        ),
        .function_node = node,
    };
}
} // namespace astfri::csharp::util