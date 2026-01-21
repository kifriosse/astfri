#include <libastfri-cs/impl/data/CSModifiers.hpp>
#include <libastfri-cs/impl/regs/Registries.hpp>
#include <libastfri-cs/impl/util/astfri_util.hpp>
#include <libastfri-cs/impl/util/ts_util.hpp>
#include <libastfri-cs/impl/util/utils.hpp>
#include <libastfri/inc/Astfri.hpp>

#include <tree_sitter/api.h>

#include <functional>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>

namespace astfri::csharp::util
{

Scope create_scope(const TSNode& node, const std::string_view source)
{
    enum NodeType
    {
        Class,
        Interface,
        Namespace,
        Root,
    };

    // todo add other type like struct and record
    static const std::unordered_map<std::string_view, NodeType> node_types = {
        {"class_declaration",     Class    },
        {"struct_declaration",    Class    },
        {"interface_declaration", Interface},
        {"namespace_declaration", Namespace},
        {"compilation_unit",      Root     },
    };

    std::stack<std::string> scopes;
    Scope scope      = {};
    TSNode n_current = node;
    TSNode n_parent  = ts_node_parent(n_current);

    bool found_nms   = false;
    while (! ts_node_is_null(n_parent))
    {
        const auto it = node_types.find(ts_node_type(n_parent));
        n_current     = n_parent;
        n_parent      = ts_node_parent(n_current);

        if (it == node_types.end())
            continue;

        switch (it->second)
        {
        case Class:
        case Interface:
        {
            const TSNode n_name = child_by_field_name(n_current, "name");
            std::string name    = extract_text(n_name, source);
            scopes.push(std::move(name));
            break;
        }
        case Root:
        {
            if (found_nms)
                break;

            TSNode n_namespace{};
            auto process = [&n_namespace](const TSQueryMatch& match)
            { n_namespace = match.captures[0].node; };
            for_each_match(n_current, regs::QueryType::FileNamespace, process);

            if (ts_node_is_null(n_namespace))
                break;

            const TSNode n_name    = child_by_field_name(n_namespace, "name");
            const std::string name = extract_text(n_name, source);
            split_namespace(scopes, name);
            break;
        }
        case Namespace:
        {
            found_nms              = true;
            const TSNode n_name    = child_by_field_name(n_current, "name");
            const std::string name = extract_text(n_name, source);
            split_namespace(scopes, name);
            break;
        }
        }
    }

    while (! scopes.empty())
    {
        scope.names_.push_back(scopes.top());
        scopes.pop();
    }
    return scope;
}

Scope create_scope(const std::string_view qualifier)
{
    Scope scope;
    auto current      = qualifier.begin();
    auto slice_begin  = current;
    const auto it_end = qualifier.end();

    while (current != it_end)
    {
        if (*current == '.')
        {
            scope.names_.emplace_back(slice_begin, current);
            slice_begin = current + 1;
        }
        ++current;
    }

    if (slice_begin != it_end)
    {
        scope.names_.emplace_back(slice_begin, current);
    }

    return scope;
}

ParamVarDefStmt* make_param_def(
    const TSNode& node,
    const std::string_view src,
    TypeTranslator& type_tr
)
{
    const TSNode n_type         = child_by_field_name(node, "type");
    const TSNode n_name         = child_by_field_name(node, "name");
    const TypeHandler th        = RegManager::get_type_handler(n_type);
    const CSModifiers param_mod = CSModifiers::handle_modifs_param(node, src);
    Type* param_type = param_mod.get_indection_type(th(&type_tr, n_type));

    return StmtFactory::get_instance()
        .mk_param_var_def(extract_text(n_name, src), param_type, nullptr);
}

ParamSignature discover_params(
    const TSNode& node,
    const std::string_view src_code,
    TypeTranslator& type_tr
)
{
    std::vector<ParamVarDefStmt*> params;
    std::vector<ParamMetadata> params_data;

    auto collector = [&](const TSNode& current) -> void
    {
        const TSNode n_name        = child_by_field_name(current, "name");
        ParamVarDefStmt* param_def = make_param_def(current, src_code, type_tr);
        params.push_back(param_def);
        params_data.emplace_back(
            param_def,
            current,
            ts_node_next_named_sibling(n_name)
        );
    };
    process_param_list(node, collector);
    return {std::move(params), std::move(params_data)};
}

FuncMetadata make_func_metadata(
    const TSNode& node,
    const std::string_view src,
    TypeTranslator& type_tr
)
{
    static auto& stmt_f        = StmtFactory::get_instance();
    const TSNode n_name        = child_by_field_name(node, "name");
    const TSNode n_ret         = child_by_field_name(node, "type");
    const TSNode n_params      = child_by_field_name(node, "parameters");
    std::string name           = extract_text(n_name, src);
    const TypeHandler th       = RegManager::get_type_handler(n_ret);
    Type* ret_type             = th(&type_tr, n_ret);
    auto [params, params_data] = discover_params(n_params, src, type_tr);
    return FuncMetadata{
        .params   = std::move(params_data),
        .func_def = stmt_f.mk_function_def(
            std::move(name),
            std::move(params),
            ret_type,
            nullptr
        ),
        .function_node = node,
    };
}
} // namespace astfri::csharp::util